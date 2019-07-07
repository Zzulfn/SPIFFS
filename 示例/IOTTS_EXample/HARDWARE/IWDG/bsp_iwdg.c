/**
  ******************************************************************************
  * @file    bsp_iwdg.c
  * @author  Jack Lu
  * @version V1.0
  * @date    2017-09-20
  * @brief   
  ******************************************************************************
  */
  
#include "bsp_iwdg.h"

 /************************************
 * 函数名：BSP_IWDGinit
 * 描述  ：看门狗初始化
 * 输入  ：无
 * 输出  ：无
 *************************************/
void BSP_IWDGinit(void)
{
	/*看门狗的溢出时间是t=((pre*re)/40 */
	//使能看门狗写
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	//预分频值 40k/256=156hz(6.4ms)  取值范围在2^c   1<c<9
	IWDG_SetPrescaler(IWDG_Prescaler_256);
	//设置装载值 最大值不能超过0xfff=4095
	IWDG_SetReload(2000);
	//debug模式下关闭看门狗
	DBGMCU->CR |= 0x100;
	//使能看门狗
	IWDG_Enable(); 
	//先喂狗	
	IWDG_ReloadCounter(); 	
}

/************************************
 * 函数名：BSP_IWDGFeed
 * 描述  : 看门狗喂狗操作
 * 输入  ：
 * 输出  : 
 ************************************/
void BSP_IWDGFeed(void)
{
	//喂狗操作
	IWDG_ReloadCounter();//reload 
}

/*********************************************END OF FILE**********************/
