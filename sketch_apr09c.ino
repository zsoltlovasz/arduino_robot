#include <Adafruit_NeoPixel.h>
#define PIN        2
#define NUMPIXELS 12

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 200

void setup() {
  // put your setup code here, to run once:
  pixels.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  pixels.clear();
  int j;
  for(int i=0; i<NUMPIXELS*2; i++) {
    pixels.clear();
    j=i;
    if(j>=NUMPIXELS)j=NUMPIXELS-(i-NUMPIXELS+1);
    pixels.setPixelColor(j, pixels.Color(0, 10, 0));
    pixels.show();
    delay(DELAYVAL);
  }
}
