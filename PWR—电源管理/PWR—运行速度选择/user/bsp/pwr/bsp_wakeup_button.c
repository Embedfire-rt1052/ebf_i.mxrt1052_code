/**
  ******************************************************************
  * @file    bsp_wakeup_button.c
  * @author  fire
  * @version V1.1
  * @date    2018-xx-xx
  * @brief   wakeup_button用函数接口
  ******************************************************************
  * @attention
  *
  * 实验平台:野火  i.MXRT1052开发板
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************
  */
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"

#include "pad_config.h"
#include "./pwr/bsp_wakeup_button.h"


/**
  * @brief  初始化唤醒按键的IO
  * @param  无
  * @retval 无
  */
void Wakeup_Button_Config(void)
{
    /* 定义输入开关引脚的init结构 */
    gpio_pin_config_t swConfig;

		/** 底板或核心板的	WAKEUP	按键 配置 **/
    swConfig.direction = kGPIO_DigitalInput;   //输出模式
    swConfig.interruptMode = 0;                //默认低电平
    swConfig.outputLogic = kGPIO_IntRisingEdge;//上升沿中断

    /* Init输入开关GPIO. */
    GPIO_PinInit(WAKEUP_BUTTON_GPIO, WAKEUP_BUTTON_GPIO_PIN, &swConfig);
    GPIO_EnableInterrupts(WAKEUP_BUTTON_GPIO, 1U << WAKEUP_BUTTON_GPIO_PIN);

}





