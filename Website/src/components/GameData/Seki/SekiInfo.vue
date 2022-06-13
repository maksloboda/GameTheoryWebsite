<template>

  <div class = "SekiGameplay">
    <h2>
      {{ $t('message.SekiInfo.header') }}
    </h2>
    <p>
      {{ $t('message.SekiInfo.part1') }}
    </p>
    <p>
      <ul>
        <li>
          {{ $t('message.SekiInfo.option1') }}
        </li>
        <li>
          {{ $t('message.SekiInfo.option2') }}
        </li>
      </ul>
    </p>
    <p>
      {{ $t('message.SekiInfo.part2') }}
    <ul>
      <li>
        {{ $t('message.SekiInfo.mode1') }}
      </li>
      <li>
        {{ $t('message.SekiInfo.mode2') }}
      </li>
      <li>
        {{ $t('message.SekiInfo.mode3') }}
      </li>
      <li>
        {{ $t('message.SekiInfo.mode4') }}
      </li>
    </ul>
        {{ $t('message.SekiInfo.endInfo') }}
  </p>
  </div>
</template>