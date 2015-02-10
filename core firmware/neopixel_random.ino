/*
 * This is a minimal example, see extra-examples.cpp for a version
 * with more explantory documentation, example routines, how to 
 * hook up your pixels and all of the pixel types that are supported.
 *
 */

#include "application.h"
//#include "spark_disable_wlan.h" (for faster local debugging only)
#include "neopixel/neopixel.h"

// IMPORTANT: Set pixel COUNT, PIN and TYPE
#define PIXEL_PIN D0
#define PIXEL_COUNT 21
#define PIXEL_TYPE WS2812B

unsigned char currentColors[PIXEL_COUNT][3];
unsigned char finalColors[PIXEL_COUNT][3];





Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

void setup() 
{
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() 
{

    //strip.setPixelColor(0,255,255,255);
    //strip.show();
    //delay(500);
    

    //rainbow(10);
    randomNextColors();        //get the next colors
    
    randomize(random(10,5000));

}


void fade (uint16_t wait){
    uint16_t i;
    unsigned char deltaColorIncrement[strip.numPixels][3];
    int transitionFrames = 10;                              //how many transitional frames from one keyframe to the next
    
    //initialColors = [[255,255,255],[100,100,100]];          //note get the from the strip or global variable
    //finalColors = [[100,100,100],[255,255,255]];            //get this from somewhere.
    //deltaColorIncrement = currentColors = [[0,0,0],[0,0,0]];
    
    
    
    //loop through and find the increment for each of the n transition frames
    for(i=0;i<strip.numPixels();i++){
        deltaColorIncrement[i][0] = (finalColors[i][0] - initialColors[i][0])/transitionFrames;
        deltaColorIncrement[i][1] = (finalColors[i][1] - initialColors[i][1])/transitionFrames;
        deltaColorIncrement[i][2] = (finalColors[i][2] - initialColors[i][2])/transitionFrames;
    }
    
    currentColors = initialColors;                  //start with the initialColors
    
    for(i=0;i<transitionFrames;i++){                //for each frame
    
        for(i=0;i<strip.numPixels();i++){           //for each pixel
            currentColors[i][0] = currentColors[i][0] + deltaColorIncrement[i][0];          //set new frame r
            currentColors[i][1] = currentColors[i][1] + deltaColorIncrement[i][1];          //set new frame g
            currentColors[i][2] = currentColors[i][2] + deltaColorIncrement[i][2];          //set new frame b
            
            strip.setPixelColor(i,currentColors[i][0],currentColors[i][1],currentColors[i][2]);  //set each pixel color
        }
        strip.show();
        delay(wait);
    }
}


void randomize(uint32_t wait){
    uint16_t i;
    
    for(i=0;i<strip.numPixels();i++){
        //currentColors[i] = [random(256),random(256),random(256)];
        currentColors[i][0] = random(256);
        currentColors[i][1] = random(256);
        currentColors[i][2] = random(256);

        strip.setPixelColor(i,currentColors[i][0],currentColors[i][1],currentColors[i][2]);
    }
    strip.show();
    delay(wait);
}

void randomNextColors(){
    uint16_t i;
    
    for(i=0;i<strip.numPixels();i++){
        //currentColors[i] = [random(256),random(256),random(256)];
        finalColors[i][0] = random(256);
        finalColors[i][1] = random(256);
        finalColors[i][2] = random(256);
    }
}

void setColors(){
    uint16_t i;
    for(i=0,i<strip.numPixels();i++){
        strip.setPixelColor(i,)
    }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j=j+1) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}



