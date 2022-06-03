<template>
  <div class="container mt-4">
    <b-card><b-row>
      <b-col sm="9">
        <p>
          {{$t('message.JoinGame.LobbiesFound')}} {{ lobbies.length }}
        </p> 
      </b-col>
      <b-col sm="3">
          <b-button @click="getLobbies()" variant="success" class="w-100">{{$t('message.JoinGame.Refresh')}}</b-button>
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
            {{$t('message.JoinGame.PlayersJoined')}}: {{ lobby.players_joined.length }} 
          </b-col>
          <b-col sm="4">
            {{ makeGameStateString(lobby.state, lobby.game_name) }}
          </b-col>
          <b-col sm="3">
            <b-button 
              @click="joinLobby(lobby.id)"
              variant="primary"
              class="w-100"
            >
              {{$t('message.JoinGame.Join')}}
            </b-button>
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
import Whistette from "../GameData/Whistette"

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
          fetchPolicy: 'network-only',
          variables: {
            limit: 0,
          },
        }).then((response) =>  {
            this.$set(this, "lobbies", response.data.getPublicGames)
          }
        ).catch((response) => {
            console.error("Get public games error:", response)
          }
        )
      },

      joinLobby(game_id) {
        this.$router.push({path: "play/" + game_id})
      },

      makeGameStateString(string_state, game_name) {
        const game_state = JSON.parse(string_state)
        if (game_name == Seki.getInternalGameName()) {
          return this.$t('message.JoinGame.SekiStateString', { height: game_state.Height, width: game_state.Width } )
        } else if (game_name == SingleSuit.getInternalGameName()) {
          return this.$t('message.JoinGame.SingleSuitStateString', 
            { first_size: game_state.FirstPlayerSet.length, second_size: game_state.SecondPlayerSet.length } )
        } else if (game_name == Whistette.getInternalGameName()) {
          return this.$t('message.JoinGame.WhistetteStateString', 
            { size: game_state.FirstPlayerSet.length } )
        }
      }
    },
};
</script>