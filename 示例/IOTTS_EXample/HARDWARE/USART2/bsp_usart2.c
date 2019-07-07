/**
  ******************************************************************************
  * @file    bsp_usart2.c
  * @author  Jack Lu
  * @version V1.0
  * @date    2017-09-20
  * @brief   usartӦ��bsp
  ******************************************************************************
  */
  
#include "bsp_usart2.h"

 /**
  * @brief  USART2 GPIO ����,����ģʽ���á�
  * @param  baudrate : ����������
  * @retval None
  */
void USART2_Init(u32 baudrate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* config USART2 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	/* USART2 GPIO config */
	/* Configure USART2 Tx (PA.2) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	/* Configure USART2 Rx (PA.3) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*485ʹ�ܶ�*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
//	GPIO_ResetBits(GPIOE,GPIO_Pin_7);
   GPIO_SetBits(GPIOE,GPIO_Pin_7);

  /* USART2 mode config */
	USART_InitStructure.USART_BaudRate =             baudrate;
	USART_InitStructure.USART_WordLength =           USART_WordLength_8b;
	USART_InitStructure.USART_StopBits =             USART_StopBits_1;
	USART_InitStructure.USART_Parity =               USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl =  USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode =                 USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	
	/* ʹ�ܴ���2�����ж� */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART2, ENABLE);
	
	/* ����USART2�����ж� */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void USART2_IRQHandler(void)
{
	uint8_t temp;
	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{ 	
	   temp = USART_ReceiveData(USART2);
		 USART_SendData(USART2,temp);//�����ܵ�������ֱ�ӷ��ش�ӡ 
     printf("-----------0x%02x",temp);		
	} 
	USART_ClearFlag(USART2,USART_FLAG_RXNE);
}



// �ض���c�⺯��printf��USART1
// int fputc(int ch, FILE *f)
// {
// 		/* ����һ���ֽ����ݵ�USART1 */
// 		USART_SendData(USART1, (uint8_t) ch);
// 		
// 		/* �ȴ�������� */
// 		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);		
// 	
// 		return (ch);
// }

// /// �ض���c�⺯��scanf��USART1
// int fgetc(FILE *f)
// {
// 		/* �ȴ�����1�������� */
// 		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

// 		return (int)USART_ReceiveData(USART1);
// }
/*********************************************END OF FILE**********************/
