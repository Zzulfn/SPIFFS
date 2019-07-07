
/*************************************************************************************
 * 文件名： bsp_Ht1381.h
 * 功能	 ： Ht1381时钟芯片驱动库
 * 说明	 ： 
 * 版本  : 1.0
 * 日期	 ：2017.12.27
 * 作者  ：Jack Lu 
 **************************************************************************************/

#ifndef	__BSP_HT1381_H
#define	__BSP_HT1381_H

#include "stm32f10x.h"
#include "stdio.h"

#define RCC_RTC_CLK_PORT                     RCC_APB2Periph_GPIOB
#define RTC_CLK_PIN                          GPIO_Pin_13
#define RTC_CLK_PORT                         GPIOB
#define RCC_RTC_DATA_PORT                    RCC_APB2Periph_GPIOB
#define RTC_DATA_PIN                         GPIO_Pin_12
#define RTC_DATA_PORT                        GPIOB
#define RCC_RTC_RST_PORT                     RCC_APB2Periph_GPIOB
#define RTC_RST_PIN                          GPIO_Pin_14
#define RTC_RST_PORT                         GPIOB

#define HT1381_SET_ADDRESS 0x80		//ht1381写地址起始位  每个数据间地址差2 共7个数据
#define HT1381_GET_ADDRESS 0x81		//ht1381读地址起始位  每个数据间地址差2 共7个数据  
#define HT1381_TIME_LENGTH  7

//===================IO连接设置 x 取值 0 ，1========================================

#define HT1381_SCLK(x)	RTC_CLK_PORT->ODR=(RTC_CLK_PORT->ODR&~(1<<13))|(x ? (1<<13):0)	//HT1381_SCLK -> PB13	
#define HT1381_RST(x)   RTC_RST_PORT->ODR=(RTC_RST_PORT->ODR&~(1<<14))|(x ? (1<<14):0)	//HT1381_RST -> PB14	
#define HT1381_IO(x)		RTC_DATA_PORT->ODR=(RTC_DATA_PORT->ODR&~(1<<12))|(x ? (1<<12):0)	//HT1381_IO -> PB12	
#define IS_HT1381_IO		(RTC_DATA_PORT->IDR&(1<<12))?1:0													//读取HT1381 IO线		
//#else
//#define  HT1381_SCLK(x)  (*((uint32_t *)(0x42220190)))=x               // pc4
//#define  HT1381_RST(x)   (*((uint32_t *)(0x42218180)))=x               // PB0
////#define  HT1381_RST(x)   (*((uint32_t *)(0x42220198)))=x               // PC6
//#define  HT1381_IO(x)    (*((uint32_t *)(0x42220194)))=x               // pc5
//#define  IS_HT1381_IO    (*((uint32_t *)(0x42220114)))                 // pc5  输入
//#endif

/*RTC更新标志*/
#define rtcPowerOn  0  //设备上电
#define rtcUpdate   1  //需要更新时间
#define rtcNoUpdate 2  //RTC不需要更新时间
#define rtcUpdateOk 3  //RTC时间已经更新过了

/*RTC结构体*/
typedef struct 
{
	u8 Second;
	u8 Minutes;
	u8 Hour;
	u8 Day;
	u8 Month;
	u8 Week;
	u8 Year;
}	RTC_TimeTypeDef;

void fnHT1381_GetTime(RTC_TimeTypeDef *stuHT1381Time); //功能: 读取HT1381 当前时间 
void fnHT1381_Init(void);                              //HT1381初始化   在CPU没有备份寄存器时使用
void fnHT1381_SetTime(RTC_TimeTypeDef *stuHT1381Time); // 功能: 设置初始时间 
void  rtcSetTime(RTC_TimeTypeDef *TimeBuf);

uint64_t fnHT1381_TimeToNum(RTC_TimeTypeDef stime);
uint8_t rtcGetUpdateStatus(void);

#endif

