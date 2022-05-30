<template>
  <b-container v-if="game_state != null">
    <div class="container mt-2">
      <b-card class="text-center" :title="$t('message.SingleSuitInterface.GameType')">
        <div class="bg-secondary mt-2">
          <b-row class="my-1" v-for="row in game_state.field">
            <b-col v-for="cell in row" sm="1">
              <b-button 
                :disabled="!is_active" 
                @click="makeMoveInterface({type:'dec', pos:cell.pos})"
              >{{ cell.value }}</b-button>
            </b-col>
          </b-row>
        </div>
        <b-button 
          :disabled="!is_active" 
          @click="makeMoveInterface({type:'pass', pos:{x: 0, y:0}})" 
          variant="danger"
        >{{$t('message.SekiInterface.Pass')}}</b-button>
      </b-card>
    </div>
  </b-container>
  <b-container v-else>
    <center>
      {{$t('message.SingleSuitInterface.NullGameState')}}
    </center>
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
