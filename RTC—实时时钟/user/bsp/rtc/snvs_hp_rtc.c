/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   RTC����
  ******************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  i.MXRT1052������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************
  */
#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_snvs_hp.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "snvs_hp_rtc.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define kCLOCK_SnvsHp0 kCLOCK_SnvsHp
#define EXAMPLE_SNVS_IRQn SNVS_HP_WRAPPER_IRQn
#define EXAMPLE_SNVS_IRQHandler SNVS_HP_WRAPPER_IRQHandler

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

volatile bool busyWait;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */

void EXAMPLE_SNVS_IRQHandler(void)
{
    if (SNVS_HP_RTC_GetStatusFlags(SNVS) & kSNVS_RTC_AlarmInterruptFlag)
    {
        busyWait = false;

        /* Clear alarm flag */
        SNVS_HP_RTC_ClearStatusFlags(SNVS, kSNVS_RTC_AlarmInterruptEnable);
    }
    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
      exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}



void rtc_init(void)
{
		snvs_hp_rtc_datetime_t rtcDate;
    snvs_hp_rtc_config_t snvsRtcConfig;

    /* ��ʼ��SNVS */
    /*
     * snvsConfig->rtccalenable = false;
     * snvsConfig->rtccalvalue = 0U;
     * snvsConfig->srtccalenable = false;
     * snvsConfig->srtccalvalue = 0U;
     * snvsConfig->PIFreq = 0U;
     */
    SNVS_HP_RTC_GetDefaultConfig(&snvsRtcConfig);
    SNVS_HP_RTC_Init(SNVS, &snvsRtcConfig);

    /* �������� */
    rtcDate.year = 2018U;
    rtcDate.month = 04U;
    rtcDate.day = 21U;
    rtcDate.hour = 8U;
    rtcDate.minute = 0;
    rtcDate.second = 0;

    /* ��RTCʱ������ΪĬ��ʱ������ڲ�����RTC */
    SNVS_HP_RTC_SetDatetime(SNVS, &rtcDate);
    SNVS_HP_RTC_StartTimer(SNVS);
}

void rtc_test(void)
{
	  uint32_t sec;
    uint8_t index;
		snvs_hp_rtc_datetime_t rtcDate;

	  /* ���� SNVS �����ж� */
    SNVS_HP_RTC_EnableInterrupts(SNVS, kSNVS_RTC_AlarmInterruptEnable);

    /* ʹ�� SNVS �ж� */
    EnableIRQ(EXAMPLE_SNVS_IRQn);
    PRINTF("��������ʱ��.\r\n");
    /* ��ѭ������������ʱ�� */
    while (1)
    {
        busyWait = true;
        index = 0;
        sec = 0;

        /* ��ȡ���� */
        SNVS_HP_RTC_GetDatetime(SNVS, &rtcDate);

        /* ��ӡĬ��ʱ�� */
        PRINTF("��ǰʱ��: %04hd-%02hd-%02hd %02hd:%02hd:%02hd\r\n", rtcDate.year, rtcDate.month, rtcDate.day,
               rtcDate.hour, rtcDate.minute, rtcDate.second);

        /* �û���������ʱ�� */
        PRINTF("�����������ȴ����ӱ��������س��� \r\n");
        PRINTF("������������ֵ\r\n");

        while (index != 0x0D)
        {
            index = GETCHAR();
            if((index >= '0') && (index <= '9'))
            {
                PUTCHAR(index);
                sec = sec * 10 + (index - 0x30U);
            }
        }
        PRINTF("\r\n");

        SNVS_HP_RTC_GetDatetime(SNVS, &rtcDate);
        if ((rtcDate.second + sec) < 60)
        {
            rtcDate.second += sec;
        }
        else
        {
            rtcDate.minute += (rtcDate.second + sec) / 60U;
            rtcDate.second = (rtcDate.second + sec) % 60U;
        }

        SNVS_HP_RTC_SetAlarm(SNVS, &rtcDate);

        /* ��ȡ����ʱ�� */
        SNVS_HP_RTC_GetAlarm(SNVS, &rtcDate);

        /* ��ӡ���ӱ���ʱ�� */
        PRINTF("���ӱ���ʱ��: %04hd-%02hd-%02hd %02hd:%02hd:%02hd\r\n", rtcDate.year, rtcDate.month, rtcDate.day,
               rtcDate.hour, rtcDate.minute, rtcDate.second);

        /* �ȴ����Ӿ������� */
        while (busyWait)
        {
        }

        PRINTF("\r\n ���Ӿ������� !!!! ");
    }
}
