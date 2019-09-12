#ifndef _BSP_SNVS_HP_RTC_H_
#define _BSP_SNVS_HP_RTC_H_

#include "fsl_common.h"

void RTC_Config(void);
void RTC_AlarmSet(void);


//�ж���أ�IRQ�жϺż�IRQHandler�жϷ�����
#define kCLOCK_SnvsHp0 kCLOCK_SnvsHp
#define EXAMPLE_SNVS_IRQn SNVS_HP_WRAPPER_IRQn
#define EXAMPLE_SNVS_IRQHandler SNVS_HP_WRAPPER_IRQHandler

// ʱ��궨��
#define HOURS                8U            // ʱ  ��Χ��0~23��
#define MINUTES              0             // ��  ��Χ��0~59��
#define SECONDS              0             // ��  ��Χ��0~59��

// ���ں궨��
#define YEAR                 2018U         // ��  ��Χ��1970~2099��
#define MONTH                04U           // ��  ��Χ��1~12��
#define DATE                 21U           // ��  ��Χ��1~31��


#endif /* _BSP_SNVS_HP_RTC_H_ */

