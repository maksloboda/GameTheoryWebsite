<template>
  <div>
    <b-card class="mb-2">
      <h5 class="card-title">
        <center>
          {{ this.$t("message.GameNames." + game_object.getCardGameName()) }}
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

    <b-modal ref='info_modal' hide-footer size="xl">
      <component :is="game_object.getInfoComponent()"></component>
    </b-modal>

    <b-modal ref='config_modal' hide-footer size="xl"
      :title="this.$t('message.GameNames.' + game_object.getCardGameName())">
      <component :is="game_object.getSettingsComponent()" ref="game_settings_component"></component>
      <b-row>
        <b-col>
          <b-button variant="primary" class="mt-2" @click="startGame(false)">{{ $t('message.PlayGame.StartPrivate') }}
          </b-button>
        </b-col>
        <b-col>
          <b-button variant="primary" class="mt-2 float-end" @click="startGame(true)">
            {{ $t('message.PlayGame.StartPublic') }}</b-button>
        </b-col>
      </b-row>
    </b-modal>


  </div>
</template>

<style>
.close {
  font-size: 28px;
  line-height: 1;
  color: #ef1616;
  filter: alpha(opacity=20);
  border: none;
  background-color: inherit;
}

.custom-btn {
  width: 200px;
  height: 40px;
  color: #fff;
  border-radius: 5px;
  transition: all 0.3s ease;
  position: relative;
  display: inline-block;
  outline: none;
  margin: 20px;
  background: #0d6efd;
  border: none;
  z-index: 1;
}

.custom-btn:after {
  position: absolute;
  content: "";
  width: 0;
  height: 100%;
  top: 0;
  right: 0;
  z-index: -1;
  background-color: #210b99;
  border-radius: 5px;
  transition: all 0.3s ease;
}

.custom-btn:hover:after {
  left: 0;
  width: 100%;
}
</style>

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
    startGame(is_public) {
      const settings = this.$refs["game_settings_component"].getSettings()
      console.log("Settings:", settings, is_public)
      this.$emit("createGame",
        this.game_object.getInternalGameName(),
        this.game_object.initStateFromSettings(settings),
        is_public,
        settings.time_limit);
    },
  },
}
</script>
