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

import { GET_GAME_INFO_QUERY } from '../constants/graphql'
import { useQuery } from "@vue/apollo-composable"

export default {
  mounted() {
    console.log("Game mounted")

    const id = this.$route.params.game_uid;
    const { game_info, loading, error } = this.getGameInfo(id);
    console.log("Game info", id, game_info, loading, error)

    this.game_name = "seki"

    this.game_state = {
      field: [
        [ {pos: {y:0, x:0}, value:3},  {pos: {y:0, x:1}, value:3},  {pos: {y:0, x:2}, value:3},  {pos: {y:0, x:3}, value:3}],
        [ {pos: {y:1, x:0}, value:3},  {pos: {y:1, x:1}, value:3},  {pos: {y:1, x:2}, value:3},  {pos: {y:1, x:3}, value:3}],
        [ {pos: {y:2, x:0}, value:3},  {pos: {y:2, x:1}, value:3},  {pos: {y:2, x:2}, value:3},  {pos: {y:2, x:3}, value:3}],
        [ {pos: {y:3, x:0}, value:3},  {pos: {y:3, x:1}, value:3},  {pos: {y:3, x:2}, value:3},  {pos: {y:3, x:3}, value:3}]
      ],
      width: 4,
      height: 4
    }

    this.ga
    
    for (const i of GameData.games) {
      if (i.getInternalGameName() == this.game_name) {
        this.game_object = i
        this.game_component = i.getInterfaceComponent()
        console.log(this.game_component)
        break
      }
    }
    console.log("Component spawned") 
  },

  data() {
    return {
      game_object: null,
      game_component: null,
      game_state: null,
      game_name: "",

      skipGameInfoQuery: true,
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

    getGameInfo(id) {
      const { game_info, loading, error } = useQuery(
        GET_GAME_INFO_QUERY,
        {
          id: id,
        }
      )

      return { game_info, loading, error }
    }
  },
}
</script>
