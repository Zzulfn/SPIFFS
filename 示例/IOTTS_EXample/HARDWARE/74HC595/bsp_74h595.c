/**
  ******************************************************************************
  * @file    bsp_74h595.c
  * @author  Jack Lu
  * @version V1.0
  * @date    2017-12-28
  * @brief   
  ******************************************************************************
	*/
	
#include "bsp_74h595.h"
#include "delay.h"

/**********************************************************
 * ������ : H595_Init
 * ����   ��none
 * ���   ��none 
 * ����   ��74hc595��ʼ��
 **********************************************************/
void H595_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_HC595_CLK_PORT | RCC_HC595_DATA_PORT | RCC_HC595_RCLK_PORT | RCC_HC595_OE_PORT,ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = HC595_CLK_PIN ;        //74HC595��������ʱ������
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(HC595_CLK_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = HC595_DATA_PIN ;       // 74HC595����������������
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(HC595_DATA_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = HC595_RCLK_PIN ;       // 74HC595����洢������ʱ����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(HC595_RCLK_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = HC595_OE_PIN ;         // 74HC595���ʹ������
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(HC595_OE_PORT, &GPIO_InitStructure);
}

/**********************************************************
 * ������ : write_smg
 * ����   ��temp 8λ��������
 * ���   ��none 
 * ����   ����������д��
 **********************************************************/
void H595_WriteData(u8 *value)
{
	u8 i,j;
	u8 temp[4]={0,};
	memcpy(temp,value,4);	
	HC595_OE_L();
	for(j=0;j<4;j++)
	{
		for(i=0;i<8;i++)
		{
			HC595_CLK_L();
			if((temp[j]&0x80) == 0x80)						
			{
				HC595_DAT_H(); //�����������
			}
			else
			{
				HC595_DAT_L();
			} 
			HC595_CLK_H(); //��λ����ʱ�ӣ�����������
			temp[j]<<=1;
			delay_us(3);
			HC595_CLK_L();
		}
	}
}
/**********************************************************
 * ������ : H595_OutputEn
 * ����   ��temp 8λ��������
 * ���   ��none 
 * ����   ����������д��
 **********************************************************/
void H595_OutputEn(void)
{
	HC595_RCLK_L(); //�������ʱ��
	delay_us(3);	
	HC595_RCLK_H();
	delay_us(3);	
//	HC595_RCLK_L();	
}
/*******************************************************************************************/
