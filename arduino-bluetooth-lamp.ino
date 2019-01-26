#include <SoftwareSerial.h>
#include <FastLED.h>
SoftwareSerial EEBlue(10, 11); // RX | TX

//define some variables
#define LEDPIN     6
#define FPS  100
#define LED_TYPE     NEOPIXEL
#define NUM_LEDS    133


int BRIGHTNESS = 50;
CRGB leds[NUM_LEDS];
CRGBPalette16 gPal;

boolean lampOn;

void setup()
{

  Serial.begin(9600);
  EEBlue.begin(9600);  //Default Baud for comm, it may be different for your Module.
  delay(3000);
  Serial.println("The bluetooth gates are open.\n Connect to HC-05 from any other bluetooth device with 1234 as pairing key!.");

  // First, a gradient from black to red to yellow to white -- similar to HeatColors_p
  //   gPal = HeatColors_p;
  //   gPal = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::Yellow, CRGB::White);

  // Second, this palette is like the heat colors, but blue/aqua instead of red/yellow
     gPal = CRGBPalette16( CRGB::Black, CRGB::Blue, CRGB::Aqua,  CRGB::White);

  // Third, here's a simpler, three-step gradient, from black to red to white
  //   gPal = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::White);

  setupFastLED();
  lampOn = false;
}

inline void setupFastLED()
{
  delay(1000); // sanity delay
  FastLED.addLeds<LED_TYPE, LEDPIN>(leds, NUM_LEDS).setCorrection(UncorrectedColor );
  FastLED.setBrightness(BRIGHTNESS);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
}


int mode = 0;
char data[10];

void loop()
{

  // Feed any data from bluetooth to Terminal.
  if (EEBlue.available()) {
    Serial.println("New command, collecting...");
    int count = 0;

    while (EEBlue.available()) {
      char character = EEBlue.read();

      data[count] = character;
      count++;
    }
  } else {
    if (strlen(data) > 0) {
      Serial.println("Collected a new command, process that first");
      Serial.println(data);
      if (strcmp(data, "on") == 0)
        lampOn = true;

      if (strcmp(data, "off") == 0) {
        lampOn = false;
        fill_solid(leds, NUM_LEDS, CRGB::Black);
        FastLED.show();
      }

      if (strcmp(data, "+") == 0) {
        if (BRIGHTNESS < 250) {
          BRIGHTNESS += 10;
        }
        FastLED.setBrightness(BRIGHTNESS);
        FastLED.show();
      }

      //vol- = brightness down
      if (strcmp(data, "-") == 0) {
        if (BRIGHTNESS > 10) {
          BRIGHTNESS -= 10;
        }
        FastLED.setBrightness(BRIGHTNESS);
        FastLED.show();
      }

      for (int i = 0; i < 10; i++)
        data[i] = 0;
    } else {
      if (lampOn) {
        random16_add_entropy( random());
        Fire2012WithPalette();
        FastLED.show();
        FastLED.delay(1000 / FPS);
        delay(100);
      }
    }
  }
}

// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 55, suggested range 20-100
#define COOLING  80

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 120
bool gReverseDirection = false;

void Fire2012WithPalette()
{
  // Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
  for ( int i = 0; i < NUM_LEDS; i++) {
    heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for ( int k = NUM_LEDS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
  }

  // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  if ( random8() < SPARKING ) {
    int y = random8(7);
    heat[y] = qadd8( heat[y], random8(160, 255) );
  }

  // Step 4.  Map from heat cells to LED colors
  for ( int j = 0; j < NUM_LEDS; j++) {
    // Scale the heat value from 0-255 down to 0-240
    // for best results with color palettes.
    byte colorindex = scale8( heat[j], 240);
    CRGB color = ColorFromPalette( gPal, colorindex);
    int pixelnumber;
    if ( gReverseDirection ) {
      pixelnumber = (NUM_LEDS - 1) - j;
    } else {
      pixelnumber = j;
    }
    leds[pixelnumber] = color;
  }
}
