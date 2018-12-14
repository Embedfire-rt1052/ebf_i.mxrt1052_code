/**
	* GPIO外设控制相关的库函数示例
  */
#include "fsl_gpio.h"

/*
 *  设置一个GPIO引脚输出高（1）低（0）电平
 *
 *  base: GPIO_Type类型的指针，如GPIO1、GPIO2等宏
 *  pin:要控制引脚的编号
 *  output:要输出的逻辑电平
 *        - 0:低电平
 *        - 1:高电平
 *	使用范例，控制GPIO1_IO09引脚输出低电平：GPIO_PinWrite(GPIO1,9,0)
 */
void GPIO_PinWrite(GPIO_Type* base, uint32_t pin, uint8_t output)
{
    if (output == 0U)
    {
        base->DR &= ~(1U << pin);  /* 设置引脚输出低电平 */
    }
    else
    {
        base->DR |= (1U << pin);  /* 设置引脚输出高电平 */
    }
}

/*
 *  设置指定引脚的中断模式 
 *  base: GPIO_Type类型的指针，如GPIO1、GPIO2等宏
 *  pin:要控制引脚的编号
 *  pininterruptMode: gpio_interrupt_mode_t类型的指针 
 *   		该结构体包含中断配置的信息
 */
void GPIO_PinSetInterruptConfig(GPIO_Type* base, uint32_t pin, 
                               gpio_interrupt_mode_t pinInterruptMode)
{
    volatile uint32_t *icr;
    uint32_t icrShift;

		/* icrShift初值为引脚号，后面用来定位引脚对应的寄存器配置位
     * 如pin0的配置位为bit0、bit1，pin1的配置位为bit2、bit3
  	*/
    icrShift = pin;

		/* 编号小于16的使用ICR1寄存器控制，其它在ICR2控制 */
    if(pin < 16)
    {
				/* icr指针指向ICR1 */
        icr = &(base->ICR1);
    }
    else
    {
				/* icr指针指向ICR2 */
        icr = &(base->ICR2);
				/* 对应引脚配置位跟引脚号的关系要减16 
			   * 如pin16的配置位为bit0、bit1，pin17的配置位为bit2、bit3
				*/
        icrShift -= 16;
    }

	    /* 先对EDGE_SEL寄存器相应引脚的控制位清零，
		因为 EDGE_SEL非零的话ICR寄存器的配置无效，
		引脚会被直接配置为双边沿模式 */
    base->EDGE_SEL &= ~(1U << pin);
		
		/* 根据中断模式配置寄存器 */
    switch(pinInterruptMode)
    {
				/* 高低电平或单边沿触发配置ICR寄存器 */
        case(kGPIO_IntLowLevel):
			/* 对应ICR寄存器位清零：0b00，低电平触发 */
            *icr &= ~(3U << (2 * icrShift));
            break;
        case(kGPIO_IntHighLevel):
			/* 对应ICR寄存器位清零后赋值为1：0b01 ，高电平触发 */
            *icr = (*icr & (~(3U << (2 * icrShift)))) | (1U << (2 * icrShift));
            break;
        case(kGPIO_IntRisingEdge):
			/* 对应ICR寄存器位清零后赋值为2：0b10 ，上升沿触发 */
            *icr = (*icr & (~(3U << (2 * icrShift)))) | (2U << (2 * icrShift));
            break;
        case(kGPIO_IntFallingEdge):
			/* 对应ICR寄存器位赋值为3：0b11 ，下降沿触发 */
            *icr |= (3U << (2 * icrShift));
            break;
				
				/* 双边沿触发配置EDGE_SEL寄存器 */
        case(kGPIO_IntRisingOrFallingEdge):
			/* 对应EDGE_SEL寄存器位置1，配置为双边沿触发 */
            base->EDGE_SEL |= (1U << pin);
            break;
        default:
            break;
    }
}

/*
 *  根据输入参数初始化GPIO外设
 *
 *  base:GPIO_Type类型的指针，如GPIO1、GPIO2等宏
 *  pin: 要控制引脚的编号
 *  initConfig: gpio_pin_config_t类型的指针，
					该结构体指针包含各种初始化信息
 */
void GPIO_PinInit(GPIO_Type* base, uint32_t pin, const gpio_pin_config_t* Config)
{
    /* 对相应引脚IMR寄存器的控制位清零，先关闭中断 */
    base->IMR &= ~(1U << pin);

    /* 配置GPIO引脚的方向 */
    if (Config->direction == kGPIO_DigitalInput)
    {
				/* 输入模式 */
        base->GDIR &= ~(1U << pin);
    }
    else
    {
				/* 输出模式 */
				/* 先对DR寄存器赋值默认电平 */
        GPIO_PinWrite(base, pin, Config->outputLogic);
				/* 配置为输出模式 */
        base->GDIR |= (1U << pin);
    }

    /* 配置GPIO引脚的中断模式 */
    GPIO_PinSetInterruptConfig(base, pin, Config->interruptMode);
}
