/**
  ******************************************************************************
  * @file    bsp_usart1.c
  * @author  Jack Lu
  * @version V1.0
  * @date    2017-09-20
  * @brief   usart应用bsp
  ******************************************************************************
  */
  
#include "bsp_usart3.h"

 /**
  * @brief  USART3 GPIO 配置,工作模式配置。
  * @param  baudrate : 波特率设置
  * @retval None
  */
void USART3_Init(u32 baudrate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* config USART3 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
	/* USART3 GPIO config */
	/* Configure USART3 Tx (PB.10) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);    
	
	/* Configure USART3 Rx (PB.11) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/*485使能端*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOE,GPIO_Pin_15);
	
	/* USART3 mode config */
	USART_InitStructure.USART_BaudRate =            baudrate;
	USART_InitStructure.USART_WordLength =          USART_WordLength_8b;
	USART_InitStructure.USART_StopBits =            USART_StopBits_1;
	USART_InitStructure.USART_Parity =              USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode =                USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);
	
	/* 使能串口3接收中断 */
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART3, ENABLE);
	
	/* 配置USART3接收中断 */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void USART3_IRQHandler(void)
{
	uint8_t temp;
	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{ 	
		temp = USART_ReceiveData(USART3);
		USART_SendData(USART3,temp);//将接受到的数据直接返回打印  
		printf("////-----------0x%02x",temp);	
	} 
 	USART_ClearFlag(USART3,USART_FLAG_RXNE); 
}


// 重定向c库函数printf到USART1
// int fputc(int ch, FILE *f)
// {
// 		/* 发送一个字节数据到USART1 */
// 		USART_SendData(USART3, (uint8_t) ch);
// 		
// 		/* 等待发送完毕 */
// 		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);		
// 	
// 		return (ch);
// }

// /// 重定向c库函数scanf到USART1
// int fgetc(FILE *f)
// {
// 		/* 等待串口1输入数据 */
// 		while (USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET);

// 		return (int)USART_ReceiveData(USART3);
// }
/*********************************************END OF FILE**********************/
