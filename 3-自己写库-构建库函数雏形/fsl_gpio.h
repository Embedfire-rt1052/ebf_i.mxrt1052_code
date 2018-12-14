
/**
	* GPIO外设控制相关头文件
  */

#include "MIMXRT1052.h"

/* GPIO 方向枚举定义 */
typedef enum _gpio_pin_direction
{
    kGPIO_DigitalInput  = 0U, /* 设置引脚为输入方向 */
    kGPIO_DigitalOutput = 1U, /* 设置引脚为输出方向 */
} gpio_pin_direction_t;

/* GPIO 中断模式枚举定义 */
typedef enum _gpio_interrupt_mode
{
	kGPIO_NoIntmode = 0U, 						 /* 设置引脚为通用IO功能（不使用中断） */
    kGPIO_IntLowLevel = 1U,					   /* 设置引脚低电平引起中断 */
    kGPIO_IntHighLevel = 2U, 					 /* 设置引脚高电平引起中断 */
    kGPIO_IntRisingEdge = 3U, 				 /* 设置引脚上升沿引起中断 */
    kGPIO_IntFallingEdge = 4U, 				 /* 设置引脚下降沿引起中断 */
    kGPIO_IntRisingOrFallingEdge = 5U, /* 设置引脚上升沿和下降沿都引脚中断 */
} gpio_interrupt_mode_t;

/* GPIO 引脚配置结构体定义 */
typedef struct _gpio_pin_config
{
    gpio_pin_direction_t  direction;      /* 指定引脚的方向 */
    uint8_t outputLogic; 									/* 设置一个默认的输出电平，在输入方向时本设置无效 */
    gpio_interrupt_mode_t interruptMode;  /* 设置引脚的中断模式，可选值为 gpio_interrupt_mode_t枚举变量 */
} gpio_pin_config_t;

/* 函数声明 */
void GPIO_PinInit(GPIO_Type* base, uint32_t pin, const gpio_pin_config_t* Config);
void GPIO_PinWrite(GPIO_Type* base, uint32_t pin, uint8_t output);






