#define rowCount 13
byte rain[rowCount] = {0};

void rainbowLoop() {              //-m3-LOOP HSV RAINBOW
  static byte hue = 0;
  static int currentFrame = 0;
  currentFrame++;

  fill_rainbow(leds, NUM_LEDS, hue++, 10);
  FastLED.show();
  delay(15);
  hue++;
}

void storm()
{
  static int stormCount = 0;
  static bool inLightning = false;

  fill_solid(leds, NUM_LEDS, CHSV(40, 1, 128));

  FastLED.show();

  if (random(500) == 0)
  {
    inLightning = true;

  }

  if (inLightning)
  {
    for (byte i = 0; i < 128; i++)
    {
      flicker(240, 128);
      FastLED.show();
    }
    inLightning = false;
  }
}

void flicker(int thishue, int thissat) {            //-m9-FLICKER EFFECT
  int random_bright = random(0, 255);
  int random_delay = random(10, 100);
  int random_bool = random(0, random_bright);
  if (random_bool < 10) {
    delay(random_delay);
    for (byte i = 0 ; i < NUM_LEDS; i++ ) {
      leds[i] = CHSV(thishue, thissat, random_bright);
    }
  }
}


void animateRain()
{
  fadeToBlackBy( leds, NUM_LEDS, 30);
  int pos = random16(NUM_LEDS);
  leds[pos] = CHSV(135, 80, 200);

  FastLED.show();
}

void starryNight() {
  fill_gradient_RGB(leds, 0, CHSV(160, 255, 150), NUM_LEDS - 1, CHSV(180, 230, 80));
  addGlitter(80);
  FastLED.show();
}

void addGlitter( fract8 chanceOfGlitter) {
  if ( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}
