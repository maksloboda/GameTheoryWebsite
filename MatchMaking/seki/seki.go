package seki

import (
	"encoding/json"
	"errors"
	"matchmaking/graph/model"
)

type SekiGameState struct {
	CurrentPlayer  string
	Width          int
	Height         int
	FlattenedField []int
}

type SekiGameMove struct {
	X int
	Y int
}

func getStateFromJson(state_json string) (*SekiGameState, error) {
	var state SekiGameState
	parse_err := json.Unmarshal([]byte(state_json), &state)
	if parse_err != nil {
		return nil, parse_err
	}
	return &state, nil
}

func IsStateValid(state_json string) bool {
	_, err := getStateFromJson(state_json)
	return err == nil
}

func JoinGame(game_ptr *model.GameInfo, pid string) (string, error) {
	if pid == "C" || pid == "R" {
		for _, i := range game_ptr.PlayersJoined {
			if pid == i {
				return "", errors.New("Game is full")
			}
		}
		game_ptr.PlayersJoined = append(game_ptr.PlayersJoined, pid)
		game_ptr.EventClock += 1
		return pid, nil
	}
	return "", errors.New("Invalid PID")
}

func AddEvent(player_token string, game_ptr *model.GameInfo, move_json string) (string, error) {
	state, state_error := getStateFromJson(game_ptr.State)
	if state_error != nil {
		return "", state_error
	}
	if len(game_ptr.PlayersJoined) != 2 {
		return "", errors.New("Not enough players")
	}
	if state.CurrentPlayer != player_token {
		return "", errors.New("Wrong player")
	}
	var move SekiGameMove
	move_parse_error := json.Unmarshal([]byte(move_json), &move)
	if move_parse_error != nil {
		return "", move_parse_error
	}
	idx := move.X + move.Y*state.Width
	state.FlattenedField[idx] -= 1
	switch state.CurrentPlayer {
	case "R":
		state.CurrentPlayer = "C"
	case "C":
		state.CurrentPlayer = "R"
	}
	game_ptr.EventClock += 1
	result, _ := json.Marshal(state)
	return string(result), nil
}
