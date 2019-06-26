#ifndef __485_H
#define __485_H


#include "pad_config.h" 
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"  

/*485 发送、接收控制引脚定义
*为高电平，接收使能
*为低电平，发送使能
*/

#define 485_RE_GPIO             GPIO1
#define 485_RE_GPIO_PIN         (26U)
#define 485_RE_IOMUXC           IOMUXC_GPIO_AD_B1_10_GPIO1_IO26




/* 485 发送、接收控制引脚的PAD配置 */
#define 485_RE_PAD_CONFIG_DATA          (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_2_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_0_PULL_KEEPER_DISABLED| \
                                        PUE_0_KEEPER_SELECTED| \
                                        PUS_0_100K_OHM_PULL_DOWN| \
                                        HYS_0_HYSTERESIS_DISABLED)   
    /* 配置说明 : 
      转换速率: 转换速率慢
      驱动强度: R0/6 
      带宽配置 : medium(100MHz)
      开漏配置: 关闭 
      拉/保持器配置: 关闭
      拉/保持器选择: 保持器（上面已关闭，配置无效）
      上拉/下拉选择: 100K欧姆下拉（上面已关闭，配置无效）*/

#endif /* __485_H */