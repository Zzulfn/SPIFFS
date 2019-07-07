#ifndef __BSP_USART1_H
#define __BSP_USART1_H
#include "sys.h"


#define RE_DATA_MAX 20


#define __DEBUG 	//¿ªÆô´®¿Úµ÷ÊÔ

#ifdef __DEBUG

#define DEBUG(format,...) printf("\nFILE: "__FILE__", LINE: %d: "format"\n", __LINE__, ##__VA_ARGS__)

#else

#define DEBUG(format,...)

#endif

void USART1_Init(u32 baudrate); 
//extern u8 commandDown;
//extern char usartWriteBuf[];
#endif
