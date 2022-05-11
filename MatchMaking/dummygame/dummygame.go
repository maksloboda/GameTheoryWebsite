package dummygame

import (
	"encoding/json"
	"errors"
	"matchmaking/graph/model"
)

type DummyGameState struct {
	CurrentPlayer uint8
	Number        int
}

type DummyGameMove struct {
	NewNumber int
}

func getStateFromJson(state_json string) (DummyGameState, error) {
	var state DummyGameState
	err := json.Unmarshal([]byte(state_json), &state)
	return state, err
}

func IsStateValid(state_json string) bool {
	_, err := getStateFromJson(state_json)
	return err == nil
}

func AddEvent(player_token string, game_ptr *model.GameInfo, move_json string) (string, error) {
	state, state_error := getStateFromJson(game_ptr.State)
	if state_error != nil {
		return "", state_error
	}
	expected_player_token, _ := json.Marshal(state.CurrentPlayer)
	if string(expected_player_token) != player_token {
		return "", errors.New("Wrong player")
	}
	var move DummyGameMove
	move_parse_error := json.Unmarshal([]byte(move_json), &move)
	if move_parse_error != nil {
		return "", move_parse_error
	}
	state.Number = move.NewNumber
	state.CurrentPlayer = 1 - state.CurrentPlayer
	game_ptr.EventClock += 1
	result, _ := json.Marshal(state)
	return string(result), nil
}
