/**********************************************************									 
���ܣa��������
ʱ�䣺2017/06/08
�汾��1.0
���ߣ�jack lu
**********************************************************/
#include "bap_key.h"
#include "delay.h"

u8 key_data[4][4]={{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,16}};//��ֵ
__IO u8 Keyboard_Val =0;

/**********************************************************
 * ������ : key_init
 * ����   ��none
 * ���   ��none 
 * ����   ��������ʼ��
 **********************************************************/
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE); // IO��ʱ��ʹ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_10 | GPIO_Pin_11;  //��ʼ������IO��
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //IO ������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //IO ����ģʽ ������� ɨ�谴��
	GPIO_Init(GPIOB, &GPIO_InitStructure);             //��ʼ��GPIOB
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;          //��ʼ������IO��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;      //IO ����ģʽ ��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);             //��ʼ��GPIOA
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;          //��ʼ������IO��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;      //IO ����ģʽ ��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);             //��ʼ��GPIOB
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;          //��ʼ������IO��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;      //IO ����ģʽ ��������
	GPIO_Init(GPIOC, &GPIO_InitStructure);             //��ʼ��GPIOC
}
/**********************************************************
 * ������ : KEY_Interrupt
 * ����   ��none
 * ���   ��none 
 * ����   ��LED�ӿڳ�ʼ��
 **********************************************************/
void KEY_Interrupt(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��(ʹ���жϺ�����ӳ��ʱҪʹ�ܸ��ù���ʱ��)

	KEY_Init();//��ʼ������IO

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0);
	EXTI_InitStructure.EXTI_Line= EXTI_Line0;	
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource5);
	EXTI_InitStructure.EXTI_Line= EXTI_Line5;	
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource4);
	EXTI_InitStructure.EXTI_Line= EXTI_Line4;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	
	
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource4);
	EXTI_InitStructure.EXTI_Line= EXTI_Line4;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;//             		
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); 

	NVIC_InitStructure.NVIC_IRQChannel =EXTI9_5_IRQn;//             		
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); 	
	
	NVIC_InitStructure.NVIC_IRQChannel =  EXTI0_IRQn;//               		
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);
	KEY_Control(0);
}
/**********************************************************
 * ������ : KEY_Control
 * ����   ��ɨ��ģʽ
 * ���   ��none 
 * ����   ��ɨ�谴�����Ƴ���
 **********************************************************/
void KEY_Control(u8 i)
{
	switch(i)
	{
		case 0:
		GPIO_SetBits(GPIOB,GPIO_Pin_11);
		GPIO_SetBits(GPIOB,GPIO_Pin_10);
		GPIO_SetBits(GPIOB,GPIO_Pin_2);
		GPIO_SetBits(GPIOB,GPIO_Pin_1);
		break;
		case 1:
		GPIO_SetBits(GPIOB,GPIO_Pin_11);
		GPIO_ResetBits(GPIOB,GPIO_Pin_10);
		GPIO_ResetBits(GPIOB,GPIO_Pin_1);
		GPIO_ResetBits(GPIOB,GPIO_Pin_2);
		break;
	  case 2:
		GPIO_SetBits(GPIOB,GPIO_Pin_10);
		GPIO_ResetBits(GPIOB,GPIO_Pin_11);
		GPIO_ResetBits(GPIOB,GPIO_Pin_1);
		GPIO_ResetBits(GPIOB,GPIO_Pin_2);
		break;
		case 3:
		GPIO_SetBits(GPIOB,GPIO_Pin_2);
		GPIO_ResetBits(GPIOB,GPIO_Pin_11);
		GPIO_ResetBits(GPIOB,GPIO_Pin_1);
		GPIO_ResetBits(GPIOB,GPIO_Pin_10);
    break;	
		case 4:
		GPIO_SetBits(GPIOB,GPIO_Pin_1);
		GPIO_ResetBits(GPIOB,GPIO_Pin_11);
		GPIO_ResetBits(GPIOB,GPIO_Pin_10);
		GPIO_ResetBits(GPIOB,GPIO_Pin_2);
    break;
		default:break;		
	}
}
//�ⲿ�ж�0������� 
void EXTI0_IRQHandler(void)
{
	u8 i=0;
	delay_ms(10);//����
	printf("�����ж�0");
	for(i=1;i<4;i++)//��������ɨ��
	{
		KEY_Control(i);
		
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)==1)//��һ��ɨ��
		{
			Keyboard_Val=key_data[i-1][0];//���ض�Ӧ��ֵ
		}
	}
	KEY_Control(0);
	EXTI_ClearITPendingBit(EXTI_Line0); //���LINE0�ϵ��жϱ�־λ  
}
 
//�ⲿ�ж�2�������
void EXTI9_5_IRQHandler(void)
{
	u8 i=0;
	delay_ms(10);//����
	printf("�����ж�5");
	for(i=1;i<4;i++)
	{
		KEY_Control(i);
		
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)==1)//�ڶ���ɨ��
		{
			Keyboard_Val=key_data[i-1][1];
		}
	}
	KEY_Control(0);
	EXTI_ClearITPendingBit(EXTI_Line5);  //���LINE5�ϵ��жϱ�־λ  
}

void EXTI4_IRQHandler(void)
{
	u8 i=0;
	delay_ms(10);//����
	printf("�����ж�4");
	for(i=1;i<4;i++)
	{
		KEY_Control(i);
		
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4)==1)//������ɨ��
		{
			Keyboard_Val=key_data[i-1][2];
		}
		else if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==1)//������ɨ��
		{
			Keyboard_Val=key_data[i-1][3];
		}
	}	
	KEY_Control(0);//�ȴ���������
	EXTI_ClearITPendingBit(EXTI_Line4);  //���LINE4�ϵ��жϱ�־λ  
}

/**********************************************************
 * ������ : KEY_Read
 * ����   ��none
 * ���   ��none 
 * ����   �������������
 **********************************************************/
uint8_t KEY_Read(void)
{
	u8 i=0;
	static u8 key_stay_flag=0;
	u8 Keyboard_Val =0;
	
	for(i=1;i<4;i++)
	{
		KEY_Control(i);
		
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)==1&&(key_stay_flag==0))
		{
			key_stay_flag=1;
			Keyboard_Val=key_data[i][0];
		}
		else if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)==1&&(key_stay_flag==0))
		{
			key_stay_flag=1;
			Keyboard_Val=key_data[i][1];
		}
		else if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4)==1&&(key_stay_flag==0))
		{
			key_stay_flag=1;
			Keyboard_Val=key_data[i][2];
		}
		else if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==1&&(key_stay_flag==0))
		{
			key_stay_flag=1;
			Keyboard_Val=key_data[i][3];
		}
		key_stay_flag=0;
  }
	return Keyboard_Val;
}


