#define PIR_PIN 4  //датчик движения
#define RELAY_LIGHT_PIN 7 //выход на реле включающее свет
#define PHOTORESIS_PIN A0 //вход с фоторезистора

boolean auto_light = true; //свет управляется датчиком движения
boolean motion=false; //наличие движения
byte light_delay_min = 3; //время которое горит свет после окончания фиксации движения.
unsigned long last_motion_time = -light_delay_min*60*1000UL; //время когда было зафикисированно последнее движение
int lightness_threshold = 700; //пороговое значение освещённости. Если больше, что свет включать не надо.



void setup(){
  pinMode(PIR_PIN, INPUT);
  pinMode(RELAY_LIGHT_PIN, OUTPUT);
  pinMode(PHOTORESIS_PIN, INPUT);
  Serial.begin(9600);
  //Дачтику движения нужно время на инициализауию. Секунд 30.
  Serial.println("Init start");
  delay(30*1000);
  Serial.println("Init finished");
}

void loop(){
  //Если есть что-то в серийном порте - прочесть
  if (Serial.available()) fromSerial_readLine();
  //@!Функция ещё дописывается! 
  
  //проверка движения
  if (digitalRead(PIR_PIN)) 
  {
    //Если движение, то инфо на комп. Включить таймер движения
    if (!motion) {Serial.print("Motion at: "); Serial.println(millis());}
    motion = true; 
    last_motion_time=millis();
  }
  else if (millis()-last_motion_time>=light_delay_min*60*1000UL && motion) 
  {
    //Если нет движения. но было, то по истечению таймера инфу на комп (и выключить свет).
    motion=false;
    Serial.print("Motion finished at: ");
    Serial.println(millis());
  }
  
  //Управляем светом только в автоматическом режиме
  if (auto_light)
  {
    //Если есть движение и освещённость ниже пороговой включить свет
    if(motion && !digitalRead(RELAY_LIGHT_PIN) && analogRead(PHOTORESIS_PIN)<lightness_threshold) digitalWrite(RELAY_LIGHT_PIN, HIGH);
    //Нет движения- нет света
    if (!motion) digitalWrite(RELAY_LIGHT_PIN, LOW);
    //Если света много выключить подсветку(?)  
  }
  
}

//Чтение с портаа. Если конец строки, то разбор на команду и параметры.
//@Вынести в библиотеку для всех модулей.
void fromSerial_readLine()
{
  static String input="";
  static boolean srting_end=false;
  static unsigned long last_simbol_time=millis();
  
  while (Serial.available() && !srting_end)
  {
    char in_c = Serial.read();
    if (in_c == '\n'||in_c == '\r'||in_c=='|') srting_end=true;
    else input+=in_c;
    last_simbol_time=millis();
  }
    
  if (srting_end)
  {
    //@разобрать на проверку адреса модуля, команду и параметры для команды.
    doCommand(input);
    input="";    
    srting_end=false;
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
    Serial.print(digitalRead(RELAY_LIGHT_PIN));
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
  
  else if ((String)command=="light on"||(String)command=="lon")
  {
    auto_light=false;
    digitalWrite(RELAY_LIGHT_PIN, HIGH);
    Serial.println("Auto light off. Light on.");
  }
  
  else if ((String)command=="light off"||(String)command=="loff")
  {
    auto_light=false;
    digitalWrite(RELAY_LIGHT_PIN, LOW);
    Serial.println("Auto light off. Light off.");
  }  
}
