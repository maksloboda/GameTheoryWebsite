<template>
  <b-row>
    <div class="col-sm-6">
      <b-card title="Создать игру">
          <create-game></create-game>
      </b-card>
    </div>
    <div class="col-sm-6">
      <b-card title="Поключиться к лобби">
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