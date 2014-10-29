/***************************************************************

Msgeq7 + neopixel array spectrum analyser
https://www.adafruit.com/products/1430

***************************************************************/
#include <Adafruit_NeoPixel.h>

#define PIN 6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(40, PIN, NEO_GRB + NEO_KHZ800);

const int ANALOG = 1;
const int STROBE = 2;
const int RESET = 3;
int outputValues[7];

int barValue = 0;
int outputValuesMax[7];
int countLoop = 0;
int countLoopMax = 1;

void setup() {
  
  //setup MSGEQ7
  pinMode(ANALOG, INPUT);
  pinMode(STROBE, OUTPUT);
  pinMode(RESET, OUTPUT);

  digitalWrite(RESET, LOW);
  digitalWrite(STROBE, HIGH);
  
  Serial.begin(9600);
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

}

void loop() {
  
  int j = 0;
  
  //reset chip

  digitalWrite(RESET, HIGH);
  delayMicroseconds(1);
  digitalWrite(RESET, LOW);
  
  countLoop++;
  if (countLoop > countLoopMax){
     countLoop = 0; 
  }
 
  
  //load the data from the msgeq7
  for (int i = 0; i < 7; i++) {
    digitalWrite(STROBE, LOW);
    delayMicroseconds(30);
    outputValues[i] = analogRead(ANALOG);
    digitalWrite(STROBE, HIGH);
  }
  
  //if we are at the end of our average loop, then output the value.
  if (countLoop == 0){
    for (int i = 0; i < 8; i++) {
      
      if (i == 4){
        j = 3;
      } else if (i > 4){
        j = i-1;
      } else {
         j = i; 
      }
      
      //normalise the value
      barValue = outputValues[j];
      barValue = barValue - 150;
      if (barValue < 0){barValue = 0;}
      
      int colour = Wheel(150); 
      
      if (barValue > 10){
        colour = Wheel(60); 
      } else {
        colour = 0; 
      }
      strip.setPixelColor(i, colour);
      
      if (barValue > 150){
        colour = Wheel(10); 
      } else {
        colour = 0; 
      }
      strip.setPixelColor((i+8), colour);

      if (barValue > 300){
         colour = Wheel(0); 
      } else {
        colour = 0; 
      }
      strip.setPixelColor((i+16), colour);

      if (barValue > 500){
         colour = Wheel(100); 
      } else {
        colour = 0; 
      }
      strip.setPixelColor((i+24), colour);

      if (barValue > 700){
         colour = Wheel(180); 
      } else {
        colour = 0; 
      }
      strip.setPixelColor((i+32), colour); 
      
      Serial.print(barValue);      
      Serial.print(" ");
      
    }
    
    strip.show();
    Serial.println(" ");
    
  }
      
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
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
