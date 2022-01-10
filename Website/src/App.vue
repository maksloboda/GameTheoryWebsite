<template>
  <div id="app" class="bg-secondary">
    <nav class="navbar navbar-expand-lg navbar-dark bg-dark">
      <div class="container-fluid">
        <a class="navbar-brand" href="#">game theory website</a>
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