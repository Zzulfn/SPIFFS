/**
  ******************************************************************************
  * @file    bsp_74h595.c
  * @author  Jack Lu
  * @version V1.0
  * @date    2017-12-28
  * @brief   
  ******************************************************************************
	*/

#include "pro_logic.h"
SOFT_O_Module_Input iomanage;
/**********************************************************
 * ������ : OR_LogicModule_Manage
 * ����   ��input1 �ź�1����  input2 �ź�2����
 * ���   �������� 
 * ����   ���߼�������
 **********************************************************/
uint8_t OR_LogicModule_Manage(uint8_t input1,uint8_t input2)
{	
	return input1||input2;
}

/**********************************************************
 * ������ : AND_LogicModule_Manage
 * ����   ��input1 �ź�1����  input2 �ź�2����
 * ���   �������� 
 * ����   ���߼�������
 **********************************************************/
uint8_t AND_LogicModule_Manage(uint8_t input1,uint8_t input2)
{	
	return input1&&input2;
}

/**********************************************************
 * ������ : NOT_LogicModule_Manage
 * ����   ��input1 �ź�1����
 * ���   �������� 
 * ����   ���߼�������
 **********************************************************/
uint8_t NOT_LogicModule_Manage(uint8_t input1)
{	
	return !input1;
}

/**********************************************************
 * ������ : IO_Output_Manage
 * ����   ��none
 * ���   ��none 
 * ����   �����������ģ��
 **********************************************************/

SOFT_IO_Module_Output IO_Output_Manage(uint8_t IO_Channel_Sel,SOFT_O_Module_Input value)
{
	static u8 hc595_test[4]={0,0,0,0};
	u8 io_temp=0x01;
	SOFT_IO_Module_Output temp;
		
	if(value.IO_Channel_En==1)
	{
		io_temp<<=(IO_Channel_Sel-IO_Channel_Sel/8*8);//ѡ���Ӧͨ��
		if(value.IO_Channel_status==1)hc595_test[IO_Channel_Sel/8]|=io_temp;
		else hc595_test[IO_Channel_Sel/8]&=~io_temp;
		H595_WriteData(hc595_test);//
		H595_OutputEn();
	}
	temp.SOFT_IO_Module_status=value.IO_Channel_En;
	temp.IO_Channel_status=0;
	return temp;
}

/**********************************************************
 * ������ : IO_Manage
 * ����   ��none
 * ���   ��none 
 * ����   �����������ģ��
 **********************************************************/
SOFT_IO_Module_Output IO_Input_Manage(uint8_t IO_Channel_Sel,SOFT_I_Module_Input value)
{
	static u8 hc165_data[6];
	u8 io_temp=0x01;
	
	SOFT_IO_Module_Output temp;
	
	H165_ReadData(6,hc165_data);
	if(value.IO_Channel_En==1)
	{
		io_temp<<=(IO_Channel_Sel-IO_Channel_Sel/8*8);
		io_temp=~io_temp;	
		if(((hc165_data[IO_Channel_Sel/8])|io_temp)==io_temp)//�ж������Ƿ�Ϊ��
		{
			temp.IO_Channel_status=1;
		}
		else 
		{
			temp.IO_Channel_status=0;
		}
	}
	else temp.IO_Channel_status=0;//�ر����ģ�����0
	temp.SOFT_IO_Module_status=value.IO_Channel_En;
	
	return temp;
}



