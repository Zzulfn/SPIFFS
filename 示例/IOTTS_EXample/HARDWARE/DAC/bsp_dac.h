/**********************************************************
* @ File name -> bsp_dac.h
* @ Version   -> V1.0
* @ Date      -> 26-06-2017
* @ Brief     -> DAC相关函数头文件

 V1.*
* @ Revise    ->
**********************************************************/

#ifndef _bsp_dac_h_
#define _bsp_dac_h_

/**********************************************************
                     外部函数头文件
         应用到不同的外设头文件请在这里修改即可                        
**********************************************************/

#include "sys.h"

/*
 * DAC模块输入参数结构体
 */
typedef struct
{
	uint16_t  DAC_vol;                  /*!<   DAC输出控制电压值 */

	uint8_t  DAC_Channel_En;           /*!<   DAC输出使能 */ 
	
}SOFT_Dac_Module_Input;

/*
 * DAC模块输出参数结构体
 */
typedef struct
{
	uint8_t  DAC_Channel_Module_Status;          /*!<   DAC输出模块状态 */

}SOFT_Dac_Module_Output;

/**********************************************************
                       外部功能函数                      
**********************************************************/
void BSP_DAC_Init(void);
void Dac1_Set_Vol(u16 vol);
void Dac2_Set_Vol(u16 vol);
SOFT_Dac_Module_Output DAC_Manage( u8 DAC_Channel_Sel,SOFT_Dac_Module_Input value);
#endif

