#ifndef __BSP_PWM_H
#define __BSP_PWM_H

#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"
#include "fsl_pwm.h"
#include "fsl_xbara.h"
#include "pad_config.h"



/*********************************************************
 * UART1 GPIO端口、引脚号及IOMUXC复用宏定义
 *********************************************************/
/*GPIO_SD_B0_00 、SD1_CMD 、10k上拉、 (CN4, 68)   is configured as FLEXPWM1_PWMA00*/
#define PWM1_PWMA00_GPIO      GPIO3
#define PWM1_PWMA00_GPIO_PIN  (12U)
#define PWM1_PWMA00_IOMUXC    IOMUXC_GPIO_SD_B0_00_FLEXPWM1_PWMA00  

/* GPIO_SD_B0_01 、串有一个电阻 大小未知、    （CN4, 70)       is configured as FLEXPWM1_PWMB00 */
#define PWM1_PWMB00_GPIO       GPIO3
#define PWM1_PWMB00_GPIO_PIN   (13U)
#define PWM1_PWMB00_IOMUXC     IOMUXC_GPIO_SD_B0_01_FLEXPWM1_PWMB00    

/* GPIO_SD_B0_02  、SD1_D0、 10K上拉、（CN4,65）            is configured as FLEXPWM1_PWMA01 */
#define PWM1_PWMA01_GPIO       GPIO3
#define PWM1_PWMA01_GPIO_PIN   (14U)
#define PWM1_PWMA01_IOMUXC     IOMUXC_GPIO_SD_B0_02_FLEXPWM1_PWMA01   

/* GPIO_SD_B0_04 、SD1_D2、 （CN4,69）               is configured as FLEXPWM1_PWMA02 */
#define PWM1_PWMA02_GPIO       GPIO3
#define PWM1_PWMA02_GPIO_PIN   (16U)
#define PWM1_PWMA02_IOMUXC     IOMUXC_GPIO_SD_B0_04_FLEXPWM1_PWMA02


/*******************************************************************************
 * uart引脚配置
 ******************************************************************************/
#define PWM_PAD_CONFIG_DATA            (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_1_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_1_PULL_KEEPER_ENABLED| \
                                        PUE_0_KEEPER_SELECTED| \
                                        PUS_0_100K_OHM_PULL_DOWN| \
                                        HYS_0_HYSTERESIS_DISABLED) 
    /* 配置说明 : */
    /* 转换速率: 转换速率慢
        驱动强度: R0/6 
        带宽配置 : medium(100MHz)
        开漏配置: 关闭 
        拉/保持器配置: 使能上下拉/保持器
        拉/保持器选择: 保持器
        上拉/下拉选择: 100K欧姆下拉(选择了保持器此配置无效)
        滞回器配置: 禁止 */

/* The PWM base address */
#define BOARD_PWM_BASEADDR PWM1
#define PWM_SRC_CLK_FREQ CLOCK_GetFreq(kCLOCK_IpgClk)
void PWM_gpio_config(void);
void PWM_config(void);
static void PWM_DRV_Init3PhPwm(void);

#endif /* __BSP_PWM_H */



