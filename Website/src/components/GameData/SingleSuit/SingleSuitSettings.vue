<template>
  <div>
    <b-form>
      <b-row>

        <b-col cols=6>

          <b-row id="firstContainer">
            <b-col sm="3" id="first">
              <b-form-group id="labelOfText" :label="$t('message.SekiSettings.GameType')">
                <b-form-select id="game-type-select" v-model="game_type">
                  <b-form-select-option value="singlesuit">{{ $t('message.SingleSuitSettings.Fool') }}
                  </b-form-select-option>
                  <b-form-select-option value="d-singlesuit">{{ $t('message.SingleSuitSettings.DFool') }}
                  </b-form-select-option>
                  <b-form-select-option value="whistette">{{ $t('message.SingleSuitSettings.Whistette') }}
                  </b-form-select-option>
                </b-form-select>
              </b-form-group>
            </b-col>

            <b-col sm="3" id="second">
              <b-form-group id="labelOfText" :label="$t('message.SingleSuitSettings.FirstPlayer')">
                <b-form-select id="first-player-radio" v-model="first_player">
                  <b-form-select-option :value="FIRST_PLAYER_ID">A</b-form-select-option>
                  <b-form-select-option :value="SECOND_PLAYER_ID">B</b-form-select-option>
                </b-form-select>
              </b-form-group>
            </b-col>
          </b-row>

          <br>

          <b-row id="secondContainer2">
            <b-col sm="3" id="third">
              <label id="labelOfText1" for="field-width">{{ $t('message.SingleSuitSettings.CardNumber') }}</label>
              <b-form-input v-model="cards_number" id="cards-number" type="number" min="2" :max="8">
              </b-form-input>
            </b-col>
            <b-col sm="3" id="forth">
              <label id="labelOfText1" for="time_limit">{{ $t('message.MoveTimeLimit') }}</label>
              <b-form-input id="time_limit" v-model="time_limit" :value="5" :disabled="unlimited_time" type="number"
                :min="1" :max="300">
              </b-form-input>
            </b-col>
          </b-row>
          <br>

          <b-row id="fifthContainer2">
            <b-row sm="6">
              <b-form-checkbox id="labelOfText" v-model="unlimited_time" :value="true" :unchecked-value="false">
                {{ $t('message.UnlimitedTime') }}
              </b-form-checkbox>
            </b-row>
          </b-row>

          <b-row id="fifthContainer2">
            <b-row sm="6">
              <b-form-checkbox id="labelOfText" v-model="weighted_game" :value="true" :unchecked-value="false">
                {{ $t('message.SingleSuitSettings.WeightedVersion') }}
              </b-form-checkbox>
            </b-row>
          </b-row>

          <!-- <b-row id="fifthContainer2">
            <b-row sm="6">
              <b-form-checkbox id="labelOfText" v-model="scanty_version" :value="true" :unchecked-value="false">
                {{ $t('message.SingleSuitSettings.ScantyVersion') }}
              </b-form-checkbox>
            </b-row>
          </b-row> -->
          <br>
        </b-col>

        <b-col cols=6>
          <b-card>
            <b-row>
              <b-col sm="3"></b-col>
              <b-col sm="9" id="titlesPfTableCardsContainer">
                <b-form-text id="titleOfTableCards">A</b-form-text>
                <b-form-text id="titleOfTableCards">B</b-form-text>
                <b-form-text id="titleOfTableCards"
                  v-if="weighted_game && game_type === 'singlesuit' || weighted_game && game_type === 'd-singlesuit'">{{
                      $t('message.SingleSuitSettings.Weights')
                  }}</b-form-text>
              </b-col>
            </b-row>
            <b-row v-for="i in Array(minMax(cards_number, 1, 100)).keys()">
              <b-col sm="3" id="cardNumber">
                {{ i + 1 }}{{ $t('message.SingleSuitSettings.NthCard') }}
              </b-col>
              <b-col sm="9">
                <b-form-group>
                  <b-form-radio-group v-model="card_array[i]" buttons class="w-100">
                    <b-form-radio :value="0"></b-form-radio>
                    <b-form-radio :value="1"></b-form-radio>
                    <b-form-input class="checked_w" id="weight_number" v-model="weights_array[i]" :value="1"
                      type="number" :min="-99" :max="99"
                      v-if="weighted_game && game_type === 'singlesuit' || weighted_game && game_type === 'd-singlesuit'" />
                  </b-form-radio-group>
                </b-form-group>
                <!-- <b-form-group v-model="weights_array[i]" buttons class="w-100">
                  <b-form-input v-model="weights_array[i]" :value="1" type="number" :min="-99" :max="99"
                    v-if="weighted_game" />
                </b-form-group> -->
              </b-col>
            </b-row>
          </b-card>
        </b-col>

        <br>
        <label id="labelOfText1" for="bribes_weights" v-if="weighted_game && game_type === 'whistette'">{{
            $t('message.SingleSuitSettings.BribeWeights')
        }}</label>
        <b-col v-for="i in Array(minMax(cards_number, 1, 100) / 2).keys()">
          <label id="labelOfText1" for="bribe_weight" v-if="weighted_game && game_type === 'whistette'">{{ i + 1 }}{{
              $t('message.SingleSuitSettings.NthBribe')
          }}</label>
          <b-form-input class="checked_w" id="weight_number" v-if="weighted_game && game_type === 'whistette'"
            v-model="weights_array[i]" :value="1" type="number" :min="-99" :max="99">
          </b-form-input>
        </b-col>
      </b-row>

    </b-form>
  </div>
</template>

<style>
select.form-control {
  -moz-appearance: none;
  -webkit-appearance: none;
  appearance: none;
}

#titlesPfTableCardsContainer {
  display: flex;
  text-align: center;
  font-weight: bold;
}

#titleOfTableCards {
  width: 100%;
}

#cardNumber {
  font-weight: bold;
  color: #6c757d;
}

#labelOfText,
#cards_number {
  font-weight: bold;
}

#cards-number {
  width: 120px;
  border: 1px solid #6d6d6d;
}

#secondContainer1,
#secondContainer2,
#fifthContainer2 {
  margin-left: 20px;
}

@media (max-width: 1200px) {

  #secondContainer1,
  #secondContainer2 {
    display: block;
  }
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
      weighted_game: false,
      weights_array: Array(100).fill(1),
    }
  },
  methods: {
    getSettings() {
      let cnt = this.minMax(this.cards_number, 1, 100)
      let f = []
      let s = []
      let w = []
      for (let i = 0; i < cnt; ++i) {
        if (parseInt(this.card_array[i]) == 0) {
          f.push(i + 1)
        } else {
          s.push(i + 1)
        }

        if (this.weighted_game) {
          w.push(parseInt(this.weights_array[i]))
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
        weights: w,
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
