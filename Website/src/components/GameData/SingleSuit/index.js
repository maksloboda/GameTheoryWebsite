import SingleSuitInfo from "./SingleSuitInfo.vue"
import SingleSuitSettings from "./SingleSuitSettings.vue"
import Fool from "./Fool"
import Whistette from "./Whistette"

export const FIRST_PLAYER_ID = "A"
export const SECOND_PLAYER_ID = "B"

export default {
    getInfoComponent() {
        return SingleSuitInfo
    },

    getSettingsComponent() {
        return SingleSuitSettings
    },

    getInternalGameName() {
        return "singlesuit";
    },

    getCardGameName() {
        return "Odnomastka"
    },

    getInterfaceGameName(game_type) {
        if (game_type === "singlesuit") {
            return "SingleSuit"
        } else if (game_type === "d-singlesuit") {
            return "D-SingleSuit"
        } else if (game_type === "singlesuit-whistette") {
            return "Whistette"
        }
    },

    initStateFromSettings(settings, game_type) {
        if (game_type === "whistette") {
            return Whistette.initStateFromSettings(settings)
        } else {
            return Fool.initStateFromSettings(settings)
        }
    },

    FIRST_PLAYER_ID: FIRST_PLAYER_ID,
    SECOND_PLAYER_ID: SECOND_PLAYER_ID
}