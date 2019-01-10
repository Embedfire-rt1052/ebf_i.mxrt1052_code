#include "./tmr/bsp_tmr.h"


volatile bool qtmrIsrFlag = false;


void TMR_Init(void)
{

  qtmr_config_t qtmrConfig; /*定义TMR 定时器初始化结构体*/
  
  /*初始化TMR 定时器*/
  QTMR_GetDefaultConfig(&qtmrConfig);
  
  /*初始化第一个通道*/
  qtmrConfig.primarySource = kQTMR_ClockDivide_128;
  QTMR_Init(BOARD_QTMR_BASEADDR, BOARD_FIRST_QTMR_CHANNEL, &qtmrConfig);
  
  /* 初始化第二个通道 */
  qtmrConfig.primarySource = QTMR_ClockCounterOutput;
  QTMR_Init(BOARD_QTMR_BASEADDR, BOARD_SECOND_QTMR_CHANNEL, &qtmrConfig);
  
  /* 设置通1的计时周期为1ms */
  QTMR_SetTimerPeriod(BOARD_QTMR_BASEADDR, BOARD_FIRST_QTMR_CHANNEL, MSEC_TO_COUNT(1U, (QTMR_SOURCE_CLOCK / 128)));
  
  /* 设置通道2的计数值为 */
  QTMR_SetTimerPeriod(BOARD_QTMR_BASEADDR, BOARD_SECOND_QTMR_CHANNEL, TMR_TIMIER);
  
  
  /*使能通道2的比较中断 Enable the second channel compare interrupt */
  QTMR_EnableInterrupts(BOARD_QTMR_BASEADDR, BOARD_SECOND_QTMR_CHANNEL, kQTMR_CompareInterruptEnable);

  /* 以cascase模式启动第通道2，连接到前面设置的通道1.通道2是主定时器，通道1是从定时器*/
  QTMR_StartTimer(BOARD_QTMR_BASEADDR, BOARD_SECOND_QTMR_CHANNEL, kQTMR_CascadeCount);

  /* 开启通道1，并且设置位在时钟的上升沿计数*/
  QTMR_StartTimer(BOARD_QTMR_BASEADDR, BOARD_FIRST_QTMR_CHANNEL, kQTMR_PriSrcRiseEdge);

  /*开启中断 */
  EnableIRQ(QTMR_IRQ_ID);
}


/*TMR定时器中断服务函数*/
void QTMR_IRQ_HANDLER(void)
{
    /* 清除中断标志位*/
    QTMR_ClearStatusFlags(BOARD_QTMR_BASEADDR, BOARD_SECOND_QTMR_CHANNEL, kQTMR_CompareFlag);
    
    qtmrIsrFlag = true;
  
}








