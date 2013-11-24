#include <Adafruit_NeoPixel.h>

// David R Ratliff
// Oct 27, 2013
// Modified from my all-red "Ultimate NeoPixel KnightRider_Cylon_Sweep" sketch
// This generates a sweet Cylon sweep, but randomly assigns a new color value 
// at each turn-around 
//    --   (only need to change a few values up top & the rest reference them)
// This will work (and was designed for) Adafruit's "Trinket" ATTiny85 Arduino platform
// Values you need to set/adjust
//  - PIN - Which pin the NeoPixel strip is connected to
// -  number of pixels
// - "maxMod" - larger numbers will offer more variety of random colors
//   .....but may lead to even dimmer lighting values

#define PIN 0    // Pin that NeoPixel Strip is connected to. I use pin 0 on Adafruit Trinket

Adafruit_NeoPixel strip = Adafruit_NeoPixel(30, PIN, NEO_GRB + NEO_KHZ800); //first number controls the amount of pixels you have (add a few to help the "turnaround")
const int bright = 10;    //select 1 thru 10 ( this is multiplied by 25 to get brightest pixel value (from 0-255)
const int wait = 45;    //speed of leds - length of delay() in milliseconds
const int tailSize = 7;    // length of light "tail" including lead light
const int arraySize = tailSize + 1;

// Array to hold 'r' (RED) color values - Add more values array to lengthen 'tail' of lights
// Last value should be "0" to ensure pixels are turned off appropriately
int r[arraySize] = {
  25*bright, 20*bright, 10*bright, 4*bright, 2*bright, bright, bright, 0};    
// Due to logarithmic nature of the human eye, each aditional brightness level should be slightly less than half the previous value for best effect.

// green color values  
int g[arraySize] = {
  25*bright, 20*bright, 10*bright, 4*bright, 2*bright, bright, bright, 0}; 
// blue color values   
int b[arraySize] = {
  25*bright, 20*bright, 10*bright, 4*bright, 2*bright, bright, bright, 0}; 

// color value modifiers
int modR = 0;
int modG = 0;
int modB = 0;
//  The maximum number you want the color values divided by to create "random" values
int maxMod = 5;    // the larger number, the dimmer pixels may be

void setup() 
{
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}
void loop() 
{
  colorWipe(r[0], g[0], b[0], wait);

}    // end void loop()  

void colorWipe(int R, int G, int B, int wait) 
{
    getRandVal(maxMod);    // create color modifiers for this segment
  //    Run lights starting from pixel 0
  for(int i=0; i<strip.numPixels(); i++) 
  {
    for (int q = 0; q <= tailSize; q++)
    {
      strip.setPixelColor(i-q, (r[q]/modR), (g[q]/modG), (b[q]/modB));
    }    // end for (int q...
    strip.show();
    delay(wait); 
  }    // end for (int i...

  // Now, shrink the tail....
  for(int p = 1; p <= tailSize; p++)
  {
    for(int t = (strip.numPixels()-tailSize); t < (strip.numPixels()-2); t++)
    {
      int v = ((strip.numPixels()-t)+p);
      if (v > tailSize) v = tailSize;

      strip.setPixelColor(t, (r[v]/modR), (g[v]/modG), (b[v]/modB));
    }

    strip.show();
    // No delay() here. It causes too long of a pause at ends of strip.
  }

    getRandVal(maxMod);    // create color modifiers for this segment


  //      Run lights back to pixel 0
  for(int i=(strip.numPixels()-1); i >= 0; i--) 
  {
    for (int q = 0; q <= tailSize; q++)
    {
      strip.setPixelColor(i+q, (r[q]/modR), (g[q]/modG), (b[q]/modB));
      // strip.setPixelColor(i+4, r[4], g0, b0);
    }    // end for (int q...  
    strip.show();
    delay(wait); 
  }    // end for (int i...  

  // Now, shrink the tail....
  for(int p = 1; p <= tailSize; p++)
  {
    for(int t = tailSize; t >= 2; t--)
    {
      int v = (t+p);
      if (v > tailSize) v = tailSize;

      strip.setPixelColor(t, (r[v]/modR), (g[v]/modG), (b[v]/modB));
    }

    strip.show();
  }

}    // end void colorWipe()

void getRandVal(int range)
{
  modR = random(1, range);
  modG = random(1, range);
  modB = random(1, range);

}    // end void getRandVal








