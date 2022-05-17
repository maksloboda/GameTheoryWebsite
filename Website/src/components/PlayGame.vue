<template>
  <b-container>
    <div v-if="game_component != null">
    <component
      :is="game_component"
      ref="game_instance"
      @component_ready="onGameComponentSpawned"
      @move="onMoveMade"
    ></component>
    <b-button @click="leaveGame" variant="danger">Leave game</b-button>
    </div>
    <div v-else>Loading game</div>
  </b-container>
</template>

<script>

import GameData from "./GameData"

export default {
  mounted() {
    console.log("Game mounted")
    // TODO make an actual request to find what game to play
    setTimeout(() => {
      const game_name = "seki"
      this.current_game_state = {
        table: [
          [ {pos: {y:0, x:0}, value:3},  {pos: {y:0, x:1}, value:3},  {pos: {y:0, x:2}, value:3},  {pos: {y:0, x:3}, value:3}],
          [ {pos: {y:1, x:0}, value:3},  {pos: {y:1, x:1}, value:3},  {pos: {y:1, x:2}, value:3},  {pos: {y:1, x:3}, value:3}],
          [ {pos: {y:2, x:0}, value:3},  {pos: {y:2, x:1}, value:3},  {pos: {y:2, x:2}, value:3},  {pos: {y:2, x:3}, value:3}],
          [ {pos: {y:3, x:0}, value:3},  {pos: {y:3, x:1}, value:3},  {pos: {y:3, x:2}, value:3},  {pos: {y:3, x:3}, value:3}]
        ]
      }
      for(const i of GameData.games) {
        if(i.getInternalGameName() == game_name) {
          this.game_component = i.getInterfaceComponent()
          console.log(this.game_component)
          break
        }
      }
      console.log("Component spawned")
    }, 2000)
    
  },
  data() {
    return {
      game_component: null,
      current_game_state: null,
    }
  },
  methods: {
    /**
     * Called when interface is loaded and used for initialization
     */
    onGameComponentSpawned() {
      this.$refs["game_instance"].setState(this.current_game_state)
      this.$refs["game_instance"].setIsActive(true)
    },
    /**
     * Handles local player making a move
     * @param move opaque game state
     */
    onMoveMade(move) {
      console.log("Move performed:", move)
      this.$refs["game_instance"].setIsActive(false)
      setTimeout(() => {
        this.$refs["game_instance"].applyMove(move)
        this.$refs["game_instance"].setIsActive(true)
      }, 1000)
    },
    leaveGame() {
      console.log("Game left")
      this.$router.push({path: "/"})
    }
  }
}
</script>
