#include <RGB_led_strip.h>
#include <SPI.h>
#include "RF24.h"

RF24 radio(9,10);
const uint64_t pipe_base = 0xE8E8F0F0E1LL;
const uint64_t pipe_koridor = 0xE8E8F0F0A1LL;

#define PIR_PIN 8
#define PHRES_PIN A0


RGB_led rgb_led(4,5,6);


void setup()
{
  Serial.begin(9600);
  Serial.println("Start init");
  pinMode(PIR_PIN, INPUT);
  pinMode(PHRES_PIN, INPUT);
  
  radio.begin();
  radio.setChannel(0x22);
  radio.openReadingPipe(0,pipe_koridor);

  test_rgb();
  rgb_led.set_brightness(0);
  
  radio.startListening();
  Serial.println("Finish init");
}

void loop(){
  int massive[4];
  if( radio.available()){
   radio.read(massive,sizeof(massive));             // по адресу переменной in функция записывает принятые данные;
   rgb_led.set_color(massive[0],massive[1],massive[2]);
   rgb_led.set_brightness(massive[3]);
   Serial.print("R: ");
   Serial.print(massive[0]);
   Serial.print(" G: ");
   Serial.print(massive[1]);
   Serial.print(" B: ");
   Serial.print(massive[2]);
   Serial.print(" L: ");
   Serial.println(massive[3]);
  }
  static unsigned long current_time=millis();
  if (millis()-current_time > 1000){
    Serial.print("Motion: ");
    Serial.print(digitalRead(PIR_PIN));
    Serial.print(" lightness: ");
    Serial.println(digitalRead(PHRES_PIN));
    current_time=millis();
  }
}

void test_rgb()
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
