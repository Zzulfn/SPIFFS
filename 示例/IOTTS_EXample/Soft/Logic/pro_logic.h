#ifndef __PRO_LOGIC_H
#define __PRO_LOGIC_H

#include "stm32f10x.h"
#include "stdio.h"
#include "sys.h"

#include "bsp_74h595.h"
#include "bsp_74h165.h"


/*
 *���������ģ����������ṹ��
 */
typedef struct
{
	uint16_t  com_addr[100];       /*!<   �����׵�ַ*/
	
  uint16_t  com_count_num;           /*!<   �������� */
	
}LOGIC_PLC;
/*
 *���������ģ����������ṹ��
 */
typedef struct
{
	uint16_t  com_addr[100];       /*!<   �����׵�ַ*/
	
  uint16_t  com_count_num;           /*!<   �������� */
	
}COMMAND_PLC;

/*
 *���������ģ����������ṹ��
 */
typedef struct
{
	uint8_t  IO_Channel_status;       /*!<   ģ�����״̬���� 1 ����� 0 �����*/
	
  uint8_t  IO_Channel_En;           /*!<   ģ�鹤��ʹ�� 1 ʹ�� 0 �ر� */
	
}SOFT_O_Module_Input;

/*
 *����������ģ����������ṹ��
 */
typedef struct
{
  uint8_t  IO_Channel_En;           /*!<   ģ�鹤��ʹ�� 1 ʹ�� 0 �ر� */
	
}SOFT_I_Module_Input;

/*
 *������ģ����������ṹ��
 */
typedef struct
{
	uint8_t  IO_Channel_status;       /*!<   ģ�����״̬ 1 �ߵ�ƽ 0 �͵�ƽ*/
	
  uint8_t  SOFT_IO_Module_status;   /*!<   ģ�鹤��״̬ 1 �������� 0 ģ��ر�*/          
     
}SOFT_IO_Module_Output;

typedef struct
{
  uint8_t GPIO_Pin_in;             

  uint8_t GPIO_Pin_out;            

  uint8_t GPIO_Pin_adc;      
	
	uint8_t GPIO_Pin_dac;
	
}LOGIC_Parameter;




SOFT_IO_Module_Output IO_Output_Manage(uint8_t  IO_Channel_Sel,SOFT_O_Module_Input value);
SOFT_IO_Module_Output IO_Input_Manage(uint8_t IO_Channel_Sel,SOFT_I_Module_Input value);

uint8_t OR_LogicModule_Manage(uint8_t input1,uint8_t input2);
uint8_t AND_LogicModule_Manage(uint8_t input1,uint8_t input2);
uint8_t NOT_LogicModule_Manage(uint8_t input1);

extern SOFT_O_Module_Input iomanage;

#endif




