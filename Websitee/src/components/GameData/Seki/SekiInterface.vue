<template>
  <b-container class="container mx-0 px-0">
    <div v-if="game_state == null">
      <center>
        {{$t('message.SekiInterface.NullGameState')}}
      </center>
    </div>
    <div v-else >
      <b-card class="text-center" :title="subgameName()">
        <b-row>
          <b-col sm="12">
            <b-row 
              class="justify-content-md-center"
              v-for="row in game_state.field"
            >
              <b-col 
                class="bg-secondary mx-0 px-0"
                md="auto"
                v-for="cell in row"
              >
                <b-button 
                  class="cell"
                  size="lg"
                  squared 
                  :disabled="!is_active" 
                  @click="makeMoveInterface({type:'dec', pos:cell.pos})"
                >{{ cell.value }}</b-button>
              </b-col>
            </b-row>
          </b-col>
          <b-col sm="2">
            <b-button 
              v-if="pass_enabled" 
              :disabled="!is_active"
              @click="makeMoveInterface({type:'pass', pos:{x: 0, y:0}})" 
              variant="danger"
            >{{$t('message.SekiInterface.Pass')}}</b-button>
          </b-col>
        </b-row>
      </b-card>
    </div>
  </b-container>
</template>

<style>
  .cell {
    width:65px;
    height:65px;
  }
</style>

<script>

import Seki from "./"

export default {
  mounted() {
    this.$emit("component_ready")
  },
  data() {
    return {
      game_state: null,
      is_active: false,
      pass_enabled: true,
      game_type: "seki",
      game_name: "Seki",
    }
  },
  methods: {
    subgameName() {
      if (this.game_type == "seki") {
        return this.$t('message.SekiSettings.TypeSeki')
      } else if (this.game_type == "dseki") {
        return this.$t('message.SekiSettings.TypeDSeki')
      }
    },
    setState(state, game_type, player_id) {
      this.game_state = state
      this.game_type = game_type
      this.game_name = Seki.getInterfaceGameName(game_type)
      if (player_id == "" || state.pass_options[player_id] == 0) {
        this.pass_enabled = false
      } else {
        this.pass_enabled = true
      }
    },
    setIsActive(is_active) {
      this.is_active = is_active
    },
    makeMoveInterface(move) {
      this.$emit("move", move);
    },
    applyMoveInterface(move) {
      // this.game_state = Seki.applyMove(this.game_state, move);
    }
  }
}
</script>
