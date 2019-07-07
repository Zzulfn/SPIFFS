#ifndef __A_3983_H
#define __A_3983_H
#include "sys.h"

//#include "bspCOMMON.h"
#include "stm32f10x.h"
#define MOTOR_VERTICAL_EN_0  GPIO_ResetBits(GPIOD,GPIO_Pin_14)
#define MOTOR_VERTICAL_EN_1  GPIO_SetBits(GPIOD,GPIO_Pin_14)

#define MOTOR_ROTARY_EN_0  GPIO_ResetBits(GPIOD,GPIO_Pin_12)
#define MOTOR_ROTARY_EN_1  GPIO_SetBits(GPIOD,GPIO_Pin_12)

#define MOTOR_VERTICAL_DIR_0 GPIO_ResetBits(GPIOD,GPIO_Pin_15)
#define MOTOR_VERTICAL_DIR_1 GPIO_SetBits(GPIOD,GPIO_Pin_15)

#define MOTOR_ROTARY_DIR_0 GPIO_ResetBits(GPIOD,GPIO_Pin_13)
#define MOTOR_ROTARY_DIR_1 GPIO_SetBits(GPIOD,GPIO_Pin_13)

#define MOTOR_FRE                  720      //�������Ƶ��
#define MOTOR_SPEED                72       //��������ٶ�
#define MOTOR_VERTICAL_DIS         3300    //�����������˶�����
#define MOTOR_ROTARY_ANGLE_ONETIME 662    //ת�̵�����һ����Ʒת���ĽǶ�680
#define MOTOR_Sensor_ON            0        //���ᴫ����״̬
#define TUBE_SUM                   24       //�Թ�����

/*���״̬*/
typedef enum {MOTOR_ON = 0, MOTOR_OFF = !MOTOR_ON} MotorState;
/*���ת��*/
typedef enum {MOTOR_CLOCKWISE = 0, MOTOR_COUNTER_CLOCKWISE = !MOTOR_CLOCKWISE} MotorDir;
/*���ѡ��*/
typedef enum {MOTOR_VERTICAL = 0, MOTOR_ROTARY} MotorSelect;

/*�������*/
typedef enum {
    MOTORVTOZ,
	  MOTORSTOP,
    MOTORRTOZ,    
    MOTORMANAGE,
    MOTORDOWN,
	  MOTORWAIT,
    MOTORFromFlash,
	  MOTORRETREAT,
}MOTOR_STATUS;

void TIM6_Config(void);
void A3983_Init(void);
void CtrMotorRun(MotorState value, MotorSelect MotorSelect_value);
void SetMotorWork(u16 speed, MotorDir dir, u8 mic, MotorState EN, MotorSelect MotorSelect_value);
void CtrMotorDir(MotorDir value , MotorSelect MotorSelect_value);
void SetSpeed(void);
void MotorWork(void);
void MotorExec(void);
void CtrMotorBrake(MotorState value, MotorSelect MotorSelect_value);
uint8_t MOTORDOWN_manage(void);
uint8_t MOTORVTOZ_manage(void);
uint8_t MOTORRTOZ_manage(void);
uint8_t MOTORMANAGE_CLOCKWISE_manage(void);
uint8_t MOTORMANAGE_COUNTER_CLOCKWISE_manage(void);
void CtrMotorStepControl(u16 value, MotorSelect MotorSelect_value);

#endif
