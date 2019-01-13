# ARDUINO Practice
## Project 01 "Guess The Numbers Game 1bit"
### About
2017.3.24  ARDUINO 猜數字1bit

遊戲方法:<br/>
1.切開關 產生答案、遊戲指示燈亮 (指撥開關 LEDB)<br/>
2.轉VR選擇要猜的數字  (LEDB 隨VR改變亮度)<br/>
3.按下確認 (SW左)<br/>
4.LED顯示結果 (G: BINGO R: TOO HIGH Y: TOO LOW B:ON/OFF)<br/>
5.LCD顯示狀態 bingo時撥放音樂 <br/>
6.偷看答案 (SW右)<br/>
=============================================*/
### Online Simulation link
https://circuits.io/circuits/4492723-guess-the-numbers-game-1bit
<br/>
### Circuit
![guess the number game 1bit](https://cloud.githubusercontent.com/assets/24741602/24718316/259dde52-1a68-11e7-897d-65f4b0c956eb.png)
<br/><br/>
## Project 02 "Guess The Numbers Game 4bit"
### About
2017.4.06  ARDUINO 猜數字4bit

遊戲方法:<br/>
1.切開關 產生答案、遊戲指示燈亮 (指撥開關 LEDG)<br/>
2.利用SW(左) 依序輸入4個數字 <br/>
3.按下確認 (SW右)<br/>
4.LCD顯示狀態 <br/>
5.LCD顯示狀態 bingo時撥放音樂 <br/>
6.LED顯示結果 (G:Power light  Y:數字對位置正確(A)  R:數字對位置不正確(B) <br/>

### Online Simulation link
https://circuits.io/circuits/4536918-4-bit-guess-the-number-game
<br/>
### Circuit
![guess the number game 4bit](https://cloud.githubusercontent.com/assets/24741602/24718764/c5f63308-1a69-11e7-9677-7c979cc54ee8.png)
<br/><br/>
## Project 03 "The Small Shooting Game"
### About
2017.4.22  ARDUINO 射擊小遊戲

遊戲方法:<br/>
1.接上電源後，倒數3秒開始遊戲<br/>
2.LED黃: 玩家位置 LED紅: 敵人位置  <br/>
3.左按鈕: 玩家向左 右按鈕: 玩家向右 中間按鈕: 射擊  <br/
4.LCD顯示Level & Score <br/>
5.通過全部關卡後 LCD顯示YOU WIN! LED全亮 持續3秒<br/>
6.LCD顯示 Play again? 按下任意按鈕重新開始 <br/>

### Online Simulation link
https://circuits.io/circuits/4696616-the-shooting-game
<br/>
### Circuit
![shooting game](https://cloud.githubusercontent.com/assets/24741602/25300282/6e597aec-273e-11e7-8e46-1ed8fea82a98.png)
<br/><br/>
## Project 04 "The Password System"
### About
2017.4.23  ARDUINO 密碼鎖

動作簡介:<br/>
1.自行決定是否重設密碼
2.三次解碼機會
3.解碼成功後馬達動作(開門之類的)、LED綠亮，持續3秒
4.累積三次失敗警報響、LED紅閃爍

操作方法:<br/>
1.依照LCD指示操作鍵盤<br/>
2.鍵盤: "#":ENTER  "*":BACKSPACE  <br/>
3.初始密碼: 0000 <br/>

### Online Simulation link
https://circuits.io/circuits/4720646-easy-password-system
<br/>
### Circuit
![password system](https://cloud.githubusercontent.com/assets/24741602/25311969/e4ba6c1c-283f-11e7-9756-332eb125c14f.png)
<br/><br/>
## Project 05 "4*4 Minesweeper"
### About
2017.6.11  ARDUINO 4*4 踩地雷

動作簡介:<br/>
1.四種難度(簡單:1 中等:6 困難:8 自訂:自訂地雷數)
2.勝利條件: 1.立完所有旗桿 2.踩完所有無地雷區

操作方法:<br/>
1.六個按鈕: swup : move X axis + swdown : Move X axis - swyup : Move Y axis - swydown : Move Y axis + swenter : Enter swflag : Set Flag<br/>

### Online Simulation link
https://circuits.io/circuits/5196853-4-4-minesweeper
<br/>
### Circuit
![minesweeper](https://user-images.githubusercontent.com/24741602/27007156-bf7d2cac-4e7b-11e7-8c77-b77a3fcf0370.png)

## Project 06 "Thread Practice : Switch Drive Motor "
### About
2017.11.19  ARDUINO Thread Practice

動作簡介:<br/>
switch控制馬達運轉模式 (LED代替馬達)，LCD顯示運轉模式和時間，分為正轉模式，反轉模式和自訂模式，外加一顆正常停止鈕與緊急停止鈕。
正轉模式: 持續正轉殖到使用者切斷才可切換轉向。
反轉模式: 持續反轉殖到使用者切斷才可切換轉向。
自訂模式: 以lcd選單選擇運轉段數，每段轉向與運轉時間並重複執行直到使用者切斷。

操作方法:<br/>
1.四個按鈕: 利用switch來作狀態切換(正轉 反轉 自訂)和 緊急停止<br/>

![switchmotor](https://user-images.githubusercontent.com/24741602/32988307-4ece831c-cd3c-11e7-958f-3ff9118b8da2.jpg)

## Project 07 "Bot"
### About
2017.12.10  ARDUINO + Node js + App Practice

動作簡介:<br/>
1.藉由爬蟲取得即時資訊(PM2.5 天氣預報 PTT標題)
2.藍芽操控 Arduino 元件
3.Arduino 藍芽通知 APP 訊息 

操作方法:<br/>
1.APP via bluetooth send cmd to Arduino
2.Arduino via Serial Port send cmd to computer's Node js
3.Node js creat a server with data then APP can connect the Server

![screenshot_20171210-104123](https://user-images.githubusercontent.com/24741602/33802431-162f3d68-ddb2-11e7-8f22-0fa9c176dc2a.jpg)
![p_20171210_104001](https://user-images.githubusercontent.com/24741602/33802435-249cfed0-ddb2-11e7-95d5-345650fe1193.jpg)

## Project08(Jump & Squat GAME)
### About
2017.12.22  ARDUINO Thread Practice

動作簡介:<br/>
1.透過站立、蹲下、跳躍來躲避障礙物
2.轉VR可以改變速度
3.碰到障礙 HP-1 Led 燈熄1顆 (預設死亡重新開始)

操作方法:<br/>
1.按按鈕 做跳和蹲的動作(P:站立 b:蹲下 o、q、d:障礙物)

![01](https://user-images.githubusercontent.com/24741602/34301473-b00e07ca-e767-11e7-97f0-e9e55968375e.jpg)
![02](https://user-images.githubusercontent.com/24741602/34301475-b276e194-e767-11e7-9acb-c9675050db93.jpg)
![03](https://user-images.githubusercontent.com/24741602/34301477-b45d462e-e767-11e7-87c7-51d23bfaa3c0.jpg)

### IDE
AUTODESK CIRCUITS <br/>
https://circuits.io/
