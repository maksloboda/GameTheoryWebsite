<template>
  <div class="container mt-2">
    <div class="row">
      <div class="col-sm-4" v-for="game in games" :key="game.getCardName">
        <game-card :game_object="game"></game-card>
      </div>
    </div>
  </div>
</template>

<script>
import GameCard from "./GameCard.vue"

import Seki from "./GameData/Seki"

export default {
    components: {
      GameCard,
    },
    data() {
      return {
          games: [
            Seki,
          ]
        }
    },
};
</script>