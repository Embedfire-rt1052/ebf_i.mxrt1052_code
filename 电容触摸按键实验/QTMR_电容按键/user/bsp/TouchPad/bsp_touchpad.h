#ifndef __TOUCHPAD_H_
#define	__TOUCHPAD_H_

#include "fsl_common.h"
 
/*********************************************************
 * TPAD 定时器的通道，输入捕抓，端口、引脚号宏定义
 *********************************************************/

#define TPAD_QTMR_BASEADDR          		TMR3
#define TPAD_QTMR_INPUT_CAPTURE_CHANNEL  	kQTMR_Channel_3
#define TPAD_QTMR_CounterInputPin        	kQTMR_Counter3InputPin
#define TPAD_QTMR_IOMUXC			IOMUXC_GPIO_AD_B1_03_QTIMER3_TIMER3

#define TPAD_GPIO 				GPIO1
#define TPAD_GPIO_PIN 		    		(19U)
#define TPAD_IOMUXC			      	IOMUXC_GPIO_AD_B1_03_GPIO1_IO19


/* 中断号和中断处理程序 */
#define TPAD_QTMR_IRQ_ID 			TMR3_IRQn
#define TPAD_QTMR_IRQ_HANDLER 			TMR3_IRQHandler


/* 获取QTMR时钟源 */
#define TPAD_QTMR_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_IpgClk)


/* 输入捕获引脚的PAD配置 */
#define TMR_INPUT_PAD_CONFIG_DATA       (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_0_OUTPUT_DRIVER_DISABLED| \
                                        SPEED_2_MEDIUM_100MHz| \
                                        ODE_1_OPEN_DRAIN_ENABLED| \
                                        PKE_0_PULL_KEEPER_DISABLED| \
                                        PUE_0_KEEPER_SELECTED| \
                                        PUS_3_22K_OHM_PULL_UP| \
                                        HYS_1_HYSTERESIS_ENABLED)   
  /* 配置说明 : */
  /* 转换速率: 转换速率慢
  驱动强度: 关闭
  速度配置 : medium(100MHz)
  开漏配置: 使能 
  拉/保持器配置: 关闭
  拉/保持器选择: 保持器使能
  上拉/下拉选择: 22K欧姆上拉
  滞回器配置: 开启 （仅输入时有效，施密特触发器，使能后可以过滤输入噪声）*/


/* 输入捕获引脚的PAD配置 */
#define GPIO_PAD_CONFIG_DATA       (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_2_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_1_PULL_KEEPER_ENABLED| \
                                        PUE_1_PULL_SELECTED| \
                                        PUS_1_47K_OHM_PULL_UP| \
                                        HYS_0_HYSTERESIS_DISABLED)   
    /* 配置说明 : */
    /* 转换速率: 转换速率慢
      驱动强度: R0/6 
      带宽配置 : medium(100MHz)
      开漏配置: 关闭 
      拉/保持器配置: 开启
      拉/保持器选择: 上下拉
      上拉/下拉选择: 4.7K上拉
      滞回器配置: 关闭 */ 

        
        
/*********************************************************
 * 函数声明
 *********************************************************/
uint8_t TPAD_Init(void);
uint8_t TPAD_Scan(uint8_t mode);
 

#endif /* __TOUCHPAD_H_ */


