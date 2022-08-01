import WhistetteInfo from "./WhistetteInfo.vue"
import WhistetteSettings from "./WhistetteSettings.vue"
import WhistetteInterface from "./WhistetteInterface.vue"

export const FIRST_PLAYER_ID = "A"
export const SECOND_PLAYER_ID = "B"

export default {
  getInfoComponent() {
    return WhistetteInfo
  },
  getSettingsComponent() {
    return WhistetteSettings
  },
  getInterfaceComponent() {
    return WhistetteInterface
  },
  getInternalGameName() {
    return "whistette"
  },
  getCardGameName() {
    return "Whistette"
  },
  getInterfaceGameName(game_type) {
    if (game_type == "whistette") {
      return "Whistette"
    }
  },
  makeMoveEvent(move) {
    const event = {
      Card: move.card,
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
      game_type: state.Type,
      first_score: state.FirstScore,
      second_score: state.SecondScore,
    }

    return new_state
  },
  isMoveValid(game_state, move) {
    return true
  },
  applyMove(game_state, move) {
    if (move.type == 'move') {
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

    return game_state
  },
  makeMoveFromEvent(event) {
    return {
      card: event.Card
    }
  },
  initStateFromSettings(settings) {
    return {
      CurrentPlayer: settings.first_player,
      FirstPlayerSet: settings.first_player_array,
      SecondPlayerSet: settings.second_player_array,
      LastCard: null,
      Type: settings.game_type,
      FirstScore: 0,
      SecondScore: 0
    }
  },
  FIRST_PLAYER_ID: FIRST_PLAYER_ID,
  SECOND_PLAYER_ID: SECOND_PLAYER_ID
}