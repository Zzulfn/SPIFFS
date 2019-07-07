/**
  ******************************************************************************
  * @file    bsp_74h165.c
  * @author  Jack Lu
  * @version V1.0
  * @date    2017-12-28
  * @brief   
  ******************************************************************************
	*/
	
#include "bsp_74h165.h"
#include "delay.h"

/**********************************************************
 * ������ : H165_Init
 * ����   ��none
 * ���   ��none 
 * ����   ��74hc165��ʼ��
 **********************************************************/
void H165_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_HC165_CP_PORT | RCC_HC165_DATA_PORT | RCC_HC165_PL_PORT | RCC_HC165_CE_PORT,ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = HC165_CP_PIN ;         //74HC165��������ʱ������
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(HC165_CP_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = HC165_DATA_PIN ;       // 74HC165���������������
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(HC165_DATA_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = HC165_PL_PIN ;         // 74HC165��������ʹ��
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(HC165_PL_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = HC165_CE_PIN ;         // 74HC165�������ʹ������
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(HC165_CE_PORT, &GPIO_InitStructure);
}

/**********************************************************
 * ������ : H165_ReadData
 * ����   ��data_counter ��Ҫ��ȡ����ͨ��
 * ���   ��8λ�������� 
 * ����   ����ȡ74hc165�����������
 **********************************************************/
void H165_ReadData(u8 num,u8 *value)
{
	u8 i,j;
	u8 hc165_data=0;
	
	HC165_CE_L();
  HC165_PL_H();
	for(j=0;j<num;j++)
	{
		for(i=0;i<8;i++)
		{
			hc165_data<<=1;
			if(HC165_DAT == 0x100)			
				hc165_data+=1;
			HC165_CP_H(); //��λ����ʱ�ӣ�����������
			delay_us(10);
			HC165_CP_L();
			delay_us(10);
		}
		*value=hc165_data;
		value++;
	}
	HC165_PL_L();
}

/*****************************************************************************************************************/
