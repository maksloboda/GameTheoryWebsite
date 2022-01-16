<template>
  <b-container v-if="game_state != null">
    <center>
      {{ game_state }}
      <br>
      <b-button :disabled="!is_active" @click="makeMove(1)" variant="primary">Increment</b-button>
    </center>
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
      console.log(is_active);
      this.is_active = is_active
    },
    /**
     * Called when local player makes a move
     * @param move move that was made made
     */
    makeMove(move) {
      this.$emit("move", move)
      this.applyMove(move)
    },
    /**
     * applies move to the current state
     * @param move move that needs to be applied
     */
    applyMove(move) {
      this.game_state += move
    }
  }
}
</script>
