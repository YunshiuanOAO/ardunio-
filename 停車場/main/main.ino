#include <Servo.h>   
#include <LiquidCrystal_I2C.h> 
#include <Wire.h> 

LiquidCrystal_I2C lcd(0x27,16,2);  //設定LCD位置0x27,設定LCD大小為16*2
Servo parking_lever;  //伺服馬達宣告

const int IR1 = 2; // IR1PIN
const int IR2 = 4; // IR2PIN
const int servo_pin = 3;
const int Max_car = 4;

int now_car = 4;
int car_in = 0,car_out = 0;

void setup() {
  //初始化LCD
  lcd.init();
  lcd.backlight(); 
  //初始化伺服馬達
  parking_lever.attach(servo_pin);  
  parking_lever.write(180);

  //初始化紅外線感應器
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
}

void loop() {
    
    if(digitalRead(IR1) == LOW){
      //車出關門
      if(car_out == 1){
        parking_lever.write(180);
        car_out = 0;
      }else{
        //車進開門
        if(now_car != 0){ 
          parking_lever.write(90);
          if(car_in == 0)now_car--,car_in = 1;
          delay(200);
        }else{
          lcd.setCursor(0,0);   
          lcd.print("not free spaces"); 
          lcd.setCursor(0,1);   
          lcd.print("Sorry ^3^"); 
          delay(1000);
          lcd.clear();
        }
      }
      delay(200);

      
    }
    
    if(digitalRead(IR2) == LOW){
      //車進關門
      if(car_in == 1){
        parking_lever.write(180);
        car_in = 0;
      }else{
      //車出開門
        if(now_car < 4){ 
          parking_lever.write(90);
          if(car_out == 0)now_car++,car_out = 1;
        }else{
          lcd.setCursor(0,0);   
          lcd.print("error operate"); 
          lcd.setCursor(0,1);   
          lcd.print("place try again ^3^"); 
          delay(1000);
          lcd.clear();
        }
      }
      delay(200);
    }
    
    lcd.setCursor(0,0);   
    lcd.print("Welcome"); 
    lcd.setCursor(0,1);   
    lcd.print(now_car);
}
