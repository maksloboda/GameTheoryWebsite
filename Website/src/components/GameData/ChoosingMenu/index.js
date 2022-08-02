import SingleSuitChooseSettings from "./SingleSuitChooseSettings"
import SingleSuitInfo from "./../SingleSuit/SingleSuitInfo.vue"

export default {
  getInfoComponent() {
    return SingleSuitInfo
  },
  getSettingsComponent() {
    return SingleSuitChooseSettings
  },
  getInternalGameName() {
    return "singlesuit"
  },
  getCardGameName() {
    return "Odnomastka"
  },
}