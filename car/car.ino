#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pin set ------------------------
LiquidCrystal_I2C lcd(0x3F,2,1,0,4,5,6,7,3, POSITIVE);  // Set the LCD I2C address
SoftwareSerial I2CBT(10,11); //定義 PIN10 及 PIN11 分別為 RX 及 TX 腳位
const int LedRed = 2;
const int LedGreen = 3;
const int In1 = 7;
const int In2 = 6;
const int In3 = 5;
const int In4 = 4;

// Global Variables set ------------------------
enum MainState {None, Crawling, Control};
char cmd_convert[20];
const int speedVal = 127;

void setup() {
  // pinMode set ------------------------
  pinMode(LedRed, OUTPUT);
  pinMode(LedGreen, OUTPUT);
  pinMode(In1, OUTPUT);
  pinMode(In2, OUTPUT);
  pinMode(In3, OUTPUT);
  pinMode(In4, OUTPUT);
 
  Serial.begin(9600); 
  I2CBT.begin(9600); 
  lcd.begin(16, 2);
  Serial.println("Lcd ready!");
  
  // MotorTest();
}

void loop() {
  
  char cmd[20];
  
  while(1){
    int insize = I2CBT.available();
    if(insize > 0) { //讀取藍牙訊息
      lcd.clear();
      digitalWrite(LedRed,HIGH); //藍牙燈

      //處理藍芽訊息
      for (int i=0; i<insize; i++){
        cmd[i]=char(I2CBT.read());
        cmd_convert[i] = cmd[i];
        Serial.print(cmd_convert[i]);
        lcd.setCursor(i, 0); 
        lcd.print(cmd_convert[i]);
      }
      Serial.print("\n");

      //判斷狀態
      switch (cmd_convert[0]) {
        case 'U':
          MoveUp();
          break;
        case 'D':
          MoveDown();
          break;
        case 'L':
          MoveLeft();
          break;
        case 'R':
          MoveRight();
          break;
        case 'B':
          MoveStop();
          break;
          
        case '1':
        
          break;
        case '2':
        
          break;
        case '3':
        
          break;
        case '4':
        
          break;
        case '5':
        
          break;

        case 'S':
          digitalWrite(LedGreen,HIGH);
          break;

        case 'Q':
          digitalWrite(LedGreen,LOW);
          MoveStop();
          break;
        
        default:
          MoveStop();
          break;
      }
    }else{ //等待藍牙訊息
      
      digitalWrite(LedRed,LOW); //藍牙燈
//      lcd.setCursor(0, 1); 
//      lcd.print("Wait Input!");
    }
    
  } 
}

void MoveUp() {
//  Serial.println("up");
  analogWrite(In1, speedVal);
  analogWrite(In2, 0);
  analogWrite(In3, speedVal);
  analogWrite(In4, 0);
//  delay(2000);
}

void MoveDown() {
//  Serial.println("down");
  analogWrite(In1, 0);
  analogWrite(In2, speedVal);
  analogWrite(In3, 0);
  analogWrite(In4, speedVal);
//  delay(2000);
}

void MoveLeft() {
//  Serial.println("left");
  analogWrite(In1, 0);
  analogWrite(In2, 0);
  analogWrite(In3, speedVal);
  analogWrite(In4, 0);
//  delay(2000);
}

void MoveRight() {
//  Serial.println("right");
  analogWrite(In1, speedVal);
  analogWrite(In2, 0);
  analogWrite(In3, 0);
  analogWrite(In4, 0);
//  delay(2000);
}

void MoveStop(){
//  Serial.println("stop");
  analogWrite(In1, 0);
  analogWrite(In2, 0);
  analogWrite(In3, 0);
  analogWrite(In4, 0);
//  delay(2000);
}

void MotorTest(){
  MoveUp();
  MoveDown();
  MoveLeft();
  MoveRight();
  MoveStop();

  delay(3000);
  
  MoveUp();
  MoveStop();
  MoveDown();
  MoveStop();
  MoveLeft();
  MoveStop();
  MoveRight();
  MoveStop();
}
  
