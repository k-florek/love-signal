/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/home/krflorek/Documents/particle-photon-projects/love-light/src/love-light.ino"
/*
 * Project love-light
 * Description: A light that syncs to your lover's
 * Author: Kelsey Florek
 * Date: 2020-03-26
 */

#include "neopixel/neopixel.h"
#include "math.h"
#include "sstream"
#include "string.h"

// Define parameters for neopixel ring
void cycleColors();
void setup();
void loop();
#line 14 "/home/krflorek/Documents/particle-photon-projects/love-light/src/love-light.ino"
#define PIXEL_COUNT 16
#define PIXEL_PIN D2
#define PIXEL_TYPE WS2812

// Create Light Ring Object
Adafruit_NeoPixel ring = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

// Define Button
int button = D1;
int buttonState;

// Default Color
struct Color {
  int red;
  int green;
  int blue;
};
Color color = {0,50,200};

// Set all of our lights to a specific color
void fillColor(Color rgb) {
  int i;
  for (i=0;i<PIXEL_COUNT;i++){
    ring.setPixelColor(i,rgb.red,rgb.green,rgb.blue);
  }
  ring.show();
}

// Cycle through colors setting color each time
void cycleColors() {
  int center = 128;
  int width = 127;
  int length = 1000;
  int i = 0;
  while(buttonState==LOW){
    buttonState = digitalRead(button);
    if (i>=length){
      i = 0;
    }

    color.red = sin(0.025*i + 0) * width + center;
    color.green = sin(0.025*i + 2) * width + center;
    color.blue = sin(0.025*i + 4) * width + center;

    fillColor(color);
    delay(50);
    i++;
  }

}

// Flash Color 3 times
void colorFlash(Color rgb) {
  // flash selected color 3x
      int i;
      for(i=0;i<4;i++){
        fillColor(rgb);
        delay(250);
        ring.clear();
        ring.show();
        delay(250);
      }
}

// Convert colorTOstring
String colorTOstring(Color rgb) {
  String returnString = "";
  char buffer [3];
  //red
  itoa(rgb.red,buffer,10);
  returnString += buffer;
  returnString += ";";
  //green
  itoa(rgb.green,buffer,10);
  returnString += buffer;
  returnString += ";";
  //blue
  itoa(rgb.blue,buffer,10);
  returnString += buffer;
  returnString += ";";

  return returnString;
}

// Convert stringTOcolor
Color stringTOcolor(String scolor){
  Color rgb;
  String delimiter = ";";
  int i;
  int start = 0;
  int end = 0;
  int c[3];
  for(i=0;i<4;i++){
    end = scolor.indexOf(delimiter,start);
    if (start == 0){
      c[i] = atoi(scolor.substring(start,end));
    }else{
      c[i] = atoi(scolor.substring(start,end));
    }
    start = end+1;    
  }
  rgb.red = c[0];
  rgb.green = c[1];
  rgb.blue = c[2];
  return rgb;
}

//#########################################
//Main Program
//#########################################

// setup() runs once, when the device is first turned on.
void setup() {
  // Start the pixel ring and set off
  ring.begin();
  ring.setBrightness(100);
  ring.clear();
  ring.show();
  // Check our input button
  pinMode(button,INPUT_PULLUP);
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // Turn all LEDs off
  ring.clear();
  ring.show();

  // Get button state
  buttonState = digitalRead(button);
  if (buttonState==LOW){
    delay(1000);

    // If after 5 sec button is still pressed cycle through colors for as long as button is pressed
    buttonState = digitalRead(button);
    if (buttonState==LOW){
      cycleColors();

      // flash selected color 3x
      colorFlash(color);
      
    }
    // If we have a short press light the signal
    else {
      fillColor(color);
      bool success;
      String scolor = colorTOstring(color);
      success = Particle.publish("/particle/love/signal",scolor,60,PRIVATE);
      if(!success){
        Color red = {255,0,0};
        colorFlash(red);
      }
      else {
        delay(10000);
      }
      
    }
    
  }

}

