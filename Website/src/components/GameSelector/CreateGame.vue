<template>
  <div class="container mt-4">
    <div class="row">
      <div class="col-sm-6" v-for="game in games" :key="game.getCardName">
        <game-card 
          :game_object="game"
          @createGame="createGame"
        ></game-card>
      </div>
    </div>
  </div>
</template>

<script>
import GameCard from "./GameCard.vue"
import GameData from "./../GameData"

import { useQuery } from '@vue/apollo-composable'

export default {
  components: {
    GameCard,
  },
  data() {
    return {
        games: GameData.games
      }
  },
  methods: {
    createGame(settings) {
      //TODO make a request to find a uuid

      const uuid = "2398-3231-2313-43567"
      this.$router.push({path: "play/" + uuid})
    },

    async getGameInfoQuery() {
    },
  },
};
</script>