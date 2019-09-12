/**
******************************************************************
* @file    bsp_beep.c
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

#include "fsl_gpio.h"  
#include "./bsp/beep/bsp_beep.h" 


void Beep_GPIO_Config(void)
{		
  /* ����gpio��ʼ�����ýṹ�� */
  gpio_pin_config_t beep_config;
  
  /* ���õװ�ķ��������ŵĸ���ģʽ */
  IOMUXC_SetPinMux(
                   BOARD_BEEP_IOMUXC,    /* ����Ϊ��ͨIO */
                   0U);                      /* ��ʹ��SION���� */
  
//   /* ����BEEP���ŵ�IOMUXC PAD���� */ 
//  IOMUXC_SetPinConfig(BOARD_BEEP_IOMUXC, BEEP_PAD_CONFIG_DATA);
  		/*�������Ź���*/
		IOMUXC_SetPinConfig(BOARD_BEEP_IOMUXC,SRE_0_SLOW_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_2_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_0_PULL_KEEPER_DISABLED| \
                                        PUE_0_KEEPER_SELECTED| \
                                        PUS_0_100K_OHM_PULL_DOWN| \
                                        HYS_0_HYSTERESIS_DISABLED) ;
  
  /* �װ�ķ�������GPIO���� */       
  beep_config.direction = kGPIO_DigitalOutput; //���ģʽ
  beep_config.outputLogic =  0;                //Ĭ�ϵ͵�ƽ
  beep_config.interruptMode = kGPIO_NoIntmode; //��ʹ���ж�
  
  /* ��ʼ�� BEEP GPIO. */
  GPIO_PinInit(BOARD_BEEP_GPIO, BOARD_BEEP_GPIO_PIN, &beep_config);	
}
/*********************************************END OF FILE**********************/
