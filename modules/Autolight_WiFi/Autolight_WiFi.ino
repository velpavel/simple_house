/* 
   AutoLight + MQTT
   Свет управлемый датчиком движения 
 
*/ 

#include "SETTINGS.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//topics 
#define MODULE_ST_TOPIC "/coridor/module/status"
#define MODULE_COM_TOPIC "/coridor/module/command"
#define AUTOMODE_ST_TOPIC "/coridor/automode/status"
#define AUTOMODE_COM_TOPIC "/coridor/automode/command"
#define MOTION_ST_TOPIC "/coridor/motion_sensor/status"
#define LIGHTS_ST_TOPIC "/coridor/light/status"
#define LIGHTS_COM_TOPIC "/coridor/light/command"

#define PIR_PIN 4  //датчик движения (NodeMCU D2)
#define LIGHT_PIN 5 //выход на реле включающее свет (NodeMCU D1)
#define PHOTORESIS_PIN A0 //вход с фоторезистора

boolean auto_light = true; //свет управляется датчиком движения
boolean motion=false; //наличие движения
boolean motion_signal=false;  //Наличие сигнала с датчика движения для передачи в логах о движении.
byte light_delay_min = 2; //время которое горит свет после окончания фиксации движения.
unsigned long last_motion_time = -light_delay_min*60*1000UL; //время когда было зафикисированно последнее движение
int lightness_threshold = 700; //пороговое значение освещённости. Если больше, что свет включать не надо.
//byte brightness = 70; Used RemoteXY.slBright

IPAddress server(MQTT_IP);
WiFiClient wclient;
PubSubClient client(wclient, server);

void callback(const MQTT::Publish& sub) {
  Serial.print("Get data from subscribed topic ");
  Serial.print(sub.topic());
  Serial.print(" => ");
  Serial.println(sub.payload_string());

  if (sub.topic() == LIGHTS_COM_TOPIC) {
    if (sub.payload_string() == "1") {  
      // на реле инверсные команды
      if (auto_light){
        auto_light = false;
        client.publish(AUTOMODE_ST_TOPIC, "0");
      }
      if (digitalRead(LIGHT_PIN))turn_light_on();
    }
    else if (sub.payload_string() == "0") {
      // на реле инверсные команды
      if (auto_light){
        auto_light = false;
        client.publish(AUTOMODE_ST_TOPIC, "0");
      }
      if (!digitalRead(LIGHT_PIN)) turn_light_off();
    }
  }
  else if (sub.topic() == AUTOMODE_COM_TOPIC){
    if (sub.payload_string() == "1") {
      auto_light = true;
      client.publish(AUTOMODE_ST_TOPIC, "1");
    }
    else if (sub.payload_string() == "0") {
      auto_light = false;
      client.publish(AUTOMODE_ST_TOPIC, "0");
    }
  }
  else if (sub.topic() == MODULE_COM_TOPIC){
    if (sub.payload_string() == "status") {
      pubModuleStatus();
    }
    if (sub.payload_string() == "all_status") {
      pubAllStatuses();
    }
  }
}

void turn_light_on(){
  if (digitalRead(LIGHT_PIN)){
    digitalWrite(LIGHT_PIN, LOW);  // на реле инверсные команды
    client.publish(LIGHTS_ST_TOPIC, "1");
  }
}

void turn_light_off(){
  if (!digitalRead(LIGHT_PIN)){
    digitalWrite(LIGHT_PIN, HIGH); // на реле инверсные команды
    client.publish(LIGHTS_ST_TOPIC, "0");
  }
}

void pubModuleStatus(){
  client.publish(MODULE_ST_TOPIC, String(millis())+" ms working.");
}

void pubAllStatuses(){
  pubModuleStatus();
  if (auto_light) client.publish(AUTOMODE_ST_TOPIC, "1");
  else client.publish(AUTOMODE_ST_TOPIC, "0");
  if (!digitalRead(LIGHT_PIN)) client.publish(LIGHTS_ST_TOPIC, "1");
  else client.publish(LIGHTS_ST_TOPIC, "0");
  if (digitalRead(PIR_PIN)) client.publish(MOTION_ST_TOPIC, "1");
  else client.publish(MOTION_ST_TOPIC, "0");
}

void wifi_connect(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  if (WiFi.waitForConnectResult() != WL_CONNECTED)
    Serial.println("WiFi not connected");
  Serial.println("WiFi connected");
}

void mqtt_connect(){
  if (client.connect(MQTT_CLIENT_NAME)) {
    client.set_callback(callback);
    client.subscribe(MODULE_COM_TOPIC);
    client.subscribe(AUTOMODE_COM_TOPIC);
    client.subscribe(LIGHTS_COM_TOPIC);
    client.publish(MODULE_ST_TOPIC, "Online");
    pubAllStatuses();
    Serial.println("MQTT connected");
  }
  else{
    Serial.println("MQTT not connected");
  }
}

void setup()  
{ 

  Serial.begin(115200);
   
  pinMode(PIR_PIN, INPUT);
  pinMode(LIGHT_PIN, OUTPUT);
  digitalWrite(LIGHT_PIN, LOW);
  //pinMode(PHOTORESIS_PIN, INPUT);
  //Дачтику движения нужно время на инициализацию. Дадим секунд 15.
  delay(15*1000);
  wifi_connect();
  
  if (WiFi.status() == WL_CONNECTED)
      mqtt_connect();
} 



void loop()  
{  
  static unsigned long last_wifi_try = millis();

  //if (WiFi.status() != WL_CONNECTED && millis()-last_wifi_try > 10000L) {
  if (WiFi.status() != WL_CONNECTED) {
    wifi_connect();
    last_wifi_try = millis();
  }
  
  if (WiFi.status() == WL_CONNECTED){
    if (!client.connected()) {
      mqtt_connect();
    }
    if (client.connected())
      client.loop();
  }


  if (digitalRead(PIR_PIN)) 
  {
    //Если движение, то инфо на комп. Включить таймер движения
    motion = true; 
    if (!motion_signal){
      motion_signal = true;
      client.publish(MOTION_ST_TOPIC, "1");
    }
    last_motion_time=millis();
  }
  else{
    if (motion_signal){
      motion_signal = false;
      client.publish(MOTION_ST_TOPIC, "0");
    }
    if (millis()-last_motion_time>=light_delay_min*60*1000UL && motion) 
    {
    //Если нет движения. но было, то по истечению таймера  выключить свет.
      motion=false;
    }
  }
  
  //Управляем светом только в автоматическом режиме
  if (auto_light)
  {
    //Если есть движение и освещённость ниже пороговой включить свет
    if(motion){
      turn_light_on();
    }
    //Нет движения- нет света
    if (!motion) { 
      turn_light_off();
    }
    //Если света много выключить подсветку(?)  
  }

}
