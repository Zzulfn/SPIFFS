/**********************************************************
                       ��������
										 
���ܣ�stm32f103rct6���ƣ�75MHzʱ�ӣ� AD9834���Ҳ���Ƶ�������Χ0-30M(�ɱ༭0-100M)��
			������50k-5M�����ǲ���0-10M��ɨƵĬ�����Ҳ� ��
			��ʾ��12864cog
�ӿڣ����ƽӿ������AD9834.h  �����ӿ������key.h
ʱ�䣺2015/11/10
�汾��1.0
���ߣ���������
����������������ԭ�ӣ�����Լ���������δ��������ɣ��������������κ���;

������������뵽�Ա��꣬�������ӽ߳�Ϊ������ ^_^
https://shop110336474.taobao.com/?spm=a230r.7195193.1997079397.2.Ic3MRJ

**********************************************************/
#include "task_manage.h"
#include "delay.h"
#include "key.h"
#include "bsp_max262.h"
#include <stdio.h>
#include <ctype.h>
#include <cstring>

#define OUT_KEY  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)//��ȡ����0
#define FLASH_SAVE_ADDR  0x0801F000  				//����FLASH �����ַ(����Ϊż��)


u8 Task_Index = 0;//�����л�
u8 Param_Mode = 0;//���������л�
u8 fre_buf[StrMax]; //����ת���ַ�����
u8 display[StrMax]; //������ʾ����
u8 P_Index = 0; //�༭λ��
u8 Task_First=1;//��һ�ν�����
u8 _return=0;
//ɨƵ����
u32 SweepMinFre = 1000;
u32 SweepMaxFre = 10000;
u32 fc = 1000;
u8  qn = 5;


u32 f0;

u8 Task_Delay(u32 delay_time, u8* delay_ID)
{
	static u8 Time_Get = 0;
	static u32 Time_Triger;
    if(Time_Get == 0)
    {
      Time_Triger = SysTimer + delay_time;
      Time_Get = 1;
    }
    if(SysTimer >= Time_Triger)
    { 
      Time_Get = 0;
			*delay_ID = 1;		//	���������ѱ�ִ��һ��
			return 1;
    }
		return 0;
}

//u8 TaskOneToIdel(void)
//{
//	static u8 delay_ID0 = 0;
//	static u8 delay_ID1 = 0;
//	static u8 delay_ID2 = 0;
//	static u8 delay_ID3 = 0;
//	u8 delay_arrive0 = 0;
//	u8 delay_arrive1 = 0;
//	u8 delay_arrive2 = 0;
//	u8 delay_arrive3 = 0;
//	
//	delay_arrive0 = Task_Delay(100, &delay_ID0);
//	delay_arrive1 = Task_Delay(100, &delay_ID1);
//	delay_arrive2 = Task_Delay(100, &delay_ID2);
//	delay_arrive3 = Task_Delay(100, &delay_ID3);
//	if((delay_arrive0 == 0) && (delay_ID0 == 0))
//		return 0;
//	else if((delay_arrive0) && (delay_ID0))
//	{
//			//ִ�б���ʱ�����
//	}
//		
//	
//	
//	if(delay_ID0&&delay_ID1&&delay_ID2&&delay_ID3)
//	{
//		delay_ID0 = 0;
//		delay_ID1 = 0;
//		delay_ID2 = 0;
//		delay_ID3 = 0;
//	}
//}

u8 TaskCycleDelay(u32 delay_time, u8* Last_delay_ID, u8* Self_delay_ID)
{
	static u8 Time_Get = 0;
	static u32 Time_Triger;
	
	if(!(*Last_delay_ID))
		return 0;
	if(Time_Get == 0)
	{
		Time_Triger = SysTimer + delay_time;
		Time_Get = 1;
	}
	if(SysTimer >= Time_Triger)
	{ 
		Time_Get = 0;
		*Last_delay_ID = 0;
		*Self_delay_ID = 1;		//	���������ѱ�ִ��һ��
		return 1;
	}
	return 0;
}

//�����ݷŵ���һ�����棬��ʾ��FloatNumС����λ����CursorEn���ʹ��
void Copybuf2dis(u8 *source, u8 dis[StrMax], u8 dispoint, u8 FloatNum, u8 CursorEn)
{
	int i, len;
	
	len = strlen(source);
	i = len - FloatNum;//��������
	if(FloatNum>0)dis[i] = '.';
	for (i = 0; i < len; i++)
	{
		if(i < (len-FloatNum)) dis[i] = source[i];
		else 
		{ dis[i+1] = source[i]; }
	}
	
	if(CursorEn)
	{
		if(dispoint < (len-FloatNum)) dis[dispoint] += 128;
		else dis[dispoint+1] += 128;	
	}
}
//
void Set_PointFre(u32 Key_Value, u8* Task_ID)
{
	//�����ж�
	switch(Key_Value)
	{
		case K_4_S: fre_buf[P_Index]++;break;
		case K_4_L: fre_buf[P_Index]++;break;
		case K_5_L: P_Index++;break;
		case K_5_S: P_Index++;break;
		case K_1_L: P_Index--;break;
		case K_1_S: P_Index--;break;
		case K_3_S: fre_buf[P_Index]--;break;
		case K_3_L: fre_buf[P_Index]--;break;
		case K_2_S: Param_Mode++;break;
	}
	if(fre_buf[P_Index] == '/') fre_buf[P_Index] = '9';//<'0'
	if(fre_buf[P_Index] == ':') fre_buf[P_Index] = '0';//>'9'
	//�����л�
	if(Key_Value == K_2_L) 
	{	
		Task_Index++;
		LCD_Clear();
	}
	if(Key_Value == K_2_L || Key_Value == K_2_S) Task_First=1;//���µ�һ�ν���
	if(Task_Index >= Interface) Task_Index=0;
	switch(Task_Index)
	{
		case 0: 
			Task0_PointFre(Key_Value);
			break;
		case 1: 
			Task1_Square(Key_Value);
			break;
		case 2: 
			Task2_Triangular(Key_Value);
			break;
//		case 3: 
//			Task3_SweepFre(Key_Value);
//			break;
	}
	//��������
	LCD_Show_CEStr(0,0,"262");	
}
//����

void Task0_PointFre(u32 Key_Value)//���Ҳ� (10M) 0-100 000 000
{
	u8 showstr[StrMax]={0};
	static u8 AB_Flag;
	
	if(Task_First)
	{
		Task_First = 0;//�����һ�ν�����
		Key_Value = K_2_S;
		Param_Mode %= 3;//�������ģʽ���Ա�װ���ʼֵ
		if(Param_Mode == 0) sprintf(fre_buf, "%6d", fc);
		if(Param_Mode == 1) sprintf(fre_buf, "%2d", qn);

		LCD_Show_CEStr(64-8*3,0,"  ��ͨ");//ģʽ����
		_return=1;//������ʾ��־ 
	}
	if(Key_Value != K_NO)
	{
		if(Param_Mode == 0)//��СƵ������
		{
			P_Index %= 6;//����λ��
			fc = atol(fre_buf);//�ַ�ת��
			if(fc>140000) fc=140000;//��������
			if(fc<0) fc=0;
			sprintf(fre_buf, "%6d", fc);//��������д��
		}
		//��ʾ
		sprintf(showstr, "%6d", fc);//�������뻺����ʾ
		fre_buf_change(showstr);//fre_buf���� �� '->'0'
		if(Param_Mode == 0) Copybuf2dis(showstr, display, P_Index, 0, 1);
		else Copybuf2dis(showstr, display, P_Index, 0, 0);
		display[6]=0;//ֻ��ʾ3λ
		OLED_ShowString(56, 2, display);
		LCD_Show_CEStr(56+48,2,"Hz");
		LCD_Show_CEStr(0,2,"��ֹf:");
		
		if(Param_Mode == 1)//���Ƶ������
		{
			P_Index %= 2;//����λ��
			qn = atol(fre_buf);//�ַ�ת��
			if(qn>90) qn=90;//��������
			if(qn<0) qn=0;
			sprintf(fre_buf, "%2d", qn);//��������д��
		}
		//��ʾ
		sprintf(showstr, "%2d", qn);//�������뻺����ʾ
		fre_buf_change(showstr);//fre_buf���� �� '->'0'
		if(Param_Mode == 1) Copybuf2dis(showstr, display, P_Index, 0, 1);
		else Copybuf2dis(showstr, display, P_Index, 0, 0);
		display[2]=0;//ֻ��ʾ3λ
		OLED_ShowString(56, 4, display);
		LCD_Show_CEStr(0,4,"Qֵ  :");
		
		if(Param_Mode == 2)//Ƶ�ʲ�������
		{
			P_Index %= 3;//����λ��
			AB_Flag = P_Index;
			if(AB_Flag==0) 
			{
				display[0]='A'+128;
				display[1]=' ';
			}
			if(AB_Flag==1) 
			{	
				display[0]='B'+128;
				display[1]=' ';
			}
			if(AB_Flag==2) 
			{
				display[0]='A'+128;
				display[1]='B'+128;
			}
			display[2]=0;
			OLED_ShowString(56,6,display);
		}
		else
		{
			if(AB_Flag==0) 
			{
				display[0]='A';
				display[1]=' ';
			}
			if(AB_Flag==1) 
			{	
				display[0]='B';
				display[1]=' ';
			}
			if(AB_Flag==2) 
			{
				display[0]='A';
				display[1]='B';
			}
			display[2]=0;
			OLED_ShowString(56,6,display);
		}
		//��ʾ
		LCD_Show_CEStr(0,6,"ͨ�� :");		
		
		if(AB_Flag==0) 
		{
			f0 = lhp_WorkFclk(fc, qn, highPass, modeThree, channelA);
			PWM_Set(f0);
		}
		else if(AB_Flag==1) 
		{
			f0 = lhp_WorkFclk(fc, qn, highPass, modeThree, channelB);
			PWM_Set(f0);
		}
		else
		{
			f0 = lhp_WorkFclk(fc, qn, highPass, modeThree, channelA);
			f0 = lhp_WorkFclk(fc, qn, highPass, modeThree, channelB);
			PWM_Set(f0);
		}
		
		sprintf(showstr,"%7d", f0);
		LCD_Show_CEStr(75,6,showstr);
		
		_return=1;
	}
}

void Task1_Square(u32 Key_Value)//���� 300k
{
	u8 showstr[StrMax]={0};
	static u8 AB_Flag;
	
	if(Task_First)
	{
		Task_First = 0;//�����һ�ν�����
		Key_Value = K_2_S;
		Param_Mode %= 3;//�������ģʽ���Ա�װ���ʼֵ
		if(Param_Mode == 0) sprintf(fre_buf, "%6d", fc);
		if(Param_Mode == 1) sprintf(fre_buf, "%2d", qn);

		LCD_Show_CEStr(64-8*3,0,"  ��ͨ");//ģʽ����
		_return=1;//������ʾ��־ 
	}
	if(Key_Value != K_NO)
	{
		if(Param_Mode == 0)//��СƵ������
		{
			P_Index %= 6;//����λ��
			fc = atol(fre_buf);//�ַ�ת��
			if(fc>140000) fc=140000;//��������
			if(fc<0) fc=0;
			sprintf(fre_buf, "%6d", fc);//��������д��
		}
		//��ʾ
		sprintf(showstr, "%6d", fc);//�������뻺����ʾ
		fre_buf_change(showstr);//fre_buf���� �� '->'0'
		if(Param_Mode == 0) Copybuf2dis(showstr, display, P_Index, 0, 1);
		else Copybuf2dis(showstr, display, P_Index, 0, 0);
		display[6]=0;//ֻ��ʾ3λ
		OLED_ShowString(56, 2, display);
		LCD_Show_CEStr(56+48,2,"Hz");
		LCD_Show_CEStr(0,2,"��ֹf:");
		
		if(Param_Mode == 1)//���Ƶ������
		{
			P_Index %= 2;//����λ��
			qn = atol(fre_buf);//�ַ�ת��
			if(qn>90) qn=90;//��������
			if(qn<0) qn=0;
			sprintf(fre_buf, "%2d", qn);//��������д��
		}
		//��ʾ
		sprintf(showstr, "%2d", qn);//�������뻺����ʾ
		fre_buf_change(showstr);//fre_buf���� �� '->'0'
		if(Param_Mode == 1) Copybuf2dis(showstr, display, P_Index, 0, 1);
		else Copybuf2dis(showstr, display, P_Index, 0, 0);
		display[2]=0;//ֻ��ʾ3λ
		OLED_ShowString(56, 4, display);
		LCD_Show_CEStr(0,4,"Qֵ  :");
		
		if(Param_Mode == 2)//Ƶ�ʲ�������
		{
			P_Index %= 3;//����λ��
			AB_Flag = P_Index;
			if(AB_Flag==0) 
			{
				display[0]='A'+128;
				display[1]=' ';
			}
			if(AB_Flag==1) 
			{	
				display[0]='B'+128;
				display[1]=' ';
			}
			if(AB_Flag==2) 
			{
				display[0]='A'+128;
				display[1]='B'+128;
			}
			display[2]=0;
			OLED_ShowString(56,6,display);
		}
		else
		{
			if(AB_Flag==0) 
			{
				display[0]='A';
				display[1]=' ';
			}
			if(AB_Flag==1) 
			{	
				display[0]='B';
				display[1]=' ';
			}
			if(AB_Flag==2) 
			{
				display[0]='A';
				display[1]='B';
			}
			display[2]=0;
			OLED_ShowString(56,6,display);
		}
		//��ʾ
		LCD_Show_CEStr(0,6,"ͨ�� :");		
		
		if(AB_Flag==0) 
		{
			f0 = lhp_WorkFclk(fc, qn, lowPass, modeTwo, channelA);
			PWM_Set(f0);
		}
		else if(AB_Flag==1) 
		{
			f0 = lhp_WorkFclk(fc, qn, lowPass, modeTwo, channelB);
			PWM_Set(f0);
		}
		else
		{
			f0 = lhp_WorkFclk(fc, qn, lowPass, modeTwo, channelA); 
			f0 = lhp_WorkFclk(fc, qn, lowPass, modeTwo, channelB);
			PWM_Set(f0);
		}
		
		sprintf(showstr,"%7d", f0);
		LCD_Show_CEStr(75,6,showstr);
		
		_return=1;
	}
}
void Task2_Triangular(u32 Key_Value)//���ǲ� 1M
{	
	static u8 AB_Flag;
	u8 showstr[StrMax]={0};
	
	if(Task_First)
	{
		Task_First = 0;//�����һ�ν�����
		Key_Value = K_2_S;
		Param_Mode %= 3;//�������ģʽ���Ա�װ���ʼֵ
		if(Param_Mode == 0) sprintf(fre_buf, "%6d", SweepMinFre);
		if(Param_Mode == 1) sprintf(fre_buf, "%6d", SweepMaxFre);

		LCD_Show_CEStr(64-8*3,0,"  ��ͨ");//ģʽ����
		_return=1;//������ʾ��־ 
	}
	if(Key_Value != K_NO)
	{
		if(Param_Mode == 0)//��СƵ������
		{
			P_Index %= 6;//����λ��
			SweepMinFre = atol(fre_buf);//�ַ�ת��
			if(SweepMinFre>140000) SweepMinFre=140000;//��������
			if(SweepMinFre<0) SweepMinFre=0;
			sprintf(fre_buf, "%6d", SweepMinFre);//��������д��
		}
		//��ʾ
		sprintf(showstr, "%6d", SweepMinFre);//�������뻺����ʾ
		fre_buf_change(showstr);//fre_buf���� �� '->'0'
		if(Param_Mode == 0) Copybuf2dis(showstr, display, P_Index, 0, 1);
		else Copybuf2dis(showstr, display, P_Index, 0, 0);
		OLED_ShowString(56, 2, display);
		LCD_Show_CEStr(56+48,2,"Hz");
		LCD_Show_CEStr(0,2,"��ʼf:");
		
		if(Param_Mode == 1)//���Ƶ������
		{
			P_Index %= 6;//����λ��
			SweepMaxFre = atol(fre_buf);//�ַ�ת��
			if(SweepMaxFre>140000) SweepMaxFre=140000;//��������
			if(SweepMaxFre<0) SweepMaxFre=0;
			sprintf(fre_buf, "%6d", SweepMaxFre);//��������д��
		}
		//��ʾ
		sprintf(showstr, "%6d", SweepMaxFre);//�������뻺����ʾ
		fre_buf_change(showstr);//fre_buf���� �� '->'0'
		if(Param_Mode == 1) Copybuf2dis(showstr, display, P_Index, 0, 1);
		else Copybuf2dis(showstr, display, P_Index, 0, 0);
		OLED_ShowString(56, 4, display);
		LCD_Show_CEStr(56+48, 4,"Hz");
		LCD_Show_CEStr(0,4,"��ֹf:");
		
				if(Param_Mode == 2)//Ƶ�ʲ�������
		{
			P_Index %= 3;//����λ��
			AB_Flag = P_Index;
			if(AB_Flag==0) 
			{
				display[0]='A'+128;
				display[1]=' ';
			}
			if(AB_Flag==1) 
			{	
				display[0]='B'+128;
				display[1]=' ';
			}
			if(AB_Flag==2) 
			{
				display[0]='A'+128;
				display[1]='B'+128;
			}
			display[2]=0;
			OLED_ShowString(56,6,display);
		}
		else
		{
			if(AB_Flag==0) 
			{
				display[0]='A';
				display[1]=' ';
			}
			if(AB_Flag==1) 
			{	
				display[0]='B';
				display[1]=' ';
			}
			if(AB_Flag==2) 
			{
				display[0]='A';
				display[1]='B';
			}
			display[2]=0;
			OLED_ShowString(56,6,display);
		}
		//��ʾ
		LCD_Show_CEStr(0,6,"ͨ�� :");		
		
		if(AB_Flag==0) 
		{
			f0 = bp_WorkFclk(SweepMinFre, SweepMaxFre, modeOne, channelA);
			PWM_Set(f0);
		}
		else if(AB_Flag==1) 
		{
			f0 = bp_WorkFclk(SweepMinFre, SweepMaxFre, modeOne, channelB);
			PWM_Set(f0);
		}
		else
		{
			f0 = bp_WorkFclk(SweepMinFre, SweepMaxFre, modeOne, channelA);
			f0 = bp_WorkFclk(SweepMinFre, SweepMaxFre, modeOne, channelB);
			PWM_Set(f0);
		} 
		
		sprintf(showstr,"%7d", f0);
		LCD_Show_CEStr(75,6,showstr);
		
		_return=1;
	}
}

void Task3_SweepFre(u32 Key_Value)//ɨƵ
{}

	
 
//
void fre_buf_change(u8 *strbuf)
{
	int i;
	for (i = 0 ; i < strlen(strbuf); i++)
	 if(strbuf[i]==0x20) strbuf[i] = '0';
	for (i = 0 ; i < strlen(fre_buf); i++)
	 if(fre_buf[i]==0x20) fre_buf[i] = '0';
}
/*
ÿһ����ʾ���ݵ�λ����ͬ�Ļ���
1��P_Index %= 3;
2��sprintf(fre_buf, "%3d", SweepTime);
3��display[3]=0;//ֻ��ʾ3λ


*/


