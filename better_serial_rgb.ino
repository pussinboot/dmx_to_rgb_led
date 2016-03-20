#include <TimerOne.h>
#include "LPD6803.h"

int dataPin = 2;       // 'yellow' wire
int clockPin = 3;      // 'green' wire
// Don't forget to connect 'blue' to ground and 'red' to +5V

// Timer 1 is also used by the strip to send pixel clocks

// Set the first variable to the NUMBER of pixels. 20 = 20 pixels in a row
LPD6803 strip = LPD6803(150, dataPin, clockPin);

// Create a 15 bit color value from R,G,B
unsigned int Color(byte r, byte g, byte b)
{
  //Take the lowest 5 bits of each value and append them end to end
  return( ((unsigned int)g & 0x1F )<<10 | ((unsigned int)b & 0x1F)<<5 | (unsigned int)r & 0x1F);
}
// strip.setPixelColor(i,Color(r,g,b))
void colorWipe(uint16_t c, uint8_t wait) {
  int i;
  
  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}
unsigned int Wheel(byte WheelPos)
{
  byte r,g,b;
  switch(WheelPos >> 5)
  {
    case 0:
      r=31- WheelPos % 32;   //Red down
      g=WheelPos % 32;      // Green up
      b=0;                  //blue off
      break; 
    case 1:
      g=31- WheelPos % 32;  //green down
      b=WheelPos % 32;      //blue up
      r=0;                  //red off
      break; 
    case 2:
      b=31- WheelPos % 32;  //blue down 
      r=WheelPos % 32;      //red up
      g=0;                  //green off
      break; 
  }
  return(Color(r,g,b));
}
void setup() {
  // put your setup code here, to run once:
 strip.setCPUmax(75);  // start with 50% CPU usage. up this if the strand flickers or is slow
  
  // Start up the LED counter
  strip.begin();
  // start serial comms
  Serial.begin(9600);

  // Update the strip, to start they are all 'off'
  strip.show();
}
int serial_input[4];    // raw input from serial buffer, 4 bytes
int startbyte;       // start byte, begin reading input
int i;           // which led?
void loop() {
  // put your main code here, to run repeatedly:
 // Wait for serial input (min 4 bytes in buffer)
  if (Serial.available() > 3) {
    startbyte = Serial.read();
      if (startbyte == 255) {
         for (i=0;i<2;i++) {
        serial_input[i] = Serial.read();
      }
      strip.setPixelColor(serial_input[0],Wheel(serial_input[1]));
      //strip.setPixelColor(i,Color(serial_input[1],serial_input[2],serial_input[3]));
      //colorWipe(Color(serial_input[1],serial_input[2],serial_input[3]),i);
      }
      else if (startbyte == 128) {
         strip.show();
      }
  }
}

