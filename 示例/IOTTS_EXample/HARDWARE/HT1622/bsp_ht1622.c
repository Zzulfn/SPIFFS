/*****************************************************************
HMS17AN����Һ�������Գ��� Drive HT1622 CPU MSP430G2454 @ 1MHz

Һ�������ݣ� 8888 8888 8888 88  88 �ڶ���
              88   88   88         ��һ��

*****************************************************************/
#include "bsp_ht1622.h"
#include "delay.h"

unsigned char DIS_BUF[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18};//��ʾ������
//����� 0 1 2 3 4 5 6 7 8 9 A b C d E F - _ ��
const unsigned char DIS[]={0xBF,0x06,0x7C,0x5E,0xC6,0xDA,0xFA,0x0E,0xFE,0xDE,0xEE,0xF2,0xB8,0x76,0xF8,0xE8,0x40,0x10,0x00};

void HT1622_Init(void)
{
	u8  j = 0;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE); // IO��ʱ��ʹ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;  //��ʼ������IO��
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //IO ������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //IO ����ģʽ �������
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_15;  //��ʼ������IO��
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //IO ������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //IO ����ģʽ �������
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	for(j=0; j<32; j++)
	{
			DIS_BUF[j] = 0;
	}
	Sendcmd(RC32K);  //RC32k
	Sendcmd(SYSEN);  //��ϵͳ����
	Sendcmd(LCDON);  //�� LCDƫѹ������
}
void HT1622_R(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // IO��ʱ��ʹ��
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_15;  //��ʼ������IO��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   //IO ����ģʽ ��������
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
void HT1622_W(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // IO��ʱ��ʹ��
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_15;  //��ʼ������IO��
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //IO ������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //IO ����ģʽ �������
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
/**********************************************************
 * ������ : LCD_Wr_Clk
 * ����   ��none
 * ���   ��none 
 * ����   ��дʱ��
 **********************************************************/
void LCD_Wr_Clk(void) 
{ 
	HT1622_WR_SCK_L;
	delay_us(4);
	HT1622_WR_SCK_H;
	delay_us(4);
} 

/**********************************************************
 * ������ : LCD_Rd_Clk
 * ����   ��none
 * ���   ��none 
 * ����   ����ʱ��
 **********************************************************/
void LCD_Rd_Clk(void) 
{ 
	HT1622_RD_SCK_L;
	delay_us(4);
	HT1622_RD_SCK_H;
	delay_us(4);
} 
/**********************************************************
 * ������ : SendBit_HL
 * ����   ��dat ����/���� cnt ����λ
 * ���   ��none 
 * ����   ��dat �ĸ�cnt λд��HT1622���ȷ��͸�λ
 **********************************************************/
void SendBit_HL(u8 dat,u8 cnt) //
{
    u8 i;
    for(i=0; i<cnt;i++)
    {
			if(dat&0x80)
			{
					HT1622_DAT_H;
			}
			else
			{
					HT1622_DAT_L;
			}
			
			LCD_Wr_Clk();
			dat <<= 1;//��������1λ
    }
}
/**********************************************************
 * ������ : SendBit_LH
 * ����   ��dat ����/���� cnt ����λ
 * ���   ��none 
 * ����   ��dat �ĵ�cnt λд��HT1622���ȷ��͵�λ
 **********************************************************/
void  SendBit_LH(u8  dat,u8 cnt)
{
	u8 i;
	for(i=0; i<cnt;i++)
	{
			if(dat&0x01)
			{
					HT1622_DAT_H;
			}
			else
			{
					HT1622_DAT_L;
			}
			LCD_Wr_Clk();
			dat >>= 1;//��������1λ
	}
}
/**********************************************************
 * ������ : ReceiveBit_LH
 * ����   ��
 * ���   ��none 
 * ����   ����ȡcom��״̬
 **********************************************************/
u8  ReceiveBit_LH(void)
{
	u8 i;
	u8 j=0x01;
	u8 dat=0x00;
	HT1622_R();//���ó�����ģʽ��ȡcom��״̬
	
	for(i=0;i<4;i++)
	{   
		  LCD_Rd_Clk();
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==1)
			{
					dat|=j;
			}
			else
			{
					dat&=~j;
			}
			j<<=1;
	}
	return dat;
}

/**********************************************************
 * ������ : Sendcmd
 * ����   ��command ����
 * ���   ��none 
 * ����   ��ָ��ģʽ
 **********************************************************/
void  Sendcmd(u8  command)
{
    HT1622_CS_L;
    SendBit_HL(0x80,3);     //д���־��"100"
    SendBit_HL(command,9);  //д�� 9 λ����,����ǰ 8 λΪ command ����,��� 1 λ����
    HT1622_CS_H;
}
/**********************************************************
 * ������ : W1622_q
 * ����   ��addr ��ַ dat ����
 * ���   ��none 
 * ����   ��ָ��ģʽ
 **********************************************************/
void W1622_q(u8 addr,u8 dat)
{
    HT1622_CS_L;
    SendBit_HL(0xa0,3);    //д���־��"101"
    SendBit_HL(addr<<2,6); //д�� 6 λ addr
    SendBit_LH(dat,4);     //д���4λ����
    HT1622_CS_H;
}

/**********************************************************
 * ������ : W1622_byte
 * ����   ��SEG ��ַ dat ����
 * ���   ��none 
 * ����   ��дSEG�������е�λ��SEG( 0~31 )
 **********************************************************/
void  W1622_byte(u8 SEG,u8 dat)
{
    u8 i,addr;
    addr = SEG * 2;
    for(i=0;i<2;i++)
    {
			W1622_q(addr,dat);
			addr += 1;
			dat >>= 4;
    }
}
/**********************************************************
 * ������ : W1622_q
 * ����   ��addr ��ַ dat ����
 * ���   ��none 
 * ����   ��ָ��ģʽ
 **********************************************************/
u8 W1622_Read_byte(u8 addr)
{
	 u8 i=0;
	 u8 dat=0;
	 for(i=0;i<2;i++)
		{
			HT1622_CS_L;
			SendBit_HL(0xc0,3);    //д���־��"110"
			SendBit_HL(addr<<2,6); //д�� 6 λ addr
			dat|=ReceiveBit_LH();  //������4λ����
			HT1622_CS_H;
			dat<<=4;
		}
	 return dat;
}

/**********************************************************
 * ������ : W1622_READ_MODIFY_WRITE 
 * ����   ��SEG λѡ�� dat ��Ҫд�������
 * ���   ��none 
 * ����   ����дͬʱ����
 **********************************************************/
void W1622_READ_MODIFY_WRITE(u8 SEG,u8 dat)
{
  u8 addr;
  addr = SEG * 2;
	W1622_q(addr,dat);
	addr += 1;
	dat >>= 4;//��ȡд�����ݸ�4λ
	dat|=0x01;
	dat&=ReceiveBit_LH();//��ȡCOM8��״̬
	HT1622_W();//�������ݶ˿�Ϊ���ģʽ
	W1622_q(addr,dat);//д������
}

///**********************************************************
// * ������ : W1622_READ_MODIFY_WRITE 
// * ����   ��SEG λѡ�� dat ��Ҫд�������
// * ���   ��none 
// * ����   ����дͬʱ����,�ȶ�ȡcom�ڸ߰�λ���ݸı����λ���ݣ��ٴ�д�����ݡ������޸�COM8������ʾ
// **********************************************************/
//void W1622_READ_MODIFY_WRITE_2(u8 SEG,u8 dat)
//{
//  u8 addr;
//	u8 i;
//	u8 dat_1=dat;
//  addr = SEG * 2;
//	HT1622_CS_L;//оƬʹ��
//	for(i=0;i<7;i++)//��ȡ����дCOM8����
//	{
//		SendBit_HL(0xa0,3);     //д���־��"101"
//		SendBit_HL(addr<<2,6);  //д�� 6 λ addr

//		dat_1&=0x01;//ȡҪд���������λ
//		HT1622_R();//���ݿ����ó�����ģʽ
//		dat_1|=(ReceiveBit_LH()&0x07);        //��ȡCOM8��״̬
//		HT1622_W();//���ݿ����ó����ģʽ
//		SendBit_LH(dat,4);      //д���4λ����
//		addr+=2;
//		dat_1=dat;
//		dat_1>>=i;
//	}
//	HT1622_CS_H;
//}
/**********************************************************
 * ������ : W1622_READ_MODIFY_WRITE 
 * ����   ��SEG λѡ�� dat ��Ҫд�������
 * ���   ��none 
 * ����   ����дͬʱ����,�ȶ�ȡcom�ڸ߰�λ���ݸı����λ���ݣ��ٴ�д�����ݡ������޸�COM8������ʾ
 **********************************************************/
void W1622_READ_MODIFY_WRITE_2(u8 SEG,u8 dat)
{
  u8 addr;
	u8 dat_1=dat;
//  addr = SEG * 2;
	HT1622_CS_L;//оƬʹ��
	SendBit_HL(0xa0,3);     //д���־��"101"
	SendBit_HL(9<<2,6);  //д�� 6 λ addr
	dat_1&=0x01;//ȡҪд���������λ
	HT1622_R();//���ݿ����ó�����ģʽ
	dat_1|=(ReceiveBit_LH()&0x07);        //��ȡCOM8��״̬
	HT1622_W();//���ݿ����ó����ģʽ
	SendBit_LH(dat,4);      //д���4λ����
	addr+=2;
	dat_1=dat;
	dat_1>>=1;
	
	SendBit_HL(0xa0,3);     //д���־��"101"
	SendBit_HL(11<<2,6);  //д�� 6 λ addr
	dat_1&=0x01;//ȡҪд���������λ
	HT1622_R();//���ݿ����ó�����ģʽ
	dat_1|=(ReceiveBit_LH()&0x07);        //��ȡCOM8��״̬
	HT1622_W();//���ݿ����ó����ģʽ
	SendBit_LH(dat,4);      //д���4λ����
	addr+=2;
	dat_1=dat;
	dat_1>>=2;
	
	SendBit_HL(0xa0,3);     //д���־��"101"
	SendBit_HL(7<<2,6);  //д�� 6 λ addr
	dat_1&=0x01;//ȡҪд���������λ
	HT1622_R();//���ݿ����ó�����ģʽ
	dat_1|=(ReceiveBit_LH()&0x07);        //��ȡCOM8��״̬
	HT1622_W();//���ݿ����ó����ģʽ
	SendBit_LH(dat,4);      //д���4λ����
	addr+=2;
	dat_1=dat;
	
	dat_1>>=3;
	SendBit_HL(0xa0,3);     //д���־��"101"
	SendBit_HL(5<<2,6);  //д�� 6 λ addr
	dat_1&=0x01;//ȡҪд���������λ
	HT1622_R();//���ݿ����ó�����ģʽ
	dat_1|=(ReceiveBit_LH()&0x07);        //��ȡCOM8��״̬
	HT1622_W();//���ݿ����ó����ģʽ
	SendBit_LH(dat,4);      //д���4λ����
	addr+=2;
	dat_1=dat;
	dat_1>>=4;
	
	SendBit_HL(0xa0,3);     //д���־��"101"
	SendBit_HL(13<<2,6);  //д�� 6 λ addr
	dat_1&=0x01;//ȡҪд���������λ
	HT1622_R();//���ݿ����ó�����ģʽ
	dat_1|=(ReceiveBit_LH()&0x07);        //��ȡCOM8��״̬
	HT1622_W();//���ݿ����ó����ģʽ
	SendBit_LH(dat,4);      //д���4λ����
	addr+=2;
	dat_1=dat;
	dat_1>>=5;
	
	SendBit_HL(0xa0,3);     //д���־��"101"
	SendBit_HL(3<<2,6);  //д�� 6 λ addr
	dat_1&=0x01;//ȡҪд���������λ
	HT1622_R();//���ݿ����ó�����ģʽ
	dat_1|=(ReceiveBit_LH()&0x07);        //��ȡCOM8��״̬
	HT1622_W();//���ݿ����ó����ģʽ
	SendBit_LH(dat,4);      //д���4λ����
	addr+=2;
	dat_1=dat;
	dat_1>>=6;
	
	SendBit_HL(0xa0,3);     //д���־��"101"
	SendBit_HL(1<<2,6);    //д�� 6 λ addr
	dat_1&=0x01;//ȡҪд���������λ
	HT1622_R();//���ݿ����ó�����ģʽ
	dat_1|=(ReceiveBit_LH()&0x07);        //��ȡCOM8��״̬
	HT1622_W();//���ݿ����ó����ģʽ
	SendBit_LH(dat,4);      //д���4λ����
	
	HT1622_CS_H;
}
/**********************************************************
 * ������ : W1622_READ_MODIFY_WRITE_3 
 * ����   ��dat ��Ҫд�������
 * ���   ��none 
 * ����   ����дͬʱ����,�ȶ�ȡcom�ڸ߰�λ���ݸı����λ���ݣ��ٴ�д�����ݡ������޸�COM8������ʾ
 **********************************************************/
void W1622_READ_MODIFY_WRITE_3(u8 dat)
{
  u8 addr;
	u8 dat_1=dat;
//  addr = SEG * 2;
	HT1622_CS_L;//оƬʹ��
	
	SendBit_HL(0xa0,3);     //д���־��"101"
	SendBit_HL(19<<2,6);  //д�� 6 λ addr
	dat_1&=0x01;//ȡҪд���������λ
	HT1622_R();//���ݿ����ó�����ģʽ
	dat_1|=(ReceiveBit_LH()&0x07);        //��ȡCOM8��״̬
	HT1622_W();//���ݿ����ó����ģʽ
	SendBit_LH(dat,4);      //д���4λ����
	addr+=2;
	dat_1=dat;
	dat_1>>=1;
	SendBit_HL(0xa0,3);     //д���־��"101"
	SendBit_HL(21<<2,6);  //д�� 6 λ addr
	dat_1&=0x01;//ȡҪд���������λ
	HT1622_R();//���ݿ����ó�����ģʽ
	dat_1|=(ReceiveBit_LH()&0x07);        //��ȡCOM8��״̬
	HT1622_W();//���ݿ����ó����ģʽ
	SendBit_LH(dat,4);      //д���4λ����
	addr+=2;
	dat_1=dat;
	dat_1>>=2;
	SendBit_HL(0xa0,3);     //д���־��"101"
	SendBit_HL(17<<2,6);  //д�� 6 λ addr
	dat_1&=0x01;//ȡҪд���������λ
	HT1622_R();//���ݿ����ó�����ģʽ
	dat_1|=(ReceiveBit_LH()&0x07);        //��ȡCOM8��״̬
	HT1622_W();//���ݿ����ó����ģʽ
	SendBit_LH(dat,4);      //д���4λ����
	addr+=2;
	dat_1=dat;
	dat_1>>=3;
	SendBit_HL(0xa0,3);     //д���־��"101"
	SendBit_HL(23<<2,6);  //д�� 6 λ addr
	dat_1&=0x01;//ȡҪд���������λ
	HT1622_R();//���ݿ����ó�����ģʽ
	dat_1|=(ReceiveBit_LH()&0x07);        //��ȡCOM8��״̬
	HT1622_W();//���ݿ����ó����ģʽ
	SendBit_LH(dat,4);      //д���4λ����
	addr+=2;
	dat_1=dat;
	dat_1>>=4;
	SendBit_HL(0xa0,3);     //д���־��"101"
	SendBit_HL(25<<2,6);  //д�� 6 λ addr
	dat_1&=0x01;//ȡҪд���������λ
	HT1622_R();//���ݿ����ó�����ģʽ
	dat_1|=(ReceiveBit_LH()&0x07);        //��ȡCOM8��״̬
	HT1622_W();//���ݿ����ó����ģʽ
	SendBit_LH(dat,4);      //д���4λ����
	addr+=2;
	dat_1=dat;
	dat_1>>=5;
	SendBit_HL(0xa0,3);     //д���־��"101"
	SendBit_HL(15<<2,6);  //д�� 6 λ addr
	dat_1&=0x01;//ȡҪд���������λ
	HT1622_R();//���ݿ����ó�����ģʽ
	dat_1|=(ReceiveBit_LH()&0x07);        //��ȡCOM8��״̬
	HT1622_W();//���ݿ����ó����ģʽ
	SendBit_LH(dat,4);      //д���4λ����
	addr+=2;
	dat_1=dat;
	dat_1>>=6;
	
	SendBit_HL(0xa0,3);     //д���־��"101"
	SendBit_HL(39<<2,6);    //д�� 6 λ addr

	dat_1&=0x01;//ȡҪд���������λ
	HT1622_R();//���ݿ����ó�����ģʽ
	dat_1|=(ReceiveBit_LH()&0x07);        //��ȡCOM8��״̬
	HT1622_W();//���ݿ����ó����ģʽ
	SendBit_LH(dat,4);      //д���4λ����
	HT1622_CS_H;
}
/**********************************************************
 * ������ : W1622_READ_MODIFY_WRITE 
 * ����   ��SEG λѡ�� dat ��Ҫд�������
 * ���   ��none 
 * ����   ����дͬʱ����
 **********************************************************/
void W1622_Write_Seg(u8 SEG,u8 dat)
{
  u8 addr;
	u8 data=0x0f;
	u8 dat_1=dat;
  addr = SEG * 2;
	SendBit_HL(0xa0,3);    //д���־��"101"
  SendBit_HL((addr+1)<<2,6); //д�� 6 λ addr
	dat_1&=0x01;//��ȡд�����ݵ�λ
	data&=ReceiveBit_LH();//��ȡCOM�ڸ�4λ״̬
	HT1622_W();       //�������ݶ˿�Ϊ���ģʽ
	W1622_q(addr,dat);//д������
}
/**********************************************************
 * ������ : W1622_success
 * ����   ��
 * ���   ��none 
 * ����   ������д��Ԫ SEG(0~31)//������ʽ���������ˢ��
 **********************************************************/
void W1622_success(u8 SEG,u8 cnt,u8 *p)
{
    u8 i,addr;
	
    addr = SEG * 2;
    HT1622_CS_L;
    SendBit_HL(0xa0,3);    //д���־��"101"
    SendBit_HL(addr<<2,6); //д�� 6 λ addr
    for(i =0; i<cnt;i++)
    {
			SendBit_LH(*p,4);
			SendBit_LH(*p>>4,4);
			p++;
    }
    HT1622_CS_H;
}

void disp_renew(void)   //��ʾ����,����8x32��ȫ��ˢ��
{
    W1622_success(0,32,(u8*)DIS_BUF);
}
//com(0-7)  SEG(0-31)
void  dispON(u8 com,u8 SEG)
{
    W1622_byte(SEG,DIS[SEG]);     //ȥ��ע�ͺ�д���������ʾ
}

void  cleandisp(void)
{
    u8  i;
    HT1622_CS_L;           //����Ļ
    SendBit_HL(0xa0,3);    //д���־��"101"
    SendBit_HL(0,6);       //д�� 6 λ addr
    for(i =0; i <32; i++)  //����д������
    {
        SendBit_LH(0,4);
        SendBit_LH(0,4);
    }
    HT1622_CS_H;
    for(i=0; i<32; i++)
    {
        DIS_BUF[i] = 0x00;    //����Դ�
    }
}

void  disp_All(void)
{
    u8  i;
    HT1622_CS_L;           //ȫ��ʾ��Ļ
    SendBit_HL(0xa0,3);    //д���־��"101"
    SendBit_HL(0,6);       //д�� 6 λ addr
    for(i =0; i <32; i ++) //����д������
    {
        SendBit_LH(0xff,4);
        SendBit_LH(0xff,4);
    }
    HT1622_CS_H;
    for(i=0; i<32; i++)
    {
        DIS_BUF[i] = 0xff;    //����Դ�
    }
}


