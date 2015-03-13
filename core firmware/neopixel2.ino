#include "application.h"
//#include "spark_disable_wlan.h" (for faster local debugging only)
#include "neopixel/neopixel.h"

// IMPORTANT: Set pixel COUNT, PIN and TYPE
#define PIXEL_PIN D0
#define PIXEL_COUNT 4
#define PIXEL_TYPE WS2812B

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

int16_t httpRGB[6];					// used to hold 2 rgb values from the web API
int mode = 0;
uint32_t stripColors32[PIXEL_COUNT];
char APIStripColors32[200] = "Rebooted";

void setup() 
{
    Spark.function("generic", generic);
    Spark.function("off", offAPI);
    Spark.function("fuzz", policeAPI);
    Spark.function("gradient", gradientAPI);

    Spark.variable("mode", &mode, INT);
    Spark.variable("state", &APIStripColors32, STRING);

    Serial.begin(9600);
    strip.begin();
    strip.show(); // initialize all pixels to 'off'
    
    rainbow(10);
    off();
}

void loop() {
}

extern char* itoa(int a, char* buffer, unsigned char radix);

void updateAPIStripColors32(){
	// This function gets the current colors of the strip and 
	// populates the stripColors32[] array

	char buffer[10];		//buffer for itoa

	getColors32();			// get current strip colors and populate stripColors32
	strcpy(APIStripColors32,""); // blank out existing string

	for(int i=0;i<strip.numPixels();i++){
		strcat(APIStripColors32,itoa(stripColors32[i],buffer,10));	// convert the int to an 
		if(i < strip.numPixels()-1){		// don't put a comma at the last one
			strcat(APIStripColors32,",");
		}
	}
}


int generic(String inputString){
	byte tempReturn;
	String trimmedInputString = inputString.substring(1);	// trim off first charcter

	switch (inputString.substring(0,1).toInt()){			// switch based on first character
		case 0:
			off();
			Spark.publish("mode", "off");
			tempReturn = 0;
			break;
		case 1:
			rgbAPI(trimmedInputString);
			Spark.publish("mode", "rgb");
			tempReturn = 1;
			break;
		case 2:
			gradientAPI(trimmedInputString);
			Spark.publish("mode", "gradient");
			tempReturn = 2;
			break;
		case 3:
			rainbowAPI(trimmedInputString);
			Spark.publish("mode", "rainbow");
			tempReturn = 3;
			break;
		case 4:
			policeAPI(trimmedInputString);
			Spark.publish("mode", "fuzz");
			tempReturn = 4;
			break;
		case 5:
			sunsetAPI(trimmedInputString);
			Spark.publish("mode", "sunset");
			tempReturn = 5;
			break;
		case 6:
			animateShiftingAPI(trimmedInputString);
			Spark.publish("mode", "animate :: shifting");
			tempReturn = 6;
			break;
		case 7:
			setSinglePixelAPI(trimmedInputString);
			Spark.publish("mode", "single pixel set");
			tempReturn = 7;
			break;
		default:
			tempReturn = -1;
			break;
	}
	updateAPIStripColors32();	// set the spark variable string with strip colors
	mode = tempReturn;	// update the current spark variable mode
	return tempReturn;	// return with the mode number
}

void setSinglePixelAPI(String inputString){
	if(APIDataToIntArray(inputString) == 4){	// get values from API parser
												// IE if there aren't 4 values, don't do anything.
		strip.setPixelColor(httpRGB[0],	httpRGB[1],	httpRGB[2],	httpRGB[3]);
		strip.show();
	}
}

/*
void setRandomPixelsAPI(String inputString){
	setRandomPixels();
}
*/

void animateShiftingAPI(String inputString){
	uint32_t tempPixelColor;
	int loops = 1;								// defaults if none specified via API
	int frameTime = 50;

	if(APIDataToIntArray(inputString) == 2){	// get values from API parser
		frameTime = httpRGB[0];
		loops = httpRGB[1];
	}
	
	for(int k=0;k<loops;k++){					// run complete cycle n number of times
		for(int j=0;j<strip.numPixels();j++){			// run shift loop entire length of strip cycling back to original
			getColors32();
			tempPixelColor = stripColors32[0];			// save the first pixel
			for(int i=0;i<strip.numPixels();i++){		// shift pixel array
				stripColors32[i] = stripColors32[i+1];
			}
			stripColors32[strip.numPixels()-1] = tempPixelColor;	// set last pixel to the saved first one
			setMultipleColors32();									// apply shifted colors
			delay(frameTime);			// wait for next frame
		}
	}
}

