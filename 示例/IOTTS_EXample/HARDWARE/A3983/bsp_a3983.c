/**
  ******************************************************************************
  * @file    bsp_a3983.c
  * @author  Jack Lu
  * @version V1.0
  * @date    2017-10-25
  * @brief   �����������
  ******************************************************************************
  */
#include "bsp_a3983.h"

static u8 SetMotorEnableReg;
u8 key_flag=0;

u8 MILK_Check_Flash=0;//�����ɱ�־

static volatile uint16_t Step1_counter=0;
static volatile uint16_t Step2_counter=0;

static  MOTOR_STATUS motormode=MOTORVTOZ;//����ϵ����Z��У׼ģʽ
static  u8 Device_Start_First_Flag=0;//�豸��һ��������־

static MotorSelect motor_se=MOTOR_VERTICAL;//�ϵ����Z����

/**********************************************************
 * ������ : A3983_Init
 * ����   ��none 
 * ���   ��none 
 * ����   ������������ƽų�ʼ��
 **********************************************************/
void A3983_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure; 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO,ENABLE);
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	/*
	��ʼ���õ��ת��Ϊ˳ʱ��
	*/
  MOTOR_VERTICAL_DIR_0;
	MOTOR_ROTARY_DIR_0;
	/*
	�������ʹ�ܿ��� �͵�ƽ��Ч
	*/
	MOTOR_ROTARY_EN_0;
	MOTOR_VERTICAL_EN_0;
	
	//��ʱ����ʼ��
	TIM_DeInit(TIM4);
	TIM_TimeBaseStruct.TIM_Period=MOTOR_FRE-1;//��ֵ
	TIM_TimeBaseStruct.TIM_Prescaler=MOTOR_SPEED-1;//Ԥ��Ƶ
	TIM_TimeBaseStruct.TIM_ClockDivision=0;
	TIM_TimeBaseStruct.TIM_CounterMode=TIM_CounterMode_Up;//���ϼ���
  TIM_TimeBaseStruct.TIM_RepetitionCounter = 0x0;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStruct);
	
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	TIM_ITConfig( TIM4, TIM_IT_Update | TIM_IT_Trigger, ENABLE);
	
	//pwm ��ʼ��
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	
	TIM_OCInitStructure.TIM_Pulse = MOTOR_FRE; 
	TIM_OC3Init(TIM4,&TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);
  
	TIM_OCInitStructure.TIM_Pulse = MOTOR_FRE; 
	TIM_OC4Init(TIM4,&TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM4,ENABLE); 

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 
}

/**********************************************************
 * ������ : TIM4_IRQHandler
 * ����   ��none
 * ���   ��none 
 * ����   ��
 **********************************************************/
void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ 
		if(Step1_counter>0)//����Z������ת�Ƕ�
		{
			Step1_counter--;
		}
		if(Step2_counter>0)//����y������ת�Ƕ�
		{
			Step2_counter--;
		}	
	}
}

/*---------------------------------------�������-----------------------------------------------*/

/**********************************************************
 * ������ : SetPwmFre
 * ����   ��Fre �����õ��PWMƵ��
 * ���   ��none  
 * ����   �����õ��PWMƵ��
 **********************************************************/
void SetPwmFre(u16 Fre , MotorSelect MotorSelect_value)
{	
	if(MOTOR_VERTICAL==MotorSelect_value)TIM4->CCR3=Fre/2;
			else TIM4->CCR4=Fre/2;//open the rotary motor
}

/**********************************************************
 * ������ : SetMotorWork
 * ����   ��speed �����ת�� dir :���ת�� mic �������ֵ EN�����ʹ��
 * ���   ��none 
 * ����   �����Ƶ������
 **********************************************************/
void SetMotorWork(u16 speed, MotorDir dir, u8 mic, MotorState EN, MotorSelect MotorSelect_value)
{
	SetPwmFre(speed,MotorSelect_value);

	//CtrMicrostep(mic);
	
	CtrMotorDir(dir,MotorSelect_value);

	CtrMotorRun(EN,MotorSelect_value);

}

