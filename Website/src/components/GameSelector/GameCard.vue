<template>
  <div>
    <b-card
        class="mb-2"
      >
        <h5 class="card-title">
          <center>
          {{ game_object.getCardGameName() }}
          </center>
        </h5>
        <div class="d-flex">
            <div class="col-6 p-2">
              <b-button href="#" variant="primary" class="w-100" @click="showConfigModal">
                <b-icon-play-fill></b-icon-play-fill>
              </b-button>
            </div>
            <div class="col-6 p-2">
              <b-button variant="primary" class="w-100" @click="showInfoModal">
                <b-icon-info-circle-fill></b-icon-info-circle-fill>
              </b-button>
            </div>
        </div>


    </b-card>

  <b-modal ref='info_modal'>
     <component :is="game_object.getInfoComponent()"></component>
  </b-modal>

  <b-modal 
    ref='config_modal'
    hide-footer 
    size="xl"
    :title="game_object.getCardGameName()">
    <component 
      :is="game_object.getSettingsComponent()"
      ref="game_settings_component"
    ></component>
    <b-button variant="primary" class="mt-2" @click="startGame">Начать игру</b-button>
  </b-modal>


  </div>
</template>

<script>
export default {
  props: {
    "game_object": {
      default: {
        getInfoComponent() {
          return null;
        },
        getSettingsComponent() {
          return null;
        },
        getInternalGameName() {
          return "unknown";
        },
        getCardGameName() {
          return "unknown";
        },
        initStateFromSettings() {
          return null;
        },
        isMovePossible(game_state, move) {
          return false;
        },
      }
    }
  },
  methods: {
    showInfoModal() {
      this.$refs["info_modal"].show()
    },
    showConfigModal() {
      this.$refs["config_modal"].show()
    },
    startGame() {
      const settings = this.$refs["game_settings_component"].getSettings()
      console.log("Settings:", settings)
      this.$emit("createGame", 
        this.game_object.getInternalGameName(), 
        this.game_object.initStateFromSettings(settings));
    },
  },
}
</script>
