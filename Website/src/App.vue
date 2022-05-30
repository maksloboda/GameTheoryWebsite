<template>
  <div id="app" class="bg-secondary">
    <nav class="navbar navbar-expand-lg navbar-dark bg-dark w-100">
      <div class="container-fluid">
        <b-row>
          <b-col sm="6">
            <a class="navbar-brand" href="/">{{$t('message.WebsiteName')}}</a>
          </b-col>
          <b-col sm="6">
            <b-button>
              <b-form-select 
                v-model="selected" 
                :options="options"
                @change='changeLanguage'
              ></b-form-select>
            </b-button>
          </b-col>
        </b-row>
      </div>
    </nav>
    <router-view></router-view>
  </div>
</template>


<style lang="scss">
#app {
  touch-action: none;
  min-height: 100vh;
  min-width: 100vw;
}

body {
  min-height: 100vh;
  min-width: 100vw;
  // display: flex;
}
</style>


<script>
  export default {
    data() {
      return {
        selected: 'ru',
        options: [
          { value: 'ru', text: 'Русский' },
          { value: 'en', text: 'English' },
        ]
      }
    }, 
    methods: {
      changeLanguage() {
        this.$i18n.locale = this.selected;
      },
    },
  }
</script>