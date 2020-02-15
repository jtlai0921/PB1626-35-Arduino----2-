#include "HX711.h"#include "Arduino.h"//int DOUT;//定義資料接腳
//int SCK;//定義時脈接腳

//float scale;//定義比例係數
//long offset;//定義補償值
//int times;//定義採集次數

Hx711::Hx711(int IO_DOUT,int IO_SCK)//構造函數
{
	DOUT = IO_DOUT;
	SCK = IO_SCK;
	pinMode(SCK, OUTPUT);//設置IO口工作方式
	pinMode(DOUT, INPUT);}

void Hx711::setScale(float IO_scale)//設置比例係數
{
	scale = IO_scale;}

void Hx711::setOffset(long IO_offset)//設置補償值
{
	offset = IO_offset;}

long Hx711::getValue()//採集一次ADC值
{
	unsigned long Count;
	unsigned char i;
	digitalWrite(SCK,LOW);
	Count = 0;
	while(digitalRead(DOUT) == 1);//低電平時資料可以輸出
	for(i=0;i<24;i++)//迴圈讀取資料
	{
		digitalWrite(SCK,HIGH);
		Count = Count<<1;
		digitalWrite(SCK,LOW);(digitalRead(DOUT) == 1) Count++;//最低位置1
	}
	digitalWrite(SCK,HIGH);
	Count = Count^0x800000;//最高位置0
	digitalWrite(SCK,LOW);//128增益
	return Count;}

long Hx711::getAverageValue(char IO_times)//採集ADC平均值
{
	long sum=0;
	char i;(i=0;i<IO_times;i++){
		sum += getValue();}
	return sum/IO_times;}

float Hx711::getWeight(char IO_times)//得出重量值
{
	long temp;
	temp = getAverageValue(IO_times) - offset;
	return (float)temp/scale;}