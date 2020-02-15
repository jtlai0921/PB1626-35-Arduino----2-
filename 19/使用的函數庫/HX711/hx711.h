#ifndef HX711_h
#define HX711_h

#include "Arduino.h"

class Hx711
{
private:
	int DOUT;//定義資料接腳
	int SCK;//定義時脈接腳

	float scale;//定義比例係數
	long offset;//定義補償值
public:
	Hx711(int IO_DOUT,int IO_SCK);//構造函數
	void setScale(float IO_scale);//設置比例係數
	void setOffset(long IO_offset);//設置補償值

	long getValue();//採集一次ADC值
	long getAverageValue(char IO_times);//採集ADC平均值
	float getWeight(char IO_times);//得出重量值
};#endif