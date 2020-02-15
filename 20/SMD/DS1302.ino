 #include <DS1302.h>
#include  "config.h"
/*
5 CE
6 SCK
7 SIO
*/

int CE_PIN = 5;
int IO_PIN = 7;
int SCLK_PIN = 6;
DS1302 rtc(CE_PIN, IO_PIN, SCLK_PIN);

extern u16 timedata[7];//時間暫存器
extern u16 set_timdat[7];//設定時間暫存器

//起始化DS1302
void ds1302Init(void)
{
  u8 x;
  x = rtc.read_register(0x81);//開啟時間走時
  x = x&0x7F;
  rtc.write_register(0x80,x);
}

//取得時間
//時間儲存到timedata[7]陣列中
void getTime(void)
{
  /*取得DS1302時間 */
  Time t = rtc.time();
  
  timedata[0] = t.day;
  timedata[1] = t.sec;
  timedata[2] = t.min;
  timedata[3] = t.hr;
  timedata[4] = t.date;
  timedata[5] = t.mon;
  timedata[6] = t.yr;
}

//判斷是否是閏年
//傳回：0不是 1是
u8 Is_Leap_Year(u16 year)
{
  if((year%4==0 && year%100!=0)||(year%400==0))
    return 1;
  else
    return 0;	
}

//透過年、月、日得出星期
u8 Get_Week(u16 year,u8 month,u8 day)
{
  int week;
  if (month == 1 || month == 2)
  {
    month += 12;
    year--;
  }	
  week =(day+2*month+3*(month+1)/5+year+year/4-year/100+year/400)%7;
  week++;
  return week;
}

//設定時間
void setTime(void)
{
  u8 i;
  u8 point=0; //模式指標
  u8 k_val;//按鍵值�
  
  clearLCD();//清除螢幕 
  
  for(i=0;i<7;i++)//設定時間快取器  給予值
  {
    set_timdat[i] = timedata[i];
  }
  
  while(1)
  {
     k_val = scanKey();//得出按鍵值
     set_timdat[0] =  Get_Week(set_timdat[6],set_timdat[5],set_timdat[4]);//得出星期
     if(k_val == 1)//按下K1 資料加一
     {
       switch(point)
       {
         case 0://秒 加一
         {
           set_timdat[1]++;
           if(set_timdat[1] == 60) set_timdat[1] = 0;
           break;
         }
         case 1://分 加一       
         {
           set_timdat[2]++;
           if(set_timdat[2] == 60) set_timdat[2] = 0;
           break;
         }
         case 2://時 加一    
         {
           set_timdat[3]++;
           if(set_timdat[3] == 24) set_timdat[3] = 0;
           break;
         }
         case 3://日 加一�  
         {
           set_timdat[4]++;
           
           if(set_timdat[5]==1||set_timdat[5]==3||set_timdat[5]==5||set_timdat[5]==7||set_timdat[5]==8||set_timdat[5]==10||set_timdat[5]==12)//1 3 5 7 8 10 12月的情況（31天）
           {
             if(set_timdat[4] == 32) set_timdat[4] = 1;
           }
           else if(set_timdat[5]==4||set_timdat[5]==6||set_timdat[5]==9||set_timdat[5]==11)// 4 6 9 11月的情況（30天）
           {
             if(set_timdat[4] == 31) set_timdat[4] = 1;
           }
           else//2月的情況（28、29天）
           {
             if(Is_Leap_Year(set_timdat[6]) == 1)
             {
               if(set_timdat[4] == 30) set_timdat[4] = 1;
             }
             else
             {
               if(set_timdat[4] == 29) set_timdat[4] = 1;
             }
           }
           break;
         }
         case 4://月 加一�     
         {
           set_timdat[5]++;
           if(set_timdat[5] == 13) set_timdat[5] = 1;
           
           if(set_timdat[5]==4||set_timdat[5]==6||set_timdat[5]==9||set_timdat[5]==11)// 4 6 9 11月時 判斷日是否超出本月最大天數�     
           {
             if(set_timdat[4] > 30) set_timdat[4] = 30;
           }
           if(set_timdat[5]==2)//2月時 判斷日是否超出本月最大天數�         
           {
             if(Is_Leap_Year(set_timdat[6]) == 1)
             {
               if(set_timdat[4] > 29) set_timdat[4] = 29;
             }
             else
             {
               if(set_timdat[4] > 28) set_timdat[4] = 28;  
             }
           }
           break;
         }
         case 5://年 加一�    
         {
           set_timdat[6]++;
           if(set_timdat[6] == 3000) set_timdat[6] = 2000;
           if(Is_Leap_Year(set_timdat[6]) == 0)//判斷月為二月時  日是否超出最大天數
           {
             if(set_timdat[5]==2 && set_timdat[4]>=29) set_timdat[4] = 28;
           }
           break;
         }
       }
     }
     else if(k_val == 2)//設定位切換 加一
     {
       point++;
       if(point == 6) point = 0;
     }
     else if(k_val == 3)//設定完成 離開
     {
       break;
     }
     displaySetTime(point);//更新在LCD上的顯示
  }
  
  closeCursor();//關閉游標閃爍
  clearLCD();//清除螢幕
  Time t(set_timdat[6],set_timdat[5],set_timdat[4],set_timdat[3],set_timdat[2],set_timdat[1],set_timdat[0]);//向DS1302設定時間
  rtc.time(t);
}

