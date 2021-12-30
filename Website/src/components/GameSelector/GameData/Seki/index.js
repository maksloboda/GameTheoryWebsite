import SekiInfo from "./SekiInfo.vue"
import SekiSettings from "./SekiSettings.vue"

export default {
  getInfoComponent() {
    return SekiInfo
  },
  getSettingsComponent() {
    return SekiSettings
  },
  getInternalGameName() {
    return "seki"
  },
  getCardGameName() {
    return "Секи"
  }
}