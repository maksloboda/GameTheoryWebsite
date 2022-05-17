<template>
  <b-container v-if="game_state != null">
    <div class="container mt-2">
      <b-card class="text-center" title="Seki">
        <div class="bg-secondary mt-2">
          <b-row class="my-1" v-for="row in game_state.table">
            <b-col sm="1" v-for="cell in row">
              <b-button 
                :disabled="!is_active" 
                @click="makeMove({type:'dec', pos:cell.pos})"
              >{{ cell.value }}</b-button>
            </b-col>
          </b-row>
        </div>
        <b-button :disabled="!is_active" @click="makeMove({type:'pass'})" variant="danger">Pass</b-button>
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

export default {
  mounted() {
    this.$emit("component_ready")
  },
  data() {
    return {
      game_state: null,
      is_active: false,
    }
  },
  methods: {
    /**
     * Updates the current game state
     * @param state new game state
     */
    setState(state) {
      console.log(state);
      this.game_state = state
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
    makeMove(move) {
      this.$emit("move", move);
      console.log(this.game_state);
    },
    /**
     * applies move to the current state
     * @param move move that needs to be applied
     */
    applyMove(move) {
      if (move.type == 'pass') {
        console.log("Pass");
      }
      if (move.type == 'dec') {
        this.game_state.table[move.pos.y][move.pos.x].value -= 1;
      }
    }
  }
}
</script>
