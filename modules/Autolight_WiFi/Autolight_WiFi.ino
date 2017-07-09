/* 
   -- Light -- 
    
   This source code of graphical user interface  
   has been generated automatically by RemoteXY editor. 
   To compile this code using RemoteXY library 2.3.1 or later version  
   download by link http://remotexy.com/en/library/ 
   To connect using RemoteXY mobile app by link http://remotexy.com/en/download/                    
     - for ANDROID 4.0.1 or later version; 
     - for iOS 1.1.0 or later version; 
     
   This source code is free software; you can redistribute it and/or 
   modify it under the terms of the GNU Lesser General Public 
   License as published by the Free Software Foundation; either 
   version 2.1 of the License, or (at your option) any later version.     
*/ 

////////////////////////////////////////////// 
//        RemoteXY include library          // 
////////////////////////////////////////////// 

// RemoteXY select connection mode and include library  
#include "SETTINGS.h"
#define REMOTEXY_MODE__ESP8266WIFI_LIB
#include <ESP8266WiFi.h> 

#include <RemoteXY.h> 

////////////////////////////////////////////// 
//        RemoteXY include library          // 
////////////////////////////////////////////// 

// RemoteXY select connection mode and include library  
#define REMOTEXY_MODE__ESP8266WIFI_LIB
#include <ESP8266WiFi.h> 

#include <RemoteXY.h> 

// RemoteXY connection settings  

#define REMOTEXY_SERVER_PORT 6377 


// RemoteXY configurate   
#pragma pack(push, 1) 
uint8_t RemoteXY_CONF[] = 
  { 255,2,0,2,0,179,0,6,5,0,
  4,192,3,21,34,9,2,3,3,60,
  15,8,23,2,65,4,86,52,9,9,
  2,66,131,37,53,16,7,2,129,0,
  7,7,24,6,9,208,175,209,128,208,
  186,208,190,209,129,209,130,209,140,0,
  129,0,50,7,18,6,9,208,160,208,
  181,208,182,208,184,208,188,32,209,128,
  208,176,208,177,208,190,209,130,209,139,
  0,129,0,70,16,12,5,13,208,144,
  208,178,209,130,208,190,0,129,0,70,
  24,9,5,13,208,146,208,186,208,187,
  0,129,0,70,32,14,5,13,208,146,
  209,139,208,186,208,187,0,129,0,64,
  53,18,6,9,208,161,208,178,208,181,
  209,130,58,0,129,0,4,54,30,4,
  9,208,158,209,129,208,178,208,181,209,
  137,209,145,208,189,208,189,208,190,209,
  129,209,130,209,140,0 }; 
   
// this structure defines all the variables of your control interface  
struct { 

    // input variable
  int8_t slBright; // =0..100 slider position 
  uint8_t slMode; // =0 if select position A, =1 if position B, =2 if position C, ... 

    // output variable
  uint8_t ldLightStatus_r; // =0..255 LED Red brightness 
  int8_t lvBright; // =0..100 level position 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY; 
#pragma pack(pop) 

///////////////////////////////////////////// 
//           END RemoteXY include          // 
///////////////////////////////////////////// 

#define PIR_PIN 4  //датчик движения
#define LIGHT_PIN 5 //выход на реле включающее свет
#define PHOTORESIS_PIN A0 //вход с фоторезистора

boolean auto_light = true; //свет управляется датчиком движения
boolean motion=false; //наличие движения
byte light_delay_min = 3; //время которое горит свет после окончания фиксации движения.
unsigned long last_motion_time = -light_delay_min*60*1000UL; //время когда было зафикисированно последнее движение
int lightness_threshold = 700; //пороговое значение освещённости. Если больше, что свет включать не надо.
//byte brightness = 70; Used RemoteXY.slBright

void setup()  
{ 
  RemoteXY_Init ();  

  RemoteXY.slBright = 70;
  RemoteXY.slMode = 0;
  
   
  pinMode(PIR_PIN, INPUT);
  pinMode(LIGHT_PIN, OUTPUT);
  digitalWrite(LIGHT_PIN, LOW);
  //pinMode(PHOTORESIS_PIN, INPUT);
  //Дачтику движения нужно время на инициализауию. Секунд 30.
  delay(30*1000);
} 

void loop()  
{  
  static unsigned long last_ms = millis();
  RemoteXY_Handler (); 

  if (RemoteXY.slMode == 0) auto_light = true;
  else if (RemoteXY.slMode == 1) {
    auto_light = false;
    //analogWrite(LIGHT_PIN, map(RemoteXY.slBright, 0, 100, 0, 1023));
    digitalWrite(LIGHT_PIN, LOW);
    RemoteXY.ldLightStatus_r = 255;
  }
  else if (RemoteXY.slMode == 2) {
    auto_light = false;
    //analogWrite(LIGHT_PIN, 0);
    digitalWrite(LIGHT_PIN, HIGH);
    RemoteXY.ldLightStatus_r = 0;
  }
  

  if (digitalRead(PIR_PIN)) 
  {
    //Если движение, то инфо на комп. Включить таймер движения
    motion = true; 
    last_motion_time=millis();
  }
  else if (millis()-last_motion_time>=light_delay_min*60*1000UL && motion) 
  {
    //Если нет движения. но было, то по истечению таймера инфу на комп (и выключить свет).
    motion=false;
  }
  
  //Управляем светом только в автоматическом режиме
  if (auto_light)
  {
    //Если есть движение и освещённость ниже пороговой включить свет
    if(motion){
      //analogWrite(LIGHT_PIN, map(RemoteXY.slBright, 0, 100, 0, 1023));
      digitalWrite(LIGHT_PIN, LOW);
      RemoteXY.ldLightStatus_r = 255;
    }
    //Нет движения- нет света
    if (!motion) { 
      digitalWrite(LIGHT_PIN, HIGH);
      RemoteXY.ldLightStatus_r = 0;
    }
    //Если света много выключить подсветку(?)  
  }

}
