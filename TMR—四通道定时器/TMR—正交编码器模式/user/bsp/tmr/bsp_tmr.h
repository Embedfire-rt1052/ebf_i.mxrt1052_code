#ifndef __BSP_TMR_H
#define __BSP_TMR_H

#include "fsl_iomuxc.h"
#include "fsl_common.h"
#include "fsl_qtmr.h"

/*********************************************************
 * TMR3 输入端口、引脚号及IOMUXC复用宏定义
 
使用i.MX RT1052-Pro底板：
 *  TMR3_CH0-----GPIO_AD_B1_00-----(CN5, 21)
 *  TMR3_CH1-----GPIO_AD_B1_01-----(CN5, 23)
 *  TMR3_CH2-----GPIO_AD_B1_02-----(CN5, 10)
 *  TMR3_CH3-----GPIO_AD_B1_03-----(CN5, 8)
 *引脚功能         引脚标号     引脚在开发板上对应位置
 (CN5, 21) 含义是引脚连接到开发板CN5排针的第21脚


使用i.MX RT1052-Mini底板

 *  TMR3_CH0-----GPIO_AD_B1_00-----(CN4, 21)
 *  TMR3_CH1-----GPIO_AD_B1_01-----(CN4, 23)
 *  TMR3_CH2-----GPIO_AD_B1_02-----(CN4, 10)
 *  TMR3_CH3-----GPIO_AD_B1_03-----(CN4, 8)
 *引脚功能         引脚标号     引脚在开发板上对应位置
 (CN4, 21) 含义是引脚连接到开发板CN4排针的第21脚 

注意：TMR定时器的一个通道可以对应到一个或者多个外部引脚，详细信息请参考
      《IMXRT1050RM》第4章External Signals and Pin Multiplexing
 *********************************************************/
#define QTMR_CH0_GPIO                GPIO1
#define QTMR_CH0_GPIO_PIN            (16U)
#define QTMR_CH0_IOMUXC              IOMUXC_GPIO_AD_B1_00_QTIMER3_TIMER0  
   

#define QTMR_CH1_GPIO                GPIO1
#define QTMR_CH1_GPIO_PIN            (18U)
#define QTMR_CH1_IOMUXC              IOMUXC_GPIO_AD_B1_02_GPIO1_IO18




#define QTMR_CH2_GPIO                GPIO1
#define QTMR_CH2_GPIO_PIN            (18U)
#define QTMR_CH2_IOMUXC              IOMUXC_GPIO_AD_B1_02_QTIMER3_TIMER2

#define QTMR_CH3_GPIO                GPIO1
#define QTMR_CH3_GPIO_PIN            (19U)
#define QTMR_CH3_IOMUXC              IOMUXC_GPIO_AD_B1_03_QTIMER3_TIMER3




/* PWM输出引脚的PAD配置 */
#define TMR_PWM_OUTPUT_PAD_CONFIG_DATA       (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_2_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_1_PULL_KEEPER_ENABLED| \
                                        PUE_1_PULL_SELECTED| \
                                        PUS_2_100K_OHM_PULL_UP| \
                                        HYS_0_HYSTERESIS_DISABLED) 

/* 输入引脚的PAD配置 */
#define TMR_Quadrature_Count_PAD_CONFIG_DATA       (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_2_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_1_PULL_KEEPER_ENABLED| \
                                        PUE_1_PULL_SELECTED| \
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

/* 定义 TMR 定时器的通道 */
#define QTMR_BASEADDR TMR3
#define QTMR_PWM_CHANNEL_0 kQTMR_Channel_0
#define QTMR_PWM_CHANNEL_1 kQTMR_Channel_1
#define QTMR_PWM_CHANNEL_2 kQTMR_Channel_2
#define QTMR_PWM_CHANNEL_3 kQTMR_Channel_3

/*定义输出pwm频率和占空比*/
#define TMR3_CH0_PWM_FREQUENCY 50000  //5000 表示频率为5000Hz,
#define TMR3_CH0_PWM_DUTYCYCLE 5      //5  代表占空比为5%

#define TMR3_CH1_PWM_FREQUENCY 50000 
#define TMR3_CH1_PWM_DUTYCYCLE 30    

#define TMR3_CH2_PWM_FREQUENCY 50000 
#define TMR3_CH2_PWM_DUTYCYCLE 60 

#define TMR3_CH3_PWM_FREQUENCY 50000 
#define TMR3_CH3_PWM_DUTYCYCLE 80 



/* 中断号和中断服务函数定义 */
#define QTMR3_IRQ_ID TMR3_IRQn
#define QTMR3_IRQ_HANDLER TMR3_IRQHandler

/* 得到TMR定时器的时钟频率 */
#define QTMR_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_IpgClk)

void TMR_GPIO_Config(void);
void TMR_Init(void);    
void MR_Quadrature_Count(TMR_Type *base, qtmr_channel_selection_t channel);
void MR_Quadrature_Count_GPIO_Init(void);



#endif /* __BSP_TMR_H */





