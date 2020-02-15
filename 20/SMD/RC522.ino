#include <SPI.h>
#include "RC522.h"
#include "config.h"
/*
CLK     13
MISO    12
MOSI    11
CS      8
NRSTPD  9
*/

const int RC522_CS = 8;//片選NSS/SDA
const int RC522_NRSTPD = 9;// RST  硬停電接口

u8 LastKeyA[6]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};//A密鑰

u8 read_data[16];// 讀到的16B資料
u8 write_data[16];// 寫入的16B資料
 

//RC522起始化
void rc522_Init(void)
{
	SPI.begin();//RC522 SPI通訊埠起始化
	////////////起始化CS NRSTPD接腳////////////
        pinMode(RC522_CS, OUTPUT);
        pinMode(RC522_NRSTPD,OUTPUT);
	///////////////////////////////////////////
	PcdReset();
	PcdAntennaOff();
	M500PcdConfigISOType( 'A' );
}

/*
 * 函 數 名：ReadOneBlock
 * 功能描述：讀取IC的一個區塊的資料
 * 輸導入參數數：block: 塊位址				
 * 返 回 值：0 讀取失敗
 	     1 讀取成功
 */
u8 ReadOneBlock(u8 block)
{
        u8 RevBuffer[30],status;

	//尋天線區內未進入休眠狀態的卡，傳回卡片型態 2位元組
	status=PcdRequest(PICC_REQIDL,&RevBuffer[0]);
	if(status!=MI_OK)  return 0;

	//防沖撞取得序號 4位元組
	status=PcdAnticoll(&RevBuffer[2]);
	if(status != MI_OK)  return 0;

	//選卡
	status = PcdSelect(&RevBuffer[2]);
	if(status!=MI_OK)  return 0;

	//密碼認證 第x塊資料(也就是分區1的密碼)
	status = PcdAuthState(PICC_AUTHENT1A,block,LastKeyA,&RevBuffer[2]);
	if(status!=MI_OK)  return 0;

        //讀取M1卡 第x塊資料
	status =  PcdRead(block,&read_data[0]);	
        if(status!=MI_OK)  return 0;
        
        return 1;
}
/*
 * 函 數 名：WriteOneBlock
 * 功能描述：寫入IC的一個區塊的資料
 * 輸導入參數數：block: 塊位址				
 * 返 回 值：0 寫入失敗
 	     1 寫入成功
 */
u8 WriteOneBlock(u8 block)
{
        u8 RevBuffer[30],status;

	//尋天線區內未進入休眠狀態的卡，傳回卡片型態 2位元組
	status=PcdRequest(PICC_REQIDL,&RevBuffer[0]);
	if(status!=MI_OK)  return 0;

	//防沖撞取得序號 4位元組
	status=PcdAnticoll(&RevBuffer[2]);
	if(status != MI_OK)  return 0;

	//選卡
	status = PcdSelect(&RevBuffer[2]);
	if(status!=MI_OK)  return 0;

	//密碼認證 第x塊資料(也就是分區1的密碼)
	status = PcdAuthState(PICC_AUTHENT1A,block,LastKeyA,&RevBuffer[2]);
	if(status!=MI_OK)  return 0;

	//寫M1卡 第4塊資料
	status = PcdWrite(block,&write_data[0]);
	if(status!=MI_OK)  return 0;
        
        return 1;	
}


//寫ID值
u8 WriteID(u32 id)
{
  u8 i;
  for(i=0;i<4;i++)
  {
    write_data[i] = (id>>(i*8)) & 0xFF;
  }
  for(i=4;i<16;i++)
  {
    write_data[i] = 0x00;
  }
  return WriteOneBlock(4);//ID寫入資料區塊4
}

//讀取ID值
u8 ReadID(u32 *data)
{
  u8 i,mark;
  u32 temp=0;
  
  mark = ReadOneBlock(4);
  //讀取ID資料區塊
  for(i=0;i<4;i++)
  {
    temp <<= 8;
    temp += read_data[4-i-1];
  }
  *data = temp;
  return mark;
}

