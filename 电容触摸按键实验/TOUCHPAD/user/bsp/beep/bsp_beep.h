#ifndef __BEEP_H_
#define	__BEEP_H_

#include "fsl_common.h"

/*********************************************************
 * BEEP GPIO端口、引脚号及IOMUXC复用宏定义
 *********************************************************/
#define BOARD_BEEP_GPIO 				    GPIO1
#define BOARD_BEEP_GPIO_PIN 		    (3U)
#define BOARD_BEEP_IOMUXC			      IOMUXC_GPIO_AD_B0_03_GPIO1_IO03

/******************** BEEP 函数宏定义 **************************/
#define BEEP_ON   GPIO_PinWrite(BOARD_BEEP_GPIO, BOARD_BEEP_GPIO_PIN, 1U);
#define BEEP_OFF  GPIO_PinWrite(BOARD_BEEP_GPIO, BOARD_BEEP_GPIO_PIN, 0U);


/******************** BEEP 函数声明**********************/
void Beep_GPIO_Config(void);


#endif /* __BEEP_H_ */
