<template>
  <b-container>
    <b-card><b-row><b-col sm="10">
      <b-div
        ref="game_status"
      >
        <div v-if="!is_ready">
          {{$t('message.PlayGame.WaitPlayers')}}
        </div>
        <div v-else-if="!is_finished">
          <div v-if="current_player == player_id">
            {{ current_player }} {{$t('message.PlayGame.YourTurn')}}
          </div>
          <div v-else>
            {{ current_player }} {{$t('message.PlayGame.OpponentsTurn')}}
          </div>
        </div>
        <div v-else-if="is_finished">
          <div v-if="winner == ''">
            {{$t('message.PlayGame.Draw')}}
          </div>
          <div v-else>
            {{ winner }} {{$t('message.PlayGame.IsWinner')}}
          </div>
        </div>
      </b-div></b-col>
      <b-col>
        <b-button
          variant="outline-primary"
          class="w-100"
          @click="copyURL"
        >
          {{$t('message.PlayGame.CopyLink')}}
        </b-button>
      </b-col>
    </b-row></b-card>
    <b-card
        class="mt-2" 
        v-if="!client_joined && !is_ready"
      ><b-row>
      <b-form-group><b-form-radio-group
        class="w-100"
        button-variant="outline-primary"
        id="btn-radios-1"
        v-model="game_mode"
        :options="game_mode_options"
        buttons
      ></b-form-radio-group></b-form-group>
    </b-row>
    <br>
    <div>
      <b-row v-if="game_mode != MODE_SPECTATE">
        <b-col sm="6"><b-button 
          class="w-100"
          @click="joinLobby(FIRST_PLAYER_ID)" 
          variant="warning"
          ref="join_first_button"
          :disabled="players_joined.includes(FIRST_PLAYER_ID)"
        >
          <b> {{$t('message.PlayGame.JoinAs')}} {{FIRST_PLAYER_ID}} </b>
          <span v-if="current_player == FIRST_PLAYER_ID"> - {{$t('message.PlayGame.FirstMove')}}</span>
          <span v-if="can_first_pass"> - {{$t('message.PlayGame.CanPass')}}</span>
        </b-button></b-col>
        <b-col sm="6"><b-button 
          class="w-100"
          @click="joinLobby(SECOND_PLAYER_ID)" 
          variant="warning"
          ref="join_second_button" 
          :disabled="players_joined.includes(SECOND_PLAYER_ID)"
        >
          <b> {{$t('message.PlayGame.JoinAs')}} {{SECOND_PLAYER_ID}} </b>
          <span v-if="current_player == SECOND_PLAYER_ID"> - {{$t('message.PlayGame.FirstMove')}}</span>
          <span v-if="can_second_pass"> - {{$t('message.PlayGame.CanPass')}}</span>
        </b-button></b-col>
      </b-row>
      <b-button 
          v-else
          class="w-100"
          @click="joinLobby(null)" 
          variant="warning"
          ref="join_first_button"
          :disabled="players_joined.includes(FIRST_PLAYER_ID)"
        >
          <b>{{$t('message.PlayGame.Start')}} </b>
          <span> <b>{{current_player}}</b> {{$t('message.PlayGame.FirstMove')}}</span>
          <span v-if="can_first_pass">, <b>{{FIRST_PLAYER_ID}}</b> {{$t('message.PlayGame.CanPass')}}</span>
          <span v-if="can_second_pass">, <b>{{SECOND_PLAYER_ID}}</b> {{$t('message.PlayGame.CanPass')}}</span>
        </b-button>
    </div>
    </b-card>
    <div v-if="this.$apollo.loading">
      {{$t('message.PlayGame.Loading')}}
    </div>
    <div v-else>
      <b-row class="mt-2"><b-col sm="9"><component
        :is="game_component"
        ref="game_instance"
        @component_ready="onGameComponentSpawned"
        @move="onMoveMade"
      ></component></b-col>
      <b-col sm="3" class="flex-grow-1" v-if="!unlimited_time">
        <b-card>
          <b>{{$t('message.TimeLeft')}}:</b> {{ time_left }} <br>
          <b>{{$t('message.TimeLimit')}}:</b> {{ time_limit }} 
        </b-card>
      </b-col></b-row>
      
      <b-card v-if="is_ready" class="mt-2">
        <b-button 
          v-if="game_mode == MODE_SPECTATE"
          :disabled="!is_ready ||is_finished || current_player == player_id"
          @click="makeBotMove"
          variant="primary"
        >
          {{$t('message.PlayGame.MakeMove')}}
        </b-button>

        <b-button 
          @click="leaveGame" 
          variant="outline-danger"
        >{{$t('message.PlayGame.LeaveGame')}}</b-button>
      </b-card>
    </div>
  </b-container>