////////////////////////////////////////////////////////////////////////////////////////

/*
 * 函 數 名：WriteRawRC
 * 功能描述：向MFRC522的某一暫存器寫一個位元組資料
 * 輸導入參數數：addr--暫存器位址；val--要寫入的值
 * 返 回 值：無
 */
void WriteRawRC(u8 addr, u8 val)
{
	//SPI_RC522_CS_L;
        digitalWrite(RC522_CS, LOW);
	//寫位址格式：0XXXXXX0
   	SPI.transfer((addr<<1)&0x7E);
   	SPI.transfer(val);
   	//SPI_RC522_CS_H;
        digitalWrite(RC522_CS, HIGH);
}
 
/*
 * 函 數 名：ReadRawRC
 * 功能描述：從MFRC522的某一暫存器讀一個位元組資料
 * 輸導入參數數：addr--暫存器位址
 * 返 回 值：傳回讀取到的一個位元組資料
 */
u8 ReadRawRC(u8 addr)
{
  u8 val;

  //SPI_RC522_CS_L;
  digitalWrite(RC522_CS, LOW);
  //讀位址格式：1XXXXXX0
  SPI.transfer(((addr<<1)&0x7E) | 0x80);
  val = SPI.transfer(0x00);
  //SPI_RC522_CS_H;
  digitalWrite(RC522_CS, HIGH);
  return val;
}

/*
 * 函 數 名：SetBitMask
 * 功能描述：置RC522暫存器位
 * 輸導入參數數：reg--暫存器位址;mask--置位值
 * 返 回 值：無
 */
void SetBitMask(u8 reg, u8 mask)
{
  u8 temp= 0x00;
  temp = ReadRawRC(reg);
  WriteRawRC(reg, temp | mask);  // set bit mask
}

/*
 * 函 數 名：ClearBitMask
 * 功能描述：清RC522暫存器位
 * 輸導入參數數：reg--暫存器位址;mask--清位值
 * 返 回 值：無
 */
void ClearBitMask(u8 reg,u8 mask)
{
  u8 temp = 0x00;
  temp = ReadRawRC(reg);
  WriteRawRC(reg, temp & (~mask));
}

/*
 * 函 數 名：PcdAntennaOn
 * 功能描述：開啟天線,每次啟動或關閉天險發射之間應至少有1ms的間隔
 * 輸導入參數數：無
 * 返 回 值：無
 */
void PcdAntennaOn(void)
{
  u8 temp;
  temp = ReadRawRC(TxControlReg);
  if(!(temp & 0x03))
  {
    SetBitMask(TxControlReg,0x03);
  } 
}

/*
 * 函 數 名：PcdAntennaOff
 * 功能描述：關閉天線,每次啟動或關閉天險發射之間應至少有1ms的間隔
 * 輸導入參數數：無
 * 返 回 值：無
 */
void PcdAntennaOff(void)
{
  ClearBitMask(TxControlReg, 0x03);
}

/*
 * 函 數 名：PcdReset
 * 功能描述：重置RC522
 * 輸導入參數數：無
 * 返 回 值：無
 */
s8 PcdReset(void)
{
	//SPI_RC522_NRSTPD_H;
        digitalWrite(RC522_NRSTPD, HIGH);
  	delay(1);
  	//SPI_RC522_NRSTPD_L;
        digitalWrite(RC522_NRSTPD, LOW);
  	delay(1);
  	//SPI_RC522_NRSTPD_H;
        digitalWrite(RC522_NRSTPD, HIGH);
  	delay(1);
  	WriteRawRC(CommandReg, PCD_RESETPHASE);
  	delay(1);

  	WriteRawRC(ModeReg,0x3D);            //和Mifare卡通信，CRC初值0x6363
  	WriteRawRC(TReloadRegL,30);           
  	WriteRawRC(TReloadRegH,0);
  	WriteRawRC(TModeReg,0x8D);
	WriteRawRC(TPrescalerReg,0x3E);
	
	WriteRawRC(TxAutoReg,0x40);//必須要
   
    return MI_OK;
}

