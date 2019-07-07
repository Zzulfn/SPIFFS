/**
  ******************************************************************************
  * @file    bsp_adc.c
  * @author  Jack Lu
  * @version V1.0
  * @date    2017-12-28
  * @brief   
  ******************************************************************************
	*/
	
#include "bsp_adc.h"
#include "bsp_usart1.h"
/********************************
 * ��������BSP_ADC_Init
 * ����  ��AD��ʼ������
 * ����  ����
 * ���  ����
 *********************************/
uint16_t ADC_ConvertedValue[400];

void BSP_ADC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;

	/* Enable DMA clock *//* Enable ADC1 and GPIOC clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
	
	/* Configure AD pin  as analog input */
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	/* DMA channel1 configuration */
	DMA_DeInit(DMA1_Channel1);
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	 			//ADC��ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;	//�ڴ��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 400;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;						//�����ַ�̶�
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  										//�ڴ��ַ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//����
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;															//ѭ������
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	/* Enable DMA channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	/* ADC1 configuration */	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;			//����ADCģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 	 				//��ֹɨ��ģʽ��ɨ��ģʽ���ڶ�ͨ���ɼ�
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;			//��������ת��ģʽ������ͣ�ؽ���ADCת��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//��ʹ���ⲿ����ת��
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 	//�ɼ������Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 8;	 								//Ҫת����ͨ����Ŀ8
	ADC_Init(ADC1, &ADC_InitStructure);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);    //����ADCʱ�ӣ�ΪPCLK2��8��Ƶ����9MHz
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_239Cycles5);     //����ADC1��ͨ��11Ϊ55.	5���������ڣ�����Ϊ1 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 2, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 3, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 4, ADC_SampleTime_239Cycles5);     //����ADC1��ͨ��11Ϊ55.	5���������ڣ�����Ϊ1 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 5, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 6, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 7, ADC_SampleTime_239Cycles5);     //����ADC1��ͨ��11Ϊ55.	5���������ڣ�����Ϊ1 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 8, ADC_SampleTime_239Cycles5);
	
	ADC_DMACmd(ADC1, ENABLE);     // Enable ADC1 DMA 
	ADC_Cmd(ADC1, ENABLE);        // Enable ADC1   
	ADC_ResetCalibration(ADC1);   //��λУ׼�Ĵ���	
	while(ADC_GetResetCalibrationStatus(ADC1));     //�ȴ�У׼�Ĵ�����λ��� 
	ADC_StartCalibration(ADC1);   // ADCУ׼	
	while(ADC_GetCalibrationStatus(ADC1));      // �ȴ�У׼���	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);     // ����û�в����ⲿ����������ʹ���������ADCת�� 
}

/********************************
 * ��������BSP_GetAdcValue
 * ����  ����ȡ��AD��
 * ����  ����
 * ���  ����
 *********************************/
uint16_t BSP_GetAdcValue(void)
{ 
	u32 BattNum=0;
	uint32_t i,ADC_VAL=0;
	float ADC_ConvertedValueLocal; 
	
	for(i=0;i<400;i=i+8)BattNum+=ADC_ConvertedValue[i];
	ADC_VAL=BattNum/50;
  printf("\r\nADC0 = %d",ADC_VAL);
 	ADC_ConvertedValueLocal=(float)ADC_VAL/4096*3.3;
 	printf(",ʵ�ʵ�ѹ��ѹ = %f v\r\n",(ADC_ConvertedValueLocal*2-2.5)/0.24);
	
	BattNum=0;
	for(i=1;i<400;i=i+8)BattNum+=ADC_ConvertedValue[i];
	ADC_VAL=BattNum/50;
  printf("\r\nADC1 = %d",ADC_VAL);
 	ADC_ConvertedValueLocal=(float)ADC_VAL/4096*3.3;
 	printf(",ʵ�ʵ�ѹ��ѹ = %f v\r\n",ADC_ConvertedValueLocal);
	
	BattNum=0;
	for(i=2;i<400;i=i+8)BattNum+=ADC_ConvertedValue[i];
	ADC_VAL=BattNum/50;
  printf("\r\nADC2 = %d",ADC_VAL);
 	ADC_ConvertedValueLocal=(float)ADC_VAL/4096*3.3;
 	printf(",ʵ�ʵ�ѹ��ѹ = %f v\r\n",ADC_ConvertedValueLocal);
	
	BattNum=0;
	for(i=3;i<400;i=i+8)BattNum+=ADC_ConvertedValue[i];
	ADC_VAL=BattNum/50;
  printf("\r\nADC3 = %d",ADC_VAL);
 	ADC_ConvertedValueLocal=(float)ADC_VAL/4096*3.3;
 	printf(",ʵ�ʵ�ѹ��ѹ = %f v\r\n",ADC_ConvertedValueLocal);
	
	BattNum=0;
	for(i=4;i<400;i=i+8)BattNum+=ADC_ConvertedValue[i];
	ADC_VAL=BattNum/50;
  printf("\r\nADC4 = %d",ADC_VAL);
 	ADC_ConvertedValueLocal=(float)ADC_VAL/4096*3.3;
 	printf(",ʵ�ʵ�ѹ��ѹ = %f v\r\n",ADC_ConvertedValueLocal);
	
	BattNum=0;
	for(i=5;i<400;i=i+8)BattNum+=ADC_ConvertedValue[i];
	ADC_VAL=BattNum/50;
  printf("\r\nADC5 = %d",ADC_VAL);
 	ADC_ConvertedValueLocal=(float)ADC_VAL/4096*3.3;
 	printf(",ʵ�ʵ�ѹ��ѹ = %f v\r\n",ADC_ConvertedValueLocal);
	
	BattNum=0;
	for(i=6;i<400;i=i+8)BattNum+=ADC_ConvertedValue[i];
	ADC_VAL=BattNum/50;
  printf("\r\nADC6 = %d",ADC_VAL);
 	ADC_ConvertedValueLocal=(float)ADC_VAL/4096*3.3;
 	printf(",ʵ�ʵ�ѹ��ѹ = %f v\r\n",ADC_ConvertedValueLocal);
	
	BattNum=0;
	for(i=7;i<400;i=i+8)BattNum+=ADC_ConvertedValue[i];
	ADC_VAL=BattNum/50;
  printf("\r\nADC7 = %d",ADC_VAL);
 	ADC_ConvertedValueLocal=(float)ADC_VAL/4096*3.3;
 	printf(",ʵ�ʵ�ѹ��ѹ = %f v\r\n",ADC_ConvertedValueLocal);
	
	return ADC_VAL;
}

/**************************************************
 * ��������ADC_Manage
 * ����  ��8·ADC����ģ��
 * ����  ��value ADC��������ṹ�������������ļ�
 * ���  ��temp ADC��������ṹ�����,������ļ�
 **************************************************/
SOFT_Adc_Module_Output ADC_Manage(ADC_Channel_Sel ADC_In_Sel,SOFT_Adc_Module_Input value)
{
	SOFT_Adc_Module_Output temp;
	u32 BattNum=0;
	uint32_t i,ADC_VAL=0;
	float ADC_ConvertedValueLocal; 
	
	if(value.ADC_ChannelInput_En==1)
	{
		BattNum=0;				
		for(i=ADC_In_Sel;i<400;i=i+8)BattNum+=ADC_ConvertedValue[i];
		ADC_VAL=BattNum/50;//ADC�˲�
		ADC_ConvertedValueLocal=(float)ADC_VAL/4096*3.3;//����ʵ�ʵ�ѹֵ
	  temp.ADC_Channel_Data=ADC_VAL;//����ͨ��ADCֵ
		
		DEBUG("ADC = %d",ADC_VAL);
		DEBUG("ʵ�ʵ�ѹ��ѹ = %f v\r\n",(ADC_ConvertedValueLocal*2-2.5)/0.24);

		if(value.ADC_H_lim>0)//��������ֵ�Ƿ�Ϊ0
		{
			if(ADC_VAL>value.ADC_H_lim)temp.ADC_Out_Status=1;//�����趨��ֵ��� 1
			else if(ADC_VAL<=value.ADC_L_lim)temp.ADC_Out_Status=0;//С���趨��ֵ��� 0
			else temp.ADC_Out_Status=2;
		}
		else temp.ADC_Out_Status=-1;
	}
	else 
	{
		temp.ADC_Channel_Data=0;
		temp.ADC_Out_Status=-1;
	}
	temp.ADC_Channel_Module_Status=value.ADC_ChannelInput_En;
	return temp;
}
