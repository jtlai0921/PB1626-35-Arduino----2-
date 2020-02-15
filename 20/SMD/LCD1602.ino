#include <LiquidCrystal.h>
#include "config.h"
/*
A0 RS
A1 E
0 D4
1 D5
2 D6
3 D7
*/

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(A0, A1, 0, 1, 2, 3);

u16 timedata[7];
u16 set_timdat[7];

//1602液晶屏起始化
void lcd1602Init(void)
{
  lcd.begin(16, 2);
}

//////////////////////////////////////////////////////////////////////////////////////////
//1602清除螢幕
void clearLCD(void)
{
  lcd.clear();
}
//設定游標閃爍及位置
void setLcdCursor(u8 x,u8 y)
{
  lcd.setCursor(x,y);
  lcd.blink();
}
//關閉游標閃爍
void closeCursor(void)
{
  lcd.noBlink();  
}
//////////////////////////////////////////////////////////////////////////////////////////

//1602上顯示時間
void dispalyTime()
{
  lcd.setCursor(0,0);
  lcd.print(timedata[6]);
  lcd.print("-");
  if(timedata[5]>=10)
  {
    lcd.print(timedata[5]);
  }
  else
  {
    lcd.print(0);
    lcd.print(timedata[5]);
  }
  lcd.print("-");
  if(timedata[4]>=10)
  {
    lcd.print(timedata[4]);
  }
  else
  {
    lcd.print(0);
    lcd.print(timedata[4]);
  }
  lcd.setCursor(0, 1);
  if(timedata[3]>=10)
  {
    lcd.print(timedata[3]);
  }
  else
  {
    lcd.print(0);
    lcd.print(timedata[3]);
  }
  lcd.print(":");
  if(timedata[2]>=10)
  {
    lcd.print(timedata[2]);
  }
  else
  {
    lcd.print(0);
    lcd.print(timedata[2]);
  }
  lcd.print(":"); 
  if(timedata[1]>=10)
  {
    lcd.print(timedata[1]);
  }
  else
  {
    lcd.print(0);
    lcd.print(timedata[1]);
  }
  lcd.print(" [");
  lcd.print(timedata[0]);
  lcd.print("]");
}

//1602 顯示設定時間界面
void displaySetTime(u8 p)
{
  lcd.noBlink();
  lcd.setCursor(0,0);
  lcd.print(set_timdat[6]);
  lcd.print("-");
  if(set_timdat[5]>=10)
  {
    lcd.print(set_timdat[5]);
  }
  else
  {
    lcd.print(0);
    lcd.print(set_timdat[5]);
  }
  lcd.print("-");
  if(set_timdat[4]>=10)
  {
    lcd.print(set_timdat[4]);
  }
  else
  {
    lcd.print(0);
    lcd.print(set_timdat[4]);
  }
  lcd.print(" SET!");
  lcd.setCursor(0, 1);
  if(set_timdat[3]>=10)
  {
    lcd.print(set_timdat[3]);
  }
  else
  {
    lcd.print(0);
    lcd.print(set_timdat[3]);
  }
  lcd.print(":");
  if(set_timdat[2]>=10)
  {
    lcd.print(set_timdat[2]);
  }
  else
  {
    lcd.print(0);
    lcd.print(set_timdat[2]);
  }
  lcd.print(":"); 
  if(set_timdat[1]>=10)
  {
    lcd.print(set_timdat[1]);
  }
  else
  {
    lcd.print(0);
    lcd.print(set_timdat[1]);
  }
  lcd.print(" [");
  lcd.print(set_timdat[0]);
  lcd.print("]");
  /////////////////
  switch(p)
  {
    case 0:
    {
      setLcdCursor(7,1);
      break;
    }
    case 1:
    {
      setLcdCursor(4,1);
      break;
    }
    case 2:
    {
      setLcdCursor(1,1);
      break;
    }
    case 3:
    {
      setLcdCursor(9,0);
      break;
    }
    case 4:
    {
      setLcdCursor(6,0);
      break;
    }
    case 5:
    {
      setLcdCursor(3,0);
      break;
    }
  }
  delay(12);
}

//顯示設定ID界面
void displaySetID(u8 point,u32 ID)
{
  lcd.noBlink();//關閉游標閃爍！
  //更新8位ID號
  lcd.setCursor(0,0);//設定游標
  lcd.print("ID:");
  lcd.print(ID);
  
  //開啟對應的游標閃爍
  switch(point)
  {
    case 0:
    {
      setLcdCursor(10,0);
      break;
    }
    case 1:
    {
      setLcdCursor(9,0);
      break;
    }
    case 2:
    {
      setLcdCursor(8,0);
      break;
    }
    case 3:
    {
      setLcdCursor(7,0);
      break;
    }
    case 4:
    {
      setLcdCursor(6,0);
      break;
    }
    case 5:
    {
      setLcdCursor(5,0);
      break;
    }
    case 6:
    {
      setLcdCursor(4,0);
      break;
    }
    case 7:
    {
      setLcdCursor(3,0);
      break;
    }
  }
  delay(20);
}

///////////////////////////////////////////////////////////////////////////////////

//在1602上顯示一個數字
void showNum(u8 x,u8 y,u32 num)
{
  lcd.setCursor(x,y);
  lcd.print(num); 
}
//在1602上顯示一串字元
void showString(u8 x,u8 y,String s)
{
  lcd.setCursor(x,y);
  lcd.print(s);
}
///////////////////////////////////////////////////////////////////////////////////


