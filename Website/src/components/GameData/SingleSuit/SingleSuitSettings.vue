<template>
  <div>
    <b-form>
      <b-row>
        <b-col>
          <b-form-group 
            id="cards_number"
            :label="$t('message.SingleSuitSettings.CardNumber')"
          >            
              <b-form-input
                v-model="cards_number"
                id="cards-number"
                type="number"
                min="1"
                max="100"
              >
              </b-form-input>
          </b-form-group>
        </b-col>
        <b-col>
          <b-form-group :label="$t('message.SekiSettings.GameType')">
            <b-form-radio-group
              id="game-type-radio"
              v-model="game_type"
            >
            <b-form-radio value="singlesuit">{{$t('message.SingleSuitSettings.Fool')}}</b-form-radio>
            <b-form-radio value="d-singlesuit">{{$t('message.SingleSuitSettings.DFool')}}</b-form-radio>
          </b-form-radio-group>
          </b-form-group>
        </b-col>
        <b-col>
          <b-form-group :label="$t('message.SingleSuitSettings.FirstPlayer')">
            <b-form-radio-group
              id="first-player-radio"
              v-model="first_player"
            >
            <b-form-radio :value="FIRST_PLAYER_ID">A</b-form-radio>
            <b-form-radio :value="SECOND_PLAYER_ID">B</b-form-radio>
          </b-form-radio-group>
          </b-form-group>
        </b-col>
      </b-row>
      <b-card>
        <b-col v-for="i in Array(minMax(cards_number, 1, 100)).keys()">
            <b-form-group>
              <b-form-radio-group
                v-model="card_array[i]"
              >
                <b-form-radio :value="0">A</b-form-radio>
                {{i + 1}}{{$t('message.SingleSuitSettings.NthCard')}}
                <b-form-radio :value="1">B</b-form-radio>
              </b-form-radio-group>
            </b-form-group>
        </b-col>
      </b-card>
    </b-form>
  </div>
</template>

<style>
select.form-control {
  -moz-appearance: none;
   -webkit-appearance: none;
   appearance: none;
}
</style>

<script>
import {
  FIRST_PLAYER_ID,
  SECOND_PLAYER_ID
} from "."


export default {
  data() {
    return {
      FIRST_PLAYER_ID: FIRST_PLAYER_ID,
      SECOND_PLAYER_ID: SECOND_PLAYER_ID,
      cards_number: 8,
      first_player: FIRST_PLAYER_ID,
      game_type: "singlesuit",
      card_array: Array(100).fill(1)
    }
  },
  methods: {
    getSettings() {
      let cnt = this.minMax(this.cards_number, 1, 100)
      let f = []
      let s = []
      for (let i = 0; i < cnt; ++i) {
        if (parseInt(this.card_array[i]) == 0) {
          f.push(i + 1)
        } else {
          s.push(i + 1)
        }
      }
      return {
          game_type: this.game_type,
          cards_number: cnt,
          first_player_array: f,
          second_player_array: s
        };
    },
    minMax(val, min, max) {
      if (parseInt(val) < min) { return min; }
      if (parseInt(val) > max) { return max; }
      return parseInt(val)
    }
  }
}
</script>
