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
  // Pal = CRGBPalette16( CRGB::Black, CRGB::Blue, CRGB::Aqua,  CRGB::White);

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
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
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

      if (strcmp(data, "m0") == 0) {
        mode = 0;
      }
      if (strcmp(data, "m1") == 0) {
        mode = 1;
      }

      if (strcmp(data, "m2") == 0) {
        mode = 2;
      }

      if (strcmp(data, "m3") == 0) {
        mode = 3;
      }

      if (strcmp(data, "m4") == 0) {
        mode = 4;
      }

      if (strcmp(data, "m5") == 0) {
        mode = 5;
      }


      for (int i = 0; i < 10; i++)
        data[i] = 0;
    } else {
      if (lampOn) {
        switch (mode) {
          case 0:
            random16_add_entropy( random());
            Fire2012WithPalette();
            break;
          case 1:
            storm();
            break;
          case 2:
            animateRain();
            delay(1000 / 120);
            break;
          case 3:
            rainbowLoop();
            break;
          case 4:
            starryNight();
            delay(1000 / 120);
            break;
          case 5:
            basicGradiant();
            delay(1000 / 120);
            break;
        }

        FastLED.show();
        delay(50);
      }
    }
  }
}

CRGB endclr;
CRGB midclr;
void basicGradiant() {
  uint8_t speed = beatsin8(10, 0, 255);
  endclr = blend(CHSV(160, 185, 255), CRGB::Pink, speed);
  midclr = blend(CRGB::Purple, CRGB::Green, speed);

  fill_gradient_RGB(leds, 0, endclr, NUM_LEDS / 2, midclr);
  fill_gradient_RGB(leds, NUM_LEDS / 2 + 1, midclr, NUM_LEDS, endclr);

  FastLED.show();
}
