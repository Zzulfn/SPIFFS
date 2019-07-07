#ifndef __UART4_H
#define	__UART4_H
#include "stm32f10x.h"


//带参宏，可以像内联函数一样使用,输出高电平或低电平
#define UART4_POWER_OUT(a)	if (a)	\
					GPIO_SetBits(UART4_POWER_GPIO_PORT,UART4_POWER_GPIO_PIN);\
					else		\
					GPIO_ResetBits(UART4_POWER_GPIO_PORT,UART4_POWER_GPIO_PIN)

void UART4_Init(u32 baudrate);

#endif /* __UART4_H */
