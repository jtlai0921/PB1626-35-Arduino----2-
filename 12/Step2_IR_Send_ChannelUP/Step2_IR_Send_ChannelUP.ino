/*
* 發射紅外線訊號
* 注意! 紅外線發射器(IR LED)必須接到 pin 3, 不能接其它腳位
 
 Hisense 機頂盒 SD-16127 tested
 */
 
//By Leo 2013.02.19
//http://weibo.com/leolite
//Update: Pin 3 IR LED test function

 
#include <IRremote.h>                  // 參考 IRRemote 函數庫


IRsend irsend;                          // 定義 IRsend 物件來發射紅外線訊號

void setup()
{
  pinMode(3, OUTPUT);   
  digitalWrite(3, HIGH);   // 點亮LED測試（需透過手機攝影模式觀察）
  delay(3000);             // 等待3秒
  digitalWrite(3, LOW);    // 結束測試
}

void loop()
{
  Serial.print("SendIR: ");
  irsend.sendNEC(0x807F18E7, 32);   // 記得換成你遙控器的紅外線解碼
  delay(3000);             // 等待3秒
}




