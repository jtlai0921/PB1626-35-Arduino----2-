#include <HX711.h>

// Hx711.DOUT: A1
// Hx711.SCK:  A0
Hx711 scale(A1, A0);

void HX711Init(void)
{
  Serial.println("System Init,Please Wait...");
  
  long offset= scale.getAverageValue(30);//計算偏移量(此時稱必須保持水平且稱上不能有東西！！！)
  scale.setOffset(offset);//設定偏移
  scale.setScale(405);//設定比例(此值需要根據不同的重量感知器自己設定！)
}

void setup() 
{
  Serial.begin(9600);
  HX711Init();
}

void loop() 
{
  Serial.print((int)scale.getWeight(10));
  Serial.println("g");
}


