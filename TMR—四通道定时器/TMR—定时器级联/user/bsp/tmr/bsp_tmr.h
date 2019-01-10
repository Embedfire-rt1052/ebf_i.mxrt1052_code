#ifndef __BSP_TMR_H
#define __BSP_TMR_H

#include "fsl_common.h"
#include "fsl_qtmr.h"


/* 定义 TMR 定时器的通道和工作模式instance/channel used for board */
#define BOARD_QTMR_BASEADDR TMR3
#define BOARD_FIRST_QTMR_CHANNEL kQTMR_Channel_0
#define BOARD_SECOND_QTMR_CHANNEL kQTMR_Channel_1
#define QTMR_ClockCounterOutput kQTMR_ClockCounter0Output

/* 中断号和中断服务函数定义 */
#define QTMR_IRQ_ID TMR3_IRQn
#define QTMR_IRQ_HANDLER TMR3_IRQHandler

/* 得到TMR定时器的时钟频率 */
#define QTMR_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_IpgClk)


/*设定计时事件（单位：ms）*
*该程序使用了定时器的级联，上一级定时器（通道1）定时时间为1ms, 该用用于设置主定时器（通道2）的计数值，
*TMR_TIMIER = N 表示定时N(ms) ,N最大可取65535
*/
#define TMR_TIMIER 1000

void TMR_Init(void);


#endif /* __BSP_TMR_H */





