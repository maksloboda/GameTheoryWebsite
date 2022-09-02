<template>
  <b-container>
    <div v-if="game_state === null">
      <center>
        {{$t('message.SingleSuitInterface.NullGameState')}}
      </center>
    </div>
    <div v-else>
      <div class="container mt-2">
        <b-card class="text-center" :title="subgameName()">
          <div class="mt-2"> 
            <b-row class="justify-content-md-center" style="height: 80px">
              <b-col md="auto" v-for="c in game_state.first_player_set">
                <b-button 
                  class="card-button"
                  :disabled="!is_active || game_state.current_player == 'B' || (game_state.last_card != null && c < game_state.last_card)"
                  @click="makeMoveInterface({type:'move', card: c, do_take:false})"
                >{{ c }}</b-button>
              </b-col>
            </b-row>

            <br>
            <br>
            <br>
            <br>

            <b-row class="justify-content-md-center" style="height: 80px">
              <b-button class="card-button bg-secondary" variant="secondary"
                v-if="null != game_state.last_card"
                @click="makeMoveInterface({type:'move', card: game_state.last_card, do_take:true})"
                >
                  {{game_state.last_card}}
              </b-button>
            </b-row>
            <br>
            <br>
            <br>
            <br>

            <b-row class="justify-content-md-center" style="height: 80px">
              <b-col md="auto" v-for="c in game_state.second_player_set">
                <b-button 
                  class="card-button"
                  :disabled="!is_active || game_state.current_player == 'A' || (game_state.last_card != null && c < game_state.last_card)" 
                  @click="makeMoveInterface({type:'move', card: c, do_take:false})"
                >{{ c }}</b-button>
              </b-col>
            </b-row>
            <button>AAAAA</button>
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

import SingleSuit from "./"

export default {
  mounted() {
    this.$emit("component_ready")
  },
  data() {
    return {
      game_state: null,
      is_active: false,
      game_type: "singlesuit",
      game_name: "SingleSuit",
    }
  },
  methods: {
    subgameName() {
      if (this.game_state.game_type == "d-singlesuit") {
        return this.$t('message.SingleSuitInterface.DFool')
      } else {
        return this.$t('message.SingleSuitInterface.Fool')
      }
    },
    setState(state, game_type, player_id) {
      this.game_state = state
      this.game_type = game_type
      this.game_name = SingleSuit.getInterfaceGameName(game_type)
    },
    setIsActive(is_active) {
      this.is_active = is_active
    },
    makeMoveInterface(move) {
      this.$emit("move", move);
    },
    applyMoveInterface(move) {
      // this.game_state = SingleSuit.applyMove(this.game_state, move);
    }
  }
}
</script>
