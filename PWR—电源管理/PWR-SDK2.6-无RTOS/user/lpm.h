#ifndef _LPM_H_
#define _LPM_H_

#include <stdint.h>
#include "fsl_clock.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define CLOCK_CCM_HANDSHAKE_WAIT() \
                                   \
    do                             \
    {                              \
        while (CCM->CDHIPR != 0)   \
        {                          \
        }                          \
                                   \
    } while (0)

/* �͹��Ĺ���Ĺ���ģʽ���塣
 * ���ѳ���ʱ��ر�> Dsm>����>�ȴ�>���С�
 */
typedef enum _lpm_power_mode
{
    LPM_PowerModeOverRun = 0, /* ��������ģʽ */
    LPM_PowerModeFullRun, /* ��������ģʽ*/
    LPM_PowerModeLowSpeedRun,/* ��������ģʽ */
    LPM_PowerModeLowPowerRun,/* �͹�������ģʽ */
    LPM_PowerModeRunEnd = LPM_PowerModeLowPowerRun,
		/*
			���ģ���ϵͳ�ȴ�ģʽ�£�cpuʱ�ӱ��ſء�
      *�������趼���Ա��ֻ״̬��ʱ���ſ���CCGR���þ�����
      *��û�з���Ȩ��ʱ��DRAM�����Զ�ˢ��ģʽ��
		*/
    LPM_PowerModeSysIdle, /* ϵͳ����ģʽ */
		 /*
			���ģ��ڵ͹��Ŀ���ģʽ�£�����PLL / PFD���رգ�cpu��Դ�رա�
      *ģ��ģ���ڵ͹���ģʽ�����С�
      *���и������趼�ǵ�Դ�ſ�
      *����������Ա��ֵ�Ƶ����
      * DRAM��ˢ�¡�
		 */
    LPM_PowerModeLPIdle, /* �͹��Ŀ���ģʽ */
		 /*
		 ���ģ������˯��ģʽ�£�����PLL / PFD���رգ�XTAL�رգ�cpu��Դ�رա�
      *��32K RTCʱ���⣬����ʱ�Ӿ��ر�
      *���и������趼�ǵ�Դ�ſ�
      *����������ʱ���ſص�
      * DRAM��ˢ�¡�
      *���ʹ��RTOS������DSM�н���systick
		 */
    LPM_PowerModeSuspend, /* ��ͣģʽ */
    LPM_PowerModeSNVS, /*��Դ�ر�ģʽ��ػ�ģʽ */
    LPM_PowerModeEnd = LPM_PowerModeSNVS/*ö�ٽ�β */
} lpm_power_mode_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

AT_QUICKACCESS_SECTION_CODE(void CLOCK_SET_MUX(clock_mux_t mux, uint32_t value));
AT_QUICKACCESS_SECTION_CODE(void CLOCK_SET_DIV(clock_div_t divider, uint32_t value));
void ClockSelectXtalOsc(void);
void ClockSelectRcOsc(void);
void LPM_Init(void);
void LPM_EnableWakeupSource(uint32_t irq);
void LPM_DisableWakeupSource(uint32_t irq);
void LPM_PreEnterWaitMode(void);
void LPM_PostExitWaitMode(void);
void LPM_PreEnterStopMode(void);
void LPM_PostExitStopMode(void);
void LPM_OverDriveRun(void);
void LPM_FullSpeedRun(void);
void LPM_LowSpeedRun(void);
void LPM_LowPowerRun(void);
void LPM_EnterSystemIdle(void);
void LPM_ExitSystemIdle(void);
void LPM_EnterLowPowerIdle(void);
void LPM_ExitLowPowerIdle(void);
void LPM_EnterSuspend(void);
void LPM_EnterSNVS(void);

#if defined(__cplusplus)
}
#endif 

#endif /* _LPM_H_ */
