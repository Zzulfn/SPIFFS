
/*************************************************************************************
 * �ļ����� bsp_Ht1381.c
 * ����	 �� Ht1381ʱ��оƬ����
 * ˵��	 �� ����������STM32оƬ�����IO�иĶ���Ŀ��ļ��궨��
 *					IO��оƬ������������bsp_Ht1381.h�����á�
 * �汾  : 1.0
 * ����	 ��2017.12.27
 * ����  ��Jack Lu 
 **************************************************************************************/

#include "bsp_Ht1381.h"

uint8_t    rtcUpdateStatus=rtcUpdate;//����ʱ����� ״̬����  �ϵ�Ĭ��Ϊ  û�и��� 

/**********************************************************
 * ������ : fnHT1381_Delay
 * ����   ��ns ��ʱʱ��
 * ���   ��none 
 * ����   ����ʱ����
 **********************************************************/
void fnHT1381_Delay(u16 ns)
{
	while(--ns);
}

/**********************************************************
 * ������ : fnHT1381_GPIO_Init
 * ����   ��none
 * ���   ��none 
 * ����   ��HT1381 IO��ʼ��
 **********************************************************/
void fnHT1381_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_RTC_CLK_PORT | RCC_RTC_DATA_PORT | RCC_RTC_RST_PORT,ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = RTC_CLK_PIN ;       //RTC_CLK_PB13
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(RTC_CLK_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = RTC_DATA_PIN ;       // RTC_DATA_PB12
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(RTC_DATA_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = RTC_RST_PIN ;      // RTC_RST_PB14
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(RTC_RST_PORT, &GPIO_InitStructure);
}

/**********************************************************
 * ������ : fnHT1381_IO_Output
 * ����   ��none
 * ���   ��none 
 * ����   ��HT1381_SDA	����Ϊ�������  д������
 **********************************************************/
void fnHT1381_IO_Output(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = RTC_DATA_PIN ; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(RTC_DATA_PORT, &GPIO_InitStructure);
}

/**********************************************************
 * ������ : fnHT1381_IO_Input
 * ����   ��none
 * ���   ��none 
 * ����   ��HT1381_SDA	����Ϊ��������  ��������
 **********************************************************/
void fnHT1381_IO_Input(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = RTC_DATA_PIN ; //
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(RTC_DATA_PORT, &GPIO_InitStructure);
}

/**********************************************************
 * ������ : fnHT1381_WriteByte
 * ����   ����Ҫд������  u8��
 * ���   ��none 
 * ����   ����1381��Ӧ�Ĵ���д��һ����λ����
 **********************************************************/
void fnHT1381_WriteByte(u8 ucDat) 
{ 
	unsigned char i; 
	fnHT1381_IO_Output();
	fnHT1381_Delay(10);
	for(i=8;i>0;i--)
	{ 
		HT1381_IO(ucDat & 0x01); 
	    
		HT1381_SCLK(1); 
		fnHT1381_Delay(10);
		HT1381_SCLK(0); 
	    
		ucDat >>= 1;
	} 
} 

/**********************************************************
 * ������ : fnHT1381_ReadByte
 * ����   ��none
 * ���   ��u8������ 
 * ����   ����1381��Ӧ�Ĵ�����ȡһ����λ����
 **********************************************************/
u8 fnHT1381_ReadByte(void) 
{ 
	u8 i; 
	u8 ucDat=0; 
	
	fnHT1381_IO_Input();	
	fnHT1381_Delay(10);
	
	for(i=8;i>0;i--)
	{ 
		HT1381_IO(1);
		fnHT1381_Delay(10);
		ucDat >>= 1; 
		fnHT1381_Delay(10);
		if(IS_HT1381_IO)  ucDat|=0x80; 
		
		HT1381_SCLK(1); 
		fnHT1381_Delay(10);
		HT1381_SCLK(0);
		fnHT1381_Delay(10);
	} 
	return(ucDat); 
} 

/**********************************************************
 * ������ : fnHT1381_WriteData
 * ����   ��ucAddress:  ��ַ, data: Ҫд������
 * ���   ��none
 * ����   ��д������,��д��ַ,��д����/����
 **********************************************************/
void fnHT1381_WriteData(u8 ucAddress, u8 ucData)
{ 
	HT1381_RST(0);
	fnHT1381_Delay(10);
	HT1381_SCLK (0);
	fnHT1381_Delay(10);
	HT1381_RST(1);
	fnHT1381_Delay(10);
	fnHT1381_WriteByte(ucAddress); 		//��ַ������ 
	fnHT1381_WriteByte(ucData);			 	//д��1Byte ���� 
	HT1381_SCLK(1);
	fnHT1381_Delay(10);
	HT1381_RST(0); 
	return; 
} 

/**********************************************************
 * ������ : fnHT1381_GetDataFromAddress
 * ����   ��ucAddress ��Ҫ��ȡ�ĵ�ַ
 * ���   ����ָ����ַ��ȡ������
 * ����   ����ȡĳ��ָ����ַ������
 **********************************************************/
uint8_t fnHT1381_GetDataFromAddress(u8 ucAddress) 
{ 
	unsigned char ucDat; 
	HT1381_RST(0); 
	HT1381_SCLK(0); 
	HT1381_RST(1); 
 	fnHT1381_WriteByte(ucAddress);  //��ַ������ 
	ucDat = fnHT1381_ReadByte(); 		//��1Byte ���� 
	HT1381_SCLK(1);  
	HT1381_RST(0);  
	return(ucDat);  
}  

/**********************************************************
 * ������ : fnHT1381_Init
 * ����   ��none
 * ���   ��none
 * ����   ��HT1381��ʼ��
 **********************************************************/
void fnHT1381_Init(void)
{
	fnHT1381_GPIO_Init();
	HT1381_SCLK(0);
	fnHT1381_Delay(10);
	HT1381_RST(0);
	fnHT1381_Delay(10);
	fnHT1381_WriteData(0x8e,0x00);		//ȡ��д����
  fnHT1381_WriteData(0x90,0x00);		//���ó������������ֵ  0 ��ʹ��û����������
  //HT1381_WriteData(0x8e,0x80);		//��д����	�Լ��ӵ� ������򲻺���ȥ���˾�
}

/**********************************************************
 * ������ : fnHT1381_GetTime
 * ����   ��stuHT1381Time: �ṹ���׵�ַ
 * ���   ��none
 * ����   ����ȡHT1381 ��ǰʱ�� 7Byte (BCD ��)�Ѿ�ת��Ϊ������ʽ
 **********************************************************/
void fnHT1381_GetTime(RTC_TimeTypeDef *tb) 
{ 
	u8 ucAddress = HT1381_GET_ADDRESS;
	u8 ucTemp = 0;
  ucTemp =fnHT1381_GetDataFromAddress(ucAddress);/*��ʽΪ: ���ʱ���������� */ 
	ucTemp = ucTemp%16 + (ucTemp/16)*10;		//BCD ��ת��
  tb->Second=ucTemp;   // ��
  
  
  ucAddress+= 2; 
  ucTemp =fnHT1381_GetDataFromAddress(ucAddress);/*��ʽΪ: ���ʱ���������� */ 
	ucTemp = ucTemp%16 + (ucTemp/16)*10;		//BCD ��ת��
  tb->Minutes=ucTemp;   // ��
  
  ucAddress+= 2; 
  ucTemp =fnHT1381_GetDataFromAddress(ucAddress);/*��ʽΪ: ���ʱ���������� */ 
	ucTemp = ucTemp%16 + (ucTemp/16)*10;		//BCD ��ת��
  tb->Hour=ucTemp;     // ʱ
  
  ucAddress+= 2; 
  ucTemp =fnHT1381_GetDataFromAddress(ucAddress);/*��ʽΪ: ���ʱ���������� */ 
	ucTemp = ucTemp%16 + (ucTemp/16)*10;		//BCD ��ת��
  tb->Day=ucTemp;   //��
  
  ucAddress+= 2; 
  ucTemp =fnHT1381_GetDataFromAddress(ucAddress);/*��ʽΪ: ���ʱ���������� */ 
	ucTemp = ucTemp%16 + (ucTemp/16)*10;		//BCD ��ת��
  tb->Month=ucTemp;   // ��
  
  ucAddress+= 2; 
  ucTemp =fnHT1381_GetDataFromAddress(ucAddress);/*��ʽΪ: ���ʱ���������� */ 
	ucTemp = ucTemp%16 + (ucTemp/16)*10;		//BCD ��ת��
  tb->Week=ucTemp;   //����
  
  ucAddress+= 2; 
  ucTemp =fnHT1381_GetDataFromAddress(ucAddress);/*��ʽΪ: ���ʱ���������� */ 
	ucTemp = ucTemp%16 + (ucTemp/16)*10;		//BCD ��ת��
  tb->Year=ucTemp;   //��
} 

/**********************************************************
 * ������ : fnHT1381_SetTime
 * ����   ��stuHT1381Time: �ṹ���׵�ַ�� ʱ���ʽΪ: ���ʱ����������
 * ���   ��none
 * ����   �����ó�ʼʱ�� 7Byte (BCD ��) 1B 1B 1B 1B 1B 1B 1B 
 **********************************************************/
void fnHT1381_SetTime(RTC_TimeTypeDef *tb) 
{
  uint8_t ucAddress=HT1381_SET_ADDRESS;
  uint8_t buf,d;
	
	fnHT1381_WriteData(0x8e,0x00);  /* ��������,WP=0,д����?*/  
   
  d=tb->Second;               /* ���ʱ���������� */  
  buf=(d/10)*16+d%10;   
  fnHT1381_WriteData(ucAddress,buf);
  ucAddress+=2;
  
  d=tb->Minutes;
  buf=(d/10)*16+d%10;
  fnHT1381_WriteData(ucAddress,buf);
  ucAddress+=2;
  
  d=tb->Hour;
  buf=(d/10)*16+d%10;
  fnHT1381_WriteData(ucAddress,buf);
  ucAddress+=2;
  
  d=tb->Day;
  buf=(d/10)*16+d%10;
  fnHT1381_WriteData(ucAddress,buf);
  ucAddress+=2;
  
  d=tb->Month;
  buf=(d/10)*16+d%10;
  fnHT1381_WriteData(ucAddress,buf);
  ucAddress+=2;
  
  d=tb->Week;
  buf=(d/10)*16+d%10;
  fnHT1381_WriteData(ucAddress,buf);
  ucAddress+=2;
  
  d=tb->Year;
  buf=(d/10)*16+d%10;
  fnHT1381_WriteData(ucAddress,buf);
  ucAddress+=2;
 
	fnHT1381_WriteData(0x8e,0x80);  /* ��������,WP=1,д����*/  
}  

/**********************************************************
 * ������ : fnRTC_CheckAndConfig
 * ����   ��stuHT1381Time: �ṹ���׵�ַ�� ʱ���ʽΪ: ���ʱ����������
 * ���   ��none
 * ����   ������Ƿ��Ѿ����ù�RTC��û�����ã������������� 
 *          �˺�������STM32�������ݼĴ���д���ʼֵ,
 *          �Ա�֤RTCֻ���³�����һ���ϵ�����ʱ�䣬�Ժ�������
 **********************************************************/
void fnRTC_CheckAndConfig(RTC_TimeTypeDef *stuHT1381Time)
{
  if (BKP_ReadBackupRegister(BKP_DR1) != 0xa5a6)
  {
    /* RTC Configuration */
    fnHT1381_Init();
    /* Adjust time by values entred by the user on the hyperterminal */
    fnHT1381_SetTime(stuHT1381Time);

    BKP_WriteBackupRegister(BKP_DR1, 0xa5a6);
  }
  else
    fnHT1381_Init();
}

/**********************************************************
 * ������ : rtcGetUpdateStatus
 * ����   ��none
 * ���   ��rtcUpdateStatus ���±�־
 * ����   ����ȡRTC���±�־
 **********************************************************/
uint8_t rtcGetUpdateStatus(void)
{
  return  rtcUpdateStatus;
}

/**********************************************************
 * ������ : rtcSetTime
 * ����   ��TimeBuf: �ṹ���׵�ַ
 * ���   ��none
 * ����   ������Rtcʱ��
 **********************************************************/
void rtcSetTime(RTC_TimeTypeDef *TimeBuf)
{
  fnHT1381_SetTime(TimeBuf);    // ����ʱ��
  rtcUpdateStatus=rtcUpdateOk;  // ʱ��������
  printf("ʱ�䱣�����\r\n");
}

/**********************************************************
 * ������ : fnHT1381_Leap_Year
 * ����   �����
 * ���   ��������ǲ�������.1,��.0,����
 * ����   ���ж��Ƿ������꺯��
 *          �·�   1  2  3  4  5  6  7  8  9  10 11 12
 *          ����   31 29 31 30 31 30 31 31 30 31 30 31
 *          ������ 31 28 31 30 31 30 31 31 30 31 30 31
 **********************************************************/
uint8_t fnHT1381_Leap_Year(u16 year)
{			  
	if(year%4==0) //�����ܱ�4����
		{ 
		if(year%100==0) 
			{ 
			if(year%400==0)return 1;//�����00��β,��Ҫ�ܱ�400���� 	   
			else return 0;   
			}else return 1;   
		}else return 0;	
}	
/**********************************************************
 * ������ : fnHT1381_Leap_Year
 * ����   ��ʱ��
 * ���   ������
 * ����   ����ʱ��ת��Ϊ����
 **********************************************************/	
uint64_t fnHT1381_TimeToNum(RTC_TimeTypeDef stime)
{
	u16 t;
	uint64_t seccount=0;
	
	u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};    //ƽ����·����ڱ�
	
	if(stime.Year<14||stime.Year>99)return 1;	
	
	/*�������к��е�����*/
	for(t=0;t<stime.Year;t++)	                                //��������ݵ��������
	{
		if(fnHT1381_Leap_Year(t))seccount+=31622400;            //�����������
		else seccount+=31536000;			                          //ƽ���������
	}
	
	/*�������к��е�����*/
	stime.Month-=1;
	for(t=0;t<stime.Month;t++)	                              //��ǰ���·ݵ����������
	{
		seccount+=(u32)mon_table[t]*86400;                      //�·����������
		if(fnHT1381_Leap_Year(stime.Year)&&t==1)seccount+=86400;//����2�·�����һ���������	   
	}
	seccount+=(u32)(stime.Day-1)*86400;                       //��ǰ�����ڵ���������� 
	seccount+=(u32)stime.Hour*3600;                           //Сʱ������
	seccount+=(u32)stime.Minutes*60;	                        //����������
	seccount+=stime.Second;                                   //�������Ӽ���ȥ
	
	return seccount;	    
}

/**********************************************************************************************************************************/
