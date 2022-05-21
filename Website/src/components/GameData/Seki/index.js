import SekiInfo from "./SekiInfo.vue"
import SekiSettings from "./SekiSettings.vue"
import SekiInterface from "./SekiInterface.vue"
import { FIRST_PLAYER } from "../../../constants/players"

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
  makeMoveEvent(move) {
    const event ={
      x: move.pos.x,
      y: move.pos.y
    }
    console.log(event)
    return event
  },
  updateGameState(state) {
    const new_state =  {
      field: [],
      width: state.width,
      height: state.height
    }

    let c = 0

    for (let i = 0; i < state.height; ++i) {
      new_state.field.push([])
      for (let j = 0; j < state.width; ++j) {
        new_state.field[i].push(
          {
            value: state.flattened_field[c++],
            pos: { x: j, y: i }
          }
        )
      }
    }

    return new_state
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
  },
  initStateFromSettings(settings) {
    return {
      current_player: FIRST_PLAYER,
      width: settings.field_width,
      height: settings.field_height,
      flattened_field: settings.field,
    }
  }
}