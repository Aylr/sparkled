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
byte httpRGB[3];
byte incomingR;
byte incomingG;
byte incomingB;


//int parse(String inputString);

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

void setup() 
{
    Spark.function("rgb", parse);
    httpRGB[0] = 0;
    httpRGB[1] = 255;
    httpRGB[2] = 0;
    
    strip.begin();
    strip.show(); // initialize all pixels to 'off'
    
    initializeSingleColor(255,0,0);        //start with all LEDS a single color
    
    getSingleFinalColor(0,255,0);          //prepare the next keyframe colors (finalColors)
    fade(10,100);                          //fade from the inital colors to the final colors
    getSingleFinalColor(0,0,255);
    fade(10,100);
    getSingleFinalColor(255,255,255);
    fade(10,100);
}

void loop() 
{
    getRandomFinalColors();
    fade(10,100);
    
    //rainbow(10);

    //getRandomFinalColors();
    //fade(1000);

    //randomize(random(10,5000));
}

int parse(String inputString){
    char * temp = new char[inputString.length()+1];
    char * tok;

    tok = strtok(temp,",");
    byte i = 0;

    while (tok!=NULL){
        httpRGB[i] = atoi(tok);
        tok = strtok(NULL, ",");
        i++;
    }
/*
    incomingR = atoi(tok);
    tok = strtok(NULL, ",");
    incomingG = atoi(tok);
    tok = strtok(NULL, ",");
    incomingB = atoi(tok);
*/
    //getSingleFinalColor(incomingR,incomingG,incomingB);
    getSingleFinalColor(httpRGB[0],httpRGB[1],httpRGB[2]);

    fade(10,100);
    delay(1000);
    //return 1;
    return httpRGB[1];
}

void fade(uint16_t wait, int transitionFrames){
    // wait: ms between frames, transitionFrames: number of frames between colors

    uint16_t i, j;
    int8_t deltaColorIncrement[strip.numPixels()-1][3];
    
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
        
        applyStripColors();         //set the colors of the entire strip
        strip.show();               //show/display that finished frame
        delay(wait);                //wait for next frame time
    }
}


void randomize(uint32_t wait){
    uint16_t i;
    
    for(i=0;i<strip.numPixels();i++){
        currentColors[i][0] = random(256);
        currentColors[i][1] = random(256);
        currentColors[i][2] = random(256);
        //strip.setPixelColor(i,currentColors[i][0],currentColors[i][1],currentColors[i][2]);
    }
    applyStripColors();
    strip.show();
    delay(wait);
}

void initializeSingleColor(uint8_t r, uint8_t g, uint8_t b){
    uint16_t i;

    for(i=0;i<strip.numPixels();i++){
        currentColors[i][0] = initialColors[i][0] = r;
        currentColors[i][1] = initialColors[i][1] = g;
        currentColors[i][2] = initialColors[i][2] = b;
    }
    applyStripColors();           //set the strip colors
    strip.show();               //show the colors
}

void getSingleFinalColor(uint8_t r, uint8_t g, uint8_t b){
    uint16_t i;
    for(i=0;i<strip.numPixels();i++){
        finalColors[i][0] = r;
        finalColors[i][1] = g;
        finalColors[i][2] = b;
    }
}

void getRandomFinalColors(){
    uint16_t i;
    
    for(i=0;i<strip.numPixels();i++){
        finalColors[i][0] = random(256);
        finalColors[i][1] = random(256);
        finalColors[i][2] = random(256);
    }
}

void applyStripColors(){
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