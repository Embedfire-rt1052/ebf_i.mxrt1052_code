/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   ���ط�����Ӧ�ú����ӿ�
  ******************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  i.MXRT1052������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************
  */
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"  
	
#include "pad_config.h"  
#include "./bsp/beep/bsp_beep.h" 

void Beep_GPIO_Config(void)
{		
		/* ����gpio��ʼ�����ýṹ�� */
    gpio_pin_config_t beep_config;
		
    /** ���İ��LED�ƣ�IOMUXC MUX��PAD���� **/
		
    /* �������ŵĸ���ģʽ */
		IOMUXC_SetPinMux(
      BOARD_BEEP_IOMUXC,    /* ����Ϊ��ͨIO */
      0U);                      /* ��ʹ��SION���� */
		
		/*�������Ź���*/
		IOMUXC_SetPinConfig(
      BOARD_BEEP_IOMUXC,        
      SRE_0_SLOW_SLEW_RATE|
      DSE_6_R0_6|
      SPEED_2_MEDIUM_100MHz|
      ODE_0_OPEN_DRAIN_DISABLED|
      PKE_1_PULL_KEEPER_ENABLED|
      PUE_1_PULL_SELECTED|
      PUS_1_47K_OHM_PULL_UP|
      HYS_0_HYSTERESIS_DISABLED
      );	
	
    /* ����˵�� : */
		/* ת������: ת��������
			����ǿ��: R0/6 
			�������� : medium(100MHz)
			��©����: �ر� 
			��/����������: ��
			��/������ѡ��: ������
			����/����ѡ��: 47Kŷķ����
			�ͻ�������: �ر� */     
			
     /** �װ�ķ�������GPIO���� **/       
    beep_config.direction = kGPIO_DigitalOutput; //���ģʽ
    beep_config.outputLogic =  0;                //Ĭ�ϵ͵�ƽ
    beep_config.interruptMode = kGPIO_NoIntmode; //��ʹ���ж�
		
		/* ��ʼ�� BEEP GPIO. */
    GPIO_PinInit(BOARD_BEEP_GPIO, BOARD_BEEP_GPIO_PIN, &beep_config);	
}
/*********************************************END OF FILE**********************/
