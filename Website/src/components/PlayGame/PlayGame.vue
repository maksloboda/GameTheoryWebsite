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
      <b-card>
        Player token: {{ this.player_token }}
      </b-card>
      <div 
        v-if="this.player_token == null || 
              this.player_token == 0"
      >
        <b-button 
          @click="joinGame(game_object.first_player_id)" 
          variant="warning"
          ref="join_first_button"
          :disabled="players_joined.includes(game_object.first_player_id)"
        >Join as {{game_object.first_player_id}}</b-button>
        <b-button 
          @click="joinGame(game_object.second_player_id)" 
          variant="warning"
          ref="join_second_button" 
          :disabled="players_joined.includes(game_object.second_player_id)"
        >Join as {{game_object.second_player_id}}</b-button>
      </div>
      <b-button @click="leaveGame" variant="danger">Leave game</b-button>
    </div>
    
    <b-modal ref="placeTakenError">
      This player ID is already taken!
    </b-modal>

    <b-modal ref="joinError">
      Join error
    </b-modal>

    
    <b-modal ref="victoryModal">
      You won!
    </b-modal>

    <b-modal ref="drawModal">
      Draw!
    </b-modal>

    <b-modal ref="defeatModal">
      You lost
    </b-modal>
  </b-container>
</template>

<script>

import GameData from "./../GameData"

import { 
  GET_GAME_INFO_QUERY,
  JOIN_GAME_MUTATION,
  ADD_EVENT_MUTATION,
  GAME_SUBSCRIPTION,
} from '../../constants/graphql'

export default {
  async mounted() {
    await this.getGameInfo();
    await this.subscribeGame(this.updateGame);
  },

  data() {
    return {
      game_object: null,
      game_component: null,
      game_state: null,
      game_name: "",

      player_token: null,
      player_id: "",
      players_joined: [],
      current_player: "",

      is_finished: false,
      is_ready: false,
    }
  },

  methods: {
    /**
     * Called when interface is loaded and used for initialization
     */

    onGameComponentSpawned() {
      this.$refs["game_instance"].setState(this.game_state, this.player_id)
      this.$refs["game_instance"].setIsActive(false)
    },
    /**
     * Handles local player making a move
     * @param move opaque game state
     */

    async onMoveMade(move) {
      this.getGameInfo(this.$route.params.game_uid);
      console.log("Move performed:", move)

      if (this.game_object.isMoveValid(this.game_state, move)) {
        await this.sendMove(move);
      }
    },

    leaveGame() {
      console.log("Game left")
      this.$router.push({path: "/"})
    },

    async joinGame(pid) {
      this.player_id = pid
      await this.$apollo.mutate({
        mutation: JOIN_GAME_MUTATION,
        variables: {
          game_id: this.$route.params.game_uid,
          player_id: pid
        },
      }).then((response) =>  {
          console.log(response)
          this.player_token = response.data.joinGame

          if (this.player_token == null) {
            this.$refs["joinError"].show();
          } else if (this.player_token == 0) {
            this.$refs["placeTakenError"].show();
          }
        }
      ).catch((response) => {
          console.log("Join game error", response)
        }
      )
    },

    async getGameInfo() {
      await this.$apollo.query({
        query: GET_GAME_INFO_QUERY,
        variables: {
          game_id: this.$route.params.game_uid,
        },
      }).then((response) =>  {
          this.updateGame(response.data.gameInfo)
        }
      ).catch((response) => {
          console.error("Get info error", response)
        }
      )
    },
    async sendMove(move) {
      await this.$apollo.mutate({
        mutation: ADD_EVENT_MUTATION,
        variables: {
          game_id: this.$route.params.game_uid,
          player_token: this.player_token,
          event: JSON.stringify(this.game_object.makeMoveEvent(move)),
        },
      }).then((response) =>  {
          console.log(response)
          if (response.data.addEvent == false) {
            console.log("Bad Move")
          } else {
            console.log("Good Move")
          }
        }
      ).catch((response) => {
          console.log("Add event error", response)
        }
      )
    },

    updateGame(game_info) {
      this.game_name = game_info.game_name
      // Update game object
      for (const game of GameData.games) {
        if (game.getInternalGameName() == this.game_name) {
          this.game_object = game
          this.game_component = game.getInterfaceComponent()
          break
        }
      }
      
      // --- Update state ---
      // Position
      this.game_state = this.game_object.updateGameState(JSON.parse(game_info.state))
      console.log("Update:", game_info, this.game_state);
      this.$refs["game_instance"].setState(this.game_state, this.player_id)

      // Players
      this.players_joined = game_info.players_joined

      this.is_ready = game_info.is_ready

      // Block interface
      if (this.game_state.current_player == this.player_id && 
          this.is_ready && !this.is_finished) {
        this.$refs["game_instance"].setIsActive(true)
      } else {
        this.$refs["game_instance"].setIsActive(false)
      }

      if (!this.is_finished && game_info.is_finished) {
        this.is_finished = true
        
        if (game_info.winner == this.player_id) {
          this.gameEndVictory()
        } else if (game_info.winner == "") {
          this.gameEndDraw()
        } else {
          this.gameEndDefeat()
        }
      }
    },

    gameEndVictory() {
      this.$refs["victoryModal"].show();
    },

    gameEndDraw() {
      this.$refs["drawModal"].show();
    },

    gameEndDefeat() {
      this.$refs["defeatModal"].show();
    },

    async subscribeGame(next_callback) {
      await this.$apollo.subscribe( {
        query: GAME_SUBSCRIPTION,
        variables: {
          game_id: this.$route.params.game_uid,
        },
      }).subscribe({
        next(result) {
          next_callback(result.data.subcribeGame)
        },
        error(err) {
          console.log("Subscription error", err)
        }
      })
    },

  },
}
</script>
