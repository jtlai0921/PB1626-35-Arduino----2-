/**/
#include <stdio.h>
#define DIN_PIN 11
#define CLK_PIN 12
#define STB_PIN 13


#define DODELAY __asm__("nop\n\t""nop\n\t");

#define COUNTOF(arr) (sizeof(arr) / sizeof(arr[0]))
#define MODULE_NUM 10

#define MOD_DIN_H digitalWrite(DIN_PIN, HIGH)
#define MOD_DIN_L digitalWrite(DIN_PIN, LOW)
#define MOD_CLK_H digitalWrite(CLK_PIN, HIGH)
#define MOD_CLK_L digitalWrite(CLK_PIN, LOW)
#define MOD_STB_H digitalWrite(STB_PIN, HIGH)
#define MOD_STB_L digitalWrite(STB_PIN, LOW)


// seg bits table
#define SEG_a  0b1000000000
#define SEG_b	 0b0001000000
#define SEG_dot	 0b0000100000 // dot
#define SEG_c	 0b0000010000
#define SEG_d	 0b0000001000
#define SEG_e  0b0000000100
#define SEG_g	 0b0000000010
#define SEG_f	 0b0000000001

#define CHAR_SPACE	0
#define CHAR_NULL	0
#define CHAR_ALL	(SEG_a|SEG_b|SEG_c|SEG_d|SEG_e|SEG_f|SEG_g|SEG_h|SEG_i)
#define CHAR_0		(SEG_a|SEG_b|SEG_c|SEG_d|SEG_e|SEG_f)
#define CHAR_1		(SEG_b|SEG_c)
#define CHAR_2		(SEG_a|SEG_b|SEG_d|SEG_e|SEG_g)
#define CHAR_3		(SEG_a|SEG_b|SEG_c|SEG_d|SEG_g)
#define CHAR_4		(SEG_b|SEG_c|SEG_f|SEG_g)
#define CHAR_5		(SEG_a|SEG_c|SEG_d|SEG_f|SEG_g)
#define CHAR_6		(SEG_a|SEG_c|SEG_d|SEG_e|SEG_f|SEG_g)
#define CHAR_7		(SEG_a|SEG_b|SEG_c)
#define CHAR_8		(SEG_a|SEG_b|SEG_c|SEG_d|SEG_e|SEG_f|SEG_g)
#define CHAR_9		(SEG_a|SEG_b|SEG_c|SEG_d|SEG_f|SEG_g)

typedef struct TubeModuleData
{
  unsigned short int seg;
  unsigned char red;
  unsigned char green;
  unsigned char blue;
};

static unsigned short int digitab[] = {
 // CHAR_0,CHAR_1,CHAR_2,CHAR_3,CHAR_4,CHAR_5,CHAR_6,CHAR_7,CHAR_8,CHAR_9
 ~SEG_a,~SEG_b,~SEG_c,~SEG_d,~SEG_e,~SEG_f,~SEG_g,~SEG_f,CHAR_0,CHAR_1,
};

// 從bit7開始傳送到bit0
static void send_byte_h2l(unsigned char data)
{
  unsigned char i, temp;

  temp = data;
  for (i=0; i<8; i++)
  {
    if (temp&0b10000000) // 由高位開始送資料
      MOD_DIN_H;
    else
      MOD_DIN_L;
    temp <<= 1;

    // clock run
    DODELAY;
    MOD_CLK_H;
    DODELAY;
    MOD_CLK_L;
    DODELAY;
  }
}

// 從bit0開始傳送c中的count位
static void send_uintbits_l2h(unsigned short int data, unsigned char num)
{
  unsigned char i;
  unsigned short int temp;

  temp = data;
  for (i=0; i<num; i++)
  {
    if (temp&0x01) // 由高位開始送資料
      MOD_DIN_H;
    else
      MOD_DIN_L;
    temp >>= 1;

    // clock run
    DODELAY;
    MOD_CLK_H;
    DODELAY;
    MOD_CLK_L;
    DODELAY;
  }
}

void modSendData(struct TubeModuleData *data)
{
  if (!data)
    return;
  // send data
  MOD_STB_L;
  DODELAY;

  // 先傳送led資料,最後傳送HV6810資料	
  send_byte_h2l(data->red);
  send_byte_h2l(data->green);
  send_byte_h2l(data->blue);
  send_uintbits_l2h(data->seg, 10);

  MOD_STB_H; // 下降沿鎖存
  DODELAY;
  MOD_STB_L;
  DODELAY;
  //MOD_BLINK_L;
}

void modSendDataArr(struct TubeModuleData data[], unsigned char num)
{
  unsigned char i;
  if (!data)
    return;
  // send data
  MOD_STB_L;
  DODELAY;
  for (i=0; i<num; i++) {
    // 先傳送led資料,最後傳送HV6810資料	
    send_byte_h2l(data[i].red);
    send_byte_h2l(data[i].green);
    send_byte_h2l(data[i].blue);
    send_uintbits_l2h(data[i].seg, 10);
  }
  MOD_STB_H; // 下降沿鎖存
  DODELAY;
  MOD_STB_L;
  DODELAY;
  //MOD_BLINK_L;
}

struct TubeModuleData modData[MODULE_NUM];
void setup() {
  // set the digital pin as output:
  pinMode(DIN_PIN, OUTPUT);    
  pinMode(CLK_PIN, OUTPUT);
  pinMode(STB_PIN, OUTPUT);
  modData[0].seg = CHAR_1;
  modData[0].red = 0xff;
  modData[0].green = 0x00;
  modData[0].blue = 0x00;
  modData[1].seg = CHAR_2;
  modData[1].red = 0x00;
  modData[1].green = 0x00;
  modData[1].blue = 0x00;  
  //  modSendData(&modData[0]);
  modSendDataArr(modData, 2);
}

void loop()
{
  unsigned char i=0;
  while(1) {
  modData[1].seg = digitab[i];
   modData[1].red = 25*i;
  modSendDataArr(modData, 2);
  i++;
  if (i>=10) i=0;
  delay(1000);
  }
}








