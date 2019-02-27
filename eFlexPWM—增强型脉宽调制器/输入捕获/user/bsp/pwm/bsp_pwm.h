#ifndef __BSP_PWM_H
#define __BSP_PWM_H

#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"
#include "fsl_pwm.h"
#include "fsl_xbara.h"
#include "pad_config.h"



/*********************************************************
 * PWM1_PWMA00/PWM1_PWMB00  GPIO端口、引脚号及IOMUXC复用宏定义
 
使用i.MX RT1052-Pro底板：
 *  FLEXPWM1_PWMA00-----GPIO_SD_B0_00-----(CN5, 68)
 *  FLEXPWM1_PWMB00-----GPIO_SD_B0_01-----(CN5, 70)
 *  PWM1_PWMX0_GPIO-----GPIO_AD_B0_02-----(CN5, 14)
 *引脚功能               引脚标号        引脚在开发板上对应位置
 (CN5, 70) 含义是引脚连接到开发板CN5排针的第70脚

使用i.MX RT1052-Mini底板

 *  FLEXPWM1_PWMA00-----GPIO_SD_B0_00-----(CN4, 68)
 *  FLEXPWM1_PWMB00-----GPIO_SD_B0_01-----(CN4, 70)
 *  PWM1_PWMX0_GPIO-----GPIO_AD_B0_02-----(CN4, 14)
 *   引脚功能             引脚标号       引脚在开发板上对应位置
 (CN4, 70) 含义是引脚连接到开发板CN4排针的第70脚 

 *********************************************************/
/* FLEXPWM1_PWMA00 */
#define PWM1_PWMA00_GPIO      GPIO3
#define PWM1_PWMA00_GPIO_PIN  (12U)
#define PWM1_PWMA00_IOMUXC    IOMUXC_GPIO_SD_B0_00_FLEXPWM1_PWMA00  

/* FLEXPWM1_PWMB00 */
#define PWM1_PWMB00_GPIO       GPIO3
#define PWM1_PWMB00_GPIO_PIN   (13U)
#define PWM1_PWMB00_IOMUXC     IOMUXC_GPIO_SD_B0_01_FLEXPWM1_PWMB00    

/*FLEXPWM1_PWMX0*/
#define PWM1_PWMX0_GPIO       GPIO1
#define PWM1_PWMX0_GPIO_PIN   (2U)
#define PWM1_PWMX0_IOMUXC     IOMUXC_GPIO_AD_B0_02_FLEXPWM1_PWMX00      
   


/* 宏定义， 定义 PWM 基地址 */
#define BOARD_PWM_BASEADDR PWM1
/*宏定义， 得到IpgCLK时钟频率*/
#define PWM_SRC_CLK_FREQ CLOCK_GetFreq(kCLOCK_IpgClk)

/*能够设置的PWM范围与时钟源的选择、时钟分频有关，以本实验为例：
*时钟频率：132MHz
*时钟分频：3 ，计数时钟频率：44MHz 
*计数寄存器是16位，最大计数约为65535
*输出PWM 最低频率 = 44000000/65535 约 680Hz
*/

#define PWM_frequency_Hz 1000
#define PWM_duty_Cycle_Percent 50  //50 表示占空比50%
#define PWM_deadtime 65000   // 单位 ns (为了便于在示波器上观察，死区时间设置较大)

/*******************************************************************************
 * PWM1_PWMA00/PWM1_PWMB00 引脚配置
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


/* 输入捕获引脚的PAD配置 */
#define TMR_INPUT_PAD_CONFIG_DATA       (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_2_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_1_PULL_KEEPER_ENABLED| \
                                        PUE_0_KEEPER_SELECTED| \
                                        PUS_2_100K_OHM_PULL_UP| \
                                        HYS_0_HYSTERESIS_DISABLED)   
    /* 配置说明 : */
    /* 转换速率: 转换速率慢
      驱动强度: R0/6 
      带宽配置 : medium(100MHz)
      开漏配置: 关闭 
      拉/保持器配置: 开启
      拉/保持器选择: 保持器
      上拉/下拉选择: 100K上拉
      滞回器配置: 关闭 */ 



void PWM_gpio_config(void);
void Capture_gpio_config(void);
void PWM_config(void);
void Capture_config(void);
//static void PWM_DRV_Init3PhPwm(void);

#endif /* __BSP_PWM_H */



