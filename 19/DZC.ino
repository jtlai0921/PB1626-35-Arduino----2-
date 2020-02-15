#include <HX711.h>

// Hx711.DOUT: A1
// Hx711.SCK:  A0

Hx711 scale(A1, A0);

void setup() 
{
  Serial.begin(9600);
  if(readKey() == 0x77)//判斷是不是第一次使用
  {
    HX711Init();//起始化HX711
  }
  else//第一次使用
  {
    Adjust();//校准電子稱
    saveKey(0x77);//儲存key值到eeprom
    HX711Init();//起始化HX711
  }
}

void loop() 
{
  Serial.print((int)scale.getWeight(5));//測量和顯示重量
  Serial.println("g");
}
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

/*
HX711起始化
*/
void HX711Init(void)
{
  Serial.println("System Init,Please Wait...");
  
  long offset= scale.getAverageValue(30);//計算偏移量(此時稱必須保持水平且稱上不能有東西！！！)
  scale.setOffset(offset);//設定偏移
  scale.setScale(readScale());//設定比例(從eeprom中讀取)
}

/*
校准程式
*/
void Adjust(void)
{
  long x0,x1;
  x0 = scale.getAverageValue(30);//取0g時候的值
  Serial.println("Please put the things which you have already know it's weights.");
  Serial.println("Inpute '0' to mark sure......");
  while(true)
  {
    if(Serial.available() > 0)
    {
      if(Serial.read() == '0') break;
    }
  }
  x1 = scale.getAverageValue(30);//取放物品時候的值
  Serial.println("Inpute the weight......");
  
  //接收一字串
  String s = ""; 
  while(true)
  {
    while (Serial.available() > 0)//接收
    {
      s += char(Serial.read());
      delay(2);
    }
    if(s.length()>0)//接收完成
    {
      break;
    }
  }
  int weight = s.toInt(); //字串轉整形資料
  float temp = (float)(x1-x0)/weight;//計算比例值
  scale.setScale(temp);//比例值寫入Hx711類別
  saveScale(temp);//儲存比例值
  Serial.println("Adjust Completed!");
}





