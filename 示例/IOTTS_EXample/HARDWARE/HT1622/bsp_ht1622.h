#ifndef _HT1622_H_ 
#define _HT1622_H_  

#include "stm32f10x.h"

/*****************************************************************
�ܽŷ��� CS P1.0 WR P1.1 DAT P1.2
*****************************************************************/
#define HT1622_PORT_RD_SCK  GPIOB
#define HT1622_PORT_WR_SCK  GPIOB
#define HT1622_PORT_DAT     GPIOB
#define HT1622_PORT_CS      GPIOB
#define HT1622_PORT_LED     GPIOC

#define HT1622_RD_CLK       GPIO_Pin_13
#define HT1622_WR_CLK       GPIO_Pin_14
#define HT1622_DAT          GPIO_Pin_15
#define HT1622_CS           GPIO_Pin_12 
#define HT1622_LED          GPIO_Pin_15 

#define HT1622_RD_SCK_L		GPIO_ResetBits(HT1622_PORT_RD_SCK, HT1622_RD_CLK)
#define HT1622_RD_SCK_H		GPIO_SetBits(HT1622_PORT_RD_SCK, HT1622_RD_CLK)

#define HT1622_WR_SCK_L		GPIO_ResetBits(HT1622_PORT_WR_SCK, HT1622_WR_CLK)
#define HT1622_WR_SCK_H		GPIO_SetBits(HT1622_PORT_WR_SCK, HT1622_WR_CLK)

#define HT1622_DAT_L		  GPIO_ResetBits(HT1622_PORT_DAT, HT1622_DAT)
#define HT1622_DAT_H		  GPIO_SetBits(HT1622_PORT_DAT, HT1622_DAT)	

#define HT1622_CS_L		    GPIO_ResetBits(HT1622_PORT_CS, HT1622_CS)	
#define HT1622_CS_H		    GPIO_SetBits(HT1622_PORT_CS, HT1622_CS)


/*****************************************************************
HT1622 ָ��궨��
*****************************************************************/
#define SYSDIS   0X00 // ����ϵͳ������LCDƫѹ������
#define SYSEN    0X01 // ��ϵͳ����
#define LCDOFF   0X02 // ��LCDƫѹ
#define LCDON    0X03 // ��LCDƫѹ
#define TIMEDIS  0x04 // ��ֹʱ�����
#define WDTDIS   0x05 // ��ֹWDT��ʱ��־���
#define TIMEREN  0x08 // ����ʱ�����
#define WDTEN    0x0D // ����WDT��ʱ��־���
#define TONEOFF  0x0F // �ر��������
#define CLRTIMER 0x05 // ���ʱ��������������
#define CLRWDT   0x0F // ���WDT������
#define RC32K    0x18 // ϵͳʱ��Դ��Ƭ��RC����
#define EXT32K   0x1C // ϵͳʱ��Դ���ⲿʱ��Դ
#define TONE4K   0x40 // �������Ƶ�ʣ�4kHz��
#define TONE2K   0x60 // �������Ƶ�ʣ�2kHz��
#define IRQDIS   0x80 // ��ֹIRQ���
#define IRQEN    0x88 // ����IRQ���
#define F1       0xA0 // ʱ��ʱ�������1Hz WDT�ĳ�ʱ��־��4s
#define F2       0xA1 // ʱ��ʱ�������2Hz WDT�ĳ�ʱ��־��2s
#define F4       0xA2 // ʱ��ʱ�������4Hz WDT�ĳ�ʱ��־��1s
#define F8       0xA3 // ʱ��ʱ�������8Hz WDT�ĳ�ʱ��־��1/2s
#define F16      0xA4 // ʱ��ʱ�������16Hz WDT�ĳ�ʱ��־��1/4s
#define F32      0xA5 // ʱ��ʱ�������32Hz WDT�ĳ�ʱ��־��1/8s
#define F64      0xA6 // ʱ��ʱ�������64Hz WDT�ĳ�ʱ��־��1/16s
#define F128     0xA7 // ʱ��ʱ�������128Hz WDT�ĳ�ʱ��־��1/32s
#define TEST     0xE0 // ����ģʽ�£��û���ʹ�á�
#define NORMAL   0xE3 // ��ͨģʽ

void  Sendcmd(u8  command);
u8 W1622_Read_byte(u8 addr);
void W1622_READ_MODIFY_WRITE(u8 SEG,u8 dat);
void W1622_READ_MODIFY_WRITE_2(u8 SEG,u8 dat);
void W1622_READ_MODIFY_WRITE_3(u8 dat);

#endif
