
/*************************************************************************************
 * �ļ����� bsp_Ht1381.h
 * ����	 �� Ht1381ʱ��оƬ������
 * ˵��	 �� 
 * �汾  : 1.0
 * ����	 ��2017.12.27
 * ����  ��Jack Lu 
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

#define HT1381_SET_ADDRESS 0x80		//ht1381д��ַ��ʼλ  ÿ�����ݼ��ַ��2 ��7������
#define HT1381_GET_ADDRESS 0x81		//ht1381����ַ��ʼλ  ÿ�����ݼ��ַ��2 ��7������  
#define HT1381_TIME_LENGTH  7

//===================IO�������� x ȡֵ 0 ��1========================================

#define HT1381_SCLK(x)	RTC_CLK_PORT->ODR=(RTC_CLK_PORT->ODR&~(1<<13))|(x ? (1<<13):0)	//HT1381_SCLK -> PB13	
#define HT1381_RST(x)   RTC_RST_PORT->ODR=(RTC_RST_PORT->ODR&~(1<<14))|(x ? (1<<14):0)	//HT1381_RST -> PB14	
#define HT1381_IO(x)		RTC_DATA_PORT->ODR=(RTC_DATA_PORT->ODR&~(1<<12))|(x ? (1<<12):0)	//HT1381_IO -> PB12	
#define IS_HT1381_IO		(RTC_DATA_PORT->IDR&(1<<12))?1:0													//��ȡHT1381 IO��		
//#else
//#define  HT1381_SCLK(x)  (*((uint32_t *)(0x42220190)))=x               // pc4
//#define  HT1381_RST(x)   (*((uint32_t *)(0x42218180)))=x               // PB0
////#define  HT1381_RST(x)   (*((uint32_t *)(0x42220198)))=x               // PC6
//#define  HT1381_IO(x)    (*((uint32_t *)(0x42220194)))=x               // pc5
//#define  IS_HT1381_IO    (*((uint32_t *)(0x42220114)))                 // pc5  ����
//#endif

/*RTC���±�־*/
#define rtcPowerOn  0  //�豸�ϵ�
#define rtcUpdate   1  //��Ҫ����ʱ��
#define rtcNoUpdate 2  //RTC����Ҫ����ʱ��
#define rtcUpdateOk 3  //RTCʱ���Ѿ����¹���

/*RTC�ṹ��*/
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

void fnHT1381_GetTime(RTC_TimeTypeDef *stuHT1381Time); //����: ��ȡHT1381 ��ǰʱ�� 
void fnHT1381_Init(void);                              //HT1381��ʼ��   ��CPUû�б��ݼĴ���ʱʹ��
void fnHT1381_SetTime(RTC_TimeTypeDef *stuHT1381Time); // ����: ���ó�ʼʱ�� 
void  rtcSetTime(RTC_TimeTypeDef *TimeBuf);

uint64_t fnHT1381_TimeToNum(RTC_TimeTypeDef stime);
uint8_t rtcGetUpdateStatus(void);

#endif

