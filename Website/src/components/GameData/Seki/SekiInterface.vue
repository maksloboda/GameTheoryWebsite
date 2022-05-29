<template>
  <b-container>
    <div v-if="game_state === null">
      <center>
        Game state is null
      </center>
    </div>
    <div v-else>
      <div class="container mt-2">
        <b-card class="text-center" :title="game_name">
          <div class="bg-secondary mt-2"> 
            <b-row class="my-1" v-for="row in game_state.field">
              <b-col v-for="cell in row" sm="1">
                <b-button 
                  :disabled="!is_active" 
                  @click="makeMoveInterface({type:'dec', pos:cell.pos})"
                >{{ cell.value }}</b-button>
              </b-col>
            </b-row>
          </div> 
          <b-button 
            v-if="pass_enabled" 
            :disabled="!is_active"
            @click="makeMoveInterface({type:'pass', pos:{x: 0, y:0}})" 
            variant="danger"
          >Pass</b-button>
        </b-card>
      </div>
    </div>
  </b-container>
</template>

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