/**********************************************************
 * ������ : CtrMotorDir
 * ����   ��value : MOTOR_CLOCK_WISE ���˳ʱ�� MOTOR_COUNTER_CLOCK_WISE �����ʱ��
 * ���   ��none 
 * ����   ���������ת����
 **********************************************************/
void CtrMotorDir(MotorDir value , MotorSelect MotorSelect_value)
{	
	if(MOTOR_COUNTER_CLOCKWISE == value)
	{
		if(MOTOR_VERTICAL==MotorSelect_value)MOTOR_VERTICAL_DIR_1;
			else MOTOR_ROTARY_DIR_1;
	}
	else
	{
		if(MOTOR_VERTICAL==MotorSelect_value)MOTOR_VERTICAL_DIR_0;
			else MOTOR_ROTARY_DIR_0;
	}
}
/*
void CtrMicrostep(unsigned char value)
{

	if(Microstep != value)
	{
		Microstep = value;
		if(value == Microstep_1)
		{
			IO_MS1_0;
			IO_MS2_0;
		}
		else if(value == Microstep_2)
		{
			IO_MS1_1;
			IO_MS2_0;
		}
		else if(value == Microstep_4)
		{
			IO_MS1_0;
			IO_MS2_1;
		}
		else if(value == Microstep_16)
		{
			IO_MS1_1;
			IO_MS2_1;
		}
	}
}
*/

/**********************************************************
 * ������ : CtrMotorRun
 * ����   ��value : MOTOR_ON ������� MOTOR_OFF ���ֹͣ
 * ���   ��none 
 * ����   �����Ƶ�������Ƿ���
 **********************************************************/
void CtrMotorRun(MotorState value, MotorSelect MotorSelect_value)
{
	if(SetMotorEnableReg != value)
	{
		SetMotorEnableReg = value;
		if( value==MOTOR_ON )
		{
			if(MOTOR_VERTICAL==MotorSelect_value)MOTOR_VERTICAL_EN_0;//open the vertical motor
			else MOTOR_ROTARY_EN_0;//open the rotary motor
		}
		else
		{
			if(MOTOR_VERTICAL==MotorSelect_value)MOTOR_VERTICAL_EN_1;//close the vertical motor
			else MOTOR_ROTARY_EN_1;//close the rotary motor
		}
	}
}

/**********************************************************
 * ������ : CtrMotorBrake
 * ����   ��value : MOTOR_ON ������� MOTOR_OFF ���ֹͣ
 * ���   ��none 
 * ����   �����ɲ������
 **********************************************************/
void CtrMotorBrake(MotorState value, MotorSelect MotorSelect_value)
{
	if(motor_se!=MotorSelect_value)motor_se=MotorSelect_value;
	switch(motor_se)
		{
		case MOTOR_VERTICAL : 
			if( value==MOTOR_OFF )TIM4->CCR3=MOTOR_FRE;
		  else TIM4->CCR3=MOTOR_FRE/2;
			break;
		case MOTOR_ROTARY	:
			if( value==MOTOR_OFF )TIM4->CCR4=MOTOR_FRE;
		  else TIM4->CCR4=MOTOR_FRE/2;
			break;
		default : break;
		}
}

/**********************************************************
 * ������ : CtrMotorStepControl
 * ����   ��value : �����ת�Ƕ�
 * ���   ��none 
 * ����   �������ת�Ƕ�����
 **********************************************************/
void CtrMotorStepControl(u16 value, MotorSelect MotorSelect_value)
{
	if(motor_se!=MotorSelect_value)motor_se=MotorSelect_value;
	switch(motor_se)
		{
		case MOTOR_VERTICAL :
			Step1_counter=value;   //װ�벽��ֵ
			TIM4->CCR3=MOTOR_FRE/2;//�ɿ�ɲ��
		  while(Step1_counter>0) //�ȴ�����������
				vTaskDelay(1);
			TIM4->CCR3=MOTOR_FRE;  //ɲ��
			break;
		case MOTOR_ROTARY	:
			Step2_counter=value;   //װ�벽��ֵ 
			TIM4->CCR4=MOTOR_FRE/2;//�ɿ�ɲ��
		  while(Step2_counter>0) //�ȴ�����������
				vTaskDelay(1);
			TIM4->CCR4=MOTOR_FRE;  //ɲ��
			break;
		default : break;
		}
}

