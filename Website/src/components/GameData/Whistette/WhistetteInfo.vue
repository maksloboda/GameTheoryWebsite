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
    </ul>
  </div>
</template>