void sunsetAPI(String inputString){
	int numReceived = APIDataToIntArray(inputString);

	if(numReceived == 1){
		sunset(httpRGB[0]);
	}else{
		sunset(20);
	}
}

void sunset(int time){
	for(int i=0;i<255;i++){
		strip.setBrightness(255-i);
		strip.show();
		delay(time);
	}
	off();
}

void makeGradient(byte r0, byte g0, byte b0, byte r1, byte g1, byte b1){
	// This takes two RGB colors and makes a nice gradient along the
	// length of the strip transitioning from the first to the last.
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

int APIDataToIntArray(String inputString){
	int tempNumberDataPointsReceived = 0;					// used to keep track of how many numbers we get from the API
    char * temp = new char[inputString.length()+1];     	// make a new character array the length of the input string
    inputString.toCharArray(temp,inputString.length()+1);   //copy the string to the character array
    char * tok;

    tok = strtok(temp,",");
    byte i = 0;

    while (tok!=NULL){
    	tempNumberDataPointsReceived ++;
        httpRGB[i] = atoi(tok);
        tok = strtok(NULL, ",");
        i++;
    }

    Serial.print("APIDataToIntArray :: Total API numbers received: ");
    Serial.println(tempNumberDataPointsReceived);

    return tempNumberDataPointsReceived;					// useful to know how many values in the array are good
}

int gradientAPI(String inputString){
	// This function takes a string of presumably rgb values IE "255,255,255,0,0,0"
	// from the web API, parses it into integers and enacts the gradient
	// Bonus: if < 6 rgb values are detected, the function makes a random gradient

    int numReceived = APIDataToIntArray(inputString);		// run APIDataToIntArray to get the rgb values from the API and
    														// it returns the total number of them (6 for a valid gradient)
    if(numReceived == 6){
	    makeGradient(httpRGB[0],httpRGB[1],httpRGB[2],httpRGB[3],httpRGB[4],httpRGB[5]);
	    Serial.print("gradientAPI :: Success!");
	    return 1;
	}else{
		Serial.print("gradientAPI :: Not enough values given, making random");
		makeGradient(random(256),random(256),random(256),random(256),random(256),random(256));
		return 2;
	}
}

int rainbowAPI(String inputString){
    getColors32();		// save the original colors for after the rainbow
    
    Serial.println("API :: rainbow");
    
    for(int i=0;i<5;i++){
        rainbow(10);
    }
    
    setMultipleColors32();			// go back to the original colors
    return 0;
}

int policeAPI(String inputString){
    getColors32();		// save the original colors for after the rainbow

    for(int j=0;j<2;j++){
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
    setMultipleColors32();			// go back to the original colors
    return 0;
}

int offAPI(String junk){
    Serial.println("API :: off");
    off();
    return 0;
}

int rgbAPI(String inputString){
	// Takes a string of data from the web API, parses it and displays the rgb color.
	// Bonus: if not enough integers are detected, makes a random color.

    int numReceived = APIDataToIntArray(inputString);		// run APIDataToIntArray to get the rgb values from the API and
    														// it returns the total number of them (6 for a valid gradient)
    if(numReceived == 3){
	    singleColor(httpRGB[0],httpRGB[1],httpRGB[2]);
	    Serial.print("singleColorAPI :: Success!");
	    return 1;
	}else{
		Serial.print("singleColorAPI :: Not enough values given: Random!");
		singleColor(random(256),random(256),random(256));
		return 2;
	}
}

int off(){
    Serial.println("turning off...");
    for(int i=0;i<strip.numPixels();i++){
        strip.setPixelColor(i,0,0,0);
    }
    strip.show();
    mode = 0;
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

void setSingleColor32(uint32_t color){
	// sets entire strip 1 color

    for(int i=0;i<strip.numPixels();i++){
        strip.setPixelColor(i,color);
    }
    strip.show();
}

void setMultipleColors32(){
	// sets the strip according to stripColors32[] array

	for(int i=0;i<strip.numPixels();i++){
		strip.setPixelColor(i,stripColors32[i]);
	}
	strip.show();
}

void getColors32(){	
	// simple function that fills up the stripColors32[] array
	// with the current colors in 32 bit merged format

	for(int i=0;i<strip.numPixels();i++){
		stripColors32[i] = strip.getPixelColor(i);
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