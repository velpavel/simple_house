/*
  Test RGB.
  Start White, change breghtness (off - max), 
  then colors (Red, Green, Blue, other).
*/

#include <RGB_led_strip.h>

//init rgb. (9 - Red_pin, 10 -Green_pin, 11- Blue_pin)
//All pins must be PWM
RGB_led rgb_led(9,10,11);

void setup()
{
  //nothing. pinModes were set when init
}

void loop()
{
  rgb_led.set_color(255,255,255); //white
  delay(500);
  
  //brightness 0-max
  for (int i=0; i<256; i++)
  {
    rgb_led.set_brightness(i);
    delay(10);
  }
  
  rgb_led.set_color(255,0,0); //red
  delay(500);
  rgb_led.set_color(0,255,0); //green
  delay(500);
  rgb_led.set_color(0,0,255); //blue
  delay(500);
  
  //other colors
  for (int i=0; i<6; i++)
    for (int j=0; j<6; j++)
      for (int k=0; k<256; k++)
      {
        rgb_led.set_color(i*51, j*51, k);
        delay(5);
      }    
}
