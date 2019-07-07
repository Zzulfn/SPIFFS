/**
  ******************************************************************************
  * @file    bsp_adc.h
  * @author  Jack Lu
  * @version V1.0
  * @date    2017-12-28
  * @brief   ADC��غ���ͷ�ļ�
  ******************************************************************************
	*/

#ifndef __BSP_ADC_H_
#define __BSP_ADC_H_

/**********************************************************
                     �ⲿ����ͷ�ļ�
         Ӧ�õ���ͬ������ͷ�ļ����������޸ļ���                        
**********************************************************/

#include "sys.h"

#define ADC1_DR_Address    ((u32)0x40012400+0x4c)

/*
 * ADC��������ṹ��
 */
typedef struct
{
  uint8_t  ADC_ChannelInput_En;      /*!<   ����ͨ��ʹ�� */ 
   
	uint16_t  ADC_H_lim;                /*!<   ����ͨ������ֵ ����Ϊ0��ʾ�����������޿��� */

	uint16_t  ADC_L_lim;                /*!<   ����ͨ������ֵ */
	
}SOFT_Adc_Module_Input;//ģ����������ṹ��

/*
 * ADC��������ṹ��
 */
typedef struct
{
	uint8_t  ADC_Channel_Module_Status;    /*!<   ģ�鹤��״̬ */
	
  uint16_t  ADC_Channel_Data;            /*!<   ADCʵ����ֵ  */    
   
  signed char  ADC_Out_Status;           /*!<   ��ֵ״̬��� */
	
}SOFT_Adc_Module_Output;

typedef enum {
		ADC_0=0,
    ADC_1,
    ADC_2,
    ADC_3,
    ADC_4,
    ADC_5,
		ADC_6,
    ADC_7,
}ADC_Channel_Sel;/*!<   ����ͨ��ѡ�� */
/**********************************************************
                       �ⲿ���ܺ���                      
**********************************************************/
void BSP_ADC_Init(void);
uint16_t BSP_GetAdcValue(void);
SOFT_Adc_Module_Output ADC_Manage(ADC_Channel_Sel ADC_In_Sel,SOFT_Adc_Module_Input value);
#endif

