// Random Strip & Random Pixel sketch for Arduino & NeoPixel Strips
// By: David R Ratliff
// November 2013
// Randomly assigns color values to random pixels in random order....at random. :)
// Created in response to post on Adafruit forums requesting this effect.
// Compiles to just under 3kB
// Written and tested on Adafruit's "Trinket" ATTiny85 Arduino platform
// Will work with most Arduino & compatible boards


#include <Adafruit_NeoPixel.h>    // Adafruit's NeoPixel Library

#define PIN 0

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(30, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() 
{
  randomPixelColor(1);

}

void randomPixelColor(int wait)
{
  int i= random(strip.numPixels());

  uint32_t c = random(2147483647);
  strip.setPixelColor(i, c);
  strip.show();
  delay(wait);

}    // end void randomPixelColor()

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) 
{
  for(uint16_t i=0; i<strip.numPixels(); i++) 
  {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}



