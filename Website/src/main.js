import Vue from 'vue'
import App from './App.vue'
import { BootstrapVue, BootstrapVueIcons } from 'bootstrap-vue'
import VueRouter from "vue-router"

Vue.config.productionTip = false

import 'bootstrap/dist/css/bootstrap.css'
import 'bootstrap-vue/dist/bootstrap-vue.css'
import 'bootstrap-vue/dist/bootstrap-vue-icons.min.css'

Vue.use(BootstrapVue)
Vue.use(BootstrapVueIcons)
Vue.use(VueRouter)

import GameSelector from './components/GameSelector/GameSelector.vue';
import PlayGame from './components/PlayGame.vue';

const router = new VueRouter({
  mode: "history",
  routes: [
    {
      path: "/",
      name: "start-page",
      component: GameSelector,
    },
    {
      path: "/play/:game_uid",
      name: "play-page",
      component: PlayGame,
    },
  ],
})

let app = new Vue({
  render: h => h(App),
  router,
}).$mount('#app')