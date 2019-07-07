

#include "bsp.h"
#include "stdio.h"

char SDS_011Str[20];
typ_SDS011 SDS011;
void SDS011_Init(void)
{
		USART_InitTypeDef USART_InitStructure;
		GPIO_InitTypeDef GPIO_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		// 初始化
		RCC_APB1PeriphClockCmd(SDS011_USART_GPIO_CLK, ENABLE );
		RCC_APB2PeriphClockCmd(SDS011_RX_GPIO_CLK, ENABLE );
		RCC_APB2PeriphClockCmd(SDS011_POWER_GPIO_CLK, ENABLE );
	
		// 初始化 RXD
		GPIO_InitStructure.GPIO_Pin = SDS011_RX_GPIO_PIN; 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(SDS011_RX_GPIO_PORT, &GPIO_InitStructure);  
	
		GPIO_InitStructure.GPIO_Pin = SDS011_POWER_GPIO_PIN; 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(SDS011_POWER_GPIO_PORT, &GPIO_InitStructure);

		//初始化串口
		USART_InitStructure.USART_BaudRate            = SDS011_USART_BAUDRATE ;
		USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits            = USART_StopBits_1;
		USART_InitStructure.USART_Parity              = USART_Parity_No ;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode                = USART_Mode_Rx;
		USART_Init(SDS011_USART_PORT, &USART_InitStructure); 
		

		USART_Cmd(SDS011_USART_PORT, ENABLE);
		//开中断
		USART_ITConfig(SDS011_USART_PORT, USART_IT_RXNE, ENABLE);

		NVIC_InitStructure.NVIC_IRQChannel = SDS011_USART_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
		SDS011_POWER_OUT(0);
		
}
typ_Event SDS011_event;
void SDS011_USART_IRQHandler(void)
{
	char data;
	static char lastdata;
	static uint16_t pdataS;
	if(USART_GetITStatus(SDS011_USART_PORT, USART_IT_RXNE) != RESET)
	{ 	
		/*读取串口*/
		data=SDS011_USART_PORT->DR;
		/*接收到数据包头*/
		if(lastdata==0xaa && data==0xc0)
		{
			pdataS=0;
 			STATE_SENSOR.PM25_PM10++;
		}		
		SDS_011Str[pdataS]=data;
		
		/*一条数据已经读取完毕*/
		if(pdataS>4)
		{
			SDS011.PM10=(SDS_011Str[4]*256+SDS_011Str[3])/10;
			SDS011.PM25=(SDS_011Str[2]*256+SDS_011Str[1])/10;
			SDS011.PM25+=10;
//			else if((SDS011.PM25>=6)&&(SDS011.PM25<11))SDS011.PM25+=8;
//			else if((SDS011.PM25>=11)&&(SDS011.PM25<16))SDS011.PM25+=6;
//			else if((SDS011.PM25>=16)&&(SDS011.PM25<21))SDS011.PM25+=5;
//			else if((SDS011.PM25>=21)&&(SDS011.PM25<26))SDS011.PM25+=4;
//			else if((SDS011.PM25>=26)&&(SDS011.PM25<=30))SDS011.PM25+=2;
			if(SDS011.PM25>=999)SDS011.PM25=999;
		}
		pdataS++;
		if(pdataS>10)pdataS=10;
		lastdata=data;
	} 	
	USART_ClearFlag(SDS011_USART_PORT,USART_FLAG_RXNE);
}
typ_SDS011 SDS011_Get(void)
{
	return SDS011;
}
