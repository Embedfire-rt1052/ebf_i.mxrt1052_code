#ifndef __BSP_PWM_H
#define __BSP_PWM_H

#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"
#include "fsl_pwm.h"
#include "fsl_xbara.h"
#include "pad_config.h"



/*********************************************************
 * PWM1 输入端口、引脚号及IOMUXC复用宏定义
 
使用i.MX RT1052-Pro底板：
 *  FLEXPWM1_PWMA0-----GPIO_SD_B0_00-----(CN5, 68)
 *  FLEXPWM1_PWMB0-----GPIO_SD_B0_01-----(CN5, 70)
 *  FLEXPWM1_PWMA1-----GPIO_SD_B0_02-----(CN5,65)
 *  FLEXPWM1_PWMA2-----GPIO_SD_B0_04-----(CN5,69)
 *引脚功能            引脚标号        引脚在开发板上对应位置
 (CN5, 68) 含义是引脚连接到开发板CN5排针的第68脚


使用i.MX RT1052-Mini底板

 *  FLEXPWM1_PWMA0-----GPIO_SD_B0_00-----(CN4, 68)
 *  FLEXPWM1_PWMB0-----GPIO_SD_B0_01-----(CN4, 70)
 *  FLEXPWM1_PWMA1-----GPIO_SD_B0_02-----(CN4,65)
 *  FLEXPWM1_PWMA2-----GPIO_SD_B0_04-----(CN4,69)
 *引脚功能         引脚标号     引脚在开发板上对应位置
 (CN4, 68) 含义是引脚连接到开发板CN4排针的第68脚 

 *********************************************************/
/* FLEXPWM1_PWMA0  (CN4, 68) */
#define PWM1_PWMA00_GPIO      GPIO3
#define PWM1_PWMA00_GPIO_PIN  (12U)
#define PWM1_PWMA00_IOMUXC    IOMUXC_GPIO_SD_B0_00_FLEXPWM1_PWMA00  

/* FLEXPWM1_PWMB0、（CN4, 70)*/
#define PWM1_PWMB00_GPIO       GPIO3
#define PWM1_PWMB00_GPIO_PIN   (13U)
#define PWM1_PWMB00_IOMUXC     IOMUXC_GPIO_SD_B0_01_FLEXPWM1_PWMB00    

/* FLEXPWM1_PWMA1  （CN4,65） */
#define PWM1_PWMA01_GPIO       GPIO3
#define PWM1_PWMA01_GPIO_PIN   (14U)
#define PWM1_PWMA01_IOMUXC     IOMUXC_GPIO_SD_B0_02_FLEXPWM1_PWMA01   

/*FLEXPWM1_PWMA2   （CN4,69） */
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



#define BOARD_PWM_BASEADDR PWM1      //定义使用的 PWM 模块
#define kPWM_Module  kPWM_Module_0   //使用的module 每个PWM拥有4个 module
#define kPWM_Control_Module  kPWM_Control_Module_0 //

#define PWM_A_or_B kPWM_PwmA  //只有PWM1可选 kPWM_PwmX，其他PWM 2到4 只能选择kPWM_PwmA或kPWM_PwmB


/*获取PWM时钟频率*/
#define PWM_SRC_CLK_FREQ CLOCK_GetFreq(kCLOCK_IpgClk)


/*能够设置的PWM范围与时钟源的选择、时钟分频有关，以本实验为例：
*时钟频率：132MHz
*时钟分频：3 ，计数时钟频率：44MHz 
*计数寄存器是16位，最大计数约为65535
*输出PWM 最低频率 = 44000000/65535 约 680Hz
*
*/
#define PWM_frequency_Hz 1000
#define PWM_duty_Cycle_Percent 50  //50 表示占空比50%


void PWM_gpio_config(void);
void PWM_config(void);


#endif /* __BSP_PWM_H */



