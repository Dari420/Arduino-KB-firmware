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