/*-----------------------------------------------------------------------------------------------*/

/**********************************************************
 * ������ : MOTORVTOZ_manage
 * ����   ��none
 * ���   ��none 
 * ����   �������������������������У׼һ�Σ�ÿ�β�����ɶ���У׼
 **********************************************************/
uint8_t MOTORVTOZ_manage(void)
{
	if(Motor_Location.PS_Vertical_State!=MOTOR_Sensor_ON)//�������������ڵ�
	{
		CtrMotorDir(MOTOR_COUNTER_CLOCKWISE , MOTOR_VERTICAL);//���õ����ʱ����ת
		CtrMotorBrake(MOTOR_ON, MOTOR_VERTICAL);//�رյ��ɲ��
	}
	else if(Motor_Location.PS_Vertical_State==MOTOR_Sensor_ON)//������λ���������ڵ�
	{
		CtrMotorBrake(MOTOR_OFF, MOTOR_VERTICAL);//���ɲ��
		return 0;
	}
	return 1;
}

/**********************************************************
 * ������ : MOTORRTOZ_manage
 * ����   ��none
 * ���   ��none 
 * ����   �����ת�̹����������
 **********************************************************/
uint8_t MOTORRTOZ_manage(void)
{
	if((Motor_Location.PS_Rotary_State!=MOTOR_Sensor_ON)&&(Motor_Location.PS_Vertical_State==MOTOR_Sensor_ON))//�������������ڵ�
	{
		CtrMotorDir(MOTOR_CLOCKWISE , MOTOR_ROTARY);//���õ��˳ʱ����ת
		CtrMotorBrake(MOTOR_ON, MOTOR_ROTARY);//�رյ��ɲ��
	}
	else if(Motor_Location.PS_Vertical_State!=MOTOR_Sensor_ON)//�������ڵ�
	{
	  motormode=MOTORVTOZ;   // ����δ��λ���������λ����
	}
	else if(Motor_Location.PS_Rotary_State==MOTOR_Sensor_ON)//�������ڵ�
	{
		CtrMotorBrake(MOTOR_OFF, MOTOR_ROTARY);
		return 0;// ת���ѹ�λ��������һ��
	}
	return 1;
}

/**********************************************************
 * ������ : MOTORDOWN_manage
 * ����   ��none
 * ���   ��none 
 * ����   ����������½���������
 **********************************************************/
uint8_t MOTORDOWN_manage(void)
{
	if(Motor_Location.PS_Vertical_State==MOTOR_Sensor_ON)//����������������ʱ�������½�����
	{
		CtrMotorDir(MOTOR_CLOCKWISE , MOTOR_VERTICAL);//���õ����ʱ����ת
		CtrMotorStepControl(MOTOR_VERTICAL_DIS, MOTOR_VERTICAL);//z���½�׼������
	}
	return 0;	
}

/**********************************************************
 * ������ : MOTORWAIT_manage
 * ����   ��none
 * ���   ��none 
 * ����   ���ȴ�������
 **********************************************************/

uint8_t MOTORWAIT_manage(void)
{
	u16 i=0;
//	u8 i=0;
//  if((usart_re_flag==4)||(lcd_dis.menue_flag==59))//�����ɱ�־
//	{
//		lcd_dis.menue_flag=100;
//		usart_re_flag=0;
//		MOTORVTOZ_manage();
////		for(i=0;i<TUBE_SUM;i++)
////		{
////			
////			vTaskDelay(5);
////		}
//		key_value=MILK_SET;
//	}
//	else if(key_flag==0)//��ֹ��δ���
//	{
//		key_flag=1;
//		KEY_Manage(KEY_CONFIRM);
//	}
	while(Check_ONE_Finash_Flag==0)
	{
		vTaskDelay(10);
		if(i>9000)break;
		i++;
//		if(i%100==0)
//		DEBUG_Msg("i=%d",i/100);
	}
	vTaskDelay(5000);
	Check_ONE_Finash_Flag=0;
//vTaskDelay(50000);
//	MILK_Check_Flash=0xF1;
	return 0;
}


