import SekiInfo from "./SekiInfo.vue"
import SekiSettings from "./SekiSettings.vue"
import SekiInterface from "./SekiInterface.vue"

export const FIRST_PLAYER_ID = "R"
export const SECOND_PLAYER_ID = "C"
export const MAX_FIELD_SIZE = 6
export const MAX_CELL_VALUE = 99

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
        return "Seki"
    },

    getInterfaceGameName(game_type) {
        if (game_type === "seki") {
            return "Seki"
        } else if (game_type === "dseki") {
            return "D-Seki"
        }
    },

    makeMoveEvent(move) {
        const event = {
            X: move.pos.x,
            Y: move.pos.y,
            IsPass: false
        }
        if (move.type === "pass") {
            event.IsPass = true;
        }
        return event
    },

    makeMoveFromEvent(event) {
        if (event.IsPass) {
            return {
                type: "pass",
                pos: {x: 0, y: 0},
            }
        } else {
            return {
                type: "dec",
                pos: {x: event.X, y: event.Y},
            }
        }
    },

    updateGameState(state) {
        const new_state = {
            field: [],
            width: state.Width,
            height: state.Height,
            current_player: state.CurrentPlayer,
            game_type: state.Type,
            pass_options: {"R": 0, "C": 0},
            pass_count: state.PassCount,
        }

        let c = 0

        for (let i = 0; i < state.Height; ++i) {
            new_state.field.push([])
            for (let j = 0; j < state.Width; ++j) {
                new_state.field[i].push(
                    {
                        value: state.FlattenedField[c++],
                        pos: {x: j, y: i}
                    }
                )
            }
        }

        if (state.Pp === FIRST_PLAYER_ID) {
            new_state.pass_options = {"R": 1, "C": 0}
        } else if (state.Pp === SECOND_PLAYER_ID) {
            new_state.pass_options = {"R": 0, "C": 1}
        } else if (state.Pp === FIRST_PLAYER_ID + SECOND_PLAYER_ID) {
            new_state.pass_options = {"R": 1, "C": 1}
        } else if (state.Pp === NOONE_PLAYERS_IDS) {
            new_state.pass_options = {"R": 0, "C": 0}
        }

        return new_state
    },

    isMoveValid(game_state, move) {
        if (move.type === "dec") {
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
        } else if (move.type === "pass") {
            return true
        } else {
            console.error("Wrong move type:", move)
        }
    },

    applyMove(game_state, move) {
        if (move.type === 'pass') {
            console.log("Pass");
        }
        if (move.type === 'dec') {
            game_state.field[move.pos.y][move.pos.x].value -= 1;
        }

        return game_state
    },

    initStateFromSettings(settings, game_type) {
        let tmp_pass = ""
        if (settings.pass_options.length === 2) {
            tmp_pass = "RC"
        } else if (settings.pass_options.length === 1) {
            tmp_pass = settings.pass_options[0]
        }
        return {
            CurrentPlayer: settings.first_move,
            Width: settings.field_width,
            Height: settings.field_height,
            FlattenedField: settings.field,
            Type: settings.game_type,
            Pp: tmp_pass,
        }
    },

    FIRST_PLAYER_ID: FIRST_PLAYER_ID,
    SECOND_PLAYER_ID: SECOND_PLAYER_ID,
}