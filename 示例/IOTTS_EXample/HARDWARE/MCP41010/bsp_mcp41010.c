
/**********************************************************									 
���ܣ����ֵ�λ�����Ƴ���
ʱ�䣺2017/06/02
�汾��1.0
���ߣ�jack lu
**********************************************************/

#include "bsp_mcp41010.h" 
#include "delay.h"

/***************************************************************
 *������ ��MCP41010_Init
 *����   ��MCP41010 IO�ڳ�ʼ��
 *����   ����
 *���   ����
****************************************************************/
void MCP41010_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;            //����IO�ڳ�ʼ�����ݵĽṹ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); // IO��ʱ��ʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // IO��ʱ��ʹ��
	
	GPIO_InitStructure.GPIO_Pin = MCP41010_CS2 | MCP41010_CS3 | MCP41010_CS4 | MCP41010_CS5;  //��ʼ������IO��
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //IO ������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //IO ����ģʽ �������
	GPIO_Init(GPIOC, &GPIO_InitStructure);             //��ʼ��GPIOC
	
	GPIO_InitStructure.GPIO_Pin = MCP41010_CS1;        //��ʼ������IO��
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //IO ������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //IO ����ģʽ �������
	GPIO_Init(GPIOA, &GPIO_InitStructure);             //��ʼ��GPIOA
	
	GPIO_InitStructure.GPIO_Pin = MCP41010_CLK;        //��ʼ��MCP41010ʱ������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //IO ������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //IO ����ģʽ �������
	GPIO_Init(MCP41010_PORT_SCK, &GPIO_InitStructure); //��ʼ��GPIOC
	
	GPIO_InitStructure.GPIO_Pin = MCP41010_SI;         //��ʼ��MCP41010��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //IO ������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //IO ����ģʽ �������
	GPIO_Init(MCP41010_PORT_SI, &GPIO_InitStructure);  //��ʼ��GPIOC

}

/*
*********************************************************************************************************
*	�� �� ��: MCP41010_AmpSet
*	����    : �ı�����źŷ���ֵ
*	����    : 1.amp ������ֵ  0- 255
*	���    : ��
*********************************************************************************************************
*/ 

void MCP41010_AmpSet(uint8_t amp,uint8_t csx)
{
	uint8_t i;
	uint16_t temp;
	static uint8_t write_flag=0;
	
	if(write_flag==0)write_flag=1;//��ֹƵ�����ڵ��������
	SCK_H();
	delay_us(1);
  switch(csx)
	{
		case modeCS1: {CS1_L();CS2_H();CS3_H();CS4_H();CS5_H();} break;
		case modeCS2: {CS1_H();CS2_L();CS3_H();CS4_H();CS5_H();} break;
		case modeCS3: {CS1_H();CS2_H();CS3_L();CS4_H();CS5_H();} break;
		case modeCS4: {CS1_H();CS2_H();CS3_H();CS4_L();CS5_H();} break;
		case modeCS5: {CS1_H();CS2_H();CS3_H();CS4_H();CS5_L();} break;
	  default:  break;
	}		
	temp =0x1100|amp;//0001 0001 0000 0000 �Ե�λ��0����д���ݲ���
	for(i=0;i<16;i++)
	{
	   if(temp&0x8000)
	   	DAT_H();
	   else
			DAT_L();
		 temp<<=1;
		 delay_us(200);
		 SCK_L();
		 delay_us(200);
	   SCK_H();
	}
	delay_us(1);
	SCK_L();
	switch(csx)
	{
		case modeCS1: CS1_H(); break;
		case modeCS2: CS2_H(); break;
		case modeCS3: CS3_H(); break;
		case modeCS4: CS4_H(); break;
		case modeCS5: CS5_H(); break;
	  default:  break;
	}
	write_flag=0;
}

