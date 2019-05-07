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









/****************************第一部分*****************************/
#define TPAD_QTMR_BASEADDR                TMR3
#define TPAD_QTMR_INPUT_CAPTURE_CHANNEL   kQTMR_Channel_3
#define TPAD_QTMR_CounterInputPin         kQTMR_Counter3InputPin

/*****************************第二部分*****************************/
#define TPAD_GPIO 		GPIO1
#define TPAD_GPIO_PIN 		(19U)
#define TPAD_IOMUXC	        IOMUXC_GPIO_AD_B1_03_GPIO1_IO19
#define TPAD_QTMR_IOMUXC	 IOMUXC_GPIO_AD_B1_03_QTIMER3_TIMER3

/****************************第三部分******************************/
/* 中断号和中断处理程序 */
#define TPAD_QTMR_IRQ_ID 	TMR3_IRQn
#define TPAD_QTMR_IRQ_HANDLER 	TMR3_IRQHandler

/* 获取QTMR时钟源 */
#define TPAD_QTMR_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_IpgClk)

/****************************第四部分******************************/
/* 输入捕获引脚的PAD配置 */
#define TMR_INPUT_PAD_CONFIG_DATA    (SRE_0_SLOW_SLEW_RATE| \
                                      DSE_0_OUTPUT_DRIVER_DISABLED| \
                                      SPEED_2_MEDIUM_100MHz| \
                                      ODE_1_OPEN_DRAIN_ENABLED| \
                                      PKE_0_PULL_KEEPER_DISABLED| \
                                      PUE_0_KEEPER_SELECTED| \
                                      PUS_3_22K_OHM_PULL_UP| \
                                      HYS_1_HYSTERESIS_ENABLED)   

/* 输入捕获引脚的PAD配置 */
#define GPIO_PAD_CONFIG_DATA       (SRE_0_SLOW_SLEW_RATE| \
                                    DSE_6_R0_6| \
                                    SPEED_2_MEDIUM_100MHz| \
                                    ODE_0_OPEN_DRAIN_DISABLED| \
                                    PKE_1_PULL_KEEPER_ENABLED| \
                                    PUE_1_PULL_SELECTED| \
                                    PUS_1_47K_OHM_PULL_UP| \
                                    HYS_0_HYSTERESIS_DISABLED)   







void TMR_Init(void)
{
  qtmr_config_t qtmrConfig; /*定义TMR 定时器初始化结构体*/
  
  /*初始化输入引脚*/
  TMR_GPIO_Config();
  
  /*初始化TMR 定时器*/
  QTMR_GetDefaultConfig(&qtmrConfig);
  qtmrConfig.primarySource = kQTMR_ClockDivide_128;
  QTMR_Init(BOARD_QTMR_BASEADDR, BOARD_QTMR_INPUT_CAPTURE_CHANNEL,\
                                  &qtmrConfig);
  
   /*设置自动重装载值*/
  QTMR_SetTimerPeriod(BOARD_QTMR_BASEADDR, \
                BOARD_QTMR_INPUT_CAPTURE_CHANNEL,RELODE_COUNTER);
  
  /* 设置输入捕获模式*/
  QTMR_SetupInputCapture(BOARD_QTMR_BASEADDR, \
              BOARD_QTMR_INPUT_CAPTURE_CHANNEL, QTMR_CounterInputPin, \
              false, false, kQTMR_FallingEdge);
  
  /* 使能定时器捕获中断*/
  QTMR_EnableInterrupts(BOARD_QTMR_BASEADDR, \
             BOARD_QTMR_INPUT_CAPTURE_CHANNEL, kQTMR_EdgeInterruptEnable);
  
  /* 使能定时器比较中断*/
  QTMR_EnableInterrupts(BOARD_QTMR_BASEADDR, \
       BOARD_QTMR_INPUT_CAPTURE_CHANNEL, \
       kQTMR_CompareInterruptEnable);
  
  /*设置中断优先级,*/
  set_IRQn_Priority(QTMR_IRQ_ID,Group4_PreemptPriority_6, \
                                     Group4_SubPriority_0);
  
  /* 使能中断 */
  EnableIRQ(QTMR_IRQ_ID);
  /* 开启定时器，并设置位在时钟上升沿计数 */
  QTMR_StartTimer(BOARD_QTMR_BASEADDR, \
              BOARD_QTMR_INPUT_CAPTURE_CHANNEL, \
              kQTMR_PriSrcRiseEdge);
}




/***********************************
 *
 * 定时器输入捕获配置
 *
 ***********************************/
static void TIMx_CHx_Cap_Init(void)
{
  qtmr_config_t qtmrConfig;
  
  /*************************第一部分**************************/
  /* 设置引脚的复用模式 */
  IOMUXC_SetPinMux(
                   TPAD_QTMR_IOMUXC,    /* 配置为定时器输入捕抓IO */
                   0U);                 /* 不使用SION功能 */																							 
  /* 引脚PAD属性配置 */  
  IOMUXC_SetPinConfig(TPAD_QTMR_IOMUXC,TMR_INPUT_PAD_CONFIG_DATA); 
  
  /************************第二部分***************************/
  /* 获取系统默认配置 */
  QTMR_GetDefaultConfig(&qtmrConfig);
  /* 使用IP总线时钟，八分频 */
  qtmrConfig.primarySource = kQTMR_ClockDivide_16;
  /* 指定定时器通道初始化定时器 */
  QTMR_Init(TPAD_QTMR_BASEADDR,TPAD_QTMR_INPUT_CAPTURE_CHANNEL,\
                                        &qtmrConfig);
  
  /***********************第三部分****************************/
  /* 键入对应通道的输入捕抓功能 */
  QTMR_SetupInputCapture(TPAD_QTMR_BASEADDR,\
         TPAD_QTMR_INPUT_CAPTURE_CHANNEL,TPAD_QTMR_CounterInputPin, \
         false, true, kQTMR_RisingEdge);
  /* 使能中断 */
  EnableIRQ(TPAD_QTMR_IRQ_ID);
  /* 使能定时器比较中断 */
  QTMR_EnableInterrupts(TPAD_QTMR_BASEADDR,\
           TPAD_QTMR_INPUT_CAPTURE_CHANNEL,\
                kQTMR_EdgeInterruptEnable);
  /* 定时器计数器归0	 */
  TPAD_QTMR_BASEADDR->CHANNEL[TPAD_QTMR_INPUT_CAPTURE_CHANNEL].CNTR = 0;
  /* 启动输入通道以计算主源时钟的上升边缘 */
  QTMR_StartTimer(TPAD_QTMR_BASEADDR, TPAD_QTMR_INPUT_CAPTURE_CHANNEL,\
                                                kQTMR_PriSrcRiseEdge);	
}








