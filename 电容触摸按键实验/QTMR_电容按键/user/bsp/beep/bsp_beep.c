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
  
   /* ����BEEP���ŵ�IOMUXC PAD���� */ 
  IOMUXC_SetPinConfig(BOARD_BEEP_IOMUXC, BEEP_PAD_CONFIG_DATA);
  
  /* �װ�ķ�������GPIO���� */       
  beep_config.direction = kGPIO_DigitalOutput; //���ģʽ
  beep_config.outputLogic =  0;                //Ĭ�ϵ͵�ƽ
  beep_config.interruptMode = kGPIO_NoIntmode; //��ʹ���ж�
  
  /* ��ʼ�� BEEP GPIO. */
  GPIO_PinInit(BOARD_BEEP_GPIO, BOARD_BEEP_GPIO_PIN, &beep_config);	
}
/*********************************************END OF FILE**********************/
