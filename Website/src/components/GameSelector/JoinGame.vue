<template>
  <div class="container mt-2">
    <b-card><b-row>
      <b-col sm="9">
        <p>
          {{$t('message.JoinGame.LobbiesFound')}} {{ lobbies.length }}
        </p> 
      </b-col>
      <b-col sm="3">
          <b-button @click="getLobbies()" variant="success">{{$t('message.JoinGame.Refresh')}}</b-button>
      </b-col>
    </b-row></b-card>
    <br>
    <div id="lobbies-list" class="flex-column">
      <b-card v-for="lobby in lobbies">  
        <b-row>
          <b-col sm="2">
            <p class="font-weight-bold">
              <b>{{ lobby.game_name }}</b>
            </p>
          </b-col>
          <b-col sm="3">
            players joined: {{ lobby.players_joined.length }} 
          </b-col>
          <b-col sm="5">
            {{ getGameStateString(lobby.state, lobby.game_name) }}
          </b-col>
          <b-col sm="2">
            <b-button @click="joinLobby(lobby.game_id)" variant="primary">{{$t('message.JoinGame.Join')}}</b-button>
          </b-col>
        </b-row>
      </b-card>
    </div>
  </div>
</template>

<style>
  #lobbies-list {
    max-height: 400px;
    overflow:auto;
  }
</style>

<script>

import {
  GET_PUBLIC_GAMES_QUERY
} from "../../constants/graphql"

import Seki from "../GameData/Seki"
import SingleSuit from "../GameData/SingleSuit"

export default {
    data() {
      return {
          lobbies: []
        }
    },
    async mounted() {
      await this.getLobbies()
    },
    methods: {
      async getLobbies() {
        await this.$apollo.query({
          query: GET_PUBLIC_GAMES_QUERY,
          variables: {
            limit: 0,
          },
        }).then((response) =>  {
            this.lobbies = response.data.getPublicGames
          }
        ).catch((response) => {
            console.error("Get public games error:", response)
          }
        )
      },

      joinLobby(game_id) {
        this.$router.push({path: "play/" + game_id})
      },

      getGameStateString(string_state, game_name) {
        const game_state = JSON.parse(string_state)
        if (game_name == Seki.getInternalGameName()) {
          return `Field: ${game_state.Height}x${game_state.Width}`
        } else if (game_name == SingleSuit.getInternalGameName()) {
          return `First player has ${game_state.FirstPlayerSet.length} cards, Second player has ${game_state.SecondPlayerSet.length} cards`
        }
      }
    },
};
</script>