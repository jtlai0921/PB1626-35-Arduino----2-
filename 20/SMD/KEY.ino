#include "config.h"
/*
A2 K1 
A3 K2
A4 K3
*/
const int K1 = A2;
const int K2 = A3;
const int K3 = A4;

void myKeyInit(void)
{
  pinMode(K1,INPUT_PULLUP);//上拉輸入
  pinMode(K2,INPUT_PULLUP);
  pinMode(K3,INPUT_PULLUP);
}

//掃描按鍵
u8 scanKey(void)
{
  u8 i=0,mark=0;
  if(digitalRead(K1) == LOW)
  { 
    delay(5);//消抖
    if(digitalRead(K1) == LOW)//再次判K1鍵是否按下
    {
      while(digitalRead(K1) == LOW);
      return 1;
    }
  }
  else if(digitalRead(K2) == LOW)
  {
    delay(5);
    if(digitalRead(K2) == LOW) 
    {
      while(digitalRead(K2) == LOW);
      return 2;
    }
  }
  else if(digitalRead(K3) == LOW)
  {
    delay(5);
    if(digitalRead(K3) == LOW)
    {
      while(digitalRead(K3) == LOW);
      return 3; 
    }
  }
  else 
  {
    mark = 0;
    return 0;
  }
}