</template>

<script>

import GameData from "./../GameData"

import { 
  GET_GAME_INFO_QUERY,
  FIND_OPTIMAL_MOVE_QUERY,
  JOIN_GAME_MUTATION,
  ADD_EVENT_MUTATION,
  GAME_SUBSCRIPTION,
  ADVANCE_MUTATION,
} from '../../constants/graphql'

import {
  MODE_VS_COMP,
  MODE_VS_HUMAN,
  MODE_SPECTATE,
} from "../../constants/constants"
import { FIRST_PLAYER_ID, SECOND_PLAYER_ID } from "../GameData/Seki";

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
      
      client_joined: false,

      MODE_VS_HUMAN: MODE_VS_HUMAN, 
      MODE_SPECTATE: MODE_SPECTATE, 

      timer_interval: null,
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
    current_token() {
      if (this.game_mode == MODE_SPECTATE) {
        if (this.current_player == this.FIRST_PLAYER_ID) {
          return this.player_tokens[0]
        } else {
          return this.player_tokens[1]
        }
      }
      if (this.player_id == this.FIRST_PLAYER_ID) {
        if (this.current_player == this.FIRST_PLAYER_ID) {
          return this.player_tokens[0]
        } else {
          return this.player_tokens[1]
        }
      } else {
        if (this.current_player == this.FIRST_PLAYER_ID) {
          return this.player_tokens[1]
        } else {
          return this.player_tokens[0]
        }
      }
    },
    
    FIRST_PLAYER_ID() { return this.game_object.FIRST_PLAYER_ID },
    SECOND_PLAYER_ID() { return this.game_object.SECOND_PLAYER_ID },

    game_id() { return this.$route.params.game_uid },

    game_type() { return this.game_state.game_type },
    pass_options() { return this.game_state.pass_options },
    can_first_pass() {
      try {
        return this.game_state.pass_options[FIRST_PLAYER_ID]
      } catch (err) {
        return false
      }
    },
    can_second_pass() {
      try {
        return this.game_state.pass_options[SECOND_PLAYER_ID]
      } catch (err) {
        return false
      }
    },

    game_name() { return this.game_info.game_name },

    players_joined() { return this.game_info.players_joined },
    is_finished() { return this.game_info.is_finished },
    is_ready() { return this.game_info.is_ready },
    winner() { return this.game_info.winner },
    game_mode_options() { return [
        { text: this.$t('message.PlayGame.AgainstHuman'), value: MODE_VS_HUMAN },
        { text: this.$t('message.PlayGame.AgainstComputer'), value: MODE_VS_COMP },
        { text: this.$t('message.PlayGame.Spectator'), value: MODE_SPECTATE }
      ]
    },

    time_left: {
      get() { return this.game_info.time_control.time_left },
      set(time) { 
        this.game_info.time_control.time_left = time; 
      },
    },
    
    unlimited_time() {
      return this.game_info.time_control.time_limit == null
    },

    time_limit() {
      return this.game_info.time_control.time_limit
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
      console.log("Move performed:", move)

      if (this.game_object.isMoveValid(this.game_state, move)) {
        await this.sendMove(move, this.player_token);
      }

      if (this.game_mode == MODE_VS_COMP) {
        await this.makeBotMove();
      }
    },

    async joinLobby(pid) {
      console.log(this.game_mode)
      if (this.game_mode == MODE_SPECTATE) {
        this.player_tokens[0] = await this.joinGame(FIRST_PLAYER_ID)
        this.player_tokens[1] = await this.joinGame(SECOND_PLAYER_ID)
      } else {
        this.player_id = pid
        this.player_tokens[0] = await this.joinGame(pid)
        if (this.player_tokens[0] != 0) {
          this.client_joined = 1
        }
        if (this.game_mode != MODE_VS_HUMAN) {
          let bot_pid = FIRST_PLAYER_ID
          if (pid == FIRST_PLAYER_ID) { 
            bot_pid = SECOND_PLAYER_ID
          }
          this.player_tokens[1] = await this.joinGame(bot_pid)
        }
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
      let player_token = null;
      await this.$apollo.mutate({
        mutation: JOIN_GAME_MUTATION,
        variables: {
          game_id: this.game_id,
          player_id: pid
        },
      }).then((response) =>  {
          player_token = response.data.joinGame
        }
      ).catch((response) => {
          console.log("Join game error", response)
        }
      )
      return player_token
    },

    async getGameInfo() {
      await this.$apollo.query({
        query: GET_GAME_INFO_QUERY,
        fetchPolicy: 'network-only',
        variables: {
          game_id: this.game_id,
        },
      }).then((response) =>  {
          this.updateGame(response.data.gameInfo)
        }
      ).catch((response) => {
          console.error("Get info error", response)
        }
      )
    },

    async sendMove(move, player_token) {
      await this.$apollo.mutate({
        mutation: ADD_EVENT_MUTATION,
        variables: {
          game_id: this.game_id,
          player_token: player_token,
          event: JSON.stringify(this.game_object.makeMoveEvent(move)),
        },
      }).then((response) =>  {
          if (response.data.addEvent == false) {
            console.log("Illegal Move")
          }
        }
      ).catch((response) => {
          console.log("Add event error", response)
        }
      )
    },

    startTimer() {
      this.timer_interval = setInterval(function() {
        if (this.time_left > 0) {
          this.time_left = this.time_left - 1;
        }
        if (this.time_left == 0) {
          this.advance()
        }
      }.bind(this), 1000);
    },

    advance() {
      if (this.is_finished) {
        clearInterval(this.timer_interval);
        return;
      }
      this.$apollo.mutate({
        mutation: ADVANCE_MUTATION,
        variables: {
          game_id: this.game_id,
        },
      })
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
      this.$refs["game_instance"].setState(this.game_state, this.game_type, this.player_id)

      // Players

      console.log("Update game", this.game_info, this.current_player, this.player_id, this.is_ready, this.is_finished)
      // Block interface
      if (this.current_player == this.player_id && 
          this.is_ready && !this.is_finished) {
        this.$refs["game_instance"].setIsActive(true)
      } else {
        this.$refs["game_instance"].setIsActive(false)
      }


      if (this.is_finished) {
        clearInterval(this.timer_interval);
      } else if (this.is_ready && !this.timer_interval) {
        this.startTimer()
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

    async makeBotMove() {
      const best_move = await this.findOptimalMove();
      console.log("makeBotMove:", best_move, this.current_token)
      await this.sendMove(best_move, this.current_token)
    },

    async findOptimalMove() {
      let optimal_move = null
      await this.$apollo.query( {
        query: FIND_OPTIMAL_MOVE_QUERY,
        fetchPolicy: 'network-only',
        variables: {
          game_id: this.game_id,
        },
      }).then((response) =>  {
          console.log(response.data.findOptimalMove)
          optimal_move = this.game_object.makeMoveFromEvent(JSON.parse(response.data.findOptimalMove))
        }
      ).catch((response) => {
          console.error("Find optimal move error:", response)
        }
      )
      return optimal_move
    }
  },
}
</script>
