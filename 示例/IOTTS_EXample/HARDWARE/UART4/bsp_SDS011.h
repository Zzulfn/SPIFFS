#include "stm32f10x.h"

#define SDS011_RX_GPIO_PIN                    GPIO_Pin_11
#define SDS011_RX_GPIO_PORT                   GPIOC
#define SDS011_RX_GPIO_CLK                    RCC_APB2Periph_GPIOC 
#ifdef AIR_OFFICE
#define SDS011_POWER_GPIO_PIN                  GPIO_Pin_10//GPIO_Pin_4
#define SDS011_POWER_GPIO_PORT                 GPIOC//GPIOA        
#define SDS011_POWER_GPIO_CLK                  RCC_APB2Periph_GPIOC//RCC_APB2Periph_GPIOA
#else
#define SDS011_POWER_GPIO_PIN                  GPIO_Pin_1//GPIO_Pin_4
#define SDS011_POWER_GPIO_PORT                 GPIOB//GPIOA        
#define SDS011_POWER_GPIO_CLK                  RCC_APB2Periph_GPIOB//RCC_APB2Periph_GPIOA
#endif
#define SDS011_USART_PORT                  		UART4
#define SDS011_USART_GPIO_CLK                 RCC_APB1Periph_UART4
#define SDS011_USART_BAUDRATE                 9600
#define SDS011_USART_IRQn                  		UART4_IRQn
#define SDS011_USART_IRQHandler								UART4_IRQHandler

//带参宏，可以像内联函数一样使用,输出高电平或低电平
#define SDS011_POWER_OUT(a)	if (a)	\
					GPIO_SetBits(SDS011_POWER_GPIO_PORT,SDS011_POWER_GPIO_PIN);\
					else		\
					GPIO_ResetBits(SDS011_POWER_GPIO_PORT,SDS011_POWER_GPIO_PIN)

typedef struct                  
{
	uint16_t			PM25;
	uint16_t			PM10;
}typ_SDS011;			

void SDS011_Init(void);
typ_SDS011 SDS011_Get(void);

