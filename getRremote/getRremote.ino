
#include <IRremoteInt.h>

/*
 * IRRemote 紅外線遙控教學
 *   範例 1.1: 接收紅外線
 */
#include <IRremote.h>                    // 引用 IRRemote 函式庫

const int irReceiverPin = 2;             // 紅外線接收器 OUTPUT 訊號接在 pin 2

IRrecv irrecv(irReceiverPin);            // 定義 IRrecv 物件來接收紅外線訊號
decode_results results;                  // 解碼結果將放在 decode_results 結構的 result 變數裏

void setup()
{
  Serial.begin(9600);                     // 開啟 Serial port, 通訊速率為 9600 bps
  irrecv.enableIRIn();                   // 啟動紅外線解碼
  Serial.print("start ");
}

void loop() 
{
  if (irrecv.decode(&results)) {         // 解碼成功，收到一組紅外線訊號
    // 印到 Serial port  
    Serial.print("irCode: ");            
    Serial.print(results.value, HEX);    // 紅外線編碼
    Serial.print(",  bits: ");           
    Serial.println(results.bits);        // 紅外線編碼位元數
    irrecv.resume();                    // 繼續收下一組紅外線訊號        
  }  
}
