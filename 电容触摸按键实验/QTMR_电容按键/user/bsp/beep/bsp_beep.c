/**
******************************************************************
* @file    bsp_beep.c
* @author  fire
* @version V1.0
* @date    2018-xx-xx
* @brief   板载蜂鸣器应用函数接口
******************************************************************
* @attention
*
* 实验平台:野火  i.MXRT1052开发板 
* 论坛    :http://www.firebbs.cn
* 淘宝    :http://firestm32.taobao.com
*
******************************************************************
*/

#include "fsl_gpio.h"  
#include "./bsp/beep/bsp_beep.h" 


void Beep_GPIO_Config(void)
{		
  /* 定义gpio初始化配置结构体 */
  gpio_pin_config_t beep_config;
  
  /* 设置底板的蜂鸣器引脚的复用模式 */
  IOMUXC_SetPinMux(
                   BOARD_BEEP_IOMUXC,    /* 配置为普通IO */
                   0U);                      /* 不使用SION功能 */
  
   /* 设置BEEP引脚的IOMUXC PAD配置 */ 
  IOMUXC_SetPinConfig(BOARD_BEEP_IOMUXC, BEEP_PAD_CONFIG_DATA);
  
  /* 底板的蜂鸣器，GPIO配置 */       
  beep_config.direction = kGPIO_DigitalOutput; //输出模式
  beep_config.outputLogic =  0;                //默认低电平
  beep_config.interruptMode = kGPIO_NoIntmode; //不使用中断
  
  /* 初始化 BEEP GPIO. */
  GPIO_PinInit(BOARD_BEEP_GPIO, BOARD_BEEP_GPIO_PIN, &beep_config);	
}
/*********************************************END OF FILE**********************/
