
#include "bsp_led.h"

/**********************************************************
 * 函数名 : LED_Init
 * 输入   ：none
 * 输出   ：none 
 * 功能   ：LED接口初始化
 **********************************************************/
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE , ENABLE);	//开启GPIOC外设时钟	
  
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD , ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4| GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_Init(GPIOD, &GPIO_InitStructure);

	
	GPIOE->BSRR^=GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4| GPIO_Pin_5 | GPIO_Pin_6;
}


