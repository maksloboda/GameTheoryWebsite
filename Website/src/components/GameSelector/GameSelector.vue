<template>
  <b-row>
    <div class="col-sm-6">
      <b-card :title="$t('message.GameSelector.CreateGame')">
          <create-game></create-game>
      </b-card>
    </div>
    <div class="col-sm-6">
      <b-card :title="$t('message.GameSelector.ConnectToGame')">
          <join-game></join-game>
      </b-card>
    </div>
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