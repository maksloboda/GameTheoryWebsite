<template>
  <div>
    <b-form>
      <b-row>
        <b-col>
          <b-form-group 
            id="cards_number"
            label="Cards number:"
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
          <b-form-group label="First player">
            <b-form-radio-group
              id="first-player-radio"
              v-model="first_player"
            >
            <b-form-radio :value="FIRST_PLAYER_ID">R</b-form-radio>
            <b-form-radio :value="SECOND_PLAYER_ID">C</b-form-radio>
          </b-form-radio-group>
          </b-form-group>
        </b-col>
      </b-row>
      <b-card>
        <b-row v-for="i in Array(minMax(cards_number, 1, 100)).keys()">
            <b-col>
                <b-form-group label="First player">
                <b-form-radio-group
                    id="first-player-radio"
                    v-model="first_player"
                >
                <b-form-radio :value="FIRST_PLAYER_ID">R</b-form-radio>
                <b-form-radio :value="SECOND_PLAYER_ID">C</b-form-radio>
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
} from "./"


export default {
  data() {
    return {
      first_player_id: FIRST_PLAYER_ID,
      second_player_id: SECOND_PLAYER_ID,
      cards_number: 8,
      card_array: Array(100).fill(1)
    }
  },
  methods: {
    getSettings() {
      let cnt = this.minMax(this.cards_number, 1, 100)
      let f = Array(cnt)

      for (let i = 0; i < cnt; ++i) {
          f[i] = parseInt(this.card_array[i])
      }
      return {
          cards_number: cnt,
          field_height: h,
          card_array: f
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
