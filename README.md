# SparkLED

An easy way to use your Spark Core with Adafruit NeoPixels (or compatible RGB LEDs).

## Hardware & Software Setup

- Wire up your NeoPixels as specified on the [Adafruit NeoPixel Uberguide](https://learn.adafruit.com/adafruit-neopixel-uberguide?view=all)
	- If you are only using a few NeoPixels, you can probably power them directly from the Spark's +5V and GND pins. I've had good luck up to about 20 pixels this way.
- Customize three firmware variables to match your setup
	1. **digital pin for neopixel data** #define PIXEL_PIN D0
	2. **number of NeoPixels in strip** #define PIXEL_COUNT 4
	4. **pixel type** #define PIXEL_TYPE WS2812B
- Install this firmware on your spark core.
	- If using the web IDE be sure to include the excellent "NEOPIXEL" library via the "libraries" tab.


## Firmware Features

- A Spark.variable **mode**: with the current **mode** as an integer
- A Spark.variable **state**: which returns a string with the current color of each pixel in [merged 32bit format](https://forums.adafruit.com/viewtopic.php?f=47&p=332327)
	- **Note:** The spark clould limits this is limited to [622 bytes](http://docs.spark.io/firmware/#spark-variable), so if you have more than 68 NeoPixels, this will only return the first 68 (or so).
- A Spark.function **generic**: This is the main API handler that accepts a string as an input. **The first character of the string determines the mode**, with subsequent comma-separated integers the optional inputs for a given mode:
	- **0** Turns the strip off.
	- **1** Sets the entire strip to a single color.
		- If no RGB color is specified, it picks a random color.
		- Examples: (r,g,b)
			- To set a nice red color: "1255,0,0"
			- White: "1255,255,255"
			- Cyan: "10,255,255"
	- **2** Takes two comma separated colors (r,g,b,r,g,b), one for each end of the strip and creates a nice gradient across the strip.
		- If no RGB color is specified, it picks a random color for each side.
		- Examples:
			- random gradient: "2"
			- Green to Blue: "20,255,0,0,0,255"
			- White to purple: "2255,255,255,255,0,255"
	- **3** Plays the built-in rainbow animation. Accepts no input.
		- Example:
			- rainbow animation: "3"
	- **4** Plays an extrememly irritating police-flasher animation. Accepts no input.
		- Example:
			- police/fuzz animation: "4"
	- **5** Sunset mode: Not currently working...
	- **6** Animates the current strip colors by shifting the colors one by one until it has returned back to it's original state. That is 1 "cycle".
		- Example:
			- slow (1000 ms/frame) single cycle: "61000,1"
			- fast (50 ms/frame) for 100 cycles: "650,100";
	- **7** Sets a single pixel color. Accepts a pixel number and r,g,b value. Fun for custom setup.
		- Examples:
			- set pixel 0 white: "70,255,255,255"
			- set pixel 33 purple: "733,255,0,255"
			- set pixel 12 dim cyan: "712,0,50,50"
- Note that it is easy to add new modes by adding an additional "case" to the "generic function"