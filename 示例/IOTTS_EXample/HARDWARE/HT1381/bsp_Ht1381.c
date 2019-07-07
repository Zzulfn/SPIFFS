
/*************************************************************************************
 * 文件名： bsp_Ht1381.c
 * 功能	 ： Ht1381时钟芯片驱动
 * 说明	 ： 适用于所有STM32芯片，如果IO有改动需改库文件宏定义
 *					IO与芯片的连接配置在bsp_Ht1381.h中设置。
 * 版本  : 1.0
 * 日期	 ：2017.12.27
 * 作者  ：Jack Lu 
 **************************************************************************************/

#include "bsp_Ht1381.h"

uint8_t    rtcUpdateStatus=rtcUpdate;//定义时间更新 状态变量  上点默认为  没有更新 

/**********************************************************
 * 函数名 : fnHT1381_Delay
 * 输入   ：ns 延时时间
 * 输出   ：none 
 * 功能   ：延时函数
 **********************************************************/
void fnHT1381_Delay(u16 ns)
{
	while(--ns);
}

/**********************************************************
 * 函数名 : fnHT1381_GPIO_Init
 * 输入   ：none
 * 输出   ：none 
 * 功能   ：HT1381 IO初始化
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
 * 函数名 : fnHT1381_IO_Output
 * 输入   ：none
 * 输出   ：none 
 * 功能   ：HT1381_SDA	设置为上拉输出  写数据用
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
 * 函数名 : fnHT1381_IO_Input
 * 输入   ：none
 * 输出   ：none 
 * 功能   ：HT1381_SDA	设置为下拉输入  读数据用
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
 * 函数名 : fnHT1381_WriteByte
 * 输入   ：需要写的数据  u8型
 * 输出   ：none 
 * 功能   ：向1381相应寄存器写入一个八位数据
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
 * 函数名 : fnHT1381_ReadByte
 * 输入   ：none
 * 输出   ：u8型数据 
 * 功能   ：从1381相应寄存器读取一个八位数据
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
 * 函数名 : fnHT1381_WriteData
 * 输入   ：ucAddress:  地址, data: 要写的数据
 * 输出   ：none
 * 功能   ：写入数据,先写地址,后写命令/数据
 **********************************************************/
void fnHT1381_WriteData(u8 ucAddress, u8 ucData)
{ 
	HT1381_RST(0);
	fnHT1381_Delay(10);
	HT1381_SCLK (0);
	fnHT1381_Delay(10);
	HT1381_RST(1);
	fnHT1381_Delay(10);
	fnHT1381_WriteByte(ucAddress); 		//地址，命令 
	fnHT1381_WriteByte(ucData);			 	//写入1Byte 数据 
	HT1381_SCLK(1);
	fnHT1381_Delay(10);
	HT1381_RST(0); 
	return; 
} 

/**********************************************************
 * 函数名 : fnHT1381_GetDataFromAddress
 * 输入   ：ucAddress 需要获取的地址
 * 输出   ：从指定地址获取的数据
 * 功能   ：读取某个指定地址的数据
 **********************************************************/
uint8_t fnHT1381_GetDataFromAddress(u8 ucAddress) 
{ 
	unsigned char ucDat; 
	HT1381_RST(0); 
	HT1381_SCLK(0); 
	HT1381_RST(1); 
 	fnHT1381_WriteByte(ucAddress);  //地址，命令 
	ucDat = fnHT1381_ReadByte(); 		//读1Byte 数据 
	HT1381_SCLK(1);  
	HT1381_RST(0);  
	return(ucDat);  
}  

/**********************************************************
 * 函数名 : fnHT1381_Init
 * 输入   ：none
 * 输出   ：none
 * 功能   ：HT1381初始化
 **********************************************************/
void fnHT1381_Init(void)
{
	fnHT1381_GPIO_Init();
	HT1381_SCLK(0);
	fnHT1381_Delay(10);
	HT1381_RST(0);
	fnHT1381_Delay(10);
	fnHT1381_WriteData(0x8e,0x00);		//取消写保护
  fnHT1381_WriteData(0x90,0x00);		//设置充电限流电阻阻值  0 不使能没有限流电阻
  //HT1381_WriteData(0x8e,0x80);		//加写保护	自己加的 如果程序不好用去掉此句
}

/**********************************************************
 * 函数名 : fnHT1381_GetTime
 * 输入   ：stuHT1381Time: 结构体首地址
 * 输出   ：none
 * 功能   ：读取HT1381 当前时间 7Byte (BCD 码)已经转换为变量格式
 **********************************************************/
void fnHT1381_GetTime(RTC_TimeTypeDef *tb) 
{ 
	u8 ucAddress = HT1381_GET_ADDRESS;
	u8 ucTemp = 0;
  ucTemp =fnHT1381_GetDataFromAddress(ucAddress);/*格式为: 秒分时日月星期年 */ 
	ucTemp = ucTemp%16 + (ucTemp/16)*10;		//BCD 码转换
  tb->Second=ucTemp;   // 秒
  
  
  ucAddress+= 2; 
  ucTemp =fnHT1381_GetDataFromAddress(ucAddress);/*格式为: 秒分时日月星期年 */ 
	ucTemp = ucTemp%16 + (ucTemp/16)*10;		//BCD 码转换
  tb->Minutes=ucTemp;   // 分
  
  ucAddress+= 2; 
  ucTemp =fnHT1381_GetDataFromAddress(ucAddress);/*格式为: 秒分时日月星期年 */ 
	ucTemp = ucTemp%16 + (ucTemp/16)*10;		//BCD 码转换
  tb->Hour=ucTemp;     // 时
  
  ucAddress+= 2; 
  ucTemp =fnHT1381_GetDataFromAddress(ucAddress);/*格式为: 秒分时日月星期年 */ 
	ucTemp = ucTemp%16 + (ucTemp/16)*10;		//BCD 码转换
  tb->Day=ucTemp;   //日
  
  ucAddress+= 2; 
  ucTemp =fnHT1381_GetDataFromAddress(ucAddress);/*格式为: 秒分时日月星期年 */ 
	ucTemp = ucTemp%16 + (ucTemp/16)*10;		//BCD 码转换
  tb->Month=ucTemp;   // 月
  
  ucAddress+= 2; 
  ucTemp =fnHT1381_GetDataFromAddress(ucAddress);/*格式为: 秒分时日月星期年 */ 
	ucTemp = ucTemp%16 + (ucTemp/16)*10;		//BCD 码转换
  tb->Week=ucTemp;   //星期
  
  ucAddress+= 2; 
  ucTemp =fnHT1381_GetDataFromAddress(ucAddress);/*格式为: 秒分时日月星期年 */ 
	ucTemp = ucTemp%16 + (ucTemp/16)*10;		//BCD 码转换
  tb->Year=ucTemp;   //年
} 

/**********************************************************
 * 函数名 : fnHT1381_SetTime
 * 输入   ：stuHT1381Time: 结构体首地址， 时间格式为: 秒分时日月星期年
 * 输出   ：none
 * 功能   ：设置初始时间 7Byte (BCD 码) 1B 1B 1B 1B 1B 1B 1B 
 **********************************************************/
void fnHT1381_SetTime(RTC_TimeTypeDef *tb) 
{
  uint8_t ucAddress=HT1381_SET_ADDRESS;
  uint8_t buf,d;
	
	fnHT1381_WriteData(0x8e,0x00);  /* 控制命令,WP=0,写操作?*/  
   
  d=tb->Second;               /* 秒分时日月星期年 */  
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
 
	fnHT1381_WriteData(0x8e,0x80);  /* 控制命令,WP=1,写保护*/  
}  

/**********************************************************
 * 函数名 : fnRTC_CheckAndConfig
 * 输入   ：stuHT1381Time: 结构体首地址， 时间格式为: 秒分时日月星期年
 * 输出   ：none
 * 功能   ：检测是否已经设置过RTC，没有设置，否则跳过设置 
 *          此函数会在STM32备份数据寄存器写入初始值,
 *          以保证RTC只在下程序后第一次上电设置时间，以后不再设置
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
 * 函数名 : rtcGetUpdateStatus
 * 输入   ：none
 * 输出   ：rtcUpdateStatus 更新标志
 * 功能   ：获取RTC更新标志
 **********************************************************/
uint8_t rtcGetUpdateStatus(void)
{
  return  rtcUpdateStatus;
}

/**********************************************************
 * 函数名 : rtcSetTime
 * 输入   ：TimeBuf: 结构体首地址
 * 输出   ：none
 * 功能   ：设置Rtc时间
 **********************************************************/
void rtcSetTime(RTC_TimeTypeDef *TimeBuf)
{
  fnHT1381_SetTime(TimeBuf);    // 更新时间
  rtcUpdateStatus=rtcUpdateOk;  // 时间更新完成
  printf("时间保存完成\r\n");
}

/**********************************************************
 * 函数名 : fnHT1381_Leap_Year
 * 输入   ：年份
 * 输出   ：该年份是不是闰年.1,是.0,不是
 * 功能   ：判断是否是闰年函数
 *          月份   1  2  3  4  5  6  7  8  9  10 11 12
 *          闰年   31 29 31 30 31 30 31 31 30 31 30 31
 *          非闰年 31 28 31 30 31 30 31 31 30 31 30 31
 **********************************************************/
uint8_t fnHT1381_Leap_Year(u16 year)
{			  
	if(year%4==0) //必须能被4整除
		{ 
		if(year%100==0) 
			{ 
			if(year%400==0)return 1;//如果以00结尾,还要能被400整除 	   
			else return 0;   
			}else return 1;   
		}else return 0;	
}	
/**********************************************************
 * 函数名 : fnHT1381_Leap_Year
 * 输入   ：时间
 * 输出   ：数字
 * 功能   ：将时间转化为数字
 **********************************************************/	
uint64_t fnHT1381_TimeToNum(RTC_TimeTypeDef stime)
{
	u16 t;
	uint64_t seccount=0;
	
	u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};    //平年的月份日期表
	
	if(stime.Year<14||stime.Year>99)return 1;	
	
	/*计算年中含有的秒数*/
	for(t=0;t<stime.Year;t++)	                                //把所有年份的秒钟相加
	{
		if(fnHT1381_Leap_Year(t))seccount+=31622400;            //闰年的秒钟数
		else seccount+=31536000;			                          //平年的秒钟数
	}
	
	/*计算月中含有的秒数*/
	stime.Month-=1;
	for(t=0;t<stime.Month;t++)	                              //把前面月份的秒钟数相加
	{
		seccount+=(u32)mon_table[t]*86400;                      //月份秒钟数相加
		if(fnHT1381_Leap_Year(stime.Year)&&t==1)seccount+=86400;//闰年2月份增加一天的秒钟数	   
	}
	seccount+=(u32)(stime.Day-1)*86400;                       //把前面日期的秒钟数相加 
	seccount+=(u32)stime.Hour*3600;                           //小时秒钟数
	seccount+=(u32)stime.Minutes*60;	                        //分钟秒钟数
	seccount+=stime.Second;                                   //最后的秒钟加上去
	
	return seccount;	    
}

/**********************************************************************************************************************************/
