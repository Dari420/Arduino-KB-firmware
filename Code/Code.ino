/*
  	Made by Dari#3459 on discord
  	Keyboard firmware for HID capable arduino boards
*/

#include <HID-Project.h>
#include <Keypad.h>
#include <FastLED.h>

/*
Configuration for the firmware, don't be afraid to add or remove anything. Need help? add me on discord @Dari#3459 (case sensitive)
*/

//LEDs
#define LED_TYPE WS2812B //Put the type of LED you're using
#define NUM_LEDS 60 //Put the amount of LED's you have here
#define LED_PIN 6 //Pin LED is going to connect to
#define interval 30 //half of the time you want between each led refresh cycle\
#define NUM_MODES 2 //define the amount of led modes you have
#define LED_TOGGLE 12 //pin for led cycle toggles

//Colour presets, zero-indexed
#define RED_0 201
#define GREEN_0 279
#define BLUE_0 50

#define RED_1 255
#define GREEN_1 255
#define BLUE_1 255

//Macros
#define Message_strings 6 //enter the amount of string entries you have 
#define Functions 6 //enter the amount of functions you have

/*
  Keyswitch definitions
*/

//Row pins, add them or remove them however you'd like. Just update the matrix on line 24
#define ROWPIN1 9 //first row pin
#define ROWPIN2 8 //second row pin

//Column pins, add them or remove them however you'd like. Just update the matrix on line 25
#define COLPIN1 5 //first column pin
#define COLPIN2 4 //second column pin//first column pin
#define COLPIN3 3 //third column pin

#define Debounce 200 //ms of debounce time, change to whatever
#define ROWS 2 //Rows, horizontal, edit for your own use
#define COLS 3 //Columns, vertical, edit for your own use
#define CapsLED 10 //Pin for capslock LED
#define NumLED 14 //Pin for numlock LED
#define ScrollLED 15 //Pin for scrolllock LED

//LED cycle
unsigned long currentMillis = millis(); //check milliseconds since program started
CRGB leds[NUM_LEDS]; //initializing a block of memory to store the LEDS
long previousMillisLED = 0; //previous millis value used for async functions
int currentLEDmode = 0;

/* 
    to change your keymap, edit this. valid numbers are 1-(amount of strings) for text, and (amount of strings + amount of functions) for functions. 
    Simply follow the formulas to find which number you want to put. Function/String number can be found in the arrays. For functions, formula is 6 + function number
    For strings, the formula is the String number
*/
char keymap[ROWS][COLS] { //Rows (Y) by Columns (X)
  	{'1', '2', '3'},
  	{'4', '5', '6'}
};

/*
	Pins for your matrix, add/edit them here. You may also remove them.
	Need help? https://www.arduino.cc/reference/en/language/variables/data-types/array/
*/
byte rowPins[ROWS] = {ROWPIN1, ROWPIN2}; 
byte colPins[COLS] = {COLPIN1, COLPIN2, COLPIN3};
	

/*******************************************************************************************
	|						  NOT CONFIGURABLE, SKIP                                                     |
	******************************************************************************************
*/
//customKey intilization

//Making the button array
Keypad customKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, ROWS, COLS);

//Making customkey the result of what the keypad library gets
int customKey = customKeypad.getKey();

/*******************************************************************************************
	|						NOT CONFIGURABLE ^ (unless you know what you're doing, then go ahead :^) )   |
  ******************************************************************************************



/*
	To change your function and message macros, edit these. Just follow the formatting.
	Need help? https://www.arduino.cc/en/Reference/KeyboardModifiers and https://github.com/NicoHood/HID/wiki/Keyboard-API
*/

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

void VolumeDown() {
  	Keyboard.press(KEY_F23);
  	Keyboard.releaseAll();
}
void RecordStart() {
  	Keyboard.press(KEY_F22);
  	Keyboard.releaseAll();
}

void RecordStop() {
  	Keyboard.press(KEY_F21);
  	Keyboard.releaseAll();
}
/*
	Don't forget to include the function here!
	Function array, order your functions here so you can use them in the keymap above
*/
void (*funcMacros[])() {
  	sleep, PsFill, VolumeUp, VolumeDown, RecordStart, RecordStop
};

/*
	LED functions. Don't forget to include them in the array!
*/

void setLEDPurple() {
  	fill_solid(LEDS, NUM_LEDS, CRGB(RED_0, GREEN_0, BLUE_0))
  	FastLED.show(); 
}

void setLEDWhite() {
    fill_solid(LEDS, NUM_LEDS, CRGB(RED_1, GREEN_1, BLUE_1))
    FastLED.show(); 
}

//LED mode array, order your functions in the order you want them cycled through.
void (*LED_MODES[])() {
    setLEDPurple, setLEDWhite
};

// anything after this is more technical, feel free to read

//Main macro 
void Macro(int i) {
  	if (i <= Message_strings) {
 		Keyboard.print(messages[i - 1]); //minus 1 because arrays are zero-indexed
  	}
  	else if (i <= (Message_strings + Functions)) { //output is under or equivalent to the maxmimum amount of valid entries (functions + strings), so that there's no memory errors.
    	funcMacros[i - (Message_strings + 1)](); 	/*Subtracting away the string numbers so that it correctly finds the corresponding function in the array funcMacros. 
                                          		Additionally subtracing one more than the number of strings
                                          		because arrays are 0 indexed
										 */
  }
}

void LED_CYCLE (int cycle) {
    /*
        async led writes
        essentially every millisecond it'll check when 30 milliseconds have passed since the last refresh. Doesn't use delay() to pause the entire script
     */
    if(currentMillis - previousMillisLED > interval) {
        previousMillisLED = currentMillis; //Save last clock cycle
        LED_MODES[cycle]();
    }
}
void setup() {
    pinMode(LED_TOGGLE, INPUT);
  	pinMode(CapsLED, OUTPUT);
  	pinMode(NumLED, OUTPUT);
  	pinMode(ScrollLED, OUTPUT);
  	Keyboard.begin(); //Initialize libraries
  	BootKeyboard.begin();
  	FastLED.addLeds<LED_TYPE, DATA_PIN>(LEDS, NUM_LEDS);
}


void loop() {
  //led indicators
  	if (BootKeyboard.getLeds()) {
    	digitalWrite(CapsLED, (LED_CAPS_LOCK) ? HIGH : LOW); //basically, do something with a pin, here's the pin, check this variable (true, HIGH, false/else, LOW)
    	digitalWrite(NumLED, (LED_NUM_LOCK) ? HIGH : LOW);
    	digitalWrite(ScrollLED, (LED_SCROLL_LOCK) ? HIGH : LOW);
  	}
    if (customkey) { //async writes
        if (currentMillis - previousMillisLED > Debounce) {
            previousMillisLED = currentMillis; //Save last clock cycle
            Macro(customKey); //calls macro function with i being what customkey gives
        }
        else {
            ();
        }
    }

    if (LED_TOGGLE == HIGH) {
        if (currentMillis - previousMillisLED > Debounce) {
            previousMillisLED = currentMillis; //Save last clock cycle
            if (currentLEDmode < NUM_MODES) {
            currentLEDmode = currentLEDmode + 1;
            }
            else {
                currentLEDmode = 0;
            }
        }
        else {
            ();
        }
    }
    LED_CYCLE(currentLEDmode); //Calling cycler to refresh what mode it's on
}
