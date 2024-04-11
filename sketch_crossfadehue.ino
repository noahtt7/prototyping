#include "RGBConverter.h"
#include <CapacitiveSensor.h>

const boolean COMMON_ANODE = false; 

const int RGB_RED_PIN = 3;
const int RGB_GREEN_PIN  = 5;
const int RGB_BLUE_PIN  = 6;
const int OUTPUT_PIEZO_PIN = 9;
const int BUTTON_INPUT_PIN = 2;
const int PHOTOCELL_INPUT_PIN = A0;
const int LOFI_INPUT_PIN = A1;
const int DELAY_INTERVAL = 50; // interval in ms between incrementing hues
const byte MAX_RGB_VALUE = 255;
const int SOUND_DURATION_MS = 500;

float _hue = 0; //hue varies between 0 - 1
float _step = 0.001f;

int mode = 0;
int buttonValue = 0;

const boolean PHOTOCELL_IS_R2_IN_VOLTAGE_DIVIDER = true;

RGBConverter _rgbConverter;

void setup() {
  // Set the RGB pins to output
  pinMode(RGB_RED_PIN, OUTPUT);
  pinMode(RGB_GREEN_PIN, OUTPUT);
  pinMode(RGB_BLUE_PIN, OUTPUT);
  pinMode(OUTPUT_PIEZO_PIN, OUTPUT);

  pinMode(BUTTON_INPUT_PIN, INPUT_PULLUP);
  pinMode(PHOTOCELL_INPUT_PIN, INPUT);

  Serial.begin(9600);   
}

void loop() {
  
  buttonValue = digitalRead(BUTTON_INPUT_PIN);
  int photocellVal = analogRead(PHOTOCELL_INPUT_PIN);


  photocellVal = 1023 - photocellVal;

  int LEDBrightness = map(photocellVal, 0, 1023, 0, 255);
  float LED = LEDBrightness / 100.0;

  // Ensures the LED is never turned completely off
  if (abs(LED - 0.3) == 0.0f) {
    LED = 0.31;
  } 

  float LEDVal = (1.0 - abs(LED - 0.3));

  if (LEDVal <= 0.0) {
    LEDVal = 0.01;
  }
  if (LEDVal >= 0.5) {
    LEDVal =  0.5;
  }


   int lofiVal = analogRead(LOFI_INPUT_PIN);
  //Serial.println(lofiVal);
   
  
  // Change modes
  if (buttonValue == LOW) {
    mode = mode + 1;
    if (mode != 3) {  
      //analogWrite(OUTPUT_PIEZO_PIN, 0);
    }
    delay(70);
  }

  // Default off mode
  if (mode == 0) {
    analogWrite(RGB_RED_PIN, 0);
    analogWrite(RGB_GREEN_PIN, 0);
    analogWrite(RGB_BLUE_PIN, 0);
  }

  // MODE 1: Crossfade with adjusting brightness
  if (mode == 1) {
    byte rgb[3];
    _rgbConverter.hslToRgb(_hue, 1, LEDVal, rgb);

    
    setColor(rgb[0], rgb[1], rgb[2]); 

    // update hue based on step size
    _hue += _step;

    // hue ranges between 0-1, so if > 1, reset to 0
    if(_hue > 1.0){
      _hue = 0;
    }

    delay(DELAY_INTERVAL);
  }

  // MODE 2: LOFI INPUT
  if (mode == 2) {
    
    // Reset lights of RGB LEDs
    //analogWrite(RGB_RED_PIN, 0);
    //analogWrite(RGB_GREEN_PIN, 0);
    //analogWrite(RGB_BLUE_PIN, 0);  
    //Serial.println(lofiVal);
    
    if (lofiVal < 300) {
      analogWrite(RGB_RED_PIN, 0);
      analogWrite(RGB_GREEN_PIN, 255);
      analogWrite(RGB_BLUE_PIN, 0);  
    } else if (lofiVal > 300 && lofiVal < 400) {
      setColor(0, 255, 0);
    } else {
      setColor(255, 0, 0);
    }

    delay(DELAY_INTERVAL);
  }

  if (mode == 3) {
    analogWrite(OUTPUT_PIEZO_PIN, 200);
  }

  if (mode == 4) {
    mode = 0;
  }

  delay(10);
}

/**
 * setColor takes in values between 0 - 255 for the amount of red, green, and blue, respectively
 * where 255 is the maximum amount of that color and 0 is none of that color. You can illuminate
 * all colors by intermixing different combinations of red, green, and blue. 
 * 
 * This function is based on https://gist.github.com/jamesotron/766994
 * 
 */
void setColor(int red, int green, int blue)
{
  // If a common anode LED, invert values
  if(COMMON_ANODE == true){
    red = MAX_RGB_VALUE - red;
    green = MAX_RGB_VALUE - green;
    blue = MAX_RGB_VALUE - blue;
  }
  analogWrite(RGB_RED_PIN, red);
  analogWrite(RGB_GREEN_PIN, green);
  analogWrite(RGB_BLUE_PIN, blue);  
}