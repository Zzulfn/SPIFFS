/**
  ******************************************************************************
  * @file    bsp_dac.c
  * @author  Jack Lu
  * @version V1.0
  * @date    2017-12-28
  * @brief   
  ******************************************************************************
	*/
#include "bsp_dac.h"

/**********************************************************
 * ������ : BSP_DAC_Init
 * ����   ��none
 * ���   ��none 
 * ����   ��DAC��ʼ��
 **********************************************************/
void BSP_DAC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	DAC_InitTypeDef DAC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,ENABLE);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	DAC_InitStructure.DAC_Trigger=DAC_Trigger_None;//��ʹ�ó�������
	DAC_InitStructure.DAC_WaveGeneration=DAC_WaveGeneration_None;//��ʹ�����ǲ�
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//���� ��ֵ����
	DAC_InitStructure.DAC_OutputBuffer=DAC_OutputBuffer_Disable;//�رջ���
	DAC_Init(DAC_Channel_1,&DAC_InitStructure);//��ʼ��DACͨ��1

	DAC_Cmd(DAC_Channel_1,ENABLE);//ʹ��DAC1

	DAC_SetChannel1Data(DAC_Align_12b_R,0);//12λ �Ҷ���
	
	DAC_Init(DAC_Channel_2,&DAC_InitStructure);//��ʼ��DACͨ��1

	DAC_Cmd(DAC_Channel_2,ENABLE);//ʹ��DAC1

	DAC_SetChannel2Data(DAC_Align_12b_R,0);//12λ �Ҷ���
}

//����ͨ��1�����ѹ
//vol:0~3300,����0~3.3V
void Dac1_Set_Vol(u16 vol)
{
	float temp=vol;
	temp/=1000;
	temp=temp*4096/3.3;
	DAC_SetChannel1Data(DAC_Align_12b_R,temp);//12λ�Ҷ������ݸ�ʽ����DACֵ
}

//����ͨ��1�����ѹ
//vol:0~3300,����0~3.3V
void Dac2_Set_Vol(u16 vol)
{
	float temp=vol;
	temp/=1000;
	temp=temp*4096/3.3;
	DAC_SetChannel2Data(DAC_Align_12b_R,temp);//12λ�Ҷ������ݸ�ʽ����DACֵ
}
/**********************************************************
 * ������ : BSP_DAC_Init
 * ����   ��none
 * ���   ��none 
 * ����   ��DAC��ʼ��
 **********************************************************/
SOFT_Dac_Module_Output DAC_Manage( u8 DAC_Channel_Sel,SOFT_Dac_Module_Input value)
{
	SOFT_Dac_Module_Output temp;
	if(value.DAC_Channel_En==1)
	{
		switch(DAC_Channel_Sel)
		{
			case 0: 
				Dac1_Set_Vol(value.DAC_vol);
				break;
			case 1:
				Dac2_Set_Vol(value.DAC_vol);
				break;
			default:break;
		}
	}
	else 
		switch(DAC_Channel_Sel)
		{
			case 0: 
				Dac1_Set_Vol(0);
				break;
			case 1:
				Dac2_Set_Vol(0);
				break;
			default:break;
		}
		temp.DAC_Channel_Module_Status=value.DAC_Channel_En;
		return temp;
		
}

