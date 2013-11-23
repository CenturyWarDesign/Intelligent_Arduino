
/*
 * IRRemote 紅外線遙控教學
 *   範例 2.1v2: 發射紅外線訊號
 *     注意! 紅外線發射器(IR LED)必須接到 pin 3, 不能接其它腳位
 */
#include <IRremote.h>                    // 引用 IRRemote 函式庫

IRsend irsend;                           // 定義 IRsend 物件來發射紅外線訊號
int ledPin = 13; //定义数字10 接口
void setup()
{
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);                     // 開啟 Serial port, 通訊速率為 9600 bps
}

void loop() 
{
  // 檢查 Serial Port 是否有資料
  // 有的話&#65292;Serial.read() 會回傳非 -1 的數值   
    // 發射紅外線訊號
    digitalWrite(ledPin, HIGH); //点亮小灯
delay(10); //延时1 秒
digitalWrite(ledPin, LOW); //熄灭小灯
delay(10); // 延时1 秒

    irsend.sendRC5(0x1D2FEFF6, 32);   // 我電視 Power 鈕的紅外線編碼, 記得換成你的紅外線編碼 
   delay(2000);
}