/**********************************************************
 * ������ : MOTORSTOP_manage
 * ����   ��none
 * ���   ��none 
 * ����   �����ͣת�������� ������ʱʹ��
 **********************************************************/
uint8_t MOTORSTOP_manage(void)
{
	
	return 0;
}

/**********************************************************
 * ������ : MOTORMANAGE_CLOCKWISE_manage
 * ����   ��none
 * ���   ��none 
 * ����   ������������
 **********************************************************/
uint8_t MOTORMANAGE_CLOCKWISE_manage(void)
{
	CtrMotorDir(MOTOR_CLOCKWISE , MOTOR_ROTARY);//���õ��˳ʱ����ת
	CtrMotorStepControl(MOTOR_ROTARY_ANGLE_ONETIME, MOTOR_ROTARY);//��תһ���Թ�λ��
	return 0;
}

/**********************************************************
 * ������ : MOTORMANAGE_COUNTER_CLOCKWISE_manage
 * ����   ��none
 * ���   ��none 
 * ����   ������������
 **********************************************************/
uint8_t MOTORMANAGE_COUNTER_CLOCKWISE_manage(void)
{
	CtrMotorDir(MOTOR_COUNTER_CLOCKWISE , MOTOR_ROTARY);//���õ��˳ʱ����ת
	CtrMotorStepControl(MOTOR_ROTARY_ANGLE_ONETIME, MOTOR_ROTARY);//��תһ���Թ�λ��
	return 0;
}

/**********************************************************
 * ������ : MotorExec
 * ����   ��none
 * ���   ��none 
 * ����   ������������� 
 **********************************************************/

//void MotorExec(void)
//{
//	u8 i=0;
//	static u8 counter=0;
//	TUBE_STATUS Flash_Data_read;
//	/*
//	��������
//	*/
////	Tube_Status_Flash.Tube_sum_one=3;//װ��8���Թ�
//	/******************/
//	switch (motormode)
//   {
//    case MOTORVTOZ:     //�����λ
//		 if((MOTORVTOZ_manage()==0)&&(Device_Start_First_Flag==0))//�ж��Ƿ����״�����
//		 {
//			 Device_Start_First_Flag=1;
//			 motormode=MOTORRTOZ;
//		 }
//		 else if(MOTORVTOZ_manage()==0)
//		 {
//			motormode=MOTORRETREAT;
//			/*
//			�ж��Ƿ������Թܼ�����
//			*/
//			if(Tube_Status_Flash.Tube_num>=Tube_Status_Flash.Tube_sum_one)
//			{
//				Tube_Status_Flash.Tube_num=0;
//				/*
//				��������ʾ
//				*/
//				MILK_Check_Flash=0xFE;
//				DEBUG_Msg("������");
//				ScreenCommand(0,0,0);
//				screenControl=WAIT;
//				Tube_Status_Flash.Tube_ins_sts=0;//�л��豸״̬
//				for(i=0;i<24;i++)
//				Tube_Status_Flash.Tube_sts[i]=0;
//				motormode=MOTORRTOZ;
//			}

