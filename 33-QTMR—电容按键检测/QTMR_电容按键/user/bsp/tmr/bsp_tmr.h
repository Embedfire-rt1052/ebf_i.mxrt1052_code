#ifndef __BSP_TMR_H
#define __BSP_TMR_H

#include "fsl_iomuxc.h"
#include "fsl_common.h"
#include "fsl_qtmr.h"

/*********************************************************
 * TMR 输入端口、引脚号及IOMUXC复用宏定义
 *********************************************************/
/*
*GPIO_AD_B1_01, I2C1_SCL, CON 1X40TFT DISPLAY,  AT24C02, (CN4, 21),HDR254F-2X10摄像头
*/
#define TMR_TIMER0_INPUT_GPIO                GPIO1
#define TMR_TIMER0_INPUT_GPIO_PIN            (16U)
#define TMR_TIMER0_INPUT_IOMUXC              IOMUXC_GPIO_AD_B1_00_QTIMER3_TIMER0


/*
*GPIO_AD_B1_01, I2C1_SDA, CON 1X40TFT DISPLAY,  AT24C02, (CN4, 23),HDR254F-2X10摄像头
*/
#define TMR_TIMER1_INPUT_GPIO                GPIO1
#define TMR_TIMER1_INPUT_GPIO_PIN            (17U)
#define TMR_TIMER1_INPUT_IOMUXC              IOMUXC_GPIO_AD_B1_01_QTIMER3_TIMER1



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

// 定时器输入捕获用户自定义变量结构体声明
typedef struct
{   
	uint8_t   Capture_FinishFlag;   // 捕获结束标志位
	uint8_t   Capture_StartFlag;    // 捕获开始标志位
	uint32_t  Capture_CcrValue_1;     // 捕获寄存器的值
  uint32_t   Capture_CcrValue_2;     // 捕获寄存器的值
	uint16_t  Capture_Period;       // 定时器溢出次数 
}GPT_ICUserValueTypeDef;




/* 定义 TMR 定时器的通道和工作模式 */
#define BOARD_QTMR_BASEADDR TMR3
#define BOARD_QTMR_INPUT_CAPTURE_CHANNEL kQTMR_Channel_0
#define BOARD_QTMR_PWM_CHANNEL kQTMR_Channel_1
#define QTMR_CounterInputPin kQTMR_Counter0InputPin




/* 中断号和中断服务函数定义 */
#define QTMR_IRQ_ID TMR3_IRQn
#define QTMR_IRQ_HANDLER TMR3_IRQHandler

/* 得到TMR定时器的时钟频率 */
#define QTMR_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_IpgClk)


/*定义自动重装在值*/
#define RELODE_COUNTER 0xffff

void TMR_GPIO_Config(void);
void TMR_Init(void);


#endif /* __BSP_TMR_H */





