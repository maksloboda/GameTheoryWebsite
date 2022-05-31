<template>
  <div class = "SingleSuitGameplay">
    <h2>
        {{ $t('message.SingleSuitInfo.header') }}
    </h2>
    <ul>
      <li>
        <p>
          <h3>
            {{ $t('message.SingleSuitInfo.SuitWhistette.header') }}
          </h3>
            {{ $t('message.SingleSuitInfo.SuitWhistette.info') }}
        </p>
      </li>
      <li>
        <p>
          <h3>
            {{ $t('message.SingleSuitInfo.WeightedWhistette.header') }}
          </h3>
            {{ $t('message.SingleSuitInfo.WeightedWhistette.info') }}
        </p>
      </li>
      <li>
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
      <li>
        <p>
          <h3>
            {{ $t('message.SingleSuitInfo.WeightedSuitFool.header') }}
          </h3>
            {{ $t('message.SingleSuitInfo.WeightedSuitFool.info') }}
        </p>
      </li>
    </ul>
  </div>
</template>