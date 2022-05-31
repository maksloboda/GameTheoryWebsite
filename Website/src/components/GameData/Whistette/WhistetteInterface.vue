<template>
  <b-container>
    <div v-if="game_state === null">
      <center>
        {{$t('message.SingleSuitInterface.NullGameState')}}
      </center>
    </div>
    <div v-else>
      <div class="container mt-2">
        <b-card class="text-center" :title="$t('message.SingleSuitInterface.GameType')">
          <div class="mt-2"> 
            <b-row class="justify-content-md-center">
              <b-col md="auto" v-for="c in game_state.first_player_set">
                <b-button 
                  class="card-button"
                  :disabled="!is_active || game_state.current_player == 'B'"
                  @click="makeMoveInterface({type:'move', card: c})"
                >{{ c }}</b-button>
              </b-col>
            </b-row>

            <br>
            <br>
            <br>
            <br>

            <b-row class="justify-content-md-center">
              <b-button class="card-button bg-secondary" variant="secondary"
                v-if="null != game_state.last_card"
                @click="makeMoveInterface({type:'move', card: c})"
                >
                  {{game_state.last_card}}
              </b-button>
            </b-row>
            <br>
            <br>
            <br>
            <br>

            <b-row class="justify-content-md-center">
              <b-col md="auto" v-for="c in game_state.second_player_set">
                <b-button 
                  class="card-button"
                  :disabled="!is_active || game_state.current_player == 'A'" 
                  @click="makeMoveInterface({type:'move', card: c})"
                >{{ c }}</b-button>
              </b-col>
            </b-row>

            <br>
            <br>
            <br>

            <div class="justify-content-md-center">
              Player A score: {{game_state.first_score}}
            </div>

            <div class="justify-content-md-center">
              Player B score: {{game_state.second_score}}
            </div>
          </div>  
        </b-card>
      </div>
    </div>
  </b-container>
</template>

<style scoped>
  .card-button {
   width:55px;
   height:80px;
  }
</style>

<script>

import Whistette from "./"

export default {
  mounted() {
    this.$emit("component_ready")
  },
  data() {
    return {
      game_state: null,
      is_active: false,
      game_type: "whistette",
      game_name: "Whistette",
    }
  },
  methods: {
    setState(state, game_type, player_id) {
      this.game_state = state
      this.game_type = game_type
      this.game_name = Whistette.getInterfaceGameName(game_type)
    },
    setIsActive(is_active) {
      this.is_active = is_active
    },
    makeMoveInterface(move) {
      this.$emit("move", move);
    },
    applyMoveInterface(move) {
      // this.game_state = Whistette.applyMove(this.game_state, move);
    }
  }
}
</script>
