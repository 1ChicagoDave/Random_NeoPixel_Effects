// NeoPixel Mars Bars!
// By: David R Ratliff
// November 2013
// My semi-realistic Police-Light "Mars Bars" sketch tweaked, delayed, and re-colored for the holidays!
// Now Green and Red blinky randomness can light up your Christmas...and possibly cause a seizure.* 
//  
//                                                    (*Do not stare directly at flashinjg lights!)
//

// Please, do be careful. This version is very sensitive to any extra variables and/or conditionals (at least for Trinket!)
//  I may be pushing the limits of that little guy with this one.

#include <avr/power.h>

#include <Adafruit_NeoPixel.h>



#define PIN 0    // Pin that NeoPixel Strip is connected to. I use pin 0 on Adafruit Trinket

Adafruit_NeoPixel strip = Adafruit_NeoPixel(30, PIN, NEO_GRB + NEO_KHZ800); //first number controls the amount of pixels you have (add a few to help the "turnaround")

// *** Customizable variables *** 
// Change these to redefine parameters for 'random' variable selection
const int bright = 5;    //select 1 thru 10 ( this is multiplied by 25 to get brightest pixel value (from 0-255)
const int maxFlashes = 15;      // number of flashes of each pattern before generating new number of segments
int maxBurst = 14;          // number of pattern bursts before inverting the colors (One 'flash' == twice this number)
const int maxNumberOfSegments = 10;            // Maximum allowable number of segments to be generated - Larger numbers allow smaller segments
// this also defines size of array below
const int maxWait = maxNumberOfSegments*2;    //speed of leds (smaller segments allowed to flash faster for effect) - length of delay() in milliseconds
const int minWait = 1;                        // minimum allowable wait time
// ** END OF CUSTOMIZABLE VARIABLES **
//...of course you CAN change anything. Only those above are intended to be changed for my purposes - 

int node[maxNumberOfSegments];          // array to hold segment nodes
unsigned int numSegments = 2;                    // intial numer of segments (Leave this alone)
const int maxTailSize = 7;                // length of light "tail" including lead light
unsigned int tailSize = maxTailSize;
const int arraySize = maxTailSize + 1;    // zero-indexed array, and extra "0" at end

// Array to hold 'r' (RED) color values - Add more values array to lengthen 'tail' of lights
// Last value should be "0" to ensure pixels are turned off appropriately
// Only two colors are being used, but I left all three arrays here for easier customization and more color options
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
// This next line is for 5V Trinket (ATtiny85) only. Please remove it if using other microcontroller.
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);

  strip.begin();  // Start 'er up!
  strip.show(); // Initialize all pixels to 'off'
}    // end setup()


void loop()
{
  int flash = random(2, maxFlashes);            //  4 or 5 work nicely

  // Pick new number of segments
  numSegments = random(2, maxNumberOfSegments);   // Randomly choose how many segments in bar
  int wait = (random(minWait,maxWait));          // Choose delay() times
  maxBurst = random(2, numSegments);            // looks good at "3"

  // defining node (end pixels of segments) from number of segments defined
  for (uint8_t i = 0; i <= numSegments; i++)
  {
    node[i] = (i * (strip.numPixels() / numSegments));
  }    // end for  - numSegments - node array
  constrain (tailSize, 0, ((strip.numPixels() / numSegments)-1)); 
  // Run this many times before redifining variables
  for ( flash = 0 ; flash <= maxFlashes ; flash++)    
  {
    colorWipe(r[0], g[0], b[0], wait); 
  }    // end for (flash = ... -- Main function call--
  delay(20);
}    // end void loop()

void colorWipe(int R, int G, int B, int wait) 
{
  for (boolean inversePattern = 0; inversePattern <=1; inversePattern++)
  {
    for (int burst = 0 ; burst <= maxBurst ; burst++)
    {
      //    Run lights starting from pixel 0
      for (int segment = numSegments-1; segment >= 0; segment--)        // Example (2 segments -> segment=0, segment=1) 
        // for (int segment = 0; segment < numSegments; segment++)      // Gives slightly different results
      {
        // to set pixel colors in each segment
        for (int pixel = node[segment]; pixel < node[segment+1]; pixel++)    // Example (2 segments -> pixel=0, pixel=15 && pixel=15, pixel=30)
        {
          boolean Green;                    // Which color are we showing...?

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

          for (uint8_t tailPosition = 0; tailPosition <= tailSize; tailPosition++)      // Pixels within each segment
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
      for(uint8_t p = 1; p <= tailSize; p++)
      {
        for(uint8_t t = (strip.numPixels()-tailSize); t < (strip.numPixels()); t++)
        {
          uint8_t v = ((strip.numPixels()-t)+p);
          if (v > tailSize) v = tailSize;

          strip.setPixelColor(t, (r[v]), (g[v]), (b[v]));
        }    // end for (int t...

        strip.show();
        delay(wait);
        // No delay() here. It causes too long of a pause at ends of strip.
      }    // end for (int p...
      delay(wait);
      //    Run lights back towards 0
      for (uint8_t segment = 0; segment < numSegments; segment++)      // Example (2 segments -> segment=0, segment=1)
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
      for(uint8_t p = 1; p <= tailSize; p++)
      {
        for(int t = (tailSize); t < 0; t++)
        {
          uint8_t v = (t+p);
          if (v > tailSize) v = tailSize;

          strip.setPixelColor(t, (r[v]), (g[v]), (b[v]));
        }    // end for (int t...

        strip.show();
        delay(wait);
        // No delay() here. It causes too long of a pause at ends of strip.
      }    // end for (int p...
    }    // end for (int burst...
  }    // end for (boolean...
}  // end void colorWipe()

















