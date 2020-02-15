#include <EEPROM.h>

/*
儲存校准值(eeprom addr 1-4)
*/
void saveScale(unsigned long x)
{
  //存入內定eeprom（先存地位  再存高位 1-4）
  for(int i=0;i<4;i++)
  {
    EEPROM.write(1+i,(x>>i*8)&0xFF);
    delay(1);
  }
}

/*
讀取校准值(eeprom addr 1-4)
*/
unsigned long readScale()
{
  //float資料低位到高位 位址 ：1-4
  unsigned long temp = 0;
  for(int i=0;i<4;i++)
  {
    temp = temp<<8;
    temp |= EEPROM.read(4-i);
  }
  return temp;
}

/*
寫入密鑰值(eeprom addr 0)
*/
void saveKey(unsigned char key)
{
  EEPROM.write(0,key);
}
/*
讀取密鑰值(eeprom addr 0)
*/
unsigned char readKey()
{
  return EEPROM.read(0);
}
