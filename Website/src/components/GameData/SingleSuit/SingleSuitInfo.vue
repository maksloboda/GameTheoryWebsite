<template>
  <div class="SingleSuitGameplay">
    <h2>
      {{ $t('message.SingleSuitInfo.header') }}
    </h2>
    <div id="roolsContainer">



      <section class="carousel" aria-label="Gallery">
        <ol class="carousel__viewport">
          <li id="carousel__slide1" tabindex="0" class="carousel__slide">
            <!-- <li id="roolsItem1"> -->
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
            <!-- </li> -->
            <div class="carousel__snapper">
              <a href="#carousel__slide4" class="carousel__prev">Go to last slide</a>
              <a href="#carousel__slide2" class="carousel__next">Go to next slide</a>
            </div>
          </li>
          <li id="carousel__slide2" tabindex="0" class="carousel__slide">
            <p>
            <h3>
              {{ $t('message.SingleSuitInfo.WeightedSuitFool.header') }}
            </h3>
            {{ $t('message.SingleSuitInfo.WeightedSuitFool.info') }}
            </p>
            <div class="carousel__snapper"></div>
            <a href="#carousel__slide1" class="carousel__prev">Go to previous slide</a>
            <a href="#carousel__slide3" class="carousel__next">Go to next slide</a>
          </li>
          <li id="carousel__slide3" tabindex="0" class="carousel__slide">
            <p>
            <h3>
              {{ $t('message.SingleSuitInfo.SuitWhistette.header') }}
            </h3>
            {{ $t('message.SingleSuitInfo.SuitWhistette.info') }}
            </p>
            <div class="carousel__snapper"></div>
            <a href="#carousel__slide2" class="carousel__prev">Go to previous slide</a>
            <a href="#carousel__slide4" class="carousel__next">Go to next slide</a>
          </li>
          <li id="carousel__slide4" tabindex="0" class="carousel__slide">
            <p>
            <h3>
              {{ $t('message.SingleSuitInfo.WeightedWhistette.header') }}
            </h3>
            {{ $t('message.SingleSuitInfo.WeightedWhistette.info') }}
            </p>
            <div class="carousel__snapper"></div>
            <a href="#carousel__slide3" class="carousel__prev">Go to previous slide</a>
            <a href="#carousel__slide1" class="carousel__next">Go to first slide</a>
          </li>
        </ol>
      </section>



      <!-- <ul id="foolRools">
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
      </ul> -->
    </div>

  </div>
</template>

<style>
#roolsContainer {
  display: block;
}

#foolRools,
#whistetteRools {
  display: flex;
}

#roolsItem1,
#roolsItem2 {
  width: 50%;
}

#roolsItem1 {
  margin-right: 20px;
}

#roolsItem2 {
  margin-left: 20px;
  margin-right: 10px;
}


@keyframes tonext {
  75% {
    left: 0;
  }

  95% {
    left: 100%;
  }

  98% {
    left: 100%;
  }

  99% {
    left: 0;
  }
}

@keyframes tostart {
  75% {
    left: 0;
  }

  95% {
    left: -300%;
  }

  98% {
    left: -300%;
  }

  99% {
    left: 0;
  }
}

@keyframes snap {
  96% {
    scroll-snap-align: center;
  }

  97% {
    scroll-snap-align: none;
  }

  99% {
    scroll-snap-align: none;
  }

  100% {
    scroll-snap-align: center;
  }
}

* {
  box-sizing: border-box;
  scrollbar-color: transparent transparent;
  scrollbar-width: 0px;
}

*::-webkit-scrollbar {
  width: 0;
}

*::-webkit-scrollbar-track {
  background: transparent;
}

*::-webkit-scrollbar-thumb {
  background: transparent;
  border: none;
}

* {
  -ms-overflow-style: none;
}

ol, li {
  list-style: none;
  margin: 0;
  padding: 0;
}

.carousel {
  position: relative;
  padding-top: 55%;
}

.carousel__viewport {
  position: absolute;
  top: 0;
  right: 0;
  bottom: 0;
  left: 0;
  display: flex;
  overflow-x: scroll;
  scroll-behavior: smooth;
  scroll-snap-type: x mandatory;
}

.carousel__slide {
  position: relative;
  flex: 0 0 100%;
  width: 100%;
}

.carousel__slide:before {
  position: absolute;
  top: 50%;
  left: 50%;
  transform: translate3d(-50%, -40%, 70px);
  color: #fff;
  font-size: 2em;
}

.carousel__snapper {
  position: absolute;
  top: 0;
  left: 0;
  width: 100%;
  height: 100%;
  scroll-snap-align: center;
}

.carousel:hover .carousel__snapper,
.carousel:focus-within .carousel__snapper {
  animation-name: none;
}

.carousel__navigation {
  position: absolute;
  right: 0;
  bottom: 0;
  left: 0;
  text-align: center;
}

.carousel__navigation-list,
.carousel__navigation-item {
  display: inline-block;
}

.carousel::before,
.carousel::after,
.carousel__prev,
.carousel__next {
  position: absolute;
  top: 0;
  margin-top: 25%;
  width: 4rem;
  height: 4rem;
  transform: translateY(-50%);
  font-size: 0;
  outline: 0;
}

.carousel::before,
.carousel__prev {
  left: -1rem;
}

.carousel::after,
.carousel__next {
  right: -1rem;
}

.carousel::before,
.carousel::after {
  content: '';
  z-index: 1;
  background-color: rgba(41, 41, 41, 0.148);
  background-size: 1.5rem 1.5rem;
  background-repeat: no-repeat;
  background-position: center center;
  color: #fff;
  font-size: 2.5rem;
  line-height: 4rem;
  text-align: center;
  pointer-events: none;
}

.carousel::before {
  content: ' \25C1';
}

.carousel::after {
  content: ' \25B7';
}
</style>