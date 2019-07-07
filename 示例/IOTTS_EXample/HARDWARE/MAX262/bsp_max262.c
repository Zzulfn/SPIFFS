#include "bsp_max262.h" 
#include <math.h> 
/***************************************************************
MAX262 IO�ڳ�ʼ��
****************************************************************/
void MAX262_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;            //����IO�ڳ�ʼ�����ݵĽṹ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE); // IO��ʱ��ʹ��
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;  //��ʼ������IO��
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //IO ������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //IO ����ģʽ �������
	GPIO_Init(GPIOA, &GPIO_InitStructure);             //��ʼ��GPIOB
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;  //��ʼ������IO��
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //IO ������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //IO ����ģʽ �������
	GPIO_Init(GPIOB, &GPIO_InitStructure);             //��ʼ��GPIOB
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;  //��ʼ������IO��
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //IO ������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //IO ����ģʽ �������
	GPIO_Init(GPIOC, &GPIO_InitStructure);             //��ʼ��GPIOC
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;  //��ʼ������IO��
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //IO ������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //IO ����ģʽ �������
	GPIO_Init(GPIOD, &GPIO_InitStructure);             //��ʼ��GPIOD
}
/***************************************************************    
*д�����ص����ݵ�ĳ��ַ 
*��ڲ�����A3��A0�ĵ�ַadd,D1��D0��2bit����dat2bit 
*���ڲ������� 
*˵����D1��D0ȡdat2bit�������λ����������PDL�ڵİ�λд������ 
***************************************************************/
u8 channel_se=0;    
void write(u8 add,u8 dat2bit)    
{   
  
	u8 i;  
//	data = GPIO_ReadOutputData(GPIOB);
//  data = GPIO_ReadOutputData(GPIOC);
//  data = GPIO_ReadOutputData(GPIOD); 
	
	//data = (data&0xfff0)|(add&0x000f);            //ȡ4λ��ַ     
  if((add&0x0001)==0x0001)//�ж�A0
		GPIO_WriteBit(GPIOC, GPIO_Pin_12, Bit_SET);
  else 	
    GPIO_WriteBit(GPIOC, GPIO_Pin_12, Bit_RESET);	
  if(((add>>1)&0x0001)==0x0001)//�ж�A1
		GPIO_WriteBit(GPIOC, GPIO_Pin_11, Bit_SET);
  else 	
    GPIO_WriteBit(GPIOC, GPIO_Pin_11, Bit_RESET);	
	if(((add>>1)&0x0001)==0x0001)//�ж�A2
		GPIO_WriteBit(GPIOC, GPIO_Pin_10, Bit_SET);
  else 	
    GPIO_WriteBit(GPIOC, GPIO_Pin_10, Bit_RESET);
	if(((add>>1)&0x0001)==0x0001)//�ж�A3
		GPIO_WriteBit(GPIOA, GPIO_Pin_15, Bit_SET);
  else 	
    GPIO_WriteBit(GPIOA, GPIO_Pin_15, Bit_RESET);
	
	if((dat2bit&0x0001)==0x0001)//ȡ2λ����  
		GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET);
  else 	
    GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_RESET);	
	if(((dat2bit>>1)&0x0001)==0x0001)
		GPIO_WriteBit(GPIOB, GPIO_Pin_6, Bit_SET);
  else 	
    GPIO_WriteBit(GPIOB, GPIO_Pin_6, Bit_RESET);
//	data = (data&0xffcf)|((dat2bit<<4)&0x0030); 	  
//	GPIO_Write(GPIOC,data);    
  switch(channel_se)
	{
		case 0: {
							GPIOD->BRR=0x0004;//����1ͨ��OE,д������
							GPIOB->BSRR=0x0018;
																} break;
		case 1: {
							GPIOB->BRR=0x0008;//����2ͨ��OE
							GPIOB->BSRR=0x0010;
							GPIOD->BSRR=0x0004;
																} break;
		case 2: {
							GPIOD->BSRR=0x0004;
							GPIOB->BSRR=0x0008;
			        GPIOB->BRR=0x0010;//����3ͨ��OE
																} break;
		default: break;
	}   
	for(i=0;i<255;i++);     //����250ns        
	switch(channel_se)
	{
		case 0: {
							GPIOD->BSRR=0x0004;//����1ͨ��OE
																} break;
		case 1: {
							GPIOB->BSRR=0x0008;//����2ͨ��OE
																} break;
		case 2: {
			        GPIOB->BSRR=0x0010;//����3ͨ��OE
																} break;
		default: break;
	}      	
	for(i=0;i<100;i++);     //����10ns 
} 
//����Aͨ��Fֵ     
void setAf(u8 datF)    //6λ����Ƶ��f0����λF0-F5 ��Ӧʮ����0-63
{    
	write(1,datF);         
	datF = datF>>2;         
	write(2,datF);         
	datF = datF>>2;         
	write(3,datF);    
}     
//����Aͨ��Qֵ     
void setAQ(u8 datQ)    //7λƷ������Q����λQ0-Q6 ��Ӧʮ����0-127
{    
	write(4,datQ);         
	datQ = datQ>>2;         
	write(5,datQ);         
	datQ = datQ>>2;         
	write(6,datQ);         
	datQ = (datQ>>2)&1;  //�Ե�ַ7ֻȡһλ         
	write(7,datQ);    
} 
//����Bͨ��Fֵ     
void setBf(u8 datF)    
{    
	write(9,datF);        
	datF = datF>>2;       
	write(10,datF);      
	datF = datF>>2;       
	write(11,datF);    
} 
    
//����Bͨ��Qֵ     
void setBQ(u8 datQ)    
{    
	write(12,datQ);         
	datQ = datQ>>2;        
	write(13,datQ);         
	datQ = datQ>>2;         
	write(14,datQ);    
	datQ = (datQ>>2)&1;
	write(15,datQ);    
}   

 
//***************************************************************     
//���ݽ�ֹƵ��fc��Ʒ������Q��ͨ��ģʽpassMode�����˲���������Ƶ��f0 
//�������ڵ�ͨ�͸�ͨ
//Q = 1 ʱ
//��ͨ��fc �� 1000-10000Hz ����500Hz  f0:769.23-7692.31Hz
//��ͨ��fc �� 1000-10000Hz ����500Hz  f0:1300-13000Hz
float lh_Getf0(float fc,float Q,u8 passMode) 
{    
	float temp; 
	temp = 1-0.5/pow(Q,2);   //pow(Q, 2) ����Q��2����
	temp = sqrt(temp+sqrt(pow(temp,2)+1));  
	if(passMode==lowPass)  //��ͨ����Ƶ�ʼ��㹫ʽ 
	{     
        return  fc/temp;          
	}      
	else  //��ͨ����Ƶ�ʼ��㹫ʽ       
	{ 
        return  fc*temp;  
	} 
}     
//����ʱ��Ƶ��fclk������Ƶ��f0���͹���ģʽworkMode��Ƶ���趨ֵFN     
uint8_t getFn(float fclk,float f0,u8 workMode)    
{ 
 if (workMode==modeTwo)    
	return (u8)(fclk*2.8284271247/f0/PI-26);       
 else    
	return (u8)(fclk*2/f0/PI-26);       
}

// ���� Q ���趨ֵ QN 
uint8_t getQn(u8 workMode, float q) 
{ 
	if (1==workMode) 
	{ 
		return (128-(float)(90.51/q)); 
	} 
	else 
	{ 
		return (128-(float)(64/q)); 
	} 
} 

//����fc��Q��passMode��workMode��ͨ��channel����MAX262�ĵ�ͨ�͸�ͨ�Ĺ������� 
//����ʱ��Ƶ��fclk 
//��ͨ��60384.6-603845.6Hz
//��ͨ��102050-1020500Hz
float lhp_WorkFclk(float fc,float Q,u8 passMode,u8 workMode,u8 channel)    
{       
	float f0;  
	u8 Qn;//, Fn 
  f0 = lh_Getf0(fc, Q, passMode); 
	printf("\r\n����Ƶ�ʣ�%f",f0);
  if (workMode==modeTwo)
	{
		Qn = (u8)(128-90.51/Q);
	}		
	else    
	{
		Qn = (u8)(128-64/Q);
	}		
	if(passMode==highPass)
	{        
		workMode=modeThree;  //��ֻͨ�й���ģʽ3  
	}
	
//	Fn = FnFin_config(f0, workMode);
	
   if(channel==channelA)       
	{ 
    write(0,workMode);         
		setAf(0);  //Ҫ���ô�Χ�ɵ������ص��ݵĲ���Ƶ�ʲ���ȡ���ֵ.         
		setAQ(Qn);       
	}     
	else 
  { 
    write(8,workMode);         
		setBf(0);     //Ҫ���ô�Χ�ɵ������ص��ݵĲ���Ƶ�ʲ���ȡ���ֵ.         
		setBQ(Qn);       
	}  
  if(workMode==modeTwo)
	{        
		return  28.87872*f0;//(Fn+26)*1.11072*f0;//31.46625*PI*f0;//31.46625-63   16.26361-20
	}
	else
	{		
    return  40.84071*f0;//(26+Fn)*PI/2*f0;//44.5*PI*f0;//44.5-63  23-20
	} 
}
//����fh��fl��workMode��ͨ��channel����MAX262�Ĵ�ͨ�������� 
//����ʱ��Ƶ��fclk 

float bp_WorkFclk(float fh,float fl,u8 workMode,u8 channel)    
{   
  float f0,Q;    
	u8 Qn, Fn ;//
	
  f0 = sqrt(fh*fl);   //��������Ƶ��
	Q = f0/(fh-fl);     //����Ʒ������
  printf("\r\n����Ƶ�ʣ�%f",f0);
	printf("\r\nQֵ��%f",Q);
	Qn = getQn(workMode, Q);//����Qn
	
//	Fn = getFn(15372,f0,workMode); 
	printf("\r\nQnֵ��%d,Fn��ֵ��%d",Qn,Fn);
	if(channel==channelA) 
	{
		write(0,workMode);         
		setAf(0);  //Ҫ���ô�Χ�ɵ������ص��ݵĲ���Ƶ�ʲ���ȡ���ֵ. 
		setAQ(Qn);  
	}     
	else      
	{ 
    write(8,workMode);         
		setBf(0);     //Ҫ���ô�Χ�ɵ������ص��ݵĲ���Ƶ�ʲ���ȡ���ֵ.         
		setBQ(Qn);       
	}
	if(workMode==modeTwo)
	{        
		return 28.87872*f0;//(Fn+26)*1.11072*f0;//31.46625*PI*f0;     //�����˲���ʱ��Ƶ��
	}
	else
	{		
    return 40.84071*f0;//(26+Fn)*PI/2*f0;//44.5*PI*f0; 
	}
}
//����f0��Q��workMode��ͨ��channel����MAX262���ݲ��������� 
//����ʱ��Ƶ��fclk 
float bs_WorkFclk(float f0,float Q,u8 workMode,u8 channel) 
{ 
	u8 Qn; 
    if (workMode==modeTwo)              
	{
		Qn = (u8)(128-90.51/Q);          
	}
	else
	{		
    Qn = (u8)(128-64/Q); 
	}		
	if(channel==channelA)		
	{ 
    write(0,workMode);         
		setAf(63);  //Ҫ���ô�Χ�ɵ������ص��ݵĲ���Ƶ�ʲ���ȡ���ֵ.         
		setAQ(Qn);       
	}     
	else      
	{ 
		write(8,workMode);         
		setBf(63);     //Ҫ���ô�Χ�ɵ������ص��ݵĲ���Ƶ�ʲ���ȡ���ֵ.         
		setBQ(Qn);       
	}   
	if(workMode==modeTwo)        
	{
		return  31.46625*PI*f0;     
	}
	else
	{		
    return  44.5*PI*f0; 
	}
}
//����f0��Q��ͨ��channel����MAX262��ȫͨ�������� 
//����ʱ��Ƶ��fclk 
float ap_WorkFclk(float f0,float Q,u8 channel) 
{    
	u8 Qn; 
    Qn = (u8)(128-64/Q);   
  
	if(channel==channelA) 
   { 
        write(0,modeFour);         
		setAf(63);  //Ҫ���ô�Χ�ɵ������ص��ݵĲ���Ƶ�ʲ���ȡ���ֵ.         
		setAQ(Qn);       
	}     
	else 
    { 
        write(8,modeFour);         
		setBf(63);     //Ҫ���ô�Χ�ɵ������ص��ݵĲ���Ƶ�ʲ���ȡ���ֵ.         
		setBQ(Qn);       
	}   
	return  44.5*PI*f0; 
} 
	

//u8 FnFin_config(float f0,u8 workMode)//����f0����fn
//{
//	u8 i;
//	
//	for (i = 0; i < 64; i++)
//	{
//		if(workMode==modeTwo)
//		{        
//			//if( (i+26)*1.11072*f0 )
//		}
//		else
//		{		
//			//if( (26+i)*PI/2*f0 )
//		} 
//	}
//	
//}
/*
��С��26*1.11072 = 28.87872
      26*PI/2    = 40.8407
     


*/
