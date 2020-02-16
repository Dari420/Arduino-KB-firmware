/*
  	Made by Dari#3459 on discord
  	Keyboard firmware for HID capable arduino boards
*/

#include <HID-Project.h>
#include <Keypad.h>
#include <FastLED.h>
#include <config.h>

/* to change your keymap, edit this. valid numbers are 1-(amount of strings) for text, and (amount of strings + amount of functions) for functions. 
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
byte rowPins[ROWS] = {9, 8}; 
byte colPins[COLS] = {5, 4, 3};

//LED cycle
CRGB leds[NUM_LEDS]; //initializing a block of memory to store the LEDS
bool LED_DelayState = false; //check if led clock is on or off, don't touch unless you know what you're doing.
long previousMillis = 0; //previous millis value used for async functions
int currentLEDmode = 0;
	

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
  	sleep, PsFill, VolumeUp, VolumeDown, Record, RecordStop
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
            //put your solid led colour functions here
            LED_MODES[cycle]();
        }
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

  	if (customKey) {
    	Macro(customKey); //calls macro function with i being what customkey gives
    	delay(Debounce); //debounce5
  	}

    if (LED_TOGGLE == HIGH) {
        if (currentLEDmode < NUM_MODES) {
            currentLEDmode = currentLEDmode + 1;
        }
        else {
            currentLEDmode = 0;
        }
    }
    LED_CYCLE(currentLEDmode);
}