/**********************************************************
* @ File name -> bsp_cd4066.c
* @ Version   -> V1.0.1
* @ Date      -> 02-06-2017
* @ Brief     -> ���ӿ��س���
* @ Revise    -> 
***********************************************************/
#include "bsp_cd4066.h" 
#include "usart.h"
/***********************************************
 * ������ ��Cd4066_Init
 * ����   ����ʼ��CD4066
 * ����   ����
 * ���   ����
 ***********************************************/
void CD4066_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIOC Periph clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
}
/***********************************************
 * ������ ��Cd4066_Choose
 * ����   ��ѡ���ⲿ��Դ����������Դ����
 * ����   ��i=0 �������� i=1 AUX����
 * ���   �����سɹ�
 ***********************************************/
uint8_t CD4066_Choose(VoiceCH i)
{
	if(i==BLUETOOTH_INPUT)
	{
		GPIOC->BSRR=0x0240;
		GPIOC->BRR=0x0180;
		return 0;
	}
	else
	{
		GPIOC->BSRR=0x0180;
		GPIOC->BRR=0x0240;
		return 0;
	}
}
/***********************************************
 * ������ ��Cd4066_Read
 * ����   ����������״̬ѡ���ⲿ��Դ����������Դ����
 * ����   ��none
 * ���   ��none
 ***********************************************/
void CD4066_Read(void)
{
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==0)
	{
		CD4066_Choose(BLUETOOTH_INPUT);
	}
	else
	{
		CD4066_Choose(AUX_INPUT);
	}
}