//		 }
//        break; 
//    case MOTORRTOZ:     //ת�̹�λ
//				if(MOTORRTOZ_manage()==0)
//					motormode=MOTORMANAGE;
//        break;
//    case MOTORMANAGE:   //�����Թܸ�������ת�Ƕ�
//				if(screenControl==FINISH_CHECK_CONFIRM)MILK_Check_Flash=0x01;
//				if((screenControl==INS_TUBE_COUNTER_CLOCKWISE)||(screenControl==INS_TUBE_CLOCKWISE)||(screenControl==TAKE_DOWN_TUBE))//�յ���װ�Թ����ȡ���Թܲ���
//				{
//					if(screenControl==INS_TUBE_COUNTER_CLOCKWISE)
//					{
//						if(Tube_Status_Flash.Tube_sum_one<24)
//							{
//								/*��ǰ���ڰ�װ�Թ�*/
//								Tube_Status_Flash.Tube_ins_sts=0;
//								/*�Թ�������1 ͳ���Թ�����*/
//								Tube_Status_Flash.Tube_sum_one+=3;
//								/*���Թ������浽Flash��*/
//								for(i=0;i<3;i++)
//								/*��װ�Թ�ʱ��תһ���Թ�λ��*/
//								MOTORMANAGE_COUNTER_CLOCKWISE_manage();
//							}
//					}
//					else 
//						if(screenControl==INS_TUBE_CLOCKWISE)
//						{
//							if(Tube_Status_Flash.Tube_sum_one!=0)
//							{
//								/*��ǰ���ڰ�װ�Թ�*/
//								Tube_Status_Flash.Tube_ins_sts=0;
//								/*�Թ�������1 ͳ���Թ�����*/
//								Tube_Status_Flash.Tube_sum_one-=3;
//								
//								for(i=0;i<3;i++)
//								/*��װ�Թ�ʱ��תһ���Թ�λ��*/
//								MOTORMANAGE_CLOCKWISE_manage();
//							}
//						}
//					 else 
//						 if(screenControl==TAKE_DOWN_TUBE)
//							{
//								for(i=0;i<3;i++)
//								/*��װ�Թ�ʱ��תһ���Թ�λ��*/
//								MOTORMANAGE_CLOCKWISE_manage();
//							}
//					/*
//					�����Ļ��ʾ����
//					*/
//							
//					/***************/
////					/*���Թ������浽Flash��*/
////					SPI_Flash_Save();		
//					screenControl=WAIT;
//					/*���Թ������浽Flash��*/
//					SPI_Flash_Save();		
//					DEBUG_Msg("�Թ�����:%d", Tube_Status_Flash.Tube_sum_one);		
//				}
//				else if((screenControl==MODE_ONE_START_CHECK)||(screenControl==MODE_TWO_START_CHECK)||(screenControl==MODE_THREE_START_CHECK))//�յ���ʼ�������
//				{
////					screenControl=WAIT;
//					if(Tube_Status_Flash.Tube_ins_sts==0)//��װ��ɽ�����ǰ׼��
//					{
//						/*��ǰ���ڼ��*/
//						Tube_Status_Flash.Tube_ins_sts=1;//�л��豸״̬
//						/*
//						ת�̹���
//						*/
//						while(MOTORRTOZ_manage()!=0)vTaskDelay(10);
//						/*
//						��ת����⴫����λ��
//						*/
//						for(i=0;i<9;i++)//��ת����⴫����λ��
//						{
//							MOTORMANAGE_COUNTER_CLOCKWISE_manage();
//						}
//					}	
//					if(Tube_Status_Flash.Tube_num<Tube_Status_Flash.Tube_sum_one)//�жϵ�ǰ�Թ������Ƿ񳬹�����
//						{
//							if(Motor_Location.PS_X_State==MOTOR_Sensor_ON)//��������Թܴ������жϵ�ǰ״̬
//								{
//									Tube_Status_Flash.Tube_num++;
//									/*����Թܴ������,�жϵ�ǰ�Ƿ����Թܼ�����Թ�����ͳ��*/
//									Tube_Status_Flash.Tube_sts[Tube_Status_Flash.Tube_num]|=0x10;//��⵽�Թ�
//									GPIOD->BSRR=GPIO_Pin_5;
//									/*�������ģʽ����תһ���Թ�λ��*/
//									MOTORMANAGE_COUNTER_CLOCKWISE_manage();
//									vTaskDelay(200);	
//									for(i=0;i<2;i++)
//									{
//									 /*��������ɨ��ǹ*/
//										GPIOD->BSRR=GPIO_Pin_5;
//										vTaskDelay(1000);
//										GPIOD->BRR=GPIO_Pin_5;
//										/*--------------------*/							
//										vTaskDelay(2000);//��ʱ
//										
//										/*���ɨ���Ƿ�ɹ�*/
//										if((Tube_Status_Flash.Tube_sts[Tube_Status_Flash.Tube_num]&0x0F)==0x0F)
//										{
//											/*�������ģʽ����תһ���Թ�λ��*/
//											for(i=0;i<2;i++)
//											{
//												MOTORMANAGE_COUNTER_CLOCKWISE_manage();/*!<��ʱ�Թ�λ���������·�*/
//												vTaskDelay(500);
//											}
//											/*������һ��ģʽ*/
//											motormode=MOTORDOWN;	
//											break;
//										} 
//									 }
//								/*���Թ������浽Flash��*/
//								SPI_Flash_Save();
//								if(screenControl==MODE_ONE_START_CHECK)
//								ScreenCommand(1,1,0);	
//								else if(screenControl==MODE_TWO_START_CHECK)
//								ScreenCommand(1,1,1);	
//								else if(screenControl==MODE_THREE_START_CHECK)
//								ScreenCommand(1,1,2);						
//									DEBUG_Msg("���ڼ���Թ�:%d,�Թ�����:%d", Tube_Status_Flash.Tube_num,Tube_Status_Flash.Tube_sum_one);
//								}
//							else 
//								{
//									/*
//									 ��������Թ�ͳ��
//									*/
//									/*----------------------------------------------------------------------------------*/
//									
//			//					  Tube_Status_Flash.Tube_sts[Tube_Status_Flash.Tube_num]&=0xEF;//û�м�⵽�Թ�
//									if(counter<23)
//									/*û�м�⵽�Թܼ�����ת*/
//									{
//										MOTORMANAGE_COUNTER_CLOCKWISE_manage();
//										counter++;
//										vTaskDelay(500);
//										
//									}
//									else 
//										{
//											counter=0;
//											Tube_Status_Flash.Tube_num=0;
//											/*
//											��������ʾ
//											*/
//											MILK_Check_Flash=0xFE;
//											DEBUG_Msg("������");
//											ScreenCommand(0,0,0);
//											screenControl=WAIT;
//											Tube_Status_Flash.Tube_ins_sts=0;//�л��豸״̬
//											for(i=0;i<24;i++)
//											Tube_Status_Flash.Tube_sts[i]=0;
//											motormode=MOTORRTOZ;
//										}
//								}
//						}
//						else  motormode=MOTORVTOZ;
//				}
//				/*
//				���ȡ������Ӧ
//				*/
//				else if((screenControl==MODE_ONE_CANCEL)||(screenControl==MODE_TWO_CANCEL)||(screenControl==MODE_THREE_CANCEL))
//							{
//								Tube_Status_Flash.Tube_sum_one=3;
//								motormode=MOTORRTOZ;
//								/*���Թ������浽Flash��*/
//								SPI_Flash_Save();
//							}
//				data_re_sc.SAMPLING_LOOP=Tube_Status_Flash.Tube_sum_one; 
//				ScreenCommand(2,1,0);//����ģʽ����ʾ����Թ����� 			
//        break;
//		case MOTORDOWN:            //�����������
//				MOTORDOWN_manage();
//		    motormode = MOTORWAIT; //������Һ���ģʽ
//        break;
//		case MOTORWAIT:         //�ȴ�������
////			  /*
////				�����ڲ��Ե��Թܵ�״̬����Flash��
////				*/
////				SPI_Flash_Save();
//		    MOTOR_ROTARY_EN_1;
//				MOTOR_VERTICAL_EN_1;
//		    KEY_Manage(KEY_CONFIRM);//���¿�ʼ����
//			  MOTORWAIT_manage(); //�ȴ�������
//		    motormode=MOTORVTOZ;//��������
//				MOTOR_ROTARY_EN_0;
//				MOTOR_VERTICAL_EN_0;
//        break;
//    case MOTORSTOP:         //����ر�
//				MOTORSTOP_manage();
//        break; 
//   	
//		case MOTORRETREAT:         //�������
//			for(i=0;i<2;i++)//���������Թ�λ��
//			{
//				MOTORMANAGE_CLOCKWISE_manage();
//				vTaskDelay(500);
//			}
//			motormode=MOTORMANAGE;
//			break; 	
//    default:
//        motormode = MOTORVTOZ;//�����λ
//        break;
//    }
//}
