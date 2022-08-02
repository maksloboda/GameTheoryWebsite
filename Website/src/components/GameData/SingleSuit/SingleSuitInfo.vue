<template>
  <div class="SingleSuitGameplay">
    <h2>
      {{ $t('message.SingleSuitInfo.header') }}
    </h2>
    <div id="roolsContainer">
      <ul id="foolRools">
        <li id="roolsItem1">
          <p>
          <h3>
            {{ $t('message.SingleSuitInfo.SuitFool.header') }}
          </h3>
          {{ $t('message.SingleSuitInfo.SuitFool.part1') }}
          <ul>
            <li>
              {{ $t('message.SingleSuitInfo.SuitFool.option1') }}
            </li>
            <li>
              {{ $t('message.SingleSuitInfo.SuitFool.option2') }}
            </li>
          </ul>
          {{ $t('message.SingleSuitInfo.SuitFool.part2') }}
          </p>
        </li>
        <li id="roolsItem2">
          <p>
          <h3>
            {{ $t('message.SingleSuitInfo.WeightedSuitFool.header') }}
          </h3>
          {{ $t('message.SingleSuitInfo.WeightedSuitFool.info') }}
          </p>
        </li>
      </ul>

      <ul id="whistetteRools">
        <li id="roolsItem1">
          <p>
          <h3>
            {{ $t('message.SingleSuitInfo.SuitWhistette.header') }}
          </h3>
          {{ $t('message.SingleSuitInfo.SuitWhistette.info') }}
          </p>
        </li>
        <li id="roolsItem2">
          <p>
          <h3>
            {{ $t('message.SingleSuitInfo.WeightedWhistette.header') }}
          </h3>
          {{ $t('message.SingleSuitInfo.WeightedWhistette.info') }}
          </p>
        </li>
      </ul>
    </div>

  </div>
</template>

<style>
  #roolsContainer {
    display: block;
  }

  #foolRools, #whistetteRools {
    display: flex;
  }

  #roolsItem1, #roolsItem2 {
    width: 50%;
  }

  #roolsItem1 {
    margin-right: 20px;
  }

  #roolsItem2 {
    margin-left: 20px;
    margin-right: 10px;
  }
</style>