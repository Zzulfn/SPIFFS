/**
  ******************************************************************************
  * @file    bsp_usart1.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   usart应用bsp
  ******************************************************************************
  */
  
#include "bsp_usart1.h"

u8 commandDown=0;
char usartReBuf[RE_DATA_MAX];
 /**
  * @brief  USART1 GPIO 配置,工作模式配置。
  * @param  baudrate : 波特率设置
  * @retval None
  */
void USART1_Init(u32 baudrate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	/* config USART1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	
	/* USART1 GPIO config */
	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate =            baudrate;
	USART_InitStructure.USART_WordLength =          USART_WordLength_8b;
	USART_InitStructure.USART_StopBits =            USART_StopBits_1;
	USART_InitStructure.USART_Parity =              USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode =                USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//抢占式优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* 使能串口1接收中断 */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);
	USART_ClearFlag(USART1,USART_FLAG_TC);
}

void USART1_IRQHandler(void)
{
	uint8_t data;
	static uint8_t lastdata;
	static uint16_t pdataM=0;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{ 		
		data = USART_ReceiveData(USART1);
		USART_SendData(USART1, (uint8_t) data);//将接受到的数据直接返回打印
		
		usartReBuf[pdataM]=data;//接收缓存
		pdataM++;
		
		/*接收到数据包头*/
		if((lastdata==0x5A )&& (data==0xA5)&&(commandDown==0))//
		{
			pdataM=0;
			commandDown=1;
		}
		if((pdataM>4)&&(commandDown==1))//接收命令完成(将4修改为最大有效数据数量不包含帧头)
		{	
			commandDown=0;
		}
		if(pdataM>20)pdataM=0;
		lastdata=data;
	} 
}

// 重定向c库函数printf到USART1
int fputc(int ch, FILE *f)
{
	  /*清除SR寄存器TC标志位(默认1)，防止出现首字节丢失*/
	  USART_ClearFlag(USART1, USART_FLAG_TC);
	
		/* 发送一个字节数据到USART1 */
		USART_SendData(USART1, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);		
	
		return (ch);
}

/// 重定向c库函数scanf到USART1
int fgetc(FILE *f)
{
		/* 等待串口1输入数据 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}
/*********************************************END OF FILE**********************/
