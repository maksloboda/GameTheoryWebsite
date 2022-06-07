package gamemanager

import (
	"encoding/json"
	"errors"
	"fmt"
	"strconv"

	"matchmaking/graph/model"
	"sync"
	"time"

	"github.com/go-redis/redis"
	"github.com/google/uuid"
	"github.com/streadway/amqp"
)

var redis_client *redis.UniversalClient
var amqp_connection *amqp.Connection

// var next_id uint64 = 1
var storage map[string]*model.GameInfo = make(map[string]*model.GameInfo)
var storage_lock sync.Mutex

func SetRedisClient(client *redis.UniversalClient) {
	redis_client = client
}

func SetAMQPConnection(conn *amqp.Connection) {
	amqp_connection = conn
}

type remoteCallResult struct {
	ErrorText *string `json:"error"`
	Result    string  `json:"result"`
}

func RemoteCall(game string, method string, args []interface{},
	result *remoteCallResult) error {
	ch, chan_err := amqp_connection.Channel()
	if chan_err != nil {
		return chan_err
	}
	defer ch.Close()

	query_id := uuid.New().String()

	responce_q, resp_q_error := ch.QueueDeclare("resp_"+query_id, false, true,
		false, false, nil)

	if resp_q_error != nil {
		return resp_q_error
	}

	payload := map[string]interface{}{
		"method": method,
		"params": args,
		"id":     query_id,
	}

	encoded_payload, enc_err := json.Marshal(payload)
	if enc_err != nil {
		return enc_err
	}

	message := amqp.Publishing{
		Body: encoded_payload,
	}

	pub_err := ch.Publish("", "func_"+game+"_"+method, true, false, message)
	if pub_err != nil {
		return pub_err
	}

	msgs, msg_error := ch.Consume(responce_q.Name, query_id, true, false, false, false,
		nil)
	if msg_error != nil {
		return msg_error
	}

	responce := <-msgs

	ch.Cancel(query_id, false)

	err := json.Unmarshal(responce.Body, result)
	if err != nil {
		return err
	}
	if result.ErrorText != nil {
		return errors.New(*result.ErrorText)
	}
	return nil
}

// Creates a game and returns the id of the game
func CreateGame(data *model.GameCreateRequest) (string, error) {
	// TODO find a way to make an extendible game creation pipeline

	rcr := remoteCallResult{}

	rcr_err := RemoteCall(data.GameName, "isStartStateValid",
		[]interface{}{data.StartState}, &rcr)

	if rcr_err != nil {
		return "", rcr_err
	}

	var is_state_valid bool
	json.Unmarshal([]byte(rcr.Result), &is_state_valid)

	if !is_state_valid {
		return "", nil
	}

	// TODO deadline
	new_id := uuid.New().String()

	key := getGameKey(new_id)

	stamp := time.Now().Unix()

	new_game_state := &model.GameInfo{
		ID:            new_id,
		PlayersJoined: make([]string, 0),
		GameName:      data.GameName,
		EventClock:    0,
		State:         data.StartState,
		TimeControl: &model.TimeControl{
			LastEventTime: strconv.FormatInt(stamp, 10),
			TimeLimit:     data.TimeLimit,
			TimeLeft:      0,
		},
		IsReady:    false,
		IsFinished: false,
		Winner:     "",
	}

	op := func(transaction *redis.Tx) error {
		exists, exists_error := transaction.Exists(key).Result()
		if exists_error != nil {
			return exists_error
		} else if exists != 0 {
			return errors.New("Game already exists!")
		}

		if data.IsPublic {
			result := transaction.ZAdd("public_lobbies", redis.Z{
				Score:  float64(stamp),
				Member: new_id,
			})
			if result.Err() != nil {
				return result.Err()
			}
		}

		return setGameInfoImpl(new_id, new_game_state, transaction)
	}

	err := (*redis_client).Watch(op, key)

	return new_id, err
}

func getGameKey(id string) string {
	return "gameinfo:" + id
}

type setable interface {
	Set(string, interface{}, time.Duration) *redis.StatusCmd
}

func setGameInfoImpl(id string, game_info *model.GameInfo, dest setable) error {
	marshalled_state, _ := json.Marshal(game_info)
	status, err := dest.Set(getGameKey(id), marshalled_state, 0).Result()
	if err != nil {
		_ = fmt.Errorf("%s", status)
	}
	return err
}

type getable interface {
	Get(string) *redis.StringCmd
}

func getGameInfoImpl(id string, source getable) (*model.GameInfo, error) {
	cmd_result := source.Get(getGameKey(id))

	marshalled, err := cmd_result.Bytes()

	if err == redis.Nil {
		return nil, errors.New("Game not found")
	}

	if err != nil {
		return nil, err
	}

	var result *model.GameInfo
	json.Unmarshal(marshalled, &result)

	updateRemainingTime(result.TimeControl)

	return result, nil
}

type publishable interface {
	Publish(string, interface{}) *redis.IntCmd
}

func publishImpl(id string, game_ptr *model.GameInfo, dest publishable) error {
	// TODO retry
	marshalled, _ := json.Marshal(game_ptr)
	_, err := dest.Publish(getGameKey(id), marshalled).Result()
	return err
}

// Returns ptr to GameObject or nil if not found
func GetGameInfo(id string) (*model.GameInfo, error) {
	return getGameInfoImpl(id, *redis_client)
}

