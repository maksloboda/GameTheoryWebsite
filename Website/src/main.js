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

import { ApolloClient } from 'apollo-client'
import { createHttpLink } from 'apollo-link-http'
import { InMemoryCache } from 'apollo-cache-inmemory'
import { provideApolloClient } from "@vue/apollo-composable";
import VueApollo from 'vue-apollo'

// HTTP connection to the API
const httpLink = createHttpLink({
  // You should use an absolute URL here
  uri: 'http://localhost:3020/graphql',
})

// Cache implementation
const cache = new InMemoryCache()

// Create the apollo client
const apolloClient = new ApolloClient({
  link: httpLink,
  cache,
})

const apolloProvider = new VueApollo({
  defaultClient: apolloClient,
})

provideApolloClient(apolloClient)

let app = new Vue({
  render: h => h(App),
  router,
  apolloProvider,
}).$mount('#app')