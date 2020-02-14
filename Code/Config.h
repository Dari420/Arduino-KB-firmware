/*
Configuration for the firmware, don't be afraid to add or remove anything. Need help? add me on discord @Dari#3459 (case sensitive)
*/

//LEDs
#define LED_TYPE WS2812B //Put the type of LED you're using
#define NUM_LEDS 60 //Put the amount of LED's you have here
#define LED_PIN 6 //Pin LED is going to connect to
#define interval 30 //half of the time you want between each led refresh cycle

//Colours, zero-indexed
#define RED_0 201
#define BLUE_0 50
#define GREEN_0 279

//Macros
#define Message_strings 6 //enter the amount of string entries you have 
#define Functions 6 //enter the amount of functions you have

//Keyswitch definitions
#define Debounce 200 //ms of debounce time, change to whatever
#define ROWS 2 //Rows, horizontal, edit for your own use
#define COLS 3 //Columns, vertical, edit for your own use
#define CapsLED 10 //Pin for capslock LED
#define NumLED 14 //Pin for numlock LED
#define ScrollLED 15 //Pin for scrolllock LED
