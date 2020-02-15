#include <SD.h>
#include "config.h"
/*
4 CS
11 MOSI
12 MISO
13 CLK
*/

const int sd_cs = 4;  //設定CS接口

//起始化SD卡
void sdInit(void)
{ 
  pinMode(10, OUTPUT);
  
  if (!SD.begin(sd_cs)) 
  {
    while(1)
    {
      //若果從CS口與SD卡通訊失敗，1602輸出訊息Card failed
      showString(0,0,"Card falied!");
      delay(1000);
    } 
  }
  else
  {
    //若果從CS口與SD卡通訊失敗，1602輸出訊息Card initialized
    showString(0,0,"Card initialized!");
  }
}

//向SD卡中寫入簽到記錄資料（時間和ID號）
//id:為簽到的ID
u8 writeSd(u32 id)
{
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile)
  {
    //寫入ID
    dataFile.print(id);dataFile.print(">>");
    //寫入年
    dataFile.print(timedata[6]);dataFile.print("-");
    //寫入月
    dataFile.print(timedata[5]);dataFile.print("-");
    //寫入日
    dataFile.print(timedata[4]);dataFile.print("/");
    //寫入時
    dataFile.print(timedata[3]);dataFile.print(":");
    //寫入分
    dataFile.print(timedata[2]);dataFile.print(":");
    //寫入秒
    dataFile.print(timedata[1]);dataFile.print(" [");
    //寫入星期
    dataFile.print(timedata[0]);dataFile.print("]");
    dataFile.println(" ");//換行
    
    //關閉檔案
    dataFile.close();
    return 1;
  }  
  // 若果無法開啟檔案 傳回0
  else
  {
    return 0;
  }
  
}