/*
 * 函 數 名：M500PcdConfigISOType
 * 功能描述：設定RC632的工作模式
 * 輸導入參數數：type 卡的型態
 * 返 回 值：
 */
s8 M500PcdConfigISOType(u8 type)
{
   if(type == 'A')                     //ISO14443_A
   { 
       ClearBitMask(Status2Reg,0x08);
       WriteRawRC(ModeReg,0x3D);//3F
       WriteRawRC(RxSelReg,0x86);//84
       WriteRawRC(RFCfgReg,0x7F);   //4F
       WriteRawRC(TReloadRegL,30);//tmoLength);// TReloadVal = 'h6a =tmoLength(dec) 
       WriteRawRC(TReloadRegH,0);
       WriteRawRC(TModeReg,0x8D);
       WriteRawRC(TPrescalerReg,0x3E);
       delay(1);
       PcdAntennaOn();
   }
   else{ return -1; }
   return MI_OK;
}


/*
 * 函 數 名：CalulateCRC
 * 功能描述：用MF522計算CRC16函數
 * 輸導入參數數：
 * 返 回 值：
 */
void CalulateCRC(u8 *pIndata,u8 len,u8 *pOutData)
{
    u8 i,n;
    ClearBitMask(DivIrqReg,0x04);
    WriteRawRC(CommandReg,PCD_IDLE);
    SetBitMask(FIFOLevelReg,0x80);
    for(i=0; i<len; i++)
    {
		WriteRawRC(FIFODataReg, *(pIndata+i));
	}
    WriteRawRC(CommandReg, PCD_CALCCRC);
    i = 0xFF;
    do 
    {
        n = ReadRawRC(DivIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x04));
    pOutData[0] = ReadRawRC(CRCResultRegL);
    pOutData[1] = ReadRawRC(CRCResultRegM);
}

/*
 * 函 數 名：CalulateCRC
 * 功能描述：透過RC522和ISO14443卡通信
 * 輸導入參數數：
 			 Command[IN]: RC522指令字
             pInData[IN]: 透過RC522傳送到卡片的資料
			 InLenByte[IN]: 傳送資料的位元組長度
			 pOutData[OUT]: 接收到的卡片傳回資料
			 pOutLenBit[OUT]: 傳回資料的位長度

 * 返 回 值：
 */
s8 PcdComMF522(u8  Command, 
                 u8  *pInData, 
                 u8  InLenByte,
                 u8  *pOutData, 
                 u16 *pOutLenBit)
{
    s8 status = MI_ERR;
    u8 irqEn   = 0x00;
    u8 waitFor = 0x00;
    u8 lastBits;
    u8 n;
    u16 i;
    switch (Command)
    {
        case PCD_AUTHENT:
          irqEn   = 0x12;
          waitFor = 0x10;
          break;
        case PCD_TRANSCEIVE:
          irqEn   = 0x77;
          waitFor = 0x30;
          break;
        default:
          break;
    }
   
    WriteRawRC(ComIEnReg,irqEn|0x80);
    ClearBitMask(ComIrqReg,0x80);
    WriteRawRC(CommandReg,PCD_IDLE);
    SetBitMask(FIFOLevelReg,0x80);
    
    for (i=0; i<InLenByte; i++)
    {   WriteRawRC(FIFODataReg, pInData[i]);    }
    WriteRawRC(CommandReg, Command);
   
    
    if (Command == PCD_TRANSCEIVE)
    {    SetBitMask(BitFramingReg,0x80);  }
    
    //i = 600;//根據時鍾頻率調整，動作M1卡最大等待時間25ms
    i = 2000;
    do 
    {
        n = ReadRawRC(ComIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x01) && !(n&waitFor));
    ClearBitMask(BitFramingReg,0x80);

    if (i!=0)
    {    
        if(!(ReadRawRC(ErrorReg)&0x1B))
        {
            status = MI_OK;
            if (n & irqEn & 0x01)
            {   status = MI_NOTAGERR;   }
            if (Command == PCD_TRANSCEIVE)
            {
               	n = ReadRawRC(FIFOLevelReg);
              	lastBits = ReadRawRC(ControlReg) & 0x07;
                if (lastBits)
                {   *pOutLenBit = (n-1)*8 + lastBits;   }
                else
                {   *pOutLenBit = n*8;   }
                if (n == 0)
                {   n = 1;    }
                if (n > MAXRLEN)
                {   n = MAXRLEN;   }
                for (i=0; i<n; i++)
                {   pOutData[i] = ReadRawRC(FIFODataReg);    }
            }
        }
        else
        {   status = MI_ERR;   }
        
    }
    SetBitMask(ControlReg,0x80);           // stop timer now
    WriteRawRC(CommandReg,PCD_IDLE); 
    return status;
}



