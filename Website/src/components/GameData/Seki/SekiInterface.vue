<template>
  <b-container v-if="game_state != null">
    <div class="container mt-2">
      <b-card class="text-center" title="Seki">
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
          :disabled="!is_active || !pass_enabled" 
          @click="makeMoveInterface({type:'pass', pos:{x: 0, y:0}})" 
          variant="danger"
        >Pass</b-button>
      </b-card>
    </div>
  </b-container>
  <b-container v-else>
    <center>
      Seki state is null
    </center>
  </b-container>
</template>

<script>

import { throwServerError } from "apollo-link-http-common"
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
    }
  },
  methods: {
    /**
     * Updates the current game state
     * @param state new game state
     */
    setState(state, player_id) {
      this.game_state = state
      console.log(state, player_id, state.pass_options[player_id])
      if (state.pass_options[player_id] == 0) {
        this.pass_enabled = false
      }
    },
    /**
     * Sets current players ability to make a turn
     * @param {boolean} is_active true if player can make a turn
     */
    setIsActive(is_active) {
      this.is_active = is_active
    },
    /**
     * Called when local player makes a move
     * @param move move that was made made
     */
    makeMoveInterface(move) {
      this.$emit("move", move);
    },
    /**
     * applies move to the current state
     * @param move move that needs to be applied
     */
    applyMoveInterface(move) {
      this.game_state = Seki.applyMove(this.game_state, move);
    }
  }
}
</script>
