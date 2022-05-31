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
import { 
  CREATE_GAME_MUTATION
} from '../../constants/graphql'

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
    async createGame(game_name, start_state, is_public) {
      console.log("Create game:", start_state, Boolean(is_public))
      await this.$apollo.mutate({
        mutation: CREATE_GAME_MUTATION,
        variables: {
          game_name: game_name,
          start_state: JSON.stringify(start_state),
          is_public: Boolean(is_public),
        },
      }).then((response) =>  {    
          this.$router.push({path: "play/" + response.data.createGame})
        }
      ).catch((response) => {
          console.error("Create game error", response)
        }
      )
    },
  },
};
</script>