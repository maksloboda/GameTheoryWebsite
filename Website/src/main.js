import Vue from 'vue'
import App from './App.vue'
import { BootstrapVue, BootstrapVueIcons } from 'bootstrap-vue'
import VueRouter from "vue-router"
import VueCookies from 'vue-cookies'

Vue.config.productionTip = false

import 'bootstrap/dist/css/bootstrap.css'
import 'bootstrap-vue/dist/bootstrap-vue.css'
import 'bootstrap-vue/dist/bootstrap-vue-icons.min.css'

Vue.use(BootstrapVue)
Vue.use(BootstrapVueIcons)
Vue.use(VueRouter)
Vue.use(VueCookies, { expire: '30d'})

import GameSelector from './components/GameSelector/GameSelector.vue';
import PlayGame from './components/PlayGame/PlayGame.vue';
import { messages } from './constants/localization'

const router = new VueRouter({
  mode: "history",
  routes: [
    {
      path: "/",
      name: "start-page",
      component: GameSelector,
    },
    {
      path: "/play/:game_uid/:game_mode?",
      name: "play-page",
      component: PlayGame,
    }
  ],
})

import VueApollo from 'vue-apollo'
import { ApolloClient } from 'apollo-client'
import { createHttpLink } from 'apollo-link-http'
import { InMemoryCache } from 'apollo-cache-inmemory'
import VueI18n from 'vue-i18n'

import { ApolloLink, split } from 'apollo-link'
import { WebSocketLink } from 'apollo-link-ws'
import { getMainDefinition } from 'apollo-utilities'

Vue.use(VueApollo)
Vue.use(VueI18n)

import { PORT, HOST } from "./constants/network"

// Use default values from constants if env vars are undefined
const host = process.env.VUE_APP_HOST || HOST
const port = process.env.VUE_APP_PORT || PORT

// HTTP connection to the API
const httpLink = createHttpLink({
  // You should use an absolute URL here
  uri: `http://${host}:${port}/query`,
})

// Create the subscription websocket link
const wsLink = new WebSocketLink({
  uri: `ws://${host}:${port}/query`,
  options: {
    reconnect: true,
  },
})

const terminatingLink  = split(
  // split based on operation type
  ({ query }) => {
    const definition = getMainDefinition(query)
    return definition.kind === 'OperationDefinition' &&
      definition.operation === 'subscription'
  },
  wsLink,
  httpLink
)

// Cache implementation
const cache = new InMemoryCache()

// Create the apollo client
const apolloClient = new ApolloClient({
  link: ApolloLink.from([terminatingLink]),
  cache,
})

const apolloProvider = new VueApollo({
  defaultClient: apolloClient,
})

const i18n = new VueI18n({
  locale: 'ru',
  messages,
})



let app = new Vue({
  render: h => h(App),
  router,
  apolloProvider,
  i18n
}).$mount('#app')