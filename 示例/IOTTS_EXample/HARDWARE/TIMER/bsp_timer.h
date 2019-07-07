#ifndef __BSP_TIMER_H
#define __BSP_TIMER_H

#include "stm32f10x.h"
#include "stdio.h"
#include "math.h"
//���index��Ӧ��ʱ�䳬ʱ�����㲢����1��δ��ʱ����0
#define IS_TIMEOUT_1MS(index, count)    ((g_Tim6Array[(u16)(index)] >= (count))? ((g_Tim6Array[(u16)(index)] = 0) == 0): 0) 
                                        
//���������ʱ������
#define SOFT_TIMERS_MAX 8  

/*PWM����ģ����������ṹ��*/
typedef struct
{
	uint8_t  PWM_InputChannel_En;      /*!<   ���ͨ��ʹ�� */ 
	
}SOFT_Module_PwmIn_Input;

/*
 * PWM����ģ����������ṹ��
 */
typedef struct
{
	uint8_t  SOFT_Module_PwmIn_Module_Status;     /*!<   ���ģ�鹤��״̬ */
	
  float  PWM_InputChannel_Fre;               /*!<   Ƶ�� */ 	
  
	float  PWM_InputChannel_Duty_Cycle;        /*!<   PWMռ�ձ� */
		
}SOFT_Module_PwmIn_Output;

/*  
 * PWM���ģ����������ṹ��
 */
typedef struct
{
  uint16_t  PWM_OutputChannelt_Fre;            /*!< ���Ƶ�� */
  
	uint16_t  PWM_OutputCounter;
	
	uint16_t  PWM_OutputNumCounter;
	
	float  PWM_OutputChannel_Duty_Cycle;     /*!< PWMռ�ձ� */ 	
	
	uint8_t  PWM_OutputChannel_En;             /*!< PWM���ģ��ʹ�� */ 	
    
}SOFT_Module_PwmOut_Input;

/*
 * PWM���ģ����������ṹ��
 */
typedef struct
{
	uint16_t  PWM_Count; /*!< ���� */
	uint8_t  SOFT_Module_PwmOut_Module_Status; /*!< ���ģ�鹤��״̬ */
		
}SOFT_Module_PwmOut_Output;

/*
 * ������ģ����������ṹ��
 */
typedef struct
{
	uint8_t  TIM_Channel_Sel;    /*!< ������ѡ�� */
	
  uint8_t  TIM_Channel_En;     /*!< ������ʹ�� */

  uint8_t  TIM_Channel_Trig;   /*!< ���������� */

  uint16_t TIM_Counter;        /*!< ����������ֵ */   

  uint16_t TIM_Cycle_Times;    /*!< ��������װ�ش��� 0 ������*/
	
}SOFT_Module_Tim_Input;

/*
 * ������ģ����������ṹ��
 */
typedef struct
{
  uint16_t  TIM_Counter;       /*!< ����������ֵ��ʾ��� */     

  uint8_t TIM_Out_status;      /*!< �����������״̬  */     

  uint8_t SOFT_Module_TIM_Module_Status;   /*!< ������ģ�鹤��״̬  */    
	
}SOFT_Module_Tim_Output;

enum {
		Tim6_0,
    Tim6_1,
    Tim6_2,
    Tim6_3,
    Tim6_4,
    Tim6_5,
		Tim6_6,
    Tim6_7,
};

typedef struct  {
	  const char * const timer_name;
    uint32_t time_counter;
    uint32_t time_out;                      //��ֵ
    void (*time_out_callback)(uint8_t TimerID); //�ص�����
    uint8_t TimerID;                     //��ʱ�����Ա�־
}soft_pwm_t;

/*
 * 
 */
typedef struct soft_timer {
	  const char * const timer_name;
    uint32_t time_counter;
    uint32_t time_out;                      //��ֵ
    void (*time_out_callback)(uint8_t TimerID); //�ص�����
    uint8_t TimerID;                     //��ʱ�����Ա�־
}soft_timer_t;


typedef void (*time_out_callback)(uint8_t TimerID);
typedef void * TimerHandle_t;

void Timerx_Init(u16 arr,u16 psc); 
void TIM5_PWM_Init(u16 arr,u16 psc);
void TIM4_PWM_Init(u16 arr,u16 psc);
void TIMx_Cap_Init(TIM_TypeDef* TIMx,u16 arr,u16 psc);
void TIM6_Config(u16 arr,u16 psc);
TimerHandle_t soft_timer_registered(const char * const timer_name,
                                 const uint32_t period, 
																 const uint32_t AutoReload,   
                                 uint8_t TimerID ,
                                 time_out_callback callback);
void soft_timer_exec(void);
void callbackfun(uint8_t TimerID);
SOFT_Module_PwmIn_Output PWM_InManage(u8 PWM_InputChannel_Sel,SOFT_Module_PwmIn_Input value);																 
SOFT_Module_PwmOut_Output PWM_OutManage(uint8_t  PWM_OutputChannel_Sel,SOFT_Module_PwmOut_Input value,u8 ret_data);
void PWM_Out_Init(void);
																 	    				
																	 
extern volatile int g_Tim6Array[(u16)Tim6_7];
#endif
