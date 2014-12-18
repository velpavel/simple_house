/*
  RGB_led_stpip.h - Library for control RGB strip.
  Created by PaVel, December, 2014.
*/
#ifndef Morse_h
#define Morse_h
 
#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif 

class RGB_led
{
  public:
    RGB_led(byte pinR, byte pinG, byte pinB);
    void set_color(byte R_color, byte G_color, byte B_color);
    void set_brightness(byte brightness);
    byte show_brightness();
  private:
    byte _pinR;
    byte _pinG;
    byte _pinB;
    byte _R_color;
    byte _G_color;
    byte _B_color;
    byte _brightness;
    void led_light(); //apply changes colors or brightness
};

#endif