#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h>
#endif

#define PIN        A0

#define NUMPIXELS 8 

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define ONDELAYVAL 300 // Time (in milliseconds)
#define OFFDELAYVAL 300 // Time (in milliseconds)

void setup() {

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  pixels.setBrightness(128);
  pixels.begin();
}

void loop() {
  pixels.clear();  

  // LED Light On
  for(int i=0; i<NUMPIXELS; i++) { 
    pixels.setPixelColor(i, pixels.Color(255, 0, 0));
  }
  pixels.show();
  delay(ONDELAYVAL);

  // LED Light off
  for(int i=0; i<NUMPIXELS; i++) { 
    pixels.setPixelColor(i, pixels.Color(0, 0, 0)); 
  }
  pixels.show();
  delay(OFFDELAYVAL);
}
