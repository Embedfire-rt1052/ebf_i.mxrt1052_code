#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "fsl_common.h"

void SysTick_Init(void);
void Delay_ms(__IO uint32_t nTime);

/*简单任务管理*/
#define TASK_ENABLE 0
#define NumOfTask 3

extern __IO uint32_t s_frame_count ;
void SysTick_Delay_Us( __IO uint32_t us);
void SysTick_Delay_Ms( __IO uint32_t ms);


#endif /* __SYSTICK_H */

