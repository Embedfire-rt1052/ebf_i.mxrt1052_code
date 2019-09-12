
/**
	* GPIO����������ͷ�ļ�
  */

#include "MIMXRT1052.h"

/* GPIO ����ö�ٶ��� */
typedef enum _gpio_pin_direction
{
    kGPIO_DigitalInput  = 0U, /* ��������Ϊ���뷽�� */
    kGPIO_DigitalOutput = 1U, /* ��������Ϊ������� */
} gpio_pin_direction_t;

/* GPIO �ж�ģʽö�ٶ��� */
typedef enum _gpio_interrupt_mode
{
	kGPIO_NoIntmode = 0U, 						 /* ��������Ϊͨ��IO���ܣ���ʹ���жϣ� */
    kGPIO_IntLowLevel = 1U,					   /* �������ŵ͵�ƽ�����ж� */
    kGPIO_IntHighLevel = 2U, 					 /* �������Ÿߵ�ƽ�����ж� */
    kGPIO_IntRisingEdge = 3U, 				 /* �������������������ж� */
    kGPIO_IntFallingEdge = 4U, 				 /* ���������½��������ж� */
    kGPIO_IntRisingOrFallingEdge = 5U, /* �������������غ��½��ض������ж� */
} gpio_interrupt_mode_t;

/* GPIO �������ýṹ�嶨�� */
typedef struct _gpio_pin_config
{
    gpio_pin_direction_t  direction;      /* ָ�����ŵķ��� */
    uint8_t outputLogic; 									/* ����һ��Ĭ�ϵ������ƽ�������뷽��ʱ��������Ч */
    gpio_interrupt_mode_t interruptMode;  /* �������ŵ��ж�ģʽ����ѡֵΪ gpio_interrupt_mode_tö�ٱ��� */
} gpio_pin_config_t;

/* �������� */
void GPIO_PinInit(GPIO_Type* base, uint32_t pin, const gpio_pin_config_t* Config);
void GPIO_PinWrite(GPIO_Type* base, uint32_t pin, uint8_t output);






