#ifndef __BSP_QUADRATURE_H
#define __BSP_QUADRATURE_H

#include "fsl_common.h"
#include "fsl_qtmr.h"


/* 定义 QTMR 定时器的通道 */
#define QTMR_BASEADDR_2 TMR2
#define QTMR_CH0 kQTMR_Channel_0



/* 中断号和中断服务函数定义 */
#define QTMR2_IRQ_ID TMR2_IRQn
#define QTMR2_IRQ_HANDLER TMR2_IRQHandler

/* 得到TMR定时器的时钟频率 */
#define QTMR_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_IpgClk)


/*定义定时时间（单位：ms）*
*注意:TMR定时器的计数寄存器是16位，使用kCLOCK_IpgClk时钟，选择最大时钟分频（128分频）
*最多实现63ms的定时。
*/   
#define TMR_TIMIER 10


void Quadrature_Count_Init(void);

#endif /* __BSP_TMR_H */





