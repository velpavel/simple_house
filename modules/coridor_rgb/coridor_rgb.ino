#define PIR_PIN 4  //датчик движения
#define RELAY_LIGHT_PIN 7 //выход на реле включающее свет
#define PHOTORESIS_PIN A6 //вход с фоторезистора

#include <RGB_led_strip.h>

RGB_led rgb_led(9,10,11);
boolean auto_light = true; //свет управляется датчиком движения
boolean motion=false; //наличие движения
byte light_delay_min = 3; //время которое горит свет после окончания фиксации движения.
unsigned long last_motion_time = -light_delay_min*60*1000UL; //время когда было зафикисированно последнее движение
int lightness_threshold = 700; //пороговое значение освещённости. Если больше, что свет включать не надо.

byte r_color=255, g_color=255, b_color=255, brightness=255;

void setup(){
  pinMode(PIR_PIN, INPUT);
  pinMode(PHOTORESIS_PIN, INPUT);
  Serial.begin(9600);
  //Дачтику движения нужно время на инициализауию. Секунд 30.
  Serial.println("Init start");
  delay(30*1000);
  Serial.println("Init finished");

  rgb_led.set_brightness(brightness);
  rgb_led.set_color(r_color,g_color,b_color);
}

void loop(){
  //Если есть что-то в серийном порте - прочесть
  if (Serial.available()) fromSerial_command();
  //@!Функция ещё дописывается! 
  
  //проверка движения
  if (digitalRead(PIR_PIN)) 
  {
    //Если движение, то инфо на комп. Включить таймер движения
    if (!motion) {Serial.print("Motion at: "); Serial.println(millis()); Serial.print("*M"+String(motion)+"*");}
    motion = true; 
    last_motion_time=millis();
  }
  else if (millis()-last_motion_time>=light_delay_min*60*1000UL && motion) 
  {
    //Если нет движения. но было, то по истечению таймера инфу на комп (и выключить свет).
    motion=false;
    Serial.print("Motion finished at: ");
    Serial.println(millis());
    Serial.print("*M"+String(motion)+"*");
  }
  
  //Управляем светом только в автоматическом режиме
  if (auto_light)
  {
    //Если есть движение и освещённость ниже пороговой включить свет
    if(motion && !digitalRead(RELAY_LIGHT_PIN) && analogRead(PHOTORESIS_PIN)<lightness_threshold) rgb_led.set_brightness(brightness);
    //Нет движения- нет света
    if (!motion) rgb_led.set_brightness(0);
    //Если света много выключить подсветку(?)  
  }
  
}

//Чтение с порта. Если конец строки, то разбор на команду и параметры.
//@Вынести в библиотеку для всех модулей.
void fromSerial_command()
{
  char BT_data;
  BT_data=Serial.read();
  if(BT_data=='R') r_color = Serial.parseInt();
  if(BT_data=='G') g_color = Serial.parseInt();
  if(BT_data=='B') b_color = Serial.parseInt();
  if(BT_data=='Y') brightness = Serial.parseInt();
  rgb_led.set_brightness(brightness);
  rgb_led.set_color(r_color,g_color,b_color);
  
  if(BT_data=='O') {rgb_led.set_brightness(255); auto_light=false;}
  if(BT_data=='F') {rgb_led.set_brightness(0); auto_light=false;}
  if(BT_data=='A') {auto_light=true;}  

  if(BT_data=='S') {
    Serial.print("*A"+String(auto_light)+"*");
    Serial.print("*M"+String(motion)+"*");
    Serial.print("*L"+String((millis()-last_motion_time)/1000/60/60)+":"+String(((millis()-last_motion_time)/1000/60)%60)+ "*");
    Serial.print("*B"+String(brightness)+"*");
    Serial.print("*O"+String(analogRead(PHOTORESIS_PIN))+"*");
  }
}

//исполнение внешней команды.
//
void doCommand(String command)
{
  //@Разнести на распознание строки + определение адресата(?)(библиотека?) и часть модуля
  //проверяем что команда нашей системе
  //выполняем варианты
    //изменить задержку у включённого света
    //изменить пороговое значение фоторезистора
    //вывести статус и переменные
    //включить свет
      //вообще
      //на время
    //отключить включение (автоуправление) света
      //вообще
      //на время
  
  //тестовая часть  
  //Serial.println((String)command);
  //@Переписать с эльфов(elif)
  if ((String)command=="s"||(String)command=="status")
  {
    Serial.print("Auto mode: ");
    Serial.print(auto_light);
    Serial.print(" ms: ");
    Serial.print(millis());
    Serial.print(" Motion: ");
    Serial.print(motion);
    Serial.print(" last motion: ");
    Serial.print((millis()-last_motion_time)/1000/60/60);
    Serial.print(":");
    Serial.print(((millis()-last_motion_time)/1000/60)%60);
    Serial.print(":");
    Serial.print(((millis()-last_motion_time)/1000)%60);
    Serial.print(" light: ");
    Serial.print(" ?");
    Serial.print(" lightness: ");
    Serial.println(analogRead(PHOTORESIS_PIN));
  }
  else if ((String)command=="auto off"||(String)command=="aoff")
  {
    auto_light=false;
    Serial.println("Auto light off");
  }
  else if ((String)command=="auto on"||(String)command=="aon")
  {
    auto_light=true;
    Serial.println("Auto light on");
  }
}
