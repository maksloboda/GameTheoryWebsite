<template>
  <div>
    <b-form>
      <b-row>
        <b-col>
          <b-form-group :label="$t('message.SekiSettings.GameType')">
            <b-form-radio-group
              id="game-type-radio"
              v-model="game_type"
            >
            <b-form-radio value="seki">{{$t('message.SekiSettings.TypeSeki')}}</b-form-radio>
            <b-form-radio value="dseki">{{$t('message.SekiSettings.TypeDSeki')}}</b-form-radio>
          </b-form-radio-group>
          </b-form-group>
        </b-col>
        
        <b-col>
          <b-form-group 
            id="pass_options"
            :label="$t('message.SekiSettings.PassOptions')"
          >
            <b-form-checkbox-group
              v-model="pass_options"
              id="checkboxes-4"
            >
              <b-form-checkbox :value="FIRST_PLAYER_ID">{{$t('message.SekiSettings.RPass')}}</b-form-checkbox>
              <b-form-checkbox :value="SECOND_PLAYER_ID">{{$t('message.SekiSettings.CPass')}}</b-form-checkbox>
            </b-form-checkbox-group>
          </b-form-group>
        </b-col>

        
        <b-col>
          <b-form-group :label="$t('message.SingleSuitSettings.FirstPlayer')">
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
      <br>
      <b-row>
        <b-col sm="2">
          <label for="field-width">{{$t('message.SekiSettings.FieldWidth')}}</label>
        </b-col>
        <b-col sm="4">
          <b-form-input
            v-model="field_width"
            id="field-width"
            type="number"
            min="1"
            :max="MAX_FIELD_SIZE"
          ></b-form-input>
        </b-col>
        <b-col sm="2">
          <label for="field_height">{{$t('message.SekiSettings.FieldHeight')}}</label>
        </b-col>
        <b-col sm="4">
          <b-form-input
            v-model="field_height"
            id="field_height"
            type='number'
            min="1"
            :max="MAX_FIELD_SIZE"
          ></b-form-input>
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
        <b-row 
          v-for="i in Array(minMax(field_height, 1, MAX_FIELD_SIZE)).keys()"
          class="justify-content-md-center"
        >
          <b-col 
            v-for="j in Array(minMax(field_width, 1, MAX_FIELD_SIZE)).keys()"
            class="mx-0 px-0"
            md="auto"
          >
            <b-form-input
              v-model="field[i * field_width + j]"
              type='number'
              min="0"
              :max="MAX_CELL_VALUE"
              class="field_cell"
            > </b-form-input>
          </b-col>
        </b-row>
      </b-card>
    </b-form>
  </div>
</template>

<style>
.field_cell {
  width:70px;
  height:70px;
}
</style>

<script>
import {
  FIRST_PLAYER_ID,
  SECOND_PLAYER_ID,
  MAX_FIELD_SIZE,
  MAX_CELL_VALUE
} from "./"


export default {
  data() {
    return {
      FIRST_PLAYER_ID: FIRST_PLAYER_ID,
      SECOND_PLAYER_ID: SECOND_PLAYER_ID,
      pass_options: [],
      first_player: FIRST_PLAYER_ID,
      game_type: "seki",
      field_width: 4,
      field_height: 4,
      field: Array(MAX_FIELD_SIZE * MAX_FIELD_SIZE).fill(0),
      MAX_FIELD_SIZE: MAX_FIELD_SIZE,
      MAX_CELL_VALUE: MAX_CELL_VALUE,
      unlimited_time: false,
      time_limit: 10,
    }
  },
  methods: {
    getSettings() {
      let w = this.minMax(this.field_width, 1, MAX_FIELD_SIZE)
      let h = this.minMax(this.field_height, 1, MAX_FIELD_SIZE)
      let f = Array(w * h)

      let c = 0;
      for (let i = 0; i < h; ++i) {
        for (let j = 0; j < w; ++j) {
          f[c++] = parseInt(this.field[i * w + j])
        }
      }
      let time_limit = null
      if (!this.unlimited_time) {
        time_limit = this.time_limit
      }

      return {
        first_move: this.first_player,
        game_type: this.game_type,
        pass_options: this.pass_options,
        field_width: w,
        field_height: h,
        field: f,
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
