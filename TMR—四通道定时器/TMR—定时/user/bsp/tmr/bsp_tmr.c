#include "./bsp/nvic/bsp_nvic.h"
#include "./bsp/tmr/bsp_tmr.h"


volatile uint32_t qtmrIsrFlag = 0;


void TMR_Init(void)
{

  qtmr_config_t qtmrConfig; /*定义TMR 定时器初始化结构体*/
  
  /*初始化TMR 定时器*/
  QTMR_GetDefaultConfig(&qtmrConfig);
  qtmrConfig.primarySource = kQTMR_ClockDivide_128;
  QTMR_Init(BOARD_QTMR_BASEADDR, BOARD_SECOND_QTMR_CHANNEL, &qtmrConfig);
  
  /*设置自动重装载值*/
  QTMR_SetTimerPeriod(BOARD_QTMR_BASEADDR, BOARD_SECOND_QTMR_CHANNEL, MSEC_TO_COUNT(TMR_TIMIER, (QTMR_SOURCE_CLOCK / 128)));


  /*使能比较中断*/
  QTMR_EnableInterrupts(BOARD_QTMR_BASEADDR, BOARD_SECOND_QTMR_CHANNEL, kQTMR_CompareInterruptEnable);
    
  /*设置中断优先级,*/
  set_IRQn_Priority(QTMR_IRQ_ID,Group4_PreemptPriority_6, Group4_SubPriority_0);
  /*使能中断*/
  EnableIRQ(QTMR_IRQ_ID);

  
  /*开启通道2的计时，在时钟的上升沿计数*/
  QTMR_StartTimer(BOARD_QTMR_BASEADDR, BOARD_SECOND_QTMR_CHANNEL, kQTMR_PriSrcRiseEdge);


}



/*TMR定时器中断服务函数*/
void QTMR_IRQ_HANDLER(void)
{
    /* 清除中断标志位*/
    QTMR_ClearStatusFlags(BOARD_QTMR_BASEADDR,\
        BOARD_SECOND_QTMR_CHANNEL, kQTMR_CompareFlag);
    /*设置标志位*/
    qtmrIsrFlag++;
  
}








