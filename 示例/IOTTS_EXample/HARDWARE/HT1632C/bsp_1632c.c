/**********************************************************
* @ File name -> bsp_1632c.c
* @ Version   -> V1.0.1
* @ Date      -> 08-06-2017
* @ Brief     -> LED��ʾ����
* @ Revise    -> 
***********************************************************/
#include "bsp_1632c.h"

//����ܶ�ѡ dp-a��ʽ:0-9,A,b,c,u
unsigned char tab[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x3e};
//����ܶ�ѡ0-9����С����
unsigned char tab1[] = {0xbf, 0x86, 0xdb, 0xcf, 0xe6, 0xed, 0xfd, 0x17, 0xff, 0xef};
/****************************************
*������:HT1632C_Writer
*��    ��:HT1632C�������ݺ�������λ��ǰ
*��    ��:Data,cnt
*��    ��:
****************************************/
void HT1632C_Writer(unsigned char Data,unsigned char cnt)     
{
    unsigned char i;
    for(i=0;i<cnt;i++)
    {
        GPIOEX_BitClr(HT_WR);
        if(Data&0x80)
            GPIOEX_BitSet(HT_DATA);
        else
            GPIOEX_BitClr(HT_DATA);
        Data<<=1;
        GPIOEX_BitSet(HT_WR);
    }
}
/*******************************
*������:HT1632C_Writer_CMD
*��    ��:���������
*��    ��:cmd
*��    ��:
********************************/
void HT1632C_Writer_CMD(unsigned char cmd)                   
{
    GPIOEX_BitClr(HT_CS);                                             
    HT1632C_Writer(0x80,3);                                    //������100
    HT1632C_Writer(cmd,9);                                     //�������ݣ���9λΪ0
    GPIOEX_BitSet(HT_CS);
}
/*******************************
*������:HT1632C_Writer_DATA
*��    ��:ָ����ַд������
*��    ��:Addr,Data
*��    ��:
********************************/
void HT1632C_Writer_DATA(unsigned char Addr,unsigned char Data)
{
    GPIOEX_BitClr(HT_CS);
    HT1632C_Writer(0xa0,3);                                    //������101
    HT1632C_Writer(Addr<<1,7);                                  //д��7λ��ַ
    HT1632C_Writer(Data,4);                                    //д��4λ����
    GPIOEX_BitSet(HT_CS);   
}
/************************************************************
*������:HT1632C_Writer_AllDATA
*��    ��:ָ����ַд���������ݣ�cntΪд�����ݸ�������8λ��
*��    ��:Addr,*p,cnt
*��    ��:
*************************************************************/
void HT1632C_Writer_AllDATA(unsigned char Addr,unsigned char *p,unsigned char cnt)
{
    unsigned char i;
    GPIOEX_BitClr(HT_CS);   
    HT1632C_Writer(0xa0,3);
    HT1632C_Writer(Addr<<1,7);
    for(i=0;i<cnt;i++)
    {
        HT1632C_Writer(*p,8);
        p++;
    }
    GPIOEX_BitSet(HT_CS);
}  
/*******************************
*������:HT1632C_clr
*��    ��:��������
*��    ��:
*��    ��:
********************************/
void HT1632C_clr(void)  
{
    unsigned char i;
    GPIOEX_BitClr(HT_CS);
    HT1632C_Writer(0xa0,3);
    HT1632C_Writer(0x00<<1,7);
    for(i=0;i<36;i++)
    {
        HT1632C_Writer(0,8);            
    }
    GPIOEX_BitSet(HT_CS);
} 
/*******************************
*������:HT1632C_Init
*��    ��:HT1632C��ʼ������
*��    ��:
*��    ��:
********************************/
void HT1632C_Init(void)     
{
    GPIOEX_BitSet(HT_CS);
    GPIOEX_BitSet(HT_WR);
    GPIOEX_BitSet(HT_DATA);
     
    HT1632C_Writer_CMD(SYS_DIS);
    HT1632C_Writer_CMD(COM_OPTION);
    HT1632C_Writer_CMD(RC_MASTER_MODE);
    HT1632C_Writer_CMD(SYS_EN);
    HT1632C_Writer_CMD(PWM_DUTY);
    HT1632C_Writer_CMD(BLINK_OFF);
    HT1632C_Writer_CMD(LED_ON);
}
/*******************************
*������:display_num
*��    ��:�������ʾ����
*��    ��:
*��    ��:
********************************/
//��ʾ��ֵ����С����
void display_num(unsigned int dat)
{
    unsigned char i, j;
    unsigned char dt;//dtΪҪд�뵽ram��ֵ
    dt = 0;
    for(i=0; i<8;i++)
    {
       dt =( tab[dat]&(1<<i))<<(7-i);//ǧ
        if(i==7)
        {
            dt +=(tab[dat%1000/100 ]&(1<<i))>>(i-6);//��
            dt += (tab[dat%100/10 ]&(1<<i))>>(i-5);//ʮ
            dt += (tab[dat%10]&(1<<i))>>(i-4);   //��
        }
        else if(i == 6)
        {
            dt +=(tab[dat%1000/100 ]&(1<<i))>>(i-6);
            dt += (tab[dat%100/10]&(1<<i))>>(i-5);
            dt += (tab[dat%10]&(1<<i))>>(i-4);   
        }
        else if(i == 5)
      {
          dt+=(tab[dat%1000/100 ]&(1<<i))<<(6-i);//�� 
          dt += (tab[dat%100/10]&(1<<i))<<(5-i);
          dt += (tab[dat%10]&(1<<i))>>(i-4);  
      }
        else
       { 
          dt+=(tab[dat%1000/100 ]&(1<<i))<<(6-i);//��
          dt += (tab[dat%100/10]&(1<<i))<<(5-i);
          dt += (tab[dat%10]&(1<<i))<<(4-i);       
      }
        HT1632C_Writer_DATA((0x03+i*4),dt);
    }
}