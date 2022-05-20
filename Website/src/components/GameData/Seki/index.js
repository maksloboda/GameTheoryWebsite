import SekiInfo from "./SekiInfo.vue"
import SekiSettings from "./SekiSettings.vue"
import SekiInterface from "./SekiInterface.vue"

export default {
  getInfoComponent() {
    return SekiInfo
  },
  getSettingsComponent() {
    return SekiSettings
  },
  getInterfaceComponent() {
    return SekiInterface
  },
  getInternalGameName() {
    return "seki"
  },
  getCardGameName() {
    return "Секи"
  },
  isMoveValid(game_state, move) {
    if (move.type == "dec") {
      if (game_state == null || move == null) {
        return false
      }
      if (move.pos.x < 0 || move.pos.y < 0 || 
          move.pos.x >= game_state.width || move.pos.y >= game_state.height
        ) {
        return false
      }

      if (game_state.field[move.pos.y][move.pos.x].value <= 0) {
        return false
      }
      // ...
      return true
    } else if (move.type == "pass") {
      return true
    } else {
      console.error("Wrong move type:", move)
    }
  },
  applyMove(game_state, move) {
    if (move.type == 'pass') {
      console.log("Pass");
    }
    if (move.type == 'dec') {
      game_state.field[move.pos.y][move.pos.x].value -= 1;
    }

    return game_state
  }
}