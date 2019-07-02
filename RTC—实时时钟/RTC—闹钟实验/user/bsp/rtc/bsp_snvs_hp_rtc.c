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
#include "./bsp/rtc/bsp_snvs_hp_rtc.h"   



/* �ȴ����ӱ���������־ */
volatile bool busyWait;

/********************�жϷ�����**************************/
/**
 * @brief  RTC �жϷ����� 
 *         EXAMPLE_SNVS_IRQHandler��һ���꣬
 *         �ڱ�������ָ��������SNVS_HP_WRAPPER_IRQHandler��
 *         �жϷ��������ǹ̶��ģ������������ļ����ҵ���
 * @param  �жϷ������������������
 * @note   �жϺ���һ��ֻʹ�ñ�־λ����ָʾ����ɺ󾡿��˳���
 *         �����������ʱ�����������жϷ������У�
 * @retval �жϷ����������з���ֵ
 */
void EXAMPLE_SNVS_IRQHandler(void)
{
	  /* �ж��ж������Ƿ����� */
    if (SNVS_HP_RTC_GetStatusFlags(SNVS) & kSNVS_RTC_AlarmInterruptFlag)
    {
			  /* ���ñ����жϱ�־ */
        busyWait = false;

        /* ���������־ */
        SNVS_HP_RTC_ClearStatusFlags(SNVS, kSNVS_RTC_AlarmInterruptEnable);
    }
    /* ���ΪARM�����838869��Ӱ��Cortex-M4��Cortex-M4F�洢�����ص�
       �쳣���ز������ܻᵼ�´�����ж� */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}




/**
  * @brief  ��ʼ��RTC�������
  * @param  ��
  * @retval ��
  */
void RTC_Config(void)
{
		snvs_hp_rtc_datetime_t rtcDate;/* ���� rtc �������ýṹ�� */
    snvs_hp_rtc_config_t snvsRtcConfig;/* ���� snvsRtc ���ýṹ�� */

    /* ��ʼ��SNVS */
    /*
     * snvsConfig->rtccalenable = false;
     * snvsConfig->rtccalvalue = 0U;
     * snvsConfig->srtccalenable = false;
     * snvsConfig->srtccalvalue = 0U;
     * snvsConfig->PIFreq = 0U;
     */
    /* ��ȡĬ������ */
    SNVS_HP_RTC_GetDefaultConfig(&snvsRtcConfig);
    /* ��ʼ��RTC */
    SNVS_HP_RTC_Init(SNVS, &snvsRtcConfig);

    /* �������� */
    rtcDate.year = YEAR;
    rtcDate.month = MONTH;
    rtcDate.day = DATE;
    rtcDate.hour = HOURS;
    rtcDate.minute = MINUTES;
    rtcDate.second = SECONDS;

    /* ��RTCʱ������ΪĬ��ʱ������ڲ�����RTC */
    SNVS_HP_RTC_SetDatetime(SNVS, &rtcDate);
    SNVS_HP_RTC_StartTimer(SNVS);
}




/**
  * @brief  RTC��������
  * @param  ��
  * @retval ��
  */
void RTC_AlarmSet(void)
{
	  uint32_t sec;//�û�������ĵȴ�����ʱ��
    uint8_t index;//���ڽ��մ�������
		snvs_hp_rtc_datetime_t rtcDate;//����ȫ��RTCʱ��ṹ��

	  /* ���� SNVS �����ж� */
    SNVS_HP_RTC_EnableInterrupts(SNVS, kSNVS_RTC_AlarmInterruptEnable);

    /* ʹ�� SNVS �ж� */
    EnableIRQ(EXAMPLE_SNVS_IRQn);
    PRINTF("��������ʱ��.\r\n");
    /* ��ѭ������������ʱ�� */
    while (1)
    {
			/* ������ʱ�����ĳ�ʼֵ */
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
					/* �ȴ���ȡ�����ʱ�� */
            index = GETCHAR();
            if((index >= '0') && (index <= '9'))
            {
                PUTCHAR(index);
							/* ��ȡʵ�������ʱ������ */
                sec = sec * 10 + (index - 0x30U);
            }
        }
        PRINTF("\r\n");

        SNVS_HP_RTC_GetDatetime(SNVS, &rtcDate);
				
				/* ������60��ʱ��ֱ�ӽ�����ʱ���ۼӵ� ����λ */
        if ((rtcDate.second + sec) < 60)
        {
            rtcDate.second += sec;
        }
        else
        {
				/* ���û�����ʱ���ۼӵ�ʵ��ʱ�ӣ��������Ӧ�ñ�����ʵ��ʱ�� */
            rtcDate.minute += (rtcDate.second + sec) / 60U;
            rtcDate.second = (rtcDate.second + sec) % 60U;
        }

				/* ��������ʱ�� */				
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
