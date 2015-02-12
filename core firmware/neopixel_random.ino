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

byte currentColors[PIXEL_COUNT][3];
byte initialColors[PIXEL_COUNT][3];
byte finalColors[PIXEL_COUNT][3];


Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

void setup() 
{
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
    
    //start with some random initial colors
    uint16_t i;
     for(i=0;i<strip.numPixels();i++){
        initialColors[i][0] = 0; //random(256);
        initialColors[i][1] = 0; //random(256);
        initialColors[i][2] = 0; //random(256);

        currentColors[i][0] = initialColors[i][0];
        currentColors[i][1] = initialColors[i][1];
        currentColors[i][2] = initialColors[i][2];
    }
    
    setStripColors();           //set the strip colors
    strip.show();               //show the colors
    
    getWhiteFinalColors();   //prepare the next keyframe colors (finalColors)
    fade(100);                //fade from the inital colors to the final colors
    getBlackFinalColors();
    fade(100);
}

void loop() 
{
    
    
    getRandomFinalColors();
    fade(100);
    
    //setStripColors();
    //strip.show();
    //delay(1000);
    
    //rainbow(10);
    /*
    uint8_t i;
    for(i=0;i<strip.numPixels();i++){
        strip.setPixelColor(i,67,0,112);
    }
    
    strip.show();
    delay(2000);
    */
    
    
    //getRandomFinalColors();
    //fade(1000);

    //randomize(random(10,5000));
}


void fade(uint16_t wait){
    uint16_t i, j;
    int8_t deltaColorIncrement[strip.numPixels()-1][3];
    int transitionFrames = 10;                              //how many transitional frames from one keyframe to the next
    
    //set intital colors as the current color
    for(i=0;i<strip.numPixels();i++){
        initialColors[i][0] = currentColors[i][0];
        initialColors[i][1] = currentColors[i][1];
        initialColors[i][2] = currentColors[i][2];
    }

    //loop through and find the increment for each of the n transition frames
    for(i=0;i<strip.numPixels();i++){
        deltaColorIncrement[i][0] = (finalColors[i][0] - initialColors[i][0])/transitionFrames;
        deltaColorIncrement[i][1] = (finalColors[i][1] - initialColors[i][1])/transitionFrames;
        deltaColorIncrement[i][2] = (finalColors[i][2] - initialColors[i][2])/transitionFrames;
    }
    

    for(i=0;i<transitionFrames;i++){                //for each frame
    
        for(j=0;j<strip.numPixels();j++){           //for each pixel
            currentColors[j][0] = currentColors[j][0] + deltaColorIncrement[j][0];          //set new frame r
            currentColors[j][1] = currentColors[j][1] + deltaColorIncrement[j][1];          //set new frame g
            currentColors[j][2] = currentColors[j][2] + deltaColorIncrement[j][2];          //set new frame b
        }
        
        setStripColors();        //set the colors of the entire strip
        strip.show();
        delay(wait);
    }
}


void randomize(uint32_t wait){
    uint16_t i;
    
    for(i=0;i<strip.numPixels();i++){
        currentColors[i][0] = random(256);
        currentColors[i][1] = random(256);
        currentColors[i][2] = random(256);

        strip.setPixelColor(i,currentColors[i][0],currentColors[i][1],currentColors[i][2]);
    }
    strip.show();
    delay(wait);
}

void getRandomFinalColors(){
    uint16_t i;
    
    for(i=0;i<strip.numPixels();i++){
        finalColors[i][0] = random(256);
        finalColors[i][1] = random(256);
        finalColors[i][2] = random(256);
    }
}

void getBlackFinalColors(){
    uint16_t i;
    
    for(i=0;i<strip.numPixels();i++){
        finalColors[i][0] = 0;//random(256);
        finalColors[i][1] = 0;//random(256);
        finalColors[i][2] = 0;//random(256);
    }
}

void getWhiteFinalColors(){
    uint16_t i;
    
    for(i=0;i<strip.numPixels();i++){
        finalColors[i][0] = 255;//random(256);
        finalColors[i][1] = 255;//random(256);
        finalColors[i][2] = 255;//random(256);
    }
}

void setStripColors(){
    uint16_t i;
    
    for(i=0;i<strip.numPixels();i++){           //for each pixel
        strip.setPixelColor(i,currentColors[i][0],currentColors[i][1],currentColors[i][2]);     //set the color
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