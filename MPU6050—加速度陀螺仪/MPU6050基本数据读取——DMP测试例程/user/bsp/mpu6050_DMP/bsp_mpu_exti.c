/**
  ******************************************************************************
  * @file    bsp_exti.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   �����������ж�����
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 F767 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./mpu6050_DMP/bsp_mpu_exti.h"
#include "fsl_iomuxc.h"
#include "fsl_gpio.h" 
#include "pad_config.h" 
#include "./bsp/nvic/bsp_nvic.h"
/* �����жϼ�����ŵ�PAD���� */
#define MPU_PAD_CONFIG_DATA            (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_0_OUTPUT_DRIVER_DISABLED| \
                                        SPEED_2_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_1_PULL_KEEPER_ENABLED| \
                                        PUE_1_PULL_SELECTED| \
                                        PUS_3_22K_OHM_PULL_UP| \
                                        HYS_1_HYSTERESIS_ENABLED)   
    /* ����˵�� : */
    /* ת������: ת��������
      ����ǿ��: �ر�
      �ٶ����� : medium(100MHz)
      ��©����: �ر� 
      ��/����������: ʹ��
      ��/������ѡ��: ������
      ����/����ѡ��: 22Kŷķ����
      �ͻ�������: ���� ��������ʱ��Ч��ʩ���ش�������ʹ�ܺ���Թ�������������*/

/******************************************************************
 * ����
  ******************************************************************/
static void MPU_IOMUXC_MUX_Config(void);
static void MPU_IOMUXC_PAD_Config(void);
static void MPU_GPIO_Mode_Config(void);
/* �����Ƿ񱻰��µ��жϱ�־ */
__IO bool g_KeyDown[2] = { false};

/******************************************************************
 * ��������
  ******************************************************************/
/**
* @brief  ��ʼ���������IOMUXC��MUX��������
* @param  ��
* @retval ��
*/
static void MPU_IOMUXC_MUX_Config(void)
{
  /* ���ð������ŵĸ���ģʽΪGPIO����ʹ��SION���� */
  IOMUXC_SetPinMux(MPU_EXTI_IOMUXC, 0U);
}

/**
* @brief  ��ʼ�������������PAD����
* @param  ��
* @retval ��
*/
static void MPU_IOMUXC_PAD_Config(void)
{
  /* ���ð����������Թ��� */    
  IOMUXC_SetPinConfig(MPU_EXTI_IOMUXC, MPU_PAD_CONFIG_DATA); 
}

 /**
  * @brief  ��ʼ��������ص�GPIOģʽ
  * @param  ��
  * @retval ��
  */
static void MPU_GPIO_Mode_Config(void)
{     
  /* ����Ϊ����ģʽ���͵�ƽ�жϣ�����ͨ��GPIO_PinInit������������ */
  gpio_pin_config_t mpu_config;
  
//  /** ���İ�İ�����GPIO���� **/       
  mpu_config.direction = kGPIO_DigitalInput;    //����ģʽ
  mpu_config.outputLogic =  1;                  //Ĭ�ϸߵ�ƽ������ģʽʱ��Ч��
  mpu_config.interruptMode = kGPIO_IntFallingEdge; //�͵�ƽ�����ж�
	  /** ���İ�İ�����GPIO���� **/       
//  mpu_config.direction = kGPIO_DigitalInput;    //����ģʽ
//  mpu_config.outputLogic =  0;                  //Ĭ�ϸߵ�ƽ������ģʽʱ��Ч��
//  mpu_config.interruptMode = kGPIO_IntRisingEdge; //�͵�ƽ�����ж�
  
//	typedef enum _gpio_interrupt_mode
//{
//    kGPIO_NoIntmode = 0U,              /*!< Set current pin general IO functionality.*/
//    kGPIO_IntLowLevel = 1U,            /*!< Set current pin interrupt is low-level sensitive.*/
//    kGPIO_IntHighLevel = 2U,           /*!< Set current pin interrupt is high-level sensitive.*/
//    kGPIO_IntRisingEdge = 3U,          /*!< Set current pin interrupt is rising-edge sensitive.*/
//    kGPIO_IntFallingEdge = 4U,         /*!< Set current pin interrupt is falling-edge sensitive.*/
//    kGPIO_IntRisingOrFallingEdge = 5U, /*!< Enable the edge select bit to override the ICR register's configuration.*/
//} gpio_interrupt_mode_t;
	
  /* ��ʼ�� KEY GPIO. */
  GPIO_PinInit(MPU_EXTI_GPIO, MPU_EXTI_GPIO_PIN, &mpu_config);
}




/**
 * @brief  ��ʼ�������ж���ص�����
 * @param  ��
 * @retval ��
 */
static void MPU_Interrupt_Config(void)   
{
  /* ����GPIO���ŵ��ж� */
  GPIO_PortEnableInterrupts(MPU_EXTI_GPIO, 1U << MPU_EXTI_GPIO_PIN);                           
  /*�����ж����ȼ�,*/
  set_IRQn_Priority(MPU_EXTI_IRQ,Group4_PreemptPriority_6, Group4_SubPriority_0);
  /* ����GPIO�˿��ж� */
  EnableIRQ(MPU_EXTI_IRQ);
}


 /**
  * @brief  ��ʼ������KEY��IO    
  * @param  ��
  * @retval ��
  */
void MPU_IT_GPIO_Config(void)
{
		/* ��ʼ��GPIO���á����ԡ�ģʽ���ж�*/
		MPU_IOMUXC_MUX_Config();
		MPU_IOMUXC_PAD_Config();
		MPU_GPIO_Mode_Config();
		MPU_Interrupt_Config();
}
void EXTI_MPU_Config()
{
	MPU_IT_GPIO_Config();
}



/********************�жϷ�����**************************/
/**
 * @brief  GPIO �����жϷ�����
 *         CORE_BOARD_WAUP_KEY_IRQHandlerֻ��һ���꣬
 *         �ڱ�������ָ��������GPIO5_Combined_0_15_IRQHandler��
 *         �жϷ��������ǹ̶��ģ������������ļ����ҵ���
 * @param  �жϷ������������������
 * @note   �жϺ���һ��ֻʹ�ñ�־λ����ָʾ����ɺ󾡿��˳���
 *         �����������ʱ�����������жϷ�������
 * @retval �жϷ����������з���ֵ
 */
extern void gyro_data_ready_cb(void);
void MPU_EXTI_IRQHandler(void)
{ 
    /* ����жϱ�־λ */
    GPIO_PortClearInterruptFlags(MPU_EXTI_GPIO,1U << MPU_EXTI_GPIO_PIN);  
  
		/* Handle new gyro*/
		gyro_data_ready_cb();
	
    /* ���²�����Ϊ ARM �Ŀ���838869��ӵ�, 
       �ô���Ӱ�� Cortex-M4, Cortex-M4F�ںˣ�       
       �����洢�����ص��쳣�����·��ز������ܻ�ָ�������ж�
        CM7����Ӱ�죬�˴������ô���
    */
  
    /* ԭע�ͣ�Add for ARM errata 838869, affects Cortex-M4,
       Cortex-M4F Store immediate overlapping
       exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}










/*********************************************END OF FILE**********************/
