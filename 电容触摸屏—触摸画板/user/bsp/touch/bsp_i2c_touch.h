#ifndef __I2C_TOUCH_H
#define	__I2C_TOUCH_H

#include "fsl_common.h"
#include "fsl_lpi2c.h"
#include "fsl_iomuxc.h"


extern lpi2c_master_handle_t g_m_handle;
extern volatile bool g_MasterCompletionFlag ;
extern volatile bool g_TouchPadInputSignal;

  

/* ѡ�� USB1 PLL (PLL3 480 MHz) ��Ϊ lpi2c ʱ��Դ */
#define LPI2C_CLOCK_SOURCE_SELECT (0U)
/* ����USB1 PLLʱ�ӵ�LPI2C��ʱ�ӵ�ʱ�ӷ�Ƶ  */
#define LPI2C_CLOCK_SOURCE_DIVIDER (5U)
/* ��ȡ lpi2c ʱ��Ƶ�� */
#define LPI2C_CLOCK_FREQUENCY ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (LPI2C_CLOCK_SOURCE_DIVIDER + 1U))
#define LPI2C_MASTER_CLOCK_FREQUENCY  LPI2C_CLOCK_FREQUENCY


/*LPI2C��غ궨��*/
#define GTP_I2C_MASTER_BASE   (LPI2C1_BASE)    //����ʹ�õ�I2C
//����I2C���ƼĴ���
#define GTP_I2C_MASTER        ((LPI2C_Type *)GTP_I2C_MASTER_BASE)
#define GTP_I2C_BAUDRATE      400000U     //����I2C������

/* �ȴ���ʱʱ�� */
#define I2CT_FLAG_TIMEOUT    ((uint32_t)0x1000)  //�ȴ���ʱʱ���趨
//���ȴ�ʱ���趨
#define I2CT_LONG_TIMEOUT         ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))


/*! @brief ����оƬ���Ŷ��� */
/*LPI2Cʱ������*/ 
#define TOUCH_PAD_SCL_IOMUXC	IOMUXC_GPIO_AD_B1_00_LPI2C1_SCL
/*LPI2C��������*/ 
#define TOUCH_PAD_SDA_IOMUXC	IOMUXC_GPIO_AD_B1_01_LPI2C1_SDA

/*���崥��оƬ�ĸ�λ����*/
#define TOUCH_PAD_RST_GPIO 	     GPIO1
#define TOUCH_PAD_RST_GPIO_PIN 	    (2U)
#define TOUCH_PAD_RST_IOMUXC	    IOMUXC_GPIO_AD_B0_02_GPIO1_IO02

/*����оƬ���ж��������*/
#define TOUCH_PAD_INT_GPIO 	    GPIO1
#define TOUCH_PAD_INT_GPIO_PIN 	    (11U)
#define TOUCH_PAD_INT_IOMUXC	    IOMUXC_GPIO_AD_B0_11_GPIO1_IO11

/*�����ж϶���*/
#define TOUCH_PAD_INT_IRQ 	    GPIO1_Combined_0_15_IRQn
#define TOUCH_PAD_IRQHANDLER        GPIO1_Combined_0_15_IRQHandler

/* I2C��SCL��SDA����ʹ��ͬ����PAD���� */
#define I2C_PAD_CONFIG_DATA            (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_1_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_1_PULL_KEEPER_ENABLED| \
                                        PUE_0_KEEPER_SELECTED| \
                                        PUS_3_22K_OHM_PULL_UP| \
                                        HYS_0_HYSTERESIS_DISABLED)   
    /* ����˵�� : */
    /* ת������: ת��������
        ����ǿ��: R0/6 
        �������� : medium(100MHz)
        ��©����: �ر� 
        ��/����������: ʹ��
        ��/������ѡ��: ������
        ����/����ѡ��: 22Kŷķ����(ѡ���˱�������������Ч)
        �ͻ�������: ��ֹ */ 
        

/* ����оƬ��RST��INT����ʹ��ͬ����PAD���� */
#define GTP_RST_INT_PAD_CONFIG_DATA            (SRE_0_SLOW_SLEW_RATE| \
                                                DSE_6_R0_6| \
                                                SPEED_1_MEDIUM_100MHz| \
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
        ��/����������: ʹ��
        ��/������ѡ��: ������
        ����/����ѡ��: 100Kŷķ����(ѡ���˱�������������Ч)
        �ͻ�������: ��ֹ */ 

//�����ӿ�
int32_t GTP_I2C_ReadBytes(uint8_t client_addr, uint8_t *buf, int32_t len);

void I2C_Touch_Init(void);
uint32_t I2C_WriteBytes(uint8_t ClientAddr,uint8_t* pBuffer,  uint8_t NumByteToWrite);
uint32_t I2C_ReadBytes(uint8_t ClientAddr,uint8_t* pBuffer, uint16_t NumByteToRead);
void GTP_ResetChip(void);
void GTP_IRQDisable(void);
void GTP_IRQEnable(void);



#endif /* __I2C_TOUCH_H */







