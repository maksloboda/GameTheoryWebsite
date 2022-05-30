<template>
  <div class="container mt-2">
    <b-row>
      <b-col sm="10">
        <p>
          {{$t('message.JoinGame.LobbiesFound')}} {{ lobbies.length }}
        </p> 
      </b-col>
      <b-col sm="2">
          <b-button @click="getLobbies()" variant="success">{{$t('message.JoinGame.Refresh')}}</b-button>
      </b-col>
    </b-row>
    <b-card v-for="lobby in lobbies">  
      <b-row>
        <b-col sm="2">
          <p class="font-weight-bold">
            {{ lobby.game_name }}
          </p>
        </b-col>
        <b-col sm="2">
          {{ lobby.game_mode }}
        </b-col>
        <b-col sm="6">
          {{ lobby.comment }}
        </b-col>
        <b-col sm="2">
          <b-button @click="joinLobby(lobby.game_id)" variant="primary">{{$t('message.JoinGame.Join')}}</b-button>
        </b-col>
      </b-row>
    </b-card>
  </div>
</template>

<script>

export default {
    methods: {
      getLobbies() {
      },
      joinLobby(game_id) {
        this.$router.push({path: "play/" + game_id})
      }
    },
    data() {
      return {
          lobbies: [ 
            {
              game_name: "Seki",
              game_mode: "4x4",
              comment: "Sample lobby #1",
              game_id: "4444-4444-4444-44444"
            },
            {
              game_name: "Seki",
              game_mode: "1x44",
              comment: "Sample lobby #2",
              game_id: "3333-3333-3333-33333"
            }
          ]
        }
    },
};
</script>