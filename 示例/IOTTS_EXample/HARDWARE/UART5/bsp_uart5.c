/**
  ******************************************************************************
  * @file    bsp_uart5.c
  * @author  Jack Lu
  * @version V1.0
  * @date    2017-09-20
  * @brief   
  ******************************************************************************
	*/
#include "bsp_uart5.h"

 /**
  * @brief  UART5 GPIO 配置,工作模式配置。
  * @param  baudrate : 波特率设置
  * @retval None
  */
void UART5_Init(u32 baudrate)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* config UART5 clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE );
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE );

	/* UART5 GPIO config */
	/* Configure UART5 Rx (PD.2) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD, &GPIO_InitStructure);  

	/* Configure UART5 Tx (PC.12) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);  

	/* UART5 mode config */
	USART_InitStructure.USART_BaudRate            = baudrate ;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART5, &USART_InitStructure); 
	
	USART_Cmd(UART5, ENABLE);
	/* 使能串口5接收中断 */
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
	/* 配置USART5接收中断 */
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
			
}

void UART5_IRQHandler(void)
{
	char temp;
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
	{ 	
		/*读取串口*/
		temp=UART5->DR;
		USART_SendData(UART5,temp);
	} 	
	USART_ClearFlag(UART5,USART_FLAG_RXNE);
}

