/*
  RGB_led_stpip.h - Library for control RGB strip.
  Created by PaVel, December, 2014.
*/
#include "RGB_led_strip.h"

RGB_led::RGB_led(byte pinR, byte pinG, byte pinB)
{
    pinMode(pinR, OUTPUT);
    pinMode(pinG, OUTPUT);
    pinMode(pinB, OUTPUT);
    _pinR=pinR;
    _pinG=pinG;
    _pinB=pinB;
    //Init values. After it user can use all fuctions without errors
    _brightness=255; //max brightness
    _R_color=255;    //white color 
    _G_color=255;
    _B_color=255;
}

void RGB_led::led_light()
{
    analogWrite(_pinR, _R_color*_brightness/255);
    analogWrite(_pinG, _G_color*_brightness/255);
    analogWrite(_pinB, _B_color*_brightness/255);
}

void RGB_led::set_color(byte R_color, byte G_color, byte B_color)
{
    _R_color=R_color;
    _G_color=G_color;
    _B_color=B_color;
    led_light();
}

void RGB_led::set_brightness(byte brightness)
{
    _brightness=brightness;
    led_light();
}

byte RGB_led::show_brightness()
{
    return _brightness;
}