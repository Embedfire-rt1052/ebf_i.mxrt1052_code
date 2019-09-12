/**
	* GPIO���������صĿ⺯��ʾ��
  */
#include "fsl_gpio.h"

/*
 *  ����һ��GPIO��������ߣ�1���ͣ�0����ƽ
 *
 *  base: GPIO_Type���͵�ָ�룬��GPIO1��GPIO2�Ⱥ�
 *  pin:Ҫ�������ŵı��
 *  output:Ҫ������߼���ƽ
 *        - 0:�͵�ƽ
 *        - 1:�ߵ�ƽ
 *	ʹ�÷���������GPIO1_IO09��������͵�ƽ��GPIO_PinWrite(GPIO1,9,0)
 */
void GPIO_PinWrite(GPIO_Type* base, uint32_t pin, uint8_t output)
{
    if (output == 0U)
    {
        base->DR &= ~(1U << pin);  /* ������������͵�ƽ */
    }
    else
    {
        base->DR |= (1U << pin);  /* ������������ߵ�ƽ */
    }
}

/*
 *  ����ָ�����ŵ��ж�ģʽ 
 *  base: GPIO_Type���͵�ָ�룬��GPIO1��GPIO2�Ⱥ�
 *  pin:Ҫ�������ŵı��
 *  pininterruptMode: gpio_interrupt_mode_t���͵�ָ�� 
 *   		�ýṹ������ж����õ���Ϣ
 */
void GPIO_PinSetInterruptConfig(GPIO_Type* base, uint32_t pin, 
                               gpio_interrupt_mode_t pinInterruptMode)
{
    volatile uint32_t *icr;
    uint32_t icrShift;

		/* icrShift��ֵΪ���źţ�����������λ���Ŷ�Ӧ�ļĴ�������λ
     * ��pin0������λΪbit0��bit1��pin1������λΪbit2��bit3
  	*/
    icrShift = pin;

		/* ���С��16��ʹ��ICR1�Ĵ������ƣ�������ICR2���� */
    if(pin < 16)
    {
				/* icrָ��ָ��ICR1 */
        icr = &(base->ICR1);
    }
    else
    {
				/* icrָ��ָ��ICR2 */
        icr = &(base->ICR2);
				/* ��Ӧ��������λ�����źŵĹ�ϵҪ��16 
			   * ��pin16������λΪbit0��bit1��pin17������λΪbit2��bit3
				*/
        icrShift -= 16;
    }

	    /* �ȶ�EDGE_SEL�Ĵ�����Ӧ���ŵĿ���λ���㣬
		��Ϊ EDGE_SEL����Ļ�ICR�Ĵ�����������Ч��
		���Żᱻֱ������Ϊ˫����ģʽ */
    base->EDGE_SEL &= ~(1U << pin);
		
		/* �����ж�ģʽ���üĴ��� */
    switch(pinInterruptMode)
    {
				/* �ߵ͵�ƽ�򵥱��ش�������ICR�Ĵ��� */
        case(kGPIO_IntLowLevel):
			/* ��ӦICR�Ĵ���λ���㣺0b00���͵�ƽ���� */
            *icr &= ~(3U << (2 * icrShift));
            break;
        case(kGPIO_IntHighLevel):
			/* ��ӦICR�Ĵ���λ�����ֵΪ1��0b01 ���ߵ�ƽ���� */
            *icr = (*icr & (~(3U << (2 * icrShift)))) | (1U << (2 * icrShift));
            break;
        case(kGPIO_IntRisingEdge):
			/* ��ӦICR�Ĵ���λ�����ֵΪ2��0b10 �������ش��� */
            *icr = (*icr & (~(3U << (2 * icrShift)))) | (2U << (2 * icrShift));
            break;
        case(kGPIO_IntFallingEdge):
			/* ��ӦICR�Ĵ���λ��ֵΪ3��0b11 ���½��ش��� */
            *icr |= (3U << (2 * icrShift));
            break;
				
				/* ˫���ش�������EDGE_SEL�Ĵ��� */
        case(kGPIO_IntRisingOrFallingEdge):
			/* ��ӦEDGE_SEL�Ĵ���λ��1������Ϊ˫���ش��� */
            base->EDGE_SEL |= (1U << pin);
            break;
        default:
            break;
    }
}

/*
 *  �������������ʼ��GPIO����
 *
 *  base:GPIO_Type���͵�ָ�룬��GPIO1��GPIO2�Ⱥ�
 *  pin: Ҫ�������ŵı��
 *  initConfig: gpio_pin_config_t���͵�ָ�룬
					�ýṹ��ָ��������ֳ�ʼ����Ϣ
 */
void GPIO_PinInit(GPIO_Type* base, uint32_t pin, const gpio_pin_config_t* Config)
{
    /* ����Ӧ����IMR�Ĵ����Ŀ���λ���㣬�ȹر��ж� */
    base->IMR &= ~(1U << pin);

    /* ����GPIO���ŵķ��� */
    if (Config->direction == kGPIO_DigitalInput)
    {
				/* ����ģʽ */
        base->GDIR &= ~(1U << pin);
    }
    else
    {
				/* ���ģʽ */
				/* �ȶ�DR�Ĵ�����ֵĬ�ϵ�ƽ */
        GPIO_PinWrite(base, pin, Config->outputLogic);
				/* ����Ϊ���ģʽ */
        base->GDIR |= (1U << pin);
    }

    /* ����GPIO���ŵ��ж�ģʽ */
    GPIO_PinSetInterruptConfig(base, pin, Config->interruptMode);
}