/*
 * 函 數 名：PcdRequest
 * 功能描述：尋卡
 * 輸導入參數數：
 			 req_code[IN]: 尋卡模式
			 	0x52 = 尋感應區內所有符合14443A標准的卡
				0x26 = 尋未進入休眠狀態的卡
			 pTagType[OUT]：卡片型態程式碼
			 	0x4400 = Mifare_UltraLight
                0x0400 = Mifare_One(S50)
                0x0200 = Mifare_One(S70)
                0x0800 = Mifare_Pro(X)
                0x4403 = Mifare_DESFire

 * 返 回 值：成功傳回MI_OK
 */
s8 PcdRequest(u8 req_code,u8 *pTagType)
{
	s8 status;  
	u16 unLen;
	u8 ucComMF522Buf[MAXRLEN]; 

	ClearBitMask(Status2Reg,0x08);
	WriteRawRC(BitFramingReg,0x07);
	SetBitMask(TxControlReg,0x03);
 
	ucComMF522Buf[0] = req_code;

	status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,1,ucComMF522Buf,&unLen);

	if ((status == MI_OK) && (unLen == 0x10))
	{    
		*pTagType     = ucComMF522Buf[0];
		*(pTagType+1) = ucComMF522Buf[1];
	}
	else
	{   status = MI_ERR;   }
   
	return status;
}

/////////////////////////////////////////////////////////////////////
//功    能：防沖撞
//參數說明: pSnr[OUT]:卡片序號，4位元組
//返    回: 成功傳回MI_OK
///////////////////////////////////////////////////////////////////// 
s8 PcdAnticoll(u8 *pSnr)
{
    s8 status;
    u8 i,snr_check=0;
    u16 unLen;
    u8 ucComMF522Buf[MAXRLEN]; 
    

    ClearBitMask(Status2Reg,0x08);
    WriteRawRC(BitFramingReg,0x00);
    ClearBitMask(CollReg,0x80);
 
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x20;

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,2,ucComMF522Buf,&unLen);

    if (status == MI_OK)
    {
    	 for (i=0; i<4; i++)
         {   
             *(pSnr+i)  = ucComMF522Buf[i];
             snr_check ^= ucComMF522Buf[i];
         }
         if (snr_check != ucComMF522Buf[i])
         {   status = MI_ERR;    }
    }
    
    SetBitMask(CollReg,0x80);
    return status;
}

