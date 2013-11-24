// NeoPixel Mars Bars!
// By: David R Ratliff
// November 2013
// One step closer in my trek for realistic police lights using Arduino, minimal hardware, and minimal code.

#include <Adafruit_NeoPixel.h>



#define PIN 0    // Pin that NeoPixel Strip is connected to. I use pin 0 on Adafruit Trinket

Adafruit_NeoPixel strip = Adafruit_NeoPixel(30, PIN, NEO_GRB + NEO_KHZ800); //first number controls the amount of pixels you have (add a few to help the "turnaround")
const int bright = 5;    //select 1 thru 10 ( this is multiplied by 25 to get brightest pixel value (from 0-255)
// const unsigned long patternTime = 1500;    // how many milliseconds beween pattern changes

const int maxFlashes = 5;      // number of flashes of each pattern before generating new number of segments
int maxBurst = 4;          // number of pattern bursts before inverting
const int maxNumberOfSegments = 7;            // Maximum allowable number of segments to be generated
// this also defines size of array below
const int maxWait = maxNumberOfSegments*2;    //speed of leds - length of delay() in milliseconds

int node[maxNumberOfSegments];          // array to hold segment nodes
int numSegments = 2;

const int maxTailSize = 7;                // length of light "tail" including lead light
int tailSize = maxTailSize;
const int arraySize = maxTailSize + 1;    // zero-indexed array, and extra "0" at end

// Array to hold 'r' (RED) color values - Add more values array to lengthen 'tail' of lights
// Last value should be "0" to ensure pixels are turned off appropriately
// Make sure the length of tail correlates to maxNumberOfSegments (*if segment length can be 5 pixels, don't want 7 pixel tail)
int r[arraySize] = {
  25*bright, 1*bright, 15*bright, 25*bright, 5*bright, 2*bright, 7*bright, 0};    
// Due to logarithmic nature of the human eye, each aditional brightness level should be slightly less than half the previous value for best effect.

// green color values  
int g[arraySize] = {
  25*bright, 2*bright, 20*bright, 14*bright, 5*bright, bright, 4*bright, 0}; 
// blue color values   
int b[arraySize] = {
  25*bright, 1*bright, 20*bright, 15*bright, 5*bright, bright, 3*bright, 0}; 


void setup()
{

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  // How many segements this time?  -- random number
  //numSegments = random(1, maxNumberOfSegments);   //<---  Move this to loop() with millis() timer


}    // end setup()


void loop()
{
  int flash = random(2, maxFlashes);    //  4 or 5 work nicely

  // Pick new number of segments
  numSegments = random(2, maxNumberOfSegments);   // Randomly choose how many segments in bar
  int wait = (random(0,numSegments)/3);
  maxBurst = random(2, numSegments);    // looks good at "3"

  // defining node (end pixels of segments) from number of segments defined
  for (int i = 0; i <= numSegments; i++)
  {
    node[i] = (i * (strip.numPixels() / numSegments));
  }    // end for  - numSegments - node array
  constrain (tailSize, 0, ((strip.numPixels() / numSegments)-1)); 
  for ( flash = 0 ; flash <= maxFlashes ; flash++)
  {
    colorWipe(r[0], g[0], b[0], wait); 
  }
  delay(20);

}


void colorWipe(int R, int G, int B, int wait) 
{
  for (boolean inversePattern = 0; inversePattern <=1; inversePattern++)
  {
    for (int burst = 0 ; burst <= maxBurst ; burst++)
    {
      //    Run lights starting from pixel 0
      for (int segment = numSegments-1; segment >= 0; segment--)      // Example (2 segments -> segment=0, segment=1) 
        // for (int segment = 0; segment < numSegments; segment++)      // Example (2 segments -> segment=0, segment=1)
      {
        // to set pixel colors in each segment
        for (int pixel = node[segment]; pixel < node[segment+1]; pixel++)    // Example (2 segments -> pixel=0, pixel=15 && pixel=15, pixel=30)
        {
          boolean Green;

          if (inversePattern == 1)
          {
            //  if odd, Green
            if((segment%2)!=0)  Green = true;
            //  if even, Red
            else if ((segment%2)==0)  Green = false;
          }    // end if (inverse...

          else if (inversePattern == 0)
          {
            //  if odd, Green
            if((segment%2)!=0)  Green = false;
            //  if even, Red
            else if ((segment%2)==0)  Green = true;
          }    // end if (!inverse....

          for (int tailPosition = 0; tailPosition <= tailSize; tailPosition++)      // Pixels within each segment
          {
            if (!Green)
            {
              strip.setPixelColor(pixel-tailPosition, (r[tailPosition]), (g[tailSize]), (b[tailSize]));    // Red ONLY
            }    // end if Red...
            if (Green)
            {
              strip.setPixelColor(pixel-tailPosition, (r[tailSize]), (g[tailPosition]), (b[tailSize]));    // Green ONLY
            }    // end if Green...

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
        delay(wait);
        // No delay() here. It causes too long of a pause at ends of strip.
      }
      delay(wait);
      //    Run lights back towards 0
      for (int segment = 0; segment < numSegments; segment++)      // Example (2 segments -> segment=0, segment=1)

        //     for (int segment = (numSegments-1); segment >= 0; segment--)      // Example (2 segments -> segment=0, segment=1)
      {
        // to set pixel colors in each segment
        for (int pixel = node[segment+1]; pixel > node[segment]; pixel--)    // Example (2 segments -> pixel=0, pixel=15 && pixel=15, pixel=30)
        {
          boolean Green;
          if (inversePattern == 1)
          {
            //  if odd, Green
            if((segment%2)!=0)  Green = true;
            //  if even, Red
            else if ((segment%2)==0)  Green = false;
          }    // end if (inverse...

          else if (inversePattern == 0)
          {
            //  if odd, Green
            if((segment%2)!=0)  Green = false;
            //  if even, Red
            else if ((segment%2)==0)  Green = true;
          }    // end if (!inverse....


          for (int tailPosition = tailSize; tailPosition >= 0; tailPosition--)      // Pixels within each segment
          {
            if (!Green)
            {
              strip.setPixelColor(pixel+tailPosition, (r[tailPosition]), (g[tailSize]), (b[tailSize]));    // Red ONLY
            }    // end if Red...
            if (Green)
            {
              strip.setPixelColor(pixel+tailPosition, (r[tailSize]), (g[tailPosition]), (b[tailSize]));    // Green ONLY
            }    // end if Green...

          }    // end for (int tailPosition...
          strip.show();
          // delay(wait); 
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
        delay(wait);
        // No delay() here. It causes too long of a pause at ends of strip.
      }
    }
  }
}  // end void colorWipe()
















