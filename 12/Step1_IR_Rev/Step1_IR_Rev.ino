//本例來自於紅外接收模組的IRremote附帶范例
/*
 * IRremote: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * [url]http://arcfn.com[/url]
 */
 
#include <IRremote.h>                    // 呼叫IRRemote庫
 
int RECV_PIN = 11;
 
IRrecv irrecv(RECV_PIN);                 //定義接收器的輸入管腳
 
decode_results results;                  //定義儲存解碼的變數results
 
void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn();                   //啟動紅外接收
}
 
void loop() {
  if (irrecv.decode(&results)) {         //解碼成功，把資料放入results變數中
    Serial.println(results.value, HEX);  //以16進位格式顯示紅外解碼
    irrecv.resume();                     //接收下一個值
  }
}
