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
            <b-row class="my-1">
              <b-col v-for="c in game_state.first_player_set">
                <b-button 
                  :disabled="!is_active" 
                  @click="makeMoveInterface({type:'move', card: c})"
                >{{ c }}</b-button>
              </b-col>
            </b-row>


            <b-row class="my-1">
              <b-col v-for="c in game_state.second_player_set">
                <b-button 
                  :disabled="!is_active" 
                  @click="makeMoveInterface({type:'move', card: c})"
                >{{ c }}</b-button>
              </b-col>
            </b-row>
          </div>  
        </b-card>
      </div>
    </div>
  </b-container>
</template>


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
