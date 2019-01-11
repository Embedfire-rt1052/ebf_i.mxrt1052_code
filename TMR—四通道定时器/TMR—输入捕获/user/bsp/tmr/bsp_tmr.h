#ifndef __BSP_TMR_H
#define __BSP_TMR_H

#include "fsl_iomuxc.h"
#include "fsl_common.h"
#include "fsl_qtmr.h"

/*********************************************************
 * TMR ����˿ڡ����źż�IOMUXC���ú궨��
 *********************************************************/
/*
*GPIO_AD_B1_01, I2C1_SCL, CON 1X40TFT DISPLAY,  AT24C02, (CN4, 21),HDR254F-2X10����ͷ
*/
#define TMR_TIMER0_INPUT_GPIO                GPIO1
#define TMR_TIMER0_INPUT_GPIO_PIN            (16U)
#define TMR_TIMER0_INPUT_IOMUXC              IOMUXC_GPIO_AD_B1_00_QTIMER3_TIMER0


/*
*GPIO_AD_B1_01, I2C1_SDA, CON 1X40TFT DISPLAY,  AT24C02, (CN4, 23),HDR254F-2X10����ͷ
*/
#define TMR_TIMER1_INPUT_GPIO                GPIO1
#define TMR_TIMER1_INPUT_GPIO_PIN            (17U)
#define TMR_TIMER1_INPUT_IOMUXC              IOMUXC_GPIO_AD_B1_01_QTIMER3_TIMER1



/* ���벶�����ŵ�PAD���� */
#define TMR_INPUT_PAD_CONFIG_DATA       (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_2_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_1_PULL_KEEPER_ENABLED| \
                                        PUE_0_KEEPER_SELECTED| \
                                        PUS_2_100K_OHM_PULL_UP| \
                                        HYS_0_HYSTERESIS_DISABLED)   
    /* ����˵�� : */
    /* ת������: ת��������
      ����ǿ��: R0/6 
      �������� : medium(100MHz)
      ��©����: �ر� 
      ��/����������: ����
      ��/������ѡ��: ������
      ����/����ѡ��: 100K����
      �ͻ�������: �ر� */     

// ��ʱ�����벶���û��Զ�������ṹ������
typedef struct
{   
	uint8_t   Capture_FinishFlag;   // ���������־λ
	uint8_t   Capture_StartFlag;    // ����ʼ��־λ
	uint32_t  Capture_CcrValue_1;     // ����Ĵ�����ֵ
  uint32_t   Capture_CcrValue_2;     // ����Ĵ�����ֵ
	uint16_t  Capture_Period;       // ��ʱ��������� 
}GPT_ICUserValueTypeDef;




/* ���� TMR ��ʱ����ͨ���͹���ģʽ */
#define BOARD_QTMR_BASEADDR TMR3
#define BOARD_QTMR_INPUT_CAPTURE_CHANNEL kQTMR_Channel_0
#define BOARD_QTMR_PWM_CHANNEL kQTMR_Channel_1
#define QTMR_CounterInputPin kQTMR_Counter0InputPin




/* �жϺź��жϷ��������� */
#define QTMR_IRQ_ID TMR3_IRQn
#define QTMR_IRQ_HANDLER TMR3_IRQHandler

/* �õ�TMR��ʱ����ʱ��Ƶ�� */
#define QTMR_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_IpgClk)


/*�����Զ���װ��ֵ*/
#define RELODE_COUNTER 0xffff

void TMR_GPIO_Config(void);
void TMR_Init(void);


#endif /* __BSP_TMR_H */





