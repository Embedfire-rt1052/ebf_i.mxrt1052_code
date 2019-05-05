/**
  ******************************************************************
  * @file    main.c
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
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"  
	
#include "pad_config.h"  
#include "./bsp/beep/bsp_beep.h" 

void Beep_GPIO_Config(void)
{		
		/* 定义gpio初始化配置结构体 */
    gpio_pin_config_t beep_config;
		
    /** 核心板的LED灯，IOMUXC MUX及PAD配置 **/
		
    /* 设置引脚的复用模式 */
		IOMUXC_SetPinMux(
      BOARD_BEEP_IOMUXC,    /* 配置为普通IO */
      0U);                      /* 不使用SION功能 */
		
		/*设置引脚功能*/
		IOMUXC_SetPinConfig(
      BOARD_BEEP_IOMUXC,        
      SRE_0_SLOW_SLEW_RATE|
      DSE_6_R0_6|
      SPEED_2_MEDIUM_100MHz|
      ODE_0_OPEN_DRAIN_DISABLED|
      PKE_1_PULL_KEEPER_ENABLED|
      PUE_1_PULL_SELECTED|
      PUS_1_47K_OHM_PULL_UP|
      HYS_0_HYSTERESIS_DISABLED
      );	
	
    /* 配置说明 : */
		/* 转换速率: 转换速率慢
			驱动强度: R0/6 
			带宽配置 : medium(100MHz)
			开漏配置: 关闭 
			拉/保持器配置: 打开
			拉/保持器选择: 上下拉
			上拉/下拉选择: 47K欧姆上拉
			滞回器配置: 关闭 */     
			
     /** 底板的蜂鸣器，GPIO配置 **/       
    beep_config.direction = kGPIO_DigitalOutput; //输出模式
    beep_config.outputLogic =  0;                //默认低电平
    beep_config.interruptMode = kGPIO_NoIntmode; //不使用中断
		
		/* 初始化 BEEP GPIO. */
    GPIO_PinInit(BOARD_BEEP_GPIO, BOARD_BEEP_GPIO_PIN, &beep_config);	
}
/*********************************************END OF FILE**********************/
