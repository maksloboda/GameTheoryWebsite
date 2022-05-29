<template>
  <b-container>
    <b-card><b-row><b-col sm="10">
      <b-div
        ref="game_status"
      >
        <div v-if="!is_ready">
          Awaiting players...
        </div>
        <div v-else-if="!is_finished">
          <div v-if="game_state.current_player == player_id">
            {{ game_state.current_player }} is current player - Your turn
          </div>
          <div v-else>
            {{ game_state.current_player }} is current player - Opponent's turn
          </div>
        </div>
        <div v-else-if="is_finished">
          <div v-if="winner == ''">
            Draw
          </div>
          <div v-else>
            {{ winner }} is winner
          </div>
        </div>
      </b-div></b-col>
      <b-col>
        <b-button
          variant="outline-primary"
          class="w-100"
          @click="copyURL"
        >
          Copy link
        </b-button>
      </b-col>
    </b-row></b-card>
    
    <b-row 
        v-if="this.players_joined.length != 2 && 
              (this.player_token == null || this.player_token == 0 )" 
      >
      <b-col><b-button 
        class="w-100"
        @click="joinGame(game_object.first_player_id)" 
        variant="warning"
        ref="join_first_button"
        :disabled="players_joined.includes(game_object.first_player_id)"
      >
        <b> Join as {{game_object.first_player_id}} </b>
        <span v-if="game_state.current_player == game_object.first_player_id"> - First move</span>
        <span v-if="game_state.pass_options[game_object.first_player_id]"> - Can pass</span>
      </b-button></b-col>
      <b-col><b-button 
        class="w-100"
        @click="joinGame(game_object.second_player_id)" 
        variant="warning"
        ref="join_second_button" 
        :disabled="players_joined.includes(game_object.second_player_id)"
      >
        <b> Join as {{game_object.second_player_id}} </b>
        <span v-if="game_state.current_player == game_object.second_player_id"> - First move</span>
        <span v-if="game_state.pass_options[game_object.second_player_id]"> - Can pass</span>
      </b-button></b-col>
    </b-row>

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
      <br>
      <b-card>
        <b-button sm="6" @click="leaveGame" variant="outline-danger">Leave game</b-button>
      </b-card>
    </div>
    
    <b-modal ref="placeTakenError">
      This player ID is already taken!
    </b-modal>

    <b-modal ref="joinError">
      Join error
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
      game_name: "", // Internal game name
      game_type: "seki",

      player_token: null,
      player_id: "",
      players_joined: [],
      current_player: "",

      is_finished: false,
      is_ready: false,
      winner: "",
    }
  },

  methods: {
    /**
     * Called when interface is loaded and used for initialization
     */

    onGameComponentSpawned() {
      this.$refs["game_instance"].setState(this.game_state, this.game_type, this.player_id)
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

    copyURL() {
      try {
        navigator.clipboard.writeText(window.location.href);
      } catch (err) {
        alert('Unable to copy link, try manually');
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
            console.log("Illegal Move")
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
      this.game_type = this.game_state.game_type
      console.log("Update:", game_info, this.game_state, this.player_id);
      this.$refs["game_instance"].setState(this.game_state, this.game_type, this.player_id)

      // Players
      this.players_joined = game_info.players_joined

      this.is_ready = game_info.is_ready

      if (!this.is_finished && game_info.is_finished) {
        this.is_finished = true
        this.winner = game_info.winner
      }

      // Block interface
      if (this.game_state.current_player == this.player_id && 
          this.is_ready && !this.is_finished) {
        this.$refs["game_instance"].setIsActive(true)
      } else {
        this.$refs["game_instance"].setIsActive(false)
      }
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