/////////////////////////////////////////////////////////////////////
//功    能：選定卡片
//參數說明: pSnr[IN]:卡片序號，4位元組
//返    回: 成功傳回MI_OK
/////////////////////////////////////////////////////////////////////
s8 PcdSelect(u8 *pSnr)
{
    s8 status;
    u8 i;
    u16 unLen;
    u8 ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x70;
    ucComMF522Buf[6] = 0;
    for (i=0; i<4; i++)
    {
    	ucComMF522Buf[i+2] = *(pSnr+i);
    	ucComMF522Buf[6]  ^= *(pSnr+i);
    }
    CalulateCRC(ucComMF522Buf,7,&ucComMF522Buf[7]);
  
    ClearBitMask(Status2Reg,0x08);

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,9,ucComMF522Buf,&unLen);
    
    if ((status == MI_OK) && (unLen == 0x18))
    {   status = MI_OK;  }
    else
    {   status = MI_ERR;    }

    return status;
}

/////////////////////////////////////////////////////////////////////
//功    能：驗證卡片密碼
//參數說明: auth_mode[IN]: 密碼驗證模式
//                 0x60 = 驗證A密鑰
//                 0x61 = 驗證B密鑰 
//          addr[IN]：塊位址
//          pKey[IN]：密碼
//          pSnr[IN]：卡片序號，4位元組
//返    回: 成功傳回MI_OK
/////////////////////////////////////////////////////////////////////               
s8 PcdAuthState(u8 auth_mode,u8 addr,u8 *pKey,u8 *pSnr)
{
    s8 status;
    u16 unLen;
    u8 i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = auth_mode;
    ucComMF522Buf[1] = addr;
    for (i=0; i<6; i++)
    {    ucComMF522Buf[i+2] = *(pKey+i);   }
    for (i=0; i<6; i++)
    {    ucComMF522Buf[i+8] = *(pSnr+i);   }
 //   memcpy(&ucComMF522Buf[2], pKey, 6); 
 //   memcpy(&ucComMF522Buf[8], pSnr, 4); 
    
    status = PcdComMF522(PCD_AUTHENT,ucComMF522Buf,12,ucComMF522Buf,&unLen);
    if ((status != MI_OK) || (!(ReadRawRC(Status2Reg) & 0x08)))
    {   status = MI_ERR;   }
    
    return status;
}

/////////////////////////////////////////////////////////////////////
//功    能：讀取M1卡一塊資料
//參數說明: addr[IN]：塊位址
//          pData[OUT]：讀出的資料，16位元組
//返    回: 成功傳回MI_OK
///////////////////////////////////////////////////////////////////// 
s8 PcdRead(u8 addr,u8 *pData)
{
    s8 status;
    u16 unLen;
    u8 i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_READ;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
   
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
    if ((status == MI_OK) && (unLen == 0x90))
 //   {   memcpy(pData, ucComMF522Buf, 16);   }
    {
        for (i=0; i<16; i++)
        {    *(pData+i) = ucComMF522Buf[i];   }
    }
    else
    {   status = MI_ERR;   }
    
    return status;
}

/////////////////////////////////////////////////////////////////////
//功    能：寫資料到M1卡一塊
//參數說明: addr[IN]：塊位址
//          pData[IN]：寫入的資料，16位元組
//返    回: 成功傳回MI_OK
/////////////////////////////////////////////////////////////////////                  
s8 PcdWrite(u8 addr,u8 *pData)
{
    s8 status;
    u16 unLen;
    u8 i,ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_WRITE;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = MI_ERR;   }
        
    if (status == MI_OK)
    {
        //memcpy(ucComMF522Buf, pData, 16);
        for (i=0; i<16; i++)
        {    
        	ucComMF522Buf[i] = *(pData+i);   
        }
        CalulateCRC(ucComMF522Buf,16,&ucComMF522Buf[16]);

        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,18,ucComMF522Buf,&unLen);
        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {   status = MI_ERR;   }
    }
    
    return status;
}

/////////////////////////////////////////////////////////////////////
//功    能：指令卡片進入休眠狀態
//返    回: 成功傳回MI_OK
/////////////////////////////////////////////////////////////////////
s8 PcdHalt(void)
{
    s8 status;
    u16 unLen;
    u8 ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_HALT;
    ucComMF522Buf[1] = 0;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    return status;
}
