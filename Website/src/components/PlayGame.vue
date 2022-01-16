<template>
  <b-container>
    <component
      v-if="game_component != null"
      :is="game_component"
      ref="game_instance"
      @component_ready="onGameComponentSpawned"
      @move="onMoveMade"
      ></component>
    <div v-else>Loading game</div>
  </b-container>
</template>

<script>

import GameData from "./GameSelector/GameData"

export default {
  mounted() {
    console.log("Game mounted")
    // TODO make an actual request to find what game to play
    setTimeout(() => {
      const game_name = "seki"
      this.current_game_state = 1
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
        this.$refs["game_instance"].applyMove(+2)
        this.$refs["game_instance"].setIsActive(true)
      }, 1000)
    },
  }
}
</script>
