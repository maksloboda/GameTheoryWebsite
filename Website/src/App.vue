<template>
  <div id="app" class="bg-secondary">
    <b-navbar class="navbar navbar-expand-lg navbar-dark bg-dark w-100">
      <b-navbar-brand class="mx-4" href="/">{{$t('message.WebsiteName')}}
      </b-navbar-brand >
      <b-collapse id="nav-collapse" is-nav>
        <b-navbar-nav class="ml-auto" right>
          <b-button>
            <b-form-select 
              v-model="selected" 
              :options="options"
              @change='changeLanguage'
            ></b-form-select>
          </b-button>
        </b-navbar-nav>
      </b-collapse>
    </b-navbar>
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