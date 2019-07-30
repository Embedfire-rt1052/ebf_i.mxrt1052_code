#ifndef __MPU_EXTI_H
#define	__MPU_EXTI_H

//#include "stm32f4xx.h"
/******************************************************************
 * 宏
 ******************************************************************/
//WAUP按键
#define MPU_EXTI_GPIO          GPIO1
#define MPU_EXTI_GPIO_PIN      (14U)
#define MPU_EXTI_IOMUXC        IOMUXC_GPIO_AD_B0_14_GPIO1_IO14
//中断相关，IRQ中断号及IRQHandler中断服务函数
#define MPU_EXTI_IRQ           GPIO1_Combined_0_15_IRQn
#define MPU_EXTI_IRQHandler    GPIO1_Combined_0_15_IRQHandler


/* 按键是否被按下的中断标志 */
//extern __IO bool g_KeyDown[2];

void EXTI_MPU_Config(void);

#endif /* __EXTI_H */
