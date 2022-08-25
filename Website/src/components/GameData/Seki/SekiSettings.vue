<template>
  <div>
    <b-form>
      <b-row>
        <b-col cols=6>

          <b-row id="firstContainer">
            <b-col cols=3 id="first">
              <b-form-group id="labelOfText" :label="$t('message.SekiSettings.GameType')">
                <b-form-select id="game-type-select" v-model="game_type">
                  <b-form-select-option value="seki">{{ $t('message.SekiSettings.TypeSeki') }}</b-form-select-option>
                  <b-form-select-option value="dseki">{{ $t('message.SekiSettings.TypeDSeki') }}</b-form-select-option>
                </b-form-select>
              </b-form-group>
            </b-col>

            <b-col cols=3 id="second">
              <b-form-group id="labelOfText" :label="$t('message.SingleSuitSettings.FirstPlayer')">
                <b-form-select id="first-player-select" v-model="first_player">
                  <b-form-select-option :value="FIRST_PLAYER_ID">R</b-form-select-option>
                  <b-form-select-option :value="SECOND_PLAYER_ID">C</b-form-select-option>
                </b-form-select>
              </b-form-group>
            </b-col>
          </b-row>

          <b-row id="secondContainer">
            <b-col cols=3 id="third">
              <b-form-group id="labelOfText" :label="$t('message.SekiSettings.OpponentChoice')">
                <b-form-select id="choose-opponent-radio" button-variant="outline-primary" buttons>
                  <b-form-select-option id="option1">{{ $t('message.SekiSettings.ComputerPerson') }}
                  </b-form-select-option>
                  <b-form-select-option id="option1">{{ $t('message.SekiSettings.PersonPerson') }}
                  </b-form-select-option>
                  <b-form-select-option id="option1">{{ $t('message.SekiSettings.ComputerComputer') }}
                  </b-form-select-option>
                </b-form-select>
              </b-form-group>
            </b-col>

            <b-col cols=3 id="forth">
              <b-form-group id="labelOfText" :label="$t('message.SekiSettings.PassOptions')">
                <b-form-select id="pass_options" v-model="pass_options">
                  <b-form-select-option id="option1" :value="FIRST_PLAYER_ID">{{ $t('message.SekiSettings.RPass') }}
                  </b-form-select-option>
                  <b-form-select-option id="option1" :value="SECOND_PLAYER_ID">{{ $t('message.SekiSettings.CPass') }}
                  </b-form-select-option>
                  <b-form-select-option id="option1" :value="FIRST_PLAYER_ID + SECOND_PLAYER_ID">{{
                      $t('message.SekiSettings.BothPass')
                  }}</b-form-select-option>
                </b-form-select>
              </b-form-group>
            </b-col>
          </b-row>

          <br>
          <b-row id="thirdContainer">
            <b-col sm="3" id="fifth">
              <label id="labelOfText1" for="field-width">{{ $t('message.SekiSettings.FieldWidth') }}</label>
              <b-form-input v-model="field_width" id="field-width" type="number" min="1" :max="MAX_FIELD_SIZE">
              </b-form-input>
            </b-col>
            <b-col sm="3" id="sixth">
              <label id="labelOfText1" for="field_height">{{ $t('message.SekiSettings.FieldHeight') }}</label>
              <b-form-input v-model="field_height" id="field_height" type='number' min="1" :max="MAX_FIELD_SIZE">
              </b-form-input>
            </b-col>
          </b-row>
          <br>

          <b-row id="forthContainer">
            <b-col sm="3">
              <label id="labelOfText1" for="time_limit">{{ $t('message.MoveTimeLimit') }}</label>
              <b-form-input id="time_limit" v-model="time_limit" :value="5" :disabled="unlimited_time" type="number"
                :min="1" :max="300">
              </b-form-input>
            </b-col>
          </b-row>

          <b-row id="fifthContainer">
            <b-row sm="6">
              <b-form-checkbox id="labelOfText" v-model="unlimited_time" :value="true" :unchecked-value="false">
                {{ $t('message.UnlimitedTime') }}
              </b-form-checkbox>
            </b-row>
          </b-row>
          <br>
        </b-col>

        <b-col cols=6>
          <b-card>
            <b-row v-for="i in Array(minMax(field_height, 1, MAX_FIELD_SIZE)).keys()" class="justify-content-md-center">
              <b-col v-for="j in Array(minMax(field_width, 1, MAX_FIELD_SIZE)).keys()" class="mx-0 px-0" md="auto">
                <b-form-input v-model="field[i * field_width + j]" type='number' min="0" :max="MAX_CELL_VALUE"
                  class="field_cell"> </b-form-input>
              </b-col>
            </b-row>
          </b-card>
        </b-col>

      </b-row>
    </b-form>
  </div>
</template>

<style>
.field_cell {
  width: 70px;
  height: 70px;
}

#labelOfText,
#labelOfText1 {
  font-weight: bold;
}

#labelOfText1 {
  padding-bottom: calc(0.375rem + 1px);
}

#second,
#forth,
#sixth {
  margin-left: 150px;
}

legend,
#labelOfText,
#labelOfText1,
#option1 {
  white-space: nowrap
}

#option1 {
  width: 400px;
}

#checkboxes-4 {
  padding-right: 0;
}

#firstContainer,
#secondContainer,
#thirdContainer,
#forthContainer,
#fifthContainer {
  margin-left: 20px;
}

#secondContainer,
#fifthContainer {
  margin-top: 20px;
}

#field-width,
#field_height,
#time_limit,
#weight_number {
  width: 120px;
  border: 1px solid #6d6d6d;
}

input#labelOfText {
  margin-right: 10px;
}

@media (max-width: 1200px) {

  #secondContainer,
  #firstContainer,
  #thirdContainer {
    display: block;
  }

  #second,
  #forth,
  #sixth {
    margin-left: 0px;
    margin-top: 20px;
  }
}

select {
  width: 200px;
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
  mounted() {
    if (this.$cookies.isKey("seki-settings")) {
      this.setSettings(this.$cookies.get("seki-settings"))
    }
  },
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
    setSettings(settings) {
      this.first_player = settings.first_move
      this.game_type = settings.game_type
      this.pass_options = settings.pass_options
      this.field_width = settings.field_width
      this.field_height = settings.field_height
      this.time_limit = settings.time_limit == null ? 10 : settings.time_limit
      this.unlimited_time = settings.time_limit == null
      this.field = Array(MAX_FIELD_SIZE * MAX_FIELD_SIZE).fill(0)
    
      for (let i = 0; i < settings.field.length; i++) {
        this.field[i] = settings.field[i]
      }
    },
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
    saveSettings() {
      let settings = this.getSettings()
      this.$cookies.set("seki-settings", settings)
    },
    minMax(val, min, max) {
      if (parseInt(val) < min) { return min; }
      if (parseInt(val) > max) { return max; }
      return parseInt(val)
    }
  }
}
</script>
