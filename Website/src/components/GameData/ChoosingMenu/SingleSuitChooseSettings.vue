<template>
  <div>
    <b-row>
        <b-col>
          <b-button href="#" variant="primary" id="choosingBtn1" @click="showConfigModal">Вист</b-button>
        </b-col>
        <b-col>
          <b-button href="#" variant="primary" id="choosingBtn2" @click="showConfigModal">Дурак</b-button>
        </b-col>
      </b-row>
  </div>
</template>

<style>
  #choosingBtn1, #choosingBtn2 {
    width: 100%;
    height: 350px;
  }

  #choosingBtn2 {
    float: right;
  }
</style>

<script>
import SingleSuitSettings from "./../SingleSuit/SingleSuitSettings.vue"
import GameCardVue from "../../GameSelector/GameCard.vue"

export default {
  methods: {
    showConfigModal() {

      // this.$refs["config_modal"].show()
    }
  },
}
</script>

