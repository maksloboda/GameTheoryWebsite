<template>
  <b-container>
    <div v-if="this.$apollo.loading">
      Loading...
    </div>
    <div v-else>
      <component
        :is="game_component"
        ref="game_instance"
        @component_ready="onGameComponentSpawned"
        @move="onMoveMade"
      ></component>
      <b-button @click="leaveGame" variant="danger">Leave game</b-button>
    </div>
  </b-container>
</template>

<script>

import GameData from "./GameData"

import { GET_GAME_INFO_QUERY } from '../constants/graphql'

export default {
  mounted() {
    console.log("Game mounted")

    this.getGameInfo();
    
    console.log("Component spawned") 
  },

  data() {
    return {
      game_object: null,
      game_component: null,
      game_state: null,
      game_name: "",
      skipQuery: true,
      gameInfo: null,
    }
  },

  apollo: {
    gameInfo: {
      query: GET_GAME_INFO_QUERY,
      variables() {
        return {
          game_id: this.$route.params.game_uid
        }
      },
      // Disable the query
      skip() {
        return this.skipQuery
      },
      result ({ data, loading, networkStatus }) {
        this.updateGame(data.gameInfo)
      },
      error (error) {
        console.log("Game not found", error)
      }
    }
  }, 

  methods: {
    /**
     * Called when interface is loaded and used for initialization
     */

    onGameComponentSpawned() {
      this.$refs["game_instance"].setState(this.game_state)
      this.$refs["game_instance"].setIsActive(true)
    },
    /**
     * Handles local player making a move
     * @param move opaque game state
     */

    onMoveMade(move) {
      this.getGameInfo(this.$route.params.game_uid);
      console.log("Move performed:", move)

      if (this.game_object.isMoveValid(this.game_state, move)) {
        this.$refs["game_instance"].setIsActive(false)
        setTimeout(() => {
          this.$refs["game_instance"].applyMoveInterface(move)
          this.$refs["game_instance"].setIsActive(true)
        }, 1000)
      }
    },

    leaveGame() {
      console.log("Game left")
      this.$router.push({path: "/"})
    },

    getGameInfo() {
      this.$apollo.queries.gameInfo.skip = false
      this.$apollo.queries.gameInfo.refetch()
    },

    updateGame(game_info) {
      this.game_name = game_info.game_name

      for (const game of GameData.games) {
        if (game.getInternalGameName() == this.game_name) {
          this.game_object = game
          this.game_component = game.getInterfaceComponent()
          console.log(this.game_component)
          break
        }
      }
      
      this.game_state = this.game_object.updateGameState(game_info.state)
    }
  },
}
</script>
