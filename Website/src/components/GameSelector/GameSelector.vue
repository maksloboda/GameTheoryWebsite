<template>
  <b-row>
    <b-col sm="6">
      <b-card :title="$t('message.GameSelector.CreateGame')" class="mt-4 mx-4 px-2 py-2">
          <create-game></create-game>
      </b-card>
    </b-col>
    <b-col sm="6">
      <b-card :title="$t('message.GameSelector.ConnectToGame')" class="mt-4 mx-4 px-2 py-2">
          <join-game></join-game>
      </b-card>
    </b-col>
  </b-row>
</template>

<script>
import CreateGame from "./CreateGame.vue"
import JoinGame from "./JoinGame.vue"

export default {
    components: {
      CreateGame,
      JoinGame
    }
};
</script>