/*
  Made by Dari#3459 on discord
  Keyboard firmware for atmega32u4  based boards
*/

#include <HID-Project.h>
#include <Keypad.h>
#include <FastLED.h>

//Defining LED values
#define LED_TYPE WS2812B //Put the type of LED you're using
#define NUM_LEDS 60 //Put the amount of LED's you have here
#define LED_PIN 6 //Pin LED is going to connect to
CRGB leds[NUM_LEDS]; //initializing a block of memory to 
//choose your colour values for first solid colour
int RED_0 = 201;
int BLUE_0 = 50;
int GREEN_0 = 179;

//Defining pins/values, self explanatory
const int debounce = 200 //ms of debounce time, change to whatever
const byte ROWS = 2; //Rows, horizontal, edit for your own use
const byte COLS = 3; //Columns, vertical, edit for your own use
const int pinLed = 10; //capslock
const int pinLed2 = 14; //numlock
const int pinLed3 = 15; //scrolllock
byte rowPins[ROWS] = {9, 8}; //pin arrays
byte colPins[COLS] = {5, 4, 3};
const int message_strings = 6; //enter the amount of string entries you have 
const int functions = 6; //enter the amount of functions you have
bool LED_DelayState = false; //check if led clock is on or off, don't touch unless you know what you're doing.
long interval = 30; //how long we are going to be delaying the led clock
long previousMillis = 0; //previous millis value used for async functions

/* to change your keymap, edit this. valid numbers are 1-(amount of strings) for text, and (amount of strings + amount of functions) for functions. 
Simply follow the formulas to find which number you want to put. Function/String number can be found in the arrays. For functions, formula is 6 + function number
For strings, the formula is the String number
*/
char keymap[ROWS][COLS] { //Rows (Y) by Columns (X)
  	{'1', '2', '3'},
  	{'4', '5', '6'}
};

//to change your functions and messages, edit these. Just follow the formatting

//Text strings
String messages[] = {
  	"String1", 
  	"String2", 
  	"String3", 
  	"String4", 
  	"String5", 
  	"String6"
 };

//Functions, create/edit them here.
void sleep() {
  	Keyboard.press(KEY_LEFT_GUI);
  	Keyboard.press('x');
  	Keyboard.releaseAll();
  	Keyboard.press('u');
  	Keyboard.releaseAll();
  	Keyboard.press('s');
  	Keyboard.releaseAll();
}

void PsFill() {
  	Keyboard.press(KEY_LEFT_SHIFT);
  	Keyboard.press(KEY_F5);
  	Keyboard.releaseAll();
}

void VolumeUp() {
  	Keyboard.press(KEY_F24);
  	Keyboard.releaseAll();
}

void VolumeDown(){
  	Keyboard.press(KEY_F23);
  	Keyboard.releaseAll();
}
void RecordStart(){
  	Keyboard.press(KEY_F22);
  	Keyboard.releaseAll();
}

void RecordStop(){
  	Keyboard.press(KEY_F21);
  	Keyboard.releaseAll();
}
// Function array, order your functions here, not super useful but if you need it it's here. Read the keymap for a better explanation
void (*funcMacros[])() {
  	sleep, PsFill, VolumeUp, VolumeDown, Record, RecordStop
};

// anything after this is more technical, feel free to read

//Making the button array
Keypad customKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, ROWS, COLS);

//Making customkey the result of what the keypad library gets
int customKey = customKeypad.getKey();

//Main macro 
void Macro(int i) {
  	if (i <= message_strings) {
 		Keyboard.print(messages[i - 1]); //minus 1 because arrays are zero-indexed
  	}
  	else if (i <= (message_strings + functions)) { //output is under or equivalent to the maxmimum amount of valid entries (functions + strings), so that there's no memory errors.
    	funcMacros[i - (message_strings + 1)]();  /*Subtracting away the string numbers so that it correctly                                            finds the corresponding function in the array funcMacros. 
                                          Additionally subtracing one more than the number of strings
                                          because arrays are 0 indexed
										 */
  }
}

void setLedSolidColour(){
  	fill_solid(LEDS, NUM_LEDS, CRGB(RED_0, GREEN_0, BLUE_0))
  	FastLED.show(); 
}

void setup() {
  	pinMode(pinLed, OUTPUT);
  	pinMode(pinLed2, OUTPUT);
  	pinMode(pinLed3, OUTPUT);
  	Keyboard.begin(); //Initialize libraries
  	BootKeyboard.begin();
  	FastLED.addLeds<LED_TYPE, DATA_PIN>(LEDS, NUM_LEDS);
}


void loop() {
  //led indecators
  	if (BootKeyboard.getLeds()) {
    	digitalWrite(pinLed, (LED_CAPS_LOCK) ? HIGH : LOW); //basically, do something with a pin, here's the pin,                                                   check this variable (true, HIGH, false/else, LOW)
    	digitalWrite(pinLed2, (LED_NUM_LOCK) ? HIGH : LOW);
    	digitalWrite(pinLed3, (LED_SCROLL_LOCK) ? HIGH : LOW);
  	}

  	if (customKey) {
    	Macro(customKey); //calls macro function with i being what customkey gives
    	delay(debounce); //debounce
  	}
  	/*async led writes
  	essentially every millisecond it'll check when 30 milliseconds have passed since the last bool state change (first if statment).
	once it does, it'll change it. Note it's refreshing the LEDs every 60 milliseconds instead of 30 because it's sees it's false, changes it to true (30ms).
	then 30ms later, it'll check it, sees it's true, and set it. 30 + 30 = 60.
  	*/
  	unsigned long currentMillis = millis(); //check milliseconds since program started
  	
  	if(currentMillis - previousMillis > interval) {
  		previousMillis = currentMillis; //Save last clock cycle
  		if(!LED_DelayState){
  			LED_DelayState = !LED_DelayState; //if false, set true
  		}
  		else {
  			LED_DelayState = !LED_DelayState; //if true, set false and set the LEDs
  			setLedSolidColour();
  		}
  	}
}