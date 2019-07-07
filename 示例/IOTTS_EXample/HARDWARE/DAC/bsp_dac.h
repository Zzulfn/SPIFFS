/**********************************************************
* @ File name -> bsp_dac.h
* @ Version   -> V1.0
* @ Date      -> 26-06-2017
* @ Brief     -> DAC��غ���ͷ�ļ�

 V1.*
* @ Revise    ->
**********************************************************/

#ifndef _bsp_dac_h_
#define _bsp_dac_h_

/**********************************************************
                     �ⲿ����ͷ�ļ�
         Ӧ�õ���ͬ������ͷ�ļ����������޸ļ���                        
**********************************************************/

#include "sys.h"

/*
 * DACģ����������ṹ��
 */
typedef struct
{
	uint16_t  DAC_vol;                  /*!<   DAC������Ƶ�ѹֵ */

	uint8_t  DAC_Channel_En;           /*!<   DAC���ʹ�� */ 
	
}SOFT_Dac_Module_Input;

/*
 * DACģ����������ṹ��
 */
typedef struct
{
	uint8_t  DAC_Channel_Module_Status;          /*!<   DAC���ģ��״̬ */

}SOFT_Dac_Module_Output;

/**********************************************************
                       �ⲿ���ܺ���                      
**********************************************************/
void BSP_DAC_Init(void);
void Dac1_Set_Vol(u16 vol);
void Dac2_Set_Vol(u16 vol);
SOFT_Dac_Module_Output DAC_Manage( u8 DAC_Channel_Sel,SOFT_Dac_Module_Input value);
#endif

