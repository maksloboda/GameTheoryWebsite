<template>
  <div class="container mt-4">
    <div class="row">
      <div class="col-sm-6" v-for="game in games">
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
import {
  CREATE_GAME_MUTATION
} from '@/constants/graphql'

export default {
  components: {
    GameCard,
  },
  data() {
    return {
      games: GameData.game_cards
    }
  },
  methods: {
    async createGame(game_name, start_state, is_public, time_limit, weights_array) {
      const data = {
        game_name: game_name,
        start_state: JSON.stringify(start_state),
        is_public: Boolean(is_public),
        time_limit: parseInt(time_limit),
        weights_array: weights_array,
      }
      console.log("Create game:", data)

      await this.$apollo.mutate({
        mutation: CREATE_GAME_MUTATION,
        variables: {
          data: data,
        },
      }).then((response) => {
        if (response.data.createGame !== "") {
          this.$router.push({path: "play/" + response.data.createGame})
        }
      }).catch((response) => {
        console.error("Create game error", response)
      })
    },
  },
};
</script>