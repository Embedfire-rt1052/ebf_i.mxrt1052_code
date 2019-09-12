#ifndef __BSP_KEY_IT_H
#define __BSP_KEY_IT_H

#include "fsl_common.h"

/******************************************************************
 * 宏
 ******************************************************************/
//WAUP按键
#define CORE_BOARD_WAUP_KEY_GPIO          GPIO5
#define CORE_BOARD_WAUP_KEY_GPIO_PIN      (0U)
#define CORE_BOARD_WAUP_KEY_IOMUXC        IOMUXC_SNVS_WAKEUP_GPIO5_IO00
#define CORE_BOARD_WAUP_KEY_NAME          "CORE_BORE_WAUP_KEY"
#define CORE_BOARD_WAUP_KEY_ID            0
//中断相关，IRQ中断号及IRQHandler中断服务函数
#define CORE_BOARD_WAUP_KEY_IRQ           GPIO5_Combined_0_15_IRQn
#define CORE_BOARD_WAUP_KEY_IRQHandler    GPIO5_Combined_0_15_IRQHandler

//MODE按键
#define CORE_BOARD_MODE_KEY_GPIO          GPIO1
#define CORE_BOARD_MODE_KEY_GPIO_PIN      (5U)
#define CORE_BOARD_MODE_KEY_IOMUXC        IOMUXC_GPIO_AD_B0_05_GPIO1_IO05
#define CORE_BOARD_MODE_KEY_NAME          "CORE_BORE_MODE_KEY"
#define CORE_BOARD_MODE_KEY_ID            1
//中断相关，IRQ中断号及IRQHandler中断服务函数
#define CORE_BOARD_MODE_KEY_IRQ           GPIO1_Combined_0_15_IRQn
#define CORE_BOARD_MODE_KEY_IRQHandler    GPIO1_Combined_0_15_IRQHandler

/******************************************************************
 * 声明
 ******************************************************************/

/* 按键是否被按下的中断标志 */
extern __IO bool g_KeyDown[2];


void Key_IT_GPIO_Config(void);

#endif /* __BSP_KEY_IT_H */




