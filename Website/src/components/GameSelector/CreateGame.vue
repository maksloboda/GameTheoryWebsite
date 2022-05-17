<template>
  <div class="container mt-4">
    <div class="row">
      <div class="col-sm-6" v-for="game in games" :key="game.getCardName">
        <game-card :game_object="game"></game-card>
      </div>
    </div>
  </div>
</template>

<script>
import GameCard from "./GameCard.vue"

import GameData from "./../GameData"

export default {
    components: {
      GameCard,
    },
    data() {
      return {
          games: GameData.games
        }
    },
};
</script>