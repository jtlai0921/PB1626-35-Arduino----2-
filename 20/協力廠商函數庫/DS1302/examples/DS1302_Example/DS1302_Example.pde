/*
Arduino 連線 DS1302
程式碼來源：http://quadpoint.org/projects/arduino-ds1302
增加了序列埠調整時間程式碼
*/
#include <stdio.h>
#include <string.h>
#include <DS1302.h>

/* 接口定義
CE(DS1302 pin5) -> Arduino D5
IO(DS1302 pin6) -> Arduino D6
SCLK(DS1302 pin7) -> Arduino D7
*/
uint8_t CE_PIN   = 5;
uint8_t IO_PIN   = 6;
uint8_t SCLK_PIN = 7;

/* 日期變數快取 */
char buf[50];
char day[10];
/* 序列埠資料快取 */
String comdata = "";
int numdata[7] ={0}, j = 0, mark = 0;
/* 建立 DS1302 物件 */
DS1302 rtc(CE_PIN, IO_PIN, SCLK_PIN);


void print_time()
{
    /* 從 DS1302 取得目前時間 */
    Time t = rtc.time();
    /* 將星期從數字轉為名稱 */
    memset(day, 0, sizeof(day));
    switch (t.day)
    {
    case 1: strcpy(day, "Sunday"); break;
    case 2: strcpy(day, "Monday"); break;
    case 3: strcpy(day, "Tuesday"); break;
    case 4: strcpy(day, "Wednesday"); break;
    case 5: strcpy(day, "Thursday"); break;
    case 6: strcpy(day, "Friday"); break;
    case 7: strcpy(day, "Saturday"); break;
    }
    /* 將日期程式碼格式化湊成buf等待輸出 */
    snprintf(buf, sizeof(buf), "%s %04d-%02d-%02d %02d:%02d:%02d", day, t.yr, t.mon, t.date, t.hr, t.min, t.sec);
    /* 輸出日期到序列埠 */
    Serial.println(buf);
}


void setup()
{
    Serial.begin(9600);
    rtc.write_protect(false);
    rtc.halt(false);
}

void loop()
{

    /* 當序列埠有資料的時候，將資料拼接到變數comdata */
    while (Serial.available() > 0)
    {
        comdata += char(Serial.read());
        delay(2);
        mark = 1;
    }
    /* 以逗點分隔分解comdata的字串，分解結果變成轉換成數字到numdata[]陣列 */
    if(mark == 1)
    {
        Serial.print("You inputed : ");
        Serial.println(comdata);
        for(int i = 0; i < comdata.length() ; i++)
        {
            if(comdata[i] == ',' || comdata[i] == 0x10 || comdata[i] == 0x13)
            {
                j++;
            }
            else
            {
                numdata[j] = numdata[j] * 10 + (comdata[i] - '0');
            }
        }
        /* 將轉換好的numdata湊成時間格式，寫入DS1302 */
        Time t(numdata[0], numdata[1], numdata[2], numdata[3], numdata[4], numdata[5], numdata[6]);
        rtc.time(t);
        mark = 0;j=0;
        /* 清理 comdata 變數，以便等待下一次輸入 */
        comdata = String("");
        /* 清理 numdata */
        for(int i = 0; i < 7 ; i++) numdata[i]=0;
    }
    
    /* 列印目前時間 */
    print_time();
    delay(1000);
}
