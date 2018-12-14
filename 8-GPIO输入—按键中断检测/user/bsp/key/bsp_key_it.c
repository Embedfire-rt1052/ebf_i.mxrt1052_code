/**
  ******************************************************************
  * @file    bsp_key_it.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   ����Ӧ�ú����ӿ�(�ж�ģʽ)
  ******************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  i.MXRT1052������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************
  */
#include "fsl_iomuxc.h"
#include "fsl_gpio.h" 

#include "pad_config.h" 
#include "./key/bsp_key_it.h"

/******************************************************************
 * ��������
  ******************************************************************/
/* �����Ƿ񱻰��µ��жϱ�־ */
__IO bool g_KeyDown[2] = { false};

/******************************************************************
 * ��
  ******************************************************************/
/* �������ž�ʹ��ͬ����PAD���� */
#define KEY_PAD_CONFIG_DATA            (SRE_0_SLOW_SLEW_RATE| \
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
static void Key_IOMUXC_MUX_Config(void);
static void Key_IOMUXC_PAD_Config(void);
static void Key_GPIO_Mode_Config(void);

/******************************************************************
 * ��������
  ******************************************************************/
/**
* @brief  ��ʼ���������IOMUXC��MUX��������
* @param  ��
* @retval ��
*/
static void Key_IOMUXC_MUX_Config(void)
{
  /* ���ð������ŵĸ���ģʽΪGPIO����ʹ��SION���� */
  IOMUXC_SetPinMux(CORE_BOARD_WAUP_KEY_IOMUXC, 0U);
  IOMUXC_SetPinMux(CORE_BOARD_MODE_KEY_IOMUXC, 0U); 
}

/**
* @brief  ��ʼ���������IOMUXC��MUX��������
* @param  ��
* @retval ��
*/
static void Key_IOMUXC_PAD_Config(void)
{
  /* ���ð����������Թ��� */    
  IOMUXC_SetPinConfig(CORE_BOARD_WAUP_KEY_IOMUXC, KEY_PAD_CONFIG_DATA); 
  IOMUXC_SetPinConfig(CORE_BOARD_MODE_KEY_IOMUXC, KEY_PAD_CONFIG_DATA); 
}

 /**
  * @brief  ��ʼ��������ص�GPIOģʽ
  * @param  ��
  * @retval ��
  */
static void Key_GPIO_Mode_Config(void)
{     
  /* ����Ϊ����ģʽ���͵�ƽ�жϣ�����ͨ��GPIO_PinInit������������ */
  gpio_pin_config_t key_config;
  
  /** ���İ�İ�����GPIO���� **/       
  key_config.direction = kGPIO_DigitalInput;    //����ģʽ
  key_config.outputLogic =  1;                  //Ĭ�ϸߵ�ƽ������ģʽʱ��Ч��
  key_config.interruptMode = kGPIO_IntLowLevel; //�͵�ƽ�����ж�
  
  /* ��ʼ�� KEY GPIO. */
  GPIO_PinInit(CORE_BOARD_WAUP_KEY_GPIO, CORE_BOARD_WAUP_KEY_GPIO_PIN, &key_config);
  GPIO_PinInit(CORE_BOARD_MODE_KEY_GPIO, CORE_BOARD_MODE_KEY_GPIO_PIN, &key_config);
}


/**
 * @brief  ��ʼ�������ж���ص�����
 * @param  ��
 * @retval ��
 */
static void Key_Interrupt_Config(void)
{
  /* ��IOMUXC_SNVS ʱ�� */
  CLOCK_EnableClock(kCLOCK_IomuxcSnvs);    

  /* ����GPIO�˿��ж� */
  EnableIRQ(CORE_BOARD_WAUP_KEY_IRQ);
  EnableIRQ(CORE_BOARD_MODE_KEY_IRQ);
  
  /* ����GPIO�˿�ĳ�����ŵ��ж� */
  GPIO_PortEnableInterrupts(CORE_BOARD_WAUP_KEY_GPIO, 
                            1U << CORE_BOARD_WAUP_KEY_GPIO_PIN);  
                            
  GPIO_PortEnableInterrupts(CORE_BOARD_MODE_KEY_GPIO, 
                            1U << CORE_BOARD_MODE_KEY_GPIO_PIN);  
}


 /**
  * @brief  ��ʼ������KEY��IO
  * @param  ��
  * @retval ��
  */
void Key_IT_GPIO_Config(void)
{
  /* ��ʼ��GPIO���á����ԡ�ģʽ���ж�*/
  Key_IOMUXC_MUX_Config();
  Key_IOMUXC_PAD_Config();
  Key_GPIO_Mode_Config();
  Key_Interrupt_Config();
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
void CORE_BOARD_WAUP_KEY_IRQHandler(void)
{ 
    /* ����жϱ�־λ */
    GPIO_PortClearInterruptFlags(CORE_BOARD_WAUP_KEY_GPIO,
                                 1U << CORE_BOARD_WAUP_KEY_GPIO_PIN);  
  
    /* ���ð����жϱ�־ */
    g_KeyDown[CORE_BOARD_WAUP_KEY_ID] = true;
  
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


/**
 * @brief  GPIO �����жϷ�����
 *         CORE_BOARD_MODE_KEY_IRQHandlerֻ��һ���꣬
 *         �ڱ�������ָ��������GPIO1_Combined_0_15_IRQHandler��
 *         �жϷ��������ǹ̶��ģ������������ļ����ҵ���
 * @param  �жϷ������������������
 * @note   �жϺ���һ��ֻʹ�ñ�־λ����ָʾ����ɺ󾡿��˳���
 *         �����������ʱ�����������жϷ�������
 * @retval �жϷ����������з���ֵ
 */
void CORE_BOARD_MODE_KEY_IRQHandler(void)
{ 
    /* ����жϱ�־λ */
    GPIO_PortClearInterruptFlags(CORE_BOARD_MODE_KEY_GPIO,
                                 1U << CORE_BOARD_MODE_KEY_GPIO_PIN);  
  
    /* ���ð����жϱ�־ */
    g_KeyDown[CORE_BOARD_MODE_KEY_ID] = true;
  
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

