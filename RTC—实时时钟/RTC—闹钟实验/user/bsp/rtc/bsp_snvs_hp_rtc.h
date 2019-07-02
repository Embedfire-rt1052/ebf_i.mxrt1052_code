#ifndef _BSP_SNVS_HP_RTC_H_
#define _BSP_SNVS_HP_RTC_H_

#include "fsl_common.h"

void RTC_Config(void);
void RTC_AlarmSet(void);


//中断相关，IRQ中断号及IRQHandler中断服务函数
#define kCLOCK_SnvsHp0 kCLOCK_SnvsHp
#define EXAMPLE_SNVS_IRQn SNVS_HP_WRAPPER_IRQn
#define EXAMPLE_SNVS_IRQHandler SNVS_HP_WRAPPER_IRQHandler

// 时间宏定义
#define HOURS                8U            // 0~23
#define MINUTES              0             // 0~59
#define SECONDS              0             // 0~59

// 日期宏定义
#define YEAR                 2018U         // 1970~2099
#define MONTH                04U           // 1~12
#define DATE                 21U           // 1~31


#endif /* _BSP_SNVS_HP_RTC_H_ */

