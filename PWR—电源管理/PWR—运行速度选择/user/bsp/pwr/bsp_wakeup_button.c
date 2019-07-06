/**
  ******************************************************************
  * @file    bsp_wakeup_button.c
  * @author  fire
  * @version V1.1
  * @date    2018-xx-xx
  * @brief   wakeup_button�ú����ӿ�
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
#include "./pwr/bsp_wakeup_button.h"


/**
  * @brief  ��ʼ�����Ѱ�����IO
  * @param  ��
  * @retval ��
  */
void Wakeup_Button_Config(void)
{
    /* �������뿪�����ŵ�init�ṹ */
    gpio_pin_config_t swConfig;

		/** �װ����İ��	WAKEUP	���� ���� **/
    swConfig.direction = kGPIO_DigitalInput;   //���ģʽ
    swConfig.interruptMode = 0;                //Ĭ�ϵ͵�ƽ
    swConfig.outputLogic = kGPIO_IntRisingEdge;//�������ж�

    /* Init���뿪��GPIO. */
    GPIO_PinInit(WAKEUP_BUTTON_GPIO, WAKEUP_BUTTON_GPIO_PIN, &swConfig);
    GPIO_EnableInterrupts(WAKEUP_BUTTON_GPIO, 1U << WAKEUP_BUTTON_GPIO_PIN);

}





