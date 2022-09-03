<template>
  <b-container>
    <div v-if="game_state === null">
      <center>
        {{ $t('message.SingleSuitInterface.NullGameState') }}
      </center>
    </div>
    <div v-else>
      <div class="container mt-2">
        <b-card class="text-center" :title="$t('message.WhistetteInterface.GameType')">
          <div class="mt-2">
            <b-row class="justify-content-md-center" style="min-height: 80px">
              <b-col md="auto" v-for="c in game_state.first_player_set">
                <b-button class="card-button" :disabled="!is_active || game_state.current_player == 'B'"
                  @click="makeMoveInterface({ type: 'move', card: c })">{{ c }}</b-button>
              </b-col>
            </b-row>

            <br>
            <br>
            <br>
            <br>

            <b-row class="justify-content-md-center" style="min-height: 80px">
              <b-button class="card-button bg-secondary" variant="secondary" v-if="null != game_state.last_card"
                @click="makeMoveInterface({ type: 'move', card: c })">
                {{ game_state.last_card }}
              </b-button>
            </b-row>
            <br>
            <br>
            <br>
            <br>

            <b-row class="justify-content-md-center" style="min-height: 80px">
              <b-col md="auto" v-for="c in game_state.second_player_set">
                <b-button class="card-button" :disabled="!is_active || game_state.current_player == 'A'"
                  @click="makeMoveInterface({ type: 'move', card: c })">{{ c }}</b-button>
              </b-col>
            </b-row>

            <br>

            <b-card v-if="weighted_game">
              <b-row class="justify-content-md-center">
                <b-col md="auto">
                  <b>{{ $t('message.SingleSuitSettings.Weights') }}:</b>
                </b-col>
                <b-col md="auto" v-for="(weight, index) in weights">
                  <div v-if="current_bribe_number == index">
                    <b>{{ weight }}</b>
                  </div>
                  <div v-else>
                    {{ weight }}
                  </div>
                </b-col>
              </b-row>
            </b-card>

            <br>

            <div class="justify-content-md-center">
              {{ $t('message.Whistette.PlayerAScore') }}: {{ game_state.first_score }}
            </div>

            <div class="justify-content-md-center">
              {{ $t('message.Whistette.PlayerBScore') }}: {{ game_state.second_score }}
            </div>
          </div>
        </b-card>
      </div>
    </div>
  </b-container>
</template>

<style scoped>
.card-button {
  width: 55px;
  height: 80px;
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
      weights: null,
      current_bribe_number: 0,
    }
  },
  computed: {
    weighted_game() {
      return this.game_state.weights != null && this.game_state.weights.length > 0
    },
  },
  methods: {
    setState(state, game_type, player_id) {
      this.game_state = state
      this.game_type = game_type
      this.game_name = Whistette.getInterfaceGameName(game_type)

      if (this.weighted_game && this.weights == null) {
        this.weights = this.game_state.weights.slice(0, Math.ceil(this.game_state.weights.length / 2))
      }

      if (this.weighted_game) {
        this.updateCurrentBribeNumber();
      }
    },
    setIsActive(is_active) {
      this.is_active = is_active
    },
    makeMoveInterface(move) {
      this.$emit("move", move);
    },
    updateCurrentBribeNumber() {
      for (let i = 0; i < this.weights.length; i++) {
        if (this.game_state.weights[0] == this.weights[i]) {
          this.current_bribe_number = i;
          break;
        }
      }
    },
    applyMoveInterface(move) {
      // this.game_state = Whistette.applyMove(this.game_state, move);
    }
  }
}
</script>
