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

// there are 8 leds strip so I'm using a byte with 
// one bit to represent each column and then I have
// 14 of them since each strip is 14 pixels long
//
// in seedDrops I am randomly starting raindrops
// by setting the first bit true in some of the strips.
void seedDrop()
{
  //print("seed drop ");println();
  for(byte iColumn = 0;iColumn<8;iColumn++)
  {
    if (random(0,8) == 0) 
    {
      //print("1");  
      bitSet(rain[0], iColumn);
    }
    else
    {
      //print("0");
      bitClear(rain[0], iColumn);
    }
  }
  //println();
}

// for each frame I just copy the frame above onto it
void advanceDrops()
{
  //print("advance ");println();
  // I need to copy starting at the bottom because 
  // otherwise I end up writing over myself
  for(byte iRow = rowCount-1;iRow > 0;iRow--)
  {
    rain[iRow] = rain[iRow-1];
  }
}

void animateRain()
{
  //print("animate");println();
  advanceDrops();
  seedDrop();
  //printRainState();

  for (byte iRow = 0;iRow < rowCount;iRow++)  
  {
    for(byte iColumn = 0;iColumn < 8;iColumn++)
    {
      byte currentPixel;
      // if it's an odd column we're dripping down
      if (iColumn % 2 == 1)
      {
        currentPixel = iRow +  iColumn * rowCount;
        if (bitRead(rain[iRow], iColumn))
        {
          leds[currentPixel] = CRGB(255,255,255);
        }
        else
          leds[currentPixel] = CRGB(0,0,0);
        //print("[");print(iRow);print(".");print(iColumn);print("->");print(iRow * 8 +  iColumn);println();
      }
      else // we're dripping up
      {
        currentPixel = iRow + iColumn * rowCount;
        if (bitRead(rain[rowCount-1-iRow], iColumn))
          leds[currentPixel] = CRGB(255,255,255);
        else
          leds[currentPixel] = CRGB(0,0,0);
      }
    }
  }
  FastLED.show();
  delay(50);
}
