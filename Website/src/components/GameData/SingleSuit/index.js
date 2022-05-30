import SingleSuitInfo from "./SingleSuitInfo.vue"
import SingleSuitSettings from "./SingleSuitSettings.vue"
import SingleSuitInterface from "./SingleSuitInterface.vue"

export const FIRST_PLAYER_ID = "A"
export const SECOND_PLAYER_ID = "B"

export default {
  getInfoComponent() {
    return SingleSuitInfo
  },
  getSettingsComponent() {
    return SingleSuitSettings
  },
  getInterfaceComponent() {
    return SingleSuitInterface
  },
  getInternalGameName() {
    return "singlesuit"
  },
  getCardGameName() {
    return "Одномастка"
  },
  getInterfaceGameName(game_type) {
    if (game_type == "singlesuit") {
      return "SingleSuit"
    } else if (game_type == "d-singlesuit") {
      return "D-SingleSuit"
    }
  },
  makeMoveEvent(move) {
    const event = {
      Card: move.card,
      DoTake: move.do_take
    }
    console.log(event)
    return event
  },
  updateGameState(state) {
    const new_state =  {
      current_player: state.CurrentPlayer,
      first_player_set: state.FirstPlayerSet,
      second_player_set: state.SecondPlayerSet,
      last_card: state.LastCard,
      game_type: state.Type
    }

    return new_state
  },
  isMoveValid(game_state, move) {
    if (move.type == "move") {
      if (game_state == null || move == null) {
        return false
      }
      if (move.do_take != 0) {
        return true;
      }
      if (move.current_player == FIRST_PLAYER_ID) {
        for (var i = 0; i < game_state.first_player_set; i++) {
          if (game_state.first_player_set[i] == move.card && move.card > game_state.last_card) {
              return true;
          }
        }
      }
      if (move.current_player == SECOND_PLAYER_ID) {
        for (var i = 0; i < game_state.second_player_set; i++) {
          if (game_state.second_player_set[i] == move.card && move.card > game_state.last_card) {
              return true;
          }
        }
      }
      return false
    } else {
      console.error("Wrong move type:", move)
    }
  },
  applyMove(game_state, move) {
    if (move.type == 'move') {
      if (move.do_take) {
        if (move.current_player == FIRST_PLAYER_ID) {
          game_state.first_player_set.push(game_state.last_card)
        } else {
          game_state.second_player_set.push(game_state.last_card)
        }
      } else {
        if (move.current_player == FIRST_PLAYER_ID) {
          for (var i = 0; i < game_state.first_player_set; i++) {
            if (game_state.first_player_set[i] == move.card && move.card > game_state.last_card) {
              game_state.first_player_set = game_state.first_player_set.splice(i, 1) // erase i-th
            }
          }
        } else {
          for (var i = 0; i < game_state.second_player_set; i++) {
            if (game_state.second_player_set[i] == move.card && move.card > game_state.last_card) {
              game_state.second_player_set = game_state.second_player_set.splice(i, 1) // erase i-th
            }
          }
        }
      }
    }

    return game_state
  },
  initStateFromSettings(settings) {
    return {
      CurrentPlayer: FIRST_PLAYER_ID,
      FirstPlayerSet: [],
      SecondPlayerSet: [],
      LastCard: null,
      Type: settings.game_type,
    }
  },
  FIRST_PLAYER_ID: FIRST_PLAYER_ID,
  SECOND_PLAYER_ID: SECOND_PLAYER_ID
}