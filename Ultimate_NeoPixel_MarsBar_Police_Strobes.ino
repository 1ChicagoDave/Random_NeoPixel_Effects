
// NeoPixel Mars Bars!
// By: David R Ratliff
// November 2013
// One step closer in my trek for realistic police lights using Arduino, minimal hardware, and minimal code.

#include <Adafruit_NeoPixel.h>

#define PIN 0																 // Pin that NeoPixel Strip is connected to. I use pin 0 here for implementation on Adafruit Trinket

Adafruit_NeoPixel strip = Adafruit_NeoPixel(30, PIN, NEO_GRB + NEO_KHZ800); //first number controls the amount of pixels you have (add a few to help the "turnaround")
const int bright = 10;													    //select 1 thru 10 ( this is multiplied by 25 to get brightest pixel value (from 0-255)
const int minFlashes = 2													// minimum number of flashes/cycles of each pattern before generating new number of segments
const int maxFlashes = 8;												    // maximum number of flashes/cycles of each pattern before generating new number of segments
int maxBurst = 4;														    // number of pattern bursts before inverting within each flash/cycle
const int maxNumberOfSegments = 8;										    // Maximum allowable number of segments to be generated
																			// this also defines size of node[] array below
const int maxWait = maxNumberOfSegments;									//maximum delay() time in milliseconds - related to number of segments

int node[maxNumberOfSegments];												// array to hold segment nodes
int numSegments = 2;														// initial number of segments

const int maxTailSize = 7;												    // length of light "tail" including lead light
int tailSize = maxTailSize;													// initial tailsize - this will vary below

const int arraySize = maxTailSize + 1;    // zero-indexed array for holding brightness values of colors below, and extra "0" at end

// Array to hold 'r' (RED) color values - Add more values array to lengthen 'tail' of lights
// Last value should be "0" to ensure pixels are turned off appropriately
// Make sure the length of tail correlates to maxNumberOfSegments (*if segment length can be 5 pixels, don't want 7 pixel tail)
int r[arraySize] = {
25*bright, 20*bright, 10*bright, 4*bright, 2*bright, bright, bright, 0};
// Due to logarithmic nature of the human eye, each additional brightness level should be slightly less than half the previous value for best effect.

// green color values - NOT USED HERE -- WILL be utilized in future multi-color version of sketch
//** Remove in future version!	
// - BE CAREFUL CHANGIONG THESE - Check for loop containing "strip.setPixelColor" first!
int g[arraySize] = {
25*bright, 20*bright, 10*bright, 4*bright, 2*bright, bright, bright, 0};
// blue color values
int b[arraySize] = {
25*bright, 20*bright, 10*bright, 4*bright, 2*bright, bright, bright, 0};
void setup()
{

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

}			// end setup()


void loop()
{

 int flash = random(minFlashes, maxFlashes);

 // Pick new number of segments
 numSegments = random(2, maxNumberOfSegments);   // Randomly choose how many segments in bar
 int wait = random(0,numSegments)/3;
 maxBurst = random(3, numSegments);

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
	for (boolean inversePattern = 0; inversePattern <=1; inversePattern++)				// Alternates each segment between Red & Blue
	{
		for (int burst = 0 ; burst <= maxBurst ; burst++)								// Let's run through a few times before inverting color (red/blue)
		{
			//    Run lights starting from pixel 0
			for (int segment = numSegments-1; segment >= 0; segment--)      // Example (2 segments -> segment=0, segment=1)
			// for (int segment = 0; segment < numSegments; segment++)      // Example (2 segments -> segment=0, segment=1)
			{
				// to set pixel colors in each segment
				for (int pixel = node[segment]; pixel < node[segment+1]; pixel++)    // Example (2 segments -> pixel=0, pixel=15 && pixel=15, pixel=30)
				{
					boolean Blue;										// Will the segment be BLUE? ... or RED?  ... let's find out!

					if (inversePattern == 1)
					{
						//  if odd, Blue
						if((segment%2)!=0)  Blue = true;
						//  if even, Red
						else if ((segment%2)==0)  Blue = false;
					}    // end if (inverse...

					else if (inversePattern == 0)
					{
						//  if odd, Blue
						if((segment%2)!=0)  Blue = false;
						//  if even, Red
						else if ((segment%2)==0)  Blue = true;
					}    // end if (!inverse....

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
					boolean Blue;
					if (inversePattern == 1)
					{
						//  if odd, Blue
						if((segment%2)!=0)  Blue = true;
						//  if even, Red
						else if ((segment%2)==0)  Blue = false;
					}    // end if (inverse...

					else if (inversePattern == 0)
					{
						//  if odd, Blue
						if((segment%2)!=0)  Blue = false;
						//  if even, Red
						else if ((segment%2)==0)  Blue = true;
					}    // end if (!inverse....


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

