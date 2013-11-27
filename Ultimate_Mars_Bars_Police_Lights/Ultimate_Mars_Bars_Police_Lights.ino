#include <Adafruit_NeoPixel.h>



#define PIN 0    // Pin that NeoPixel Strip is connected to. I use pin 0 on Adafruit Trinket

Adafruit_NeoPixel strip = Adafruit_NeoPixel(30, PIN, NEO_GRB + NEO_KHZ800); //first number controls the amount of pixels you have (add a few to help the "turnaround")
const int bright = 10;    //select 1 thru 10 ( this is multiplied by 25 to get brightest pixel value (from 0-255)
const int wait = 10;    //speed of leds - length of delay() in milliseconds

const int maxNumberOfSegments = 8;            // Maximum allowable number of segments to be generated
// this also defines size of array below

int node[maxNumberOfSegments];          // array to hold segment nodes
int numSegments = 6;

const int tailSize = 7;                // length of light "tail" including lead light
const int arraySize = tailSize + 1;    // zero-indexed array, and extra "0" at end

// Array to hold 'r' (RED) color values - Add more values array to lengthen 'tail' of lights
// Last value should be "0" to ensure pixels are turned off appropriately
// Make sure the length of tail correlates to maxNumberOfSegments (*if segment length can be 5 pixels, don't want 7 pixel tail)
int r[arraySize] = {
  25*bright, 20*bright, 10*bright, 4*bright, 2*bright, bright, bright, 0};    
// Due to logarithmic nature of the human eye, each aditional brightness level should be slightly less than half the previous value for best effect.

// green color values  
int g[arraySize] = {
  25*bright, 20*bright, 10*bright, 4*bright, 2*bright, bright, bright, 0}; 
// blue color values   
int b[arraySize] = {
  25*bright, 20*bright, 10*bright, 4*bright, 2*bright, bright, bright, 0}; 

void setup()
{
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  // How many segements this time?  -- random number
  //numSegments = random(1, maxNumberOfSegments);   //<---  Move this to loop() with millis() timer

  // defining node (end pixels of segments) from number of segments defined
  for (int i = 0; i <= numSegments; i++)
  {
    node[i] = (i * (strip.numPixels() / numSegments));
  }    // end for  - numSegments - node array
}    // end setup()

void loop()
{
  colorWipe(r[0], g[0], b[0], wait); 
}


void colorWipe(int R, int G, int B, int wait) 
{
  //    Run lights starting from pixel 0
  for (int segment = numSegments-1; segment >= 0; segment--)      // Example (2 segments -> segment=0, segment=1)
  {
    // to set pixel colors in each segment
    for (int pixel = node[segment]; pixel < node[segment+1]; pixel++)    // Example (2 segments -> pixel=0, pixel=15 && pixel=15, pixel=30)
    {
      boolean Blue;
      //  if odd, Blue
      if((segment%2)!=0)  Blue = true;
      //  if even, Red
      else if ((segment%2)==0)  Blue = false;

      for (int tailPosition = 0; tailPosition <= tailSize; tailPosition++)      // Pixels within each segment
      {
        if (!Blue)
        {
          strip.setPixelColor(pixel-tailPosition, (r[tailPosition]), (g[tailSize]), (b[tailSize]));    // Red ONLY
        }    // end if Red...
        if (Blue)
        {
          strip.setPixelColor(pixel-tailPosition, (r[tailSize]), (g[tailSize]), (b[tailPosition]));    // Blue ONLY
        }    // end if Blue...

      }    // end for (int tailPosition...
      strip.show();
      delay(wait); 
    }    // end for (int pixel...

  }    // end for (int segment...

  // Now, shrink the tail....
  for(int p = 1; p <= tailSize; p++)
  {
    for(int t = (strip.numPixels()-tailSize); t < (strip.numPixels()); t++)
    {
      int v = ((strip.numPixels()-t)+p);
      if (v > tailSize) v = tailSize;

      strip.setPixelColor(t, (r[v]), (g[v]), (b[v]));
    }

    strip.show();
    // No delay() here. It causes too long of a pause at ends of strip.
  }
  //    Run lights back towards 0
  for (int segment = 0; segment < numSegments; segment++)      // Example (2 segments -> segment=0, segment=1)
  {
    // to set pixel colors in each segment
    for (int pixel = (node[segment+1]-1); pixel >= node[segment]; pixel--)    // Example (2 segments -> pixel=0, pixel=15 && pixel=15, pixel=30)
    {
      boolean Blue;
      //  if odd, Blue
      if((segment%2)!=0)  Blue = true;
      //  if even, Red
      else if ((segment%2)==0)  Blue = false;

      for (int tailPosition = tailSize; tailPosition >= 0; tailPosition--)      // Pixels within each segment
      {
        if (!Blue)
        {
          strip.setPixelColor(pixel+tailPosition, (r[tailPosition]), (g[tailSize]), (b[tailSize]));    // Red ONLY
        }    // end if Red...
        if (Blue)
        {
          strip.setPixelColor(pixel+tailPosition, (r[tailSize]), (g[tailSize]), (b[tailPosition]));    // Blue ONLY
        }    // end if Blue...

      }    // end for (int tailPosition...
      strip.show();
      delay(wait); 
    }    // end for (int pixel...

  }    // end for (int segment...
  // Now, shrink the tail....
  for(int p = 1; p <= tailSize; p++)
  {
    for(int t = (tailSize); t < 0; t++)
    {
      int v = (t+p);
      if (v > tailSize) v = tailSize;

      strip.setPixelColor(t, (r[v]), (g[v]), (b[v]));
    }

    strip.show();
    // No delay() here. It causes too long of a pause at ends of strip.
  }
}