func updateTime(tc *model.TimeControl) {
	stamp := time.Now().Unix()
	tc.LastEventTime = strconv.FormatInt(stamp, 10)
	updateRemainingTime(tc)
}

func updateRemainingTime(tc *model.TimeControl) {
	if tc.TimeLimit != nil {
		start_time, _ := strconv.ParseInt(tc.LastEventTime, 10, 64)
		current_time := time.Now().Unix()
		time_elapsed := current_time - start_time
		if time_elapsed > int64(*tc.TimeLimit) {
			tc.TimeLeft = 0
		} else {
			tc.TimeLeft = int(*tc.TimeLimit) - int(time_elapsed)
		}
	}
}

// Tries to add a player to the game
// Returns a token
func AddPlayer(id string, pid string) (string, error) {
	token := ""
	op := func(transaction *redis.Tx) error {
		game_ptr, get_err := getGameInfoImpl(id, transaction)
		if get_err != nil {
			return get_err
		}

		rcr := remoteCallResult{}

		rcr_err := RemoteCall(game_ptr.GameName, "joinGame",
			[]interface{}{game_ptr, pid}, &rcr)

		if rcr_err != nil {
			return rcr_err
		}

		token = pid

		parse_err := json.Unmarshal([]byte(rcr.Result), game_ptr)

		if parse_err != nil {
			return parse_err
		}

		game_ptr.EventClock += 1
		game_ptr.PlayersJoined = append(game_ptr.PlayersJoined, pid)
		updateTime(game_ptr.TimeControl)

		set_err := setGameInfoImpl(id, game_ptr, transaction)
		if set_err != nil {
			return set_err
		}
		return publishImpl(id, game_ptr, transaction)
	}

	err := (*redis_client).Watch(op, getGameKey(id))
	return token, err
}

func GetPublicGames(limit int) ([]*model.GameInfo, error) {
	slice := (*redis_client).ZRevRange("public_lobbies", 0, int64(limit)-1)
	ids, err := slice.Result()
	if err != nil {
		return []*model.GameInfo{}, err
	}

	result := make([]*model.GameInfo, 0)

	for _, v := range ids {
		gi, get_err := getGameInfoImpl(v, *redis_client)
		if get_err != nil {
			return []*model.GameInfo{}, get_err
		}
		result = append(result, gi)
	}

	return result, nil

}

func AddEvent(game_id string, player_token, move string) error {
	op := func(transaction *redis.Tx) error {
		game_ptr, get_err := getGameInfoImpl(game_id, transaction)
		if get_err != nil {
			return get_err
		}

		pid := player_token

		rcr := remoteCallResult{}

		rcr_err := RemoteCall(game_ptr.GameName, "addEvent",
			[]interface{}{game_ptr, pid, move}, &rcr)

		if rcr_err != nil {
			return rcr_err
		}

		parse_err := json.Unmarshal([]byte(rcr.Result), game_ptr)

		if parse_err != nil {
			return parse_err
		}

		game_ptr.EventClock += 1
		updateTime(game_ptr.TimeControl)
		set_err := setGameInfoImpl(game_id, game_ptr, transaction)
		if set_err != nil {
			return set_err
		}
		return publishImpl(game_id, game_ptr, transaction)
	}

	return (*redis_client).Watch(op, getGameKey(game_id))
}

func AdvanceGame(id string) (bool, error) {
	game_ptr, get_err := getGameInfoImpl(id, *redis_client)
	if get_err != nil {
		return false, get_err
	}

	if game_ptr.TimeControl.TimeLimit == nil || !game_ptr.IsReady {
		return false, nil
	}

	start_time, _ := strconv.ParseInt(game_ptr.TimeControl.LastEventTime, 10, 64)
	current_time := time.Now().Unix()
	if current_time-start_time > int64(*game_ptr.TimeControl.TimeLimit) {
		rcr := remoteCallResult{}
		rcr_err := RemoteCall(game_ptr.GameName, "advance",
			[]interface{}{game_ptr}, &rcr)
		if rcr_err != nil {
			return false, rcr_err
		}

		parse_err := json.Unmarshal([]byte(rcr.Result), &game_ptr)
		if parse_err != nil {
			return false, parse_err
		} else {
			updateTime(game_ptr.TimeControl)
			setGameInfoImpl(id, game_ptr, *redis_client)
			publishImpl(id, game_ptr, *redis_client)
			return true, nil
		}

	} else {
		return false, nil
	}
}

func SubscribeGame(id string) (<-chan *model.GameInfo, error) {
	ch := make(chan *model.GameInfo, 1)
	sub := (*redis_client).Subscribe(getGameKey(id))
	current, err := GetGameInfo(id)
	if err != nil {
		panic(err)
	}
	ch <- current
	go func() {
		defer sub.Close()
		channel := sub.Channel()
		for {
			msg, is_ok := <-channel
			if !is_ok {
				fmt.Println(msg)
				close(ch)
				return
			}
			var gi model.GameInfo
			json.Unmarshal([]byte(msg.Payload), &gi)
			ch <- &gi
		}
	}()
	return ch, nil
}

func FindOptimal(id string) (string, error) {
	game_ptr, err := GetGameInfo(id)
	if err != nil {
		return "", err
	}
	rcr := remoteCallResult{}

	rcr_err := RemoteCall(game_ptr.GameName, "findOptimalMove",
		[]interface{}{game_ptr}, &rcr)

	if rcr_err != nil {
		return "", rcr_err
	}

	return rcr.Result, nil
}
