#include "application.h"
//#include "spark_disable_wlan.h" (for faster local debugging only)
//#include "neopixel/neopixel.h"

// IMPORTANT: Set pixel COUNT, PIN and TYPE
#define PIXEL_PIN D0
#define PIXEL_COUNT 21
#define PIXEL_TYPE WS2812B

//Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

#include "application.h"
//#include "spark_disable_wlan.h" (for faster local debugging only)
#include "neopixel/neopixel.h"

// IMPORTANT: Set pixel COUNT, PIN and TYPE
#define PIXEL_PIN D0
#define PIXEL_COUNT 21
#define PIXEL_TYPE WS2812B

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

int16_t httpRGB[6];					// used to hold 2 rgb values from the web API

void setup() 
{
	Spark.subscribe("sms", handler);

    Spark.function("rgb", parse);
    Spark.function("off", offAPI);
    //Spark.function("rainbow", rainbowAPI);
    Spark.function("fuzz", policeAPI);
    Spark.function("gradient", gradientAPI);

    Serial.begin(9600);
    strip.begin();
    strip.show(); // initialize all pixels to 'off'
    
    rainbow(10);
    off();
}

void loop() {
}

void handler(const char *event, const char *data){
	policeAPI("junk");
}


void makeGradient(byte r0, byte g0, byte b0, byte r1, byte g1, byte b1){
	float deltaR = ((r1-r0)/(PIXEL_COUNT-1));			// calculate deltas per pixel per color
	float deltaG = ((g1-g0)/(PIXEL_COUNT-1));
	float deltaB = ((b1-b0)/(PIXEL_COUNT-1));

	for(int i=0;i<PIXEL_COUNT;i++){						// set each pixel
		strip.setPixelColor(i,round(r0+(i*deltaR)),round(g0+(i*deltaG)),round(b0+(i*deltaB)));
	}
	strip.setPixelColor(0,r0,g0,b0);					// set the first and last pixel to workaround
	strip.setPixelColor(PIXEL_COUNT-1,r1,g1,b1);		// the rounding error
	strip.show();
}

int gradientAPI(String inputString){
    Serial.print("API :: Data received: ");
    char * temp = new char[inputString.length()+1];     // make a new character array the length of the input string
    inputString.toCharArray(temp,inputString.length()+1);   //copy the string to the character array
    char * tok;

    tok = strtok(temp,",");
    byte i = 0;

    while (tok!=NULL){
        httpRGB[i] = atoi(tok);
        tok = strtok(NULL, ",");
        i++;
    }

    makeGradient(httpRGB[0],httpRGB[1],httpRGB[2],httpRGB[3],httpRGB[4],httpRGB[5]);
    
    return httpRGB[0] + httpRGB[1] + httpRGB[2];      //return the RGB total value.
}

int rainbowAPI(String inputString){
    uint32_t originalColor = strip.getPixelColor(0);        // save the original color for after the rainbow
    
    Serial.println("API :: rainbow");
    
    for(int i=0;i<5;i++){
        rainbow(10);
    }
    
    singleColor32(originalColor);                           // go back to the original color
    return 0;
}

int policeAPI(String inputString){
    uint32_t originalColor = strip.getPixelColor(0);        // save the original color for after the rainbow

    for(int j=0;j<10;j++){
        for(int i=0;i<4;i++){
            singleColor(255,0,0);
            delay(100);
            off();
            delay(50);
        }
        for(int i=0;i<4;i++){
            singleColor(0,0,255);
            delay(100);
            off();
            delay(50);
        }
    }
    
    singleColor32(originalColor);                           // go back to the original color
    return 0;
}

int offAPI(String junk){
    Serial.println("API :: off");
    off();
    return 0;
}


int parse(String inputString){
    Serial.print("API :: Data received: ");
    char * temp = new char[inputString.length()+1];     // make a new character array the length of the input string
    inputString.toCharArray(temp,inputString.length()+1);   //copy the string to the character array
    char * tok;

    tok = strtok(temp,",");
    byte i = 0;

    while (tok!=NULL){
        httpRGB[i] = atoi(tok);
        tok = strtok(NULL, ",");
        i++;
    }
    
    Serial.print("R: ");
    Serial.print(httpRGB[0]);
    Serial.print(" G: ");
    Serial.print(httpRGB[1]);
    Serial.print(" B: ");
    Serial.println(httpRGB[2]);

    singleColor(httpRGB[0],httpRGB[1],httpRGB[2]);
    
    return httpRGB[0] + httpRGB[1] + httpRGB[2];      //return the RGB total value.
}


int off(){
    Serial.println("turning off...");
    for(int i=0;i<strip.numPixels();i++){
        strip.setPixelColor(i,0,0,0);
    }
    strip.show();
    return 0;
}


void singleColor(uint8_t r, uint8_t g, uint8_t b){
    Serial.print("singleColor R: ");
    Serial.print(r);
    Serial.print(" G: ");
    Serial.print(g);
    Serial.print(" B: ");
    Serial.println(b);
    
    for(int i=0;i<strip.numPixels();i++){
        strip.setPixelColor(i,r,g,b);
    }
    strip.show();
}

void singleColor32(uint32_t color){
    for(int i=0;i<strip.numPixels();i++){
        strip.setPixelColor(i,color);
    }
    strip.show();
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


uint32_t Wheel(byte WheelPos) {
	// Input a value 0 to 255 to get a color value.
	// The colours are a transition r - g - b - back to r.
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