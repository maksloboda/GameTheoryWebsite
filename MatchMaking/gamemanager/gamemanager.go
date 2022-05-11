package gamemanager

import (
	"encoding/json"
	"errors"
	"fmt"
	"matchmaking/dummygame"
	"matchmaking/graph/model"
	"strconv"
	"sync"
	"time"

	"github.com/go-redis/redis"
	"github.com/google/uuid"
)

var redis_client *redis.UniversalClient

// var next_id uint64 = 1
var storage map[string]*model.GameInfo = make(map[string]*model.GameInfo)
var storage_lock sync.Mutex

func SetRedisClient(client *redis.UniversalClient) {
	redis_client = client
}

// Creates a game and returns the id of the game
func CreateGame(create_request model.GameCreateRequest) (string, error) {
	// TODO find a way to make an extendible game creation pipeline
	if create_request.GameName != "dummy" {
		return "", errors.New("Invalid game type")
	}
	if !dummygame.IsStateValid(create_request.StartState) {
		return "", errors.New("Invalid game state")
	}

	// TODO deadline
	new_id := uuid.New().String()

	key := getGameKey(new_id)

	new_game_state := &model.GameInfo{
		GameName:    create_request.GameName,
		State:       create_request.StartState,
		PlayerCount: 0,
		EventClock:  0,
	}

	op := func(transaction *redis.Tx) error {
		exists, exists_error := transaction.Exists(key).Result()
		if exists_error != nil {
			return exists_error
		} else if exists != 0 {
			return errors.New("Game already exists!")
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

// Tries to add a player to the game
// Returns a token
func AddPlayer(id string) (string, error) {
	token := ""
	op := func(transaction *redis.Tx) error {
		game_ptr, get_err := getGameInfoImpl(id, transaction)
		if get_err != nil {
			return get_err
		}

		if game_ptr.PlayerCount >= 2 {
			return errors.New("Game is full")
		}
		current_player_id := game_ptr.PlayerCount
		game_ptr.PlayerCount++
		// TODO give JWT
		token = strconv.FormatUint(uint64(current_player_id), 10)
		set_err := setGameInfoImpl(id, game_ptr, transaction)
		if set_err != nil {
			return set_err
		}
		return publishImpl(id, game_ptr, transaction)
	}

	err := (*redis_client).Watch(op, getGameKey(id))
	return token, err
}

func AddEvent(game_id string, player_token, move string) error {
	op := func(transaction *redis.Tx) error {
		game_ptr, get_err := getGameInfoImpl(game_id, transaction)
		if get_err != nil {
			return get_err
		}
		state, move_error := dummygame.AddEvent(player_token, game_ptr, move)
		if move_error != nil {
			return move_error
		}
		game_ptr.EventClock += 1
		game_ptr.State = state
		set_err := setGameInfoImpl(game_id, game_ptr, transaction)
		if set_err != nil {
			return set_err
		}
		return publishImpl(game_id, game_ptr, transaction)
	}

	return (*redis_client).Watch(op, getGameKey(game_id))
}

func SubscribeGame(id string) (<-chan *model.GameInfo, error) {
	ch := make(chan *model.GameInfo)
	sub := (*redis_client).Subscribe(getGameKey(id))
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
