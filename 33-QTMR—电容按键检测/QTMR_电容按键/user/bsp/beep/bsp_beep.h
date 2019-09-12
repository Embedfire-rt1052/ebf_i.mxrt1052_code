#ifndef __BEEP_H_
#define	__BEEP_H_

#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "pad_config.h" 

/*********************************************************
 * BEEP GPIO端口、引脚号及IOMUXC复用宏定义
 *********************************************************/
#define BOARD_BEEP_GPIO 		    GPIO1
#define BOARD_BEEP_GPIO_PIN 		    (3U)
#define BOARD_BEEP_IOMUXC	            IOMUXC_GPIO_AD_B0_03_GPIO1_IO03


/* 设置BEEP引脚使用的PAD配置 */
#define BEEP_PAD_CONFIG_DATA            (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_2_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_0_PULL_KEEPER_DISABLED| \
                                        PUE_0_KEEPER_SELECTED| \
                                        PUS_0_100K_OHM_PULL_DOWN| \
                                        HYS_0_HYSTERESIS_DISABLED)   
    /* 配置说明 : */
    /* 转换速率: 转换速率慢
      驱动强度: R0/6 
      带宽配置 : medium(100MHz)
      开漏配置: 关闭 
      拉/保持器配置: 关闭
      拉/保持器选择: 保持器（上面已关闭，配置无效）
      上拉/下拉选择: 100K欧姆下拉（上面已关闭，配置无效）
      滞回器配置: 关闭 */  


/******************** BEEP 函数宏定义 **************************/
#define BEEP_ON   GPIO_PinWrite(BOARD_BEEP_GPIO, BOARD_BEEP_GPIO_PIN, 1U);
#define BEEP_OFF  GPIO_PinWrite(BOARD_BEEP_GPIO, BOARD_BEEP_GPIO_PIN, 0U);


/******************** BEEP 函数声明********************/
void Beep_GPIO_Config(void);


#endif /* __BEEP_H_ */
