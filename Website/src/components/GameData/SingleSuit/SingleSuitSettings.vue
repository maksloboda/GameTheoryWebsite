<template>
  <div>
    <b-form>
      <b-row>
        <b-col sm="4">
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
        <b-col sm="4">
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
        <b-col sm="4">
          <b-form-group :label="$t('message.SingleSuitSettings.FirstPlayer')">
            <b-radio-group
              id="first-player-radio"
              v-model="first_player"
            >
              <b-radio :value="FIRST_PLAYER_ID">A</b-radio>
              <b-radio :value="SECOND_PLAYER_ID">B</b-radio>
            </b-radio-group>
          </b-form-group>
        </b-col>
      </b-row>

      <br>

      <b-row>
        <b-col sm="4">
          <label for="time_limit">{{$t('message.MoveTimeLimit')}}</label>
        </b-col>
        <b-col sm="2">
          <b-form-input
            id="time_limit"
            v-model="time_limit"
            :value="5"
            :disabled="unlimited_time"
            type="number"
            :min="1"
            :max="300"
          >
          </b-form-input>
        </b-col>
        <b-col sm="6">
          <b-form-checkbox
            v-model="unlimited_time"
            :value="true"
            :unchecked-value="false"
          >
            {{$t('message.UnlimitedTime')}}
          </b-form-checkbox>
        </b-col>
      </b-row>

      <br>
      <b-card>
        <b-row v-for="i in Array(minMax(cards_number, 1, 100)).keys()">
            <b-col sm="3">
              {{i + 1}}{{$t('message.SingleSuitSettings.NthCard')}}
            </b-col>
            <b-col sm="9">
              <b-form-group>
                <b-form-radio-group
                  v-model="card_array[i]"
                  buttons
                  class="w-100"
                >
                  <b-form-radio :value="0">A</b-form-radio>
                  <b-form-radio :value="1">B</b-form-radio>
                </b-form-radio-group>
              </b-form-group>
            </b-col>
        </b-row>
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
      card_array: Array(100).fill(1),
      unlimited_time: false,
      time_limit: 10,
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
      let time_limit = null
      if (!this.unlimited_time) {
        time_limit = this.time_limit
      }
      return {
          game_type: this.game_type,
          first_player: this.first_player,
          cards_number: cnt,
          first_player_array: f,
          second_player_array: s,
          time_limit: time_limit,
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
