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
          <div v-if="current_player == player_id">
            {{ current_player }} is current player - Your turn
          </div>
          <div v-else>
            {{ current_player }} is current player - Opponent's turn
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
        v-if="players_joined.length != 2 && 
              (player_token == null || player_token == 0 )"
        class="mt-2" 
      >
      <b-col sm="6"><b-button 
        class="w-100"
        @click="joinGame(FIRST_PLAYER_ID)" 
        variant="warning"
        ref="join_first_button"
        :disabled="players_joined.includes(FIRST_PLAYER_ID)"
      >
        <b> Join as {{FIRST_PLAYER_ID}} </b>
        <span v-if="current_player == FIRST_PLAYER_ID"> - First move</span>
        <span v-if="pass_options[FIRST_PLAYER_ID]"> - Can pass</span>
      </b-button></b-col>
      <b-col sm="6"><b-button 
        class="w-100"
        @click="joinGame(SECOND_PLAYER_ID)" 
        variant="warning"
        ref="join_second_button" 
        :disabled="players_joined.includes(SECOND_PLAYER_ID)"
      >
        <b> Join as {{SECOND_PLAYER_ID}} </b>
        <span v-if="current_player == SECOND_PLAYER_ID"> - First move</span>
        <span v-if="pass_options[SECOND_PLAYER_ID]"> - Can pass</span>
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
        <b-button 
          v-if="is_ready && game_mode != MODE_VS_HUMAN"
          :disabled="current_player"
          @click="leaveGame"
          variant="outline-danger"
        >Make move</b-button>
        <b-button 
          @click="leaveGame" 
          variant="outline-danger"
        >Leave game</b-button>
      </b-card>
    </div>
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

import {
  MODE_VS_COMP,
  MODE_VS_HUMAN,
  MODE_SPECTATE,
} from "../../constants/constants"

export default {
  async mounted() {
    await this.getGameInfo();
    await this.subscribeGame(this.updateGame);
  },

  data() {
    return {
      game_state: null,
      game_object: null,
      game_component: null,
      game_info: {
        is_ready: false,
        is_finished: false,
      },
      game_mode: MODE_VS_HUMAN, // vs human/comp/spectate

      player_tokens: [null, null], // vs human - one token, other modes - two
      player_id: "",

      MODE_VS_HUMAN: MODE_VS_HUMAN, 
    }
  },

  computed: {
    player_token: { 
      get() { return this.player_tokens[0]; },
      set(token) { 
        this.player_tokens[0] = token; 
      },
    },
    bot_token: { 
      get() { return this.player_tokens[1]; },
      set(token) { this.player_tokens[1] = token; },
    },
    current_player() { return this.game_state.current_player },
    
    FIRST_PLAYER_ID() { return this.game_object.FIRST_PLAYER_ID },
    SECOND_PLAYER_ID() { return this.game_object.SECOND_PLAYER_ID },

    game_id() { return this.$route.params.game_uid },

    game_type() { return this.game_state.game_type },
    pass_options() { return this.game_state.pass_options },

    game_name() { return this.game_info.game_name },

    players_joined() { return this.game_info.players_joined },
    is_finished() { return this.game_info.is_finished },
    is_ready() { return this.game_info.is_ready },
    winner() { return this.game_info.winner },
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
      this.getGameInfo(this.game_id);
      console.log("Move performed:", move)

      if (this.game_object.isMoveValid(this.game_state, move)) {
        await this.sendMove(move, 0);
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

    async joinGame(pid, game_mode) {
      this.player_id = pid

      await this.$apollo.mutate({
        mutation: JOIN_GAME_MUTATION,
        variables: {
          game_id: this.game_id,
          player_id: pid
        },
      }).then((response) =>  {
          console.log(response)
          this.player_token = response.data.joinGame
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
          game_id: this.game_id,
        },
      }).then((response) =>  {
          console.warn("Get game Info", response.data.gameInfo)
          this.updateGame(response.data.gameInfo)
        }
      ).catch((response) => {
          console.error("Get info error", response)
        }
      )
    },

    async sendMove(move, turn) {
      await this.$apollo.mutate({
        mutation: ADD_EVENT_MUTATION,
        variables: {
          game_id: this.game_id,
          player_token: this.player_tokens[turn],
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
      this.game_info = game_info

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
      console.log("Update:", game_info, this.game_state, this.player_id);
      this.$refs["game_instance"].setState(this.game_state, this.game_type, this.player_id)

      // Players

      // Block interface
      if (this.current_player == this.player_id && 
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
          game_id: this.game_id,
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

    async findOptimalMove() {
      await this.$apollo.query( {
        query: FIND_OPTIMAL_MOVE_QUERY,
        variables: {
          game_id: this.game_id,
        },
      }).then((response) =>  {
          return this.game_object.makeMoveFromEvent(JSON.parse(response.data.findOptimalMove))
        }
      ).catch((response) => {
          console.log("Add event error", response)
        }
      )
    }
  },
}
</script>
