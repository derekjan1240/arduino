#include <SoftwareSerial.h>
#include <Wire.h>
#include <StaticThreadController.h>
#include <Thread.h>
#include <ThreadController.h>
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F,2,1,0,4,5,6,7,3,POSITIVE);  // Set the LCD I2C address

// Pin set ------------------------  
const int Button_Timer = 2;
const int LED_Red = 3;
const int LED_Yellow = 5;
const int LED_Green = 6;
const int LED_Up = 4;
const int LED_Down = 7;
const int LED_Left = 8;
const int LED_Right = 9;
const int LED_Boom = 12;
const int Button_Value = 15;  //A1

SoftwareSerial I2CBT(10,11); //11rx 10tx


// Global Variables set ------------------------
enum MainState {None, Crawling, Control};        // 無狀態、爬蟲、機器人控制
enum LedState {Off ,Red, Yellow, Green};         // 爬蟲、機器人
enum TimerState {Counting ,Boom, Stop, Setting};           // 計時中、響鈴中、停止中、設置中 

char cmd_convert[20];
int MainState, LedState;
int TimerState;
char incomingByte;
char CrawlingMsg;
int TimerTime = 0;
unsigned long start_time;
bool isappcallstop = 0;

// Thread set ------------------------
ThreadController controll = ThreadController();
Thread ThreadBlueTooth_Listening = Thread();
Thread ThreadSerial_Sending = Thread();
Thread ThreadTimer = Thread();

void setup() {
  
  pinMode(LED_Red, OUTPUT);
  pinMode(LED_Yellow, OUTPUT);
  pinMode(LED_Green, OUTPUT);
  pinMode(LED_Up, OUTPUT);
  pinMode(LED_Down, OUTPUT);
  pinMode(LED_Left, OUTPUT);
  pinMode(LED_Right, OUTPUT);
  pinMode(LED_Boom, OUTPUT);
  pinMode(Button_Timer, INPUT);
  pinMode(Button_Value, INPUT);
 
  I2CBT.begin(9600);                            //Bluetooth baud rate 
  Serial.begin(9600);                            //Serial baud rate 
  lcd.begin(16, 2);

  ThreadBlueTooth_Listening.onRun(BlueTooth_Listening);
  ThreadBlueTooth_Listening.setInterval(50);
  ThreadSerial_Sending.onRun(Serial_Sending);
  ThreadSerial_Sending.setInterval(50);
  ThreadTimer.onRun(Countdown);
  ThreadTimer.setInterval(50);

  controll.add(&ThreadBlueTooth_Listening);     //等待使用者(APP)指令
  controll.add(&ThreadSerial_Sending);          //等待傳送爬蟲目標指令 
  controll.add(&ThreadTimer);                   
  

  MainState = None;   //初始無狀態
  LedState = Off;
  TimerState = Stop;
}

void loop() {
  controll.run(); 
}

void Countdown()
{
  if(isappcallstop == 1 )     // APP強制Reset
  {
     TimerState = Stop;
     digitalWrite(LED_Boom,LOW);
     lcd.setCursor(13, 0);
     lcd.print("   ");
     TimerTime = 0;     
     isappcallstop = 0;
  } 
  
  if(TimerState == Stop)
  {
      if(digitalRead(Button_Timer) == HIGH)
      {
          while(digitalRead(Button_Timer) == HIGH){}
          TimerState = Setting;
      }
      else
      {
         lcd.setCursor(13, 0);
         lcd.print(TimerTime);
         lcd.setCursor(15, 0);
         lcd.print("N");
      }
  }
  
  if(TimerState == Setting)
  {    
      TimerTime = map(analogRead(Button_Value), 0, 1023, 1, 100); 

      if(TimerTime >= 10)
      {
        lcd.setCursor(12, 0);
        lcd.print(TimerTime);
      }
      else
      {
        lcd.setCursor(12, 0);
         lcd.print(" ");
        lcd.setCursor(13, 0);
        lcd.print(TimerTime);
      }
     
      lcd.setCursor(15, 0);
      lcd.print("S");
     
      if(digitalRead(Button_Timer) == HIGH)
      {
         while(digitalRead(Button_Timer) == HIGH){}
         TimerState = Counting;
         start_time = millis();
      }               
  }
  
  if(TimerState == Counting)
  {
      int last_time = TimerTime - (millis() - start_time)/1000;

      if(last_time >= 10)
      {
         lcd.setCursor(12, 0);
         lcd.print(last_time);
      }
      else
      {
         lcd.setCursor(12, 0);
         lcd.print(" ");
         lcd.setCursor(13, 0);
         lcd.print(last_time);
      }
      lcd.setCursor(15, 0);
      lcd.print("C");

      if(last_time<=0) 
      {
         TimerState = Boom;
         TimerTime = 0;
      } 
  }

  if(TimerState == Boom)
  {
      byte packet_timeout = 100; //"Boom"
    
      lcd.setCursor(13, 0);
      lcd.print(TimerTime);
      lcd.setCursor(15, 0);
      lcd.print("B");

      //send msg to app
      I2CBT.write(packet_timeout);
     
      digitalWrite(LED_Boom,HIGH);

      if(digitalRead(Button_Timer) == HIGH)
      {
         while(digitalRead(Button_Timer) == HIGH){}
         digitalWrite(LED_Boom,LOW);
         TimerState = Stop;
         isappcallstop = 0;
      }     
  } 
}


