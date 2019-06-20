/*
   Пример использования библиотеки GyverButton, управляем переменной value при помощи двух кнопок
   Конструкция с isIncr делает увеличение/уменьшение переменной при нажатой кнопке с шагом по времени
*/

#define BTN1 2				// кнопка подключена сюда (PIN --- КНОПКА --- GND)
#define BTN2 6        // Для пульта  IR ДУ
#include <GyverTimer.h>
#include "GyverButton.h"
#include "IRremote.h"
int dir =0; // flag 
GButton butt1(BTN1);
GButton butt2(BTN2);
IRrecv irrecv(8); // указываем вывод, к которому подключен приемник
void setDebounce(uint16_t debounce);  // установка времени антидребезга (по умолчанию 80 мс)
// подключите пины контроллера к цифровым пинам Arduino
// первый двигатель
decode_results results; // декодировка
int in1 = 4;
int in2 = 5;
GTimer_ms myTimer1(40000);// Доп задержка


void setup() {
  Serial.begin(9600);
   myTimer1.setInterval(40000);// интервал таймера
 irrecv.enableIRIn(); // запускаем прием
   pinMode(in1, OUTPUT);
   pinMode(in2, OUTPUT);
   StopMotor();
}
void loop() {
// IR пульт ДУ
  if ( irrecv.decode( &results )) { // если данные пришли
    switch ( results.value ) {
    case 0xFF629D:
        Forward();
        break;
        StopMotor();
    case 0xFFA857:
        Backward();
        break;
        StopMotor();
    }    
    irrecv.resume(); // принимаем следующую команду
  }
  
  int value = 0;
  butt1.tick(); // обязательная функция отработки. Должна постоянно опрашиваться
 
    if (butt1.isClick()|butt2.isClick()) {
      dir^=(1<<0);
      if(dir){
        if (myTimer1.isReady()){}
        Forward();
        Serial.println(dir); 
        Serial.println("MotorForward 40 sec");
       
      }else{
        if (myTimer1.isReady()|butt2.isClick()){}
        Backward();
        Serial.println(dir);
        Serial.println("MotorBackward 40 sec");
      
      }
    }
 
}
 
  

/* FUNCION MOTOR */
void StopMotor() // Мотор выключен
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  delay(100);
 }
void Backward() //Мотор задвигает полку 
{
  // эта функция обеспечит вращение двигателей в  направлениях на установленной скорости

  // запуск двигателя 
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  //Задвигание полки на 40 сек.//
  for(uint16_t i = 0; i < 40000; i++){
    butt1.tick(); 
    if (butt1.isClick()|butt2.isClick()) {
     
      StopMotor();
      return;
    }
    delay(1);
  }
  StopMotor();
}

void Forward() //Мотор выдвигает полку
{

// эта функция обеспечит вращение двигателей в  направлениях на установленной скорости

// запуск двигателя 

digitalWrite(in1, LOW);
digitalWrite(in2, HIGH);
//Выдвигание полки на 40 сек.
  for(uint16_t i = 0; i < 40000; i++){
    butt1.tick(); 
    if (butt1.isClick()|butt2.isClick()) {
      
      StopMotor();
      return;
    }
    delay(1);
  }
  StopMotor();
}


  
