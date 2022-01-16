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
  }
}