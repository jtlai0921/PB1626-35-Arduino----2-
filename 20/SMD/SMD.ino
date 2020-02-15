#include "config.h"//自己定義的.h檔案 內含8\16\32位 有/無 符號的資料型態 
/*
//用到的接腳
//A0 A1 A2 A3 A4 
//0 1 2 3 4 5 6 7 11 12 13
*/

void setup()
{
  
  lcd1602Init();//1602起始化
  myKeyInit();//按鍵起始化  
  ds1302Init();//DS1302起始化�  
  
  sdInit();//SD卡起始化
  rc522_Init();//RC522起始化
  
  showString(4,1,"Waiting...");//顯示等待界面
  delay(3000);//3S後進入主界面
  clearLCD();//清除螢幕
}

void loop() 
{
  u8 key_val;//定義按鍵值
  key_val = scanKey();//取得按鍵值
  
  if(key_val == 1)//若果按下K1
  {
    TimeMode();//時間設定模式
  }
  else if(key_val == 2)//若果按下K2
  {
    SetIDMode();//ID設定模式
  }
  
  ReadIDMode();//預設進入刷卡模式  
  
  delay(100);//延時100ms
}


/*
時間設定模式
*/
void TimeMode(void)
{
  u8 k_val;//按鍵值
  
  clearLCD();//清除螢幕
  while(1)
  {
    getTime();//取得時間
    dispalyTime();//LCD上顯示時間
    k_val = scanKey();//取得按鍵值
    if(k_val == 3)//若果按下K3 則調整時間
    {
      setTime();
    }
    if(k_val == 1) //按下K1 則離開時間顯示模式！
    {
      clearLCD();//清除螢幕
      break;
    } 
  }
}

/*
刷卡模式
*/
void ReadIDMode(void)
{
  u32 id;
  
  showString(0,0,"Please check in");//顯示刷卡界面
  showString(0,1,"  with the card!");
  if(ReadID(&id)==1)
  {
    clearLCD();//清除螢幕
    showString(0,0,"Welcome!");//顯示刷卡成功界面
    showString(5,1,"ID:");
    showNum(8,1,id);//顯示ID號
    
    getTime();//取得時間
    writeSd(id);//向SD卡寫入ID號和時間
    delay(3000);//等待3S 防止重復刷卡！
    clearLCD();//清除螢幕
  } 
}

/*
設定ID模式
*/
void SetIDMode(void)
{
  u8 k_val;//按鍵值
  u8 point=0;//指向第一個位置
  u8 num[8]={1,0,0,0,0,0,0,1};//起始ID陣列為：10000001 
  u32 ID;//ID號
  
  clearLCD();//清除螢幕
  while(1)
  {  
    k_val = scanKey();//取得按鍵值
    ////////////////透過按鍵設定ID陣列//////////////////
    if(k_val == 1)//資料+1
    {
      num[point]++;
      if(point == 7)//最高位最小值為1
      {
        if(num[7] == 10) num[7] = 1;
      }
      else//其余位最小值為0
      {
        if(num[point] == 10) num[point] = 0;
      }
    }
    else if(k_val == 2)//指向加一
    {
      point++;
      if(point == 8) point = 0;//一共8位 0-7位
    }
    else if(k_val == 3)//設定完成 離開設定
    {
      closeCursor();//關閉游標閃爍
      break;
    }
    //////////////////////////////////////////////////////
    //透過ID陣列計算ID 
    ID = (u32)num[0]+(u32)num[1]*10+(u32)num[2]*100+(u32)num[3]*1000
        +(u32)num[4]*10000+(u32)num[5]*100000+(u32)num[6]*1000000+(u32)num[7]*10000000;
    //更新顯示
    displaySetID(point,ID);
  }
  
  showString(2,1,"Check card...");//顯示等待寫入ID界面
  while(1) //ID 寫入ID卡
  {
    if(WriteID(ID) == 1)//若果有卡進入射頻範圍則寫卡，並離開循環
    {
      break;
    }
    else//沒有卡進入射頻範圍 就等待
    {
      delay(100);
    }
  }
  //離開循環 則ID已寫入卡
  showString(0,1,"Write complete!");//ID寫入完成界面
  delay(3000);//等待3S 顯示寫入已成功
  clearLCD();//清除螢幕
}