void BlueTooth_Listening()  
{  
    byte cmd[20];
    int insize = I2CBT.available();

    if (insize > 0)
    {
       for (int i=0; i<insize; i++)
       {
          cmd[i]=char(I2CBT.read());
          cmd_convert[i] = cmd[i];
          Serial.print(cmd_convert[i]);
          Serial.print("\n"); 
       }
       
       //APP stop boom
       if(cmd_convert[0] == 'S' && cmd_convert[1] == 't'  && cmd_convert[2] == 'o'  && cmd_convert[3] == 'p' )                 
       {isappcallstop = 1;}
             
       //無狀態 --------------------------------------------------------------------------------------------------------------------------- 
       if(MainState == None)                // Waitting for MainState Change
       {
          lcd.clear();
                    
          //切換爬蟲狀態 等待爬蟲指令
          if(cmd_convert[0] == 'C' )                // buletooth msg:"Crawling"
          {MainState = Crawling;} 

          //切換控制狀態 等待控制指令   
          else if(cmd_convert[0] == 'L' )           // buletooth msg:"Led"
          {MainState = Control;} 
       }

       
       //爬蟲狀態的 Listening -------------------------------------------------------------------------------------------------------------   
       if(MainState == Crawling)            // Decide Serial_Sending'Msg
       {
          //Serial.println("Crawling");
          lcd.clear();
          lcd.print("Crawling:");
          lcd.print(cmd_convert[0]);
 
          if(cmd_convert[0] == '1' )                     
          {CrawlingMsg = '1';}  //pm2.5
          else if(cmd_convert[0] == '2' )                
          {CrawlingMsg = '2';}  // weather
          else if(cmd_convert[0] == '3' )                
          {CrawlingMsg = '3';}  // ptt
          else if(cmd_convert[0] == 'E' && cmd_convert[1] == 'n' && cmd_convert[2] == 'd' )                
          {
            MainState = None; // 回到無狀態
            BufferReset();
          }  

       }

       //控制狀態的 Listening -------------------------------------------------------------------------------------------------------------
       if(MainState == Control)
       {
          bool isdircmd = 0;

          lcd.clear();
          lcd.print("Control");
          lcd.print(cmd_convert[0]);
          
          // End Control -----------------------------------------------------
          if(cmd_convert[0] == 'E' &&  cmd_convert[1] == 'n' &&  cmd_convert[2] == 'd')                     
          {
            lcd.clear();
            lcd.print("End Control");            
            digitalWrite(LED_Up,LOW);
            digitalWrite(LED_Down,LOW);
            digitalWrite(LED_Left,LOW);
            digitalWrite(LED_Right,LOW);
            analogWrite(LED_Red, 0);
            analogWrite(LED_Yellow, 0);
            analogWrite(LED_Green, 0);

            MainState = None; // 回到無狀態
            BufferReset();
          }  
          
          // Direction Control -------------------------
          //up
          if(cmd_convert[0] == 'U' &&  cmd_convert[1] == 'p')                     
          {
            //Serial.println("up");
            isdircmd = 1;
            lcd.clear();
            lcd.print("UP");            
            digitalWrite(LED_Up,HIGH);
            digitalWrite(LED_Down,LOW);
            digitalWrite(LED_Left,LOW);
            digitalWrite(LED_Right,LOW);      
            BufferReset();
          } 
          
          //down
          else if(cmd_convert[0] == 'D' &&  cmd_convert[1] == 'o' && cmd_convert[2] == 'w' &&  cmd_convert[3] == 'n')                
          {
            //Serial.println("down");
            isdircmd = 1;
            lcd.clear();
            lcd.print("Down"); 
            digitalWrite(LED_Up,LOW);
            digitalWrite(LED_Down,HIGH);
            digitalWrite(LED_Left,LOW);
            digitalWrite(LED_Right,LOW);
            BufferReset();
          } 

          //left
          else if(cmd_convert[0] == 'L' &&  cmd_convert[1] == 'e' && cmd_convert[2] == 'f' &&  cmd_convert[3] == 't')                
          {
            //Serial.println("left");
            isdircmd = 1;
            lcd.clear();
            lcd.print("Left"); 
            digitalWrite(LED_Up,LOW);
            digitalWrite(LED_Down,LOW);
            digitalWrite(LED_Left,HIGH);
            digitalWrite(LED_Right,LOW);
            BufferReset();
          }  

          //right
          else if(cmd_convert[0] == 'R' &&  cmd_convert[1] == 'i' && cmd_convert[2] == 'g' &&  cmd_convert[3] == 'h' &&  cmd_convert[4] == 't')                
          {
            //Serial.println("right");
            isdircmd = 1;
            lcd.clear();
            lcd.print("Right"); 
            digitalWrite(LED_Up,LOW);
            digitalWrite(LED_Down,LOW);
            digitalWrite(LED_Left,LOW);
            digitalWrite(LED_Right,HIGH);
            BufferReset();
          }  
         
          // Led Control ------------------------------ 
          if(cmd_convert[0] == 'r')                     
          {
            LedState = Red;          
            //Serial.println("Red led"); 
            lcd.clear();
            lcd.print("Red led");
            BufferReset();         
          } 
          else if(cmd_convert[0] == 'y')                     
          {
            LedState = Yellow;
            //Serial.println("Yelloe led");
            lcd.clear();
            lcd.print("Yellow led");
            BufferReset();
          } 
          else if(cmd_convert[0] == 'g')                     
          {
            LedState = Green;
            //Serial.println("Green led");
            lcd.clear();
            lcd.print("Green led");
            BufferReset();
          } 

          // Led Value -----------------------------------------------------
          if(LedState == Red && isdircmd == 0)                     
          {
            int final_value = ValueAnalysis();                      // 抓最後值
            int brightness = map(final_value, 0, 100, 0, 255);
            analogWrite(LED_Red, brightness);
            lcd.setCursor(0, 1);
            lcd.print("Value:"); 
            lcd.print(final_value);
            lcd.print("% "); 
            lcd.print(brightness);
            lcd.print("/255");  
            BufferReset();
          } 
          else if(LedState == Yellow && isdircmd == 0)                     
          {
            int final_value = ValueAnalysis();           
            int brightness = map(final_value, 0, 100, 0, 255);
            analogWrite(LED_Yellow, brightness);
            lcd.setCursor(0, 1);
            lcd.print("Value:"); 
            lcd.print(final_value);
            lcd.print("% "); 
            lcd.print(brightness);
            lcd.print("/255"); 
            BufferReset();
          } 
          else if(LedState == Green && isdircmd == 0)                     
          { 
            int final_value = ValueAnalysis();
            int brightness = map(final_value, 0, 100, 0, 255);
            analogWrite(LED_Green, brightness);
            lcd.setCursor(0, 1);
            lcd.print("Value:"); 
            lcd.print(final_value);
            lcd.print("% "); 
            lcd.print(brightness);
            lcd.print("/255"); 
            BufferReset();
          }                 
       }
       
       //reset cmd
       cmd_convert[0] = '0';       
    }
}


void Serial_Sending() // MainState == Crawling => 決定爬蟲目標
{
   if( MainState == Crawling && (CrawlingMsg == '1' || CrawlingMsg == '2' || CrawlingMsg == '3'))
   {
      //send cmd to computer
      Serial.flush();
      Serial.println(CrawlingMsg);
      
      CrawlingMsg = '4'; 
      //Reset State 
      //MainState = None;
   }
}

int ValueAnalysis()
{
    int final_value = 0;

    for(int j=5; j>=0; j--)                  // 分析value
    {
       if(cmd_convert[j]!= '0')
       {
          final_value = int(cmd_convert[j]);
          break;
       }
       else
       {
          Serial.print("cmd_convert[");
          Serial.print(j);
          Serial.println("] = 0");
       }             
    }

    return final_value;
}

void BufferReset()
{
  for(int j=0; j<21; j++)
  {
    cmd_convert[j] = {'0'};
  }
}
