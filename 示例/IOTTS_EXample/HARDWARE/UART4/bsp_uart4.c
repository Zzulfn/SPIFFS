/**
  ******************************************************************************
  * @file    bsp_uart4.c
  * @author  Jack Lu
  * @version V1.0
  * @date    2017-09-20
  * @brief   
  ******************************************************************************
	*/
#include "bsp_uart4.h"

 /**
  * @brief  UART4 GPIO 配置,工作模式配置。
  * @param  baudrate : 波特率设置
  * @retval None
  */
void UART4_Init(u32 baudrate)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* config UART4 clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE );
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE );

	/* UART4 GPIO config */
	/* Configure UART4 Rx (PC.11) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);  

	/* Configure UART4 Tx (PC.11) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);  

	/* UART4 mode config */
	USART_InitStructure.USART_BaudRate            = baudrate ;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART4, &USART_InitStructure); 
	
	USART_Cmd(UART4, ENABLE);
	/* 使能串口4接收中断 */
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	/* 配置USART4接收中断 */
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
			
}

void UART4_IRQHandler(void)
{
	char data;
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
	{ 	
		/*读取串口*/
		data=UART4->DR;
		USART_SendData(USART1,data);
	} 	
//	USART_ClearFlag(UART4,USART_FLAG_RXNE);
}

