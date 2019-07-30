/**
  ******************************************************************************
  * @file    bsp_exti.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   陀螺仪数据中断配置
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火  STM32 F767 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./mpu6050_DMP/bsp_mpu_exti.h"
#include "fsl_iomuxc.h"
#include "fsl_gpio.h" 
#include "pad_config.h" 
#include "./bsp/nvic/bsp_nvic.h"
/* 按键中断检测引脚的PAD配置 */
#define MPU_PAD_CONFIG_DATA            (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_0_OUTPUT_DRIVER_DISABLED| \
                                        SPEED_2_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_1_PULL_KEEPER_ENABLED| \
                                        PUE_1_PULL_SELECTED| \
                                        PUS_3_22K_OHM_PULL_UP| \
                                        HYS_1_HYSTERESIS_ENABLED)   
    /* 配置说明 : */
    /* 转换速率: 转换速率慢
      驱动强度: 关闭
      速度配置 : medium(100MHz)
      开漏配置: 关闭 
      拉/保持器配置: 使能
      拉/保持器选择: 上下拉
      上拉/下拉选择: 22K欧姆上拉
      滞回器配置: 开启 （仅输入时有效，施密特触发器，使能后可以过滤输入噪声）*/

/******************************************************************
 * 声明
  ******************************************************************/
static void MPU_IOMUXC_MUX_Config(void);
static void MPU_IOMUXC_PAD_Config(void);
static void MPU_GPIO_Mode_Config(void);
/* 按键是否被按下的中断标志 */
__IO bool g_KeyDown[2] = { false};

/******************************************************************
 * 函数代码
  ******************************************************************/
/**
* @brief  初始化按键相关IOMUXC的MUX复用配置
* @param  无
* @retval 无
*/
static void MPU_IOMUXC_MUX_Config(void)
{
  /* 设置按键引脚的复用模式为GPIO，不使用SION功能 */
  IOMUXC_SetPinMux(MPU_EXTI_IOMUXC, 0U);
}

/**
* @brief  初始化按键相关引脚PAD属性
* @param  无
* @retval 无
*/
static void MPU_IOMUXC_PAD_Config(void)
{
  /* 设置按键引脚属性功能 */    
  IOMUXC_SetPinConfig(MPU_EXTI_IOMUXC, MPU_PAD_CONFIG_DATA); 
}

 /**
  * @brief  初始化按键相关的GPIO模式
  * @param  无
  * @retval 无
  */
static void MPU_GPIO_Mode_Config(void)
{     
  /* 配置为输入模式，低电平中断，后面通过GPIO_PinInit函数加载配置 */
  gpio_pin_config_t mpu_config;
  
//  /** 核心板的按键，GPIO配置 **/       
  mpu_config.direction = kGPIO_DigitalInput;    //输入模式
  mpu_config.outputLogic =  1;                  //默认高电平（输入模式时无效）
  mpu_config.interruptMode = kGPIO_IntFallingEdge; //低电平触发中断
	  /** 核心板的按键，GPIO配置 **/       
//  mpu_config.direction = kGPIO_DigitalInput;    //输入模式
//  mpu_config.outputLogic =  0;                  //默认高电平（输入模式时无效）
//  mpu_config.interruptMode = kGPIO_IntRisingEdge; //低电平触发中断
  
//	typedef enum _gpio_interrupt_mode
//{
//    kGPIO_NoIntmode = 0U,              /*!< Set current pin general IO functionality.*/
//    kGPIO_IntLowLevel = 1U,            /*!< Set current pin interrupt is low-level sensitive.*/
//    kGPIO_IntHighLevel = 2U,           /*!< Set current pin interrupt is high-level sensitive.*/
//    kGPIO_IntRisingEdge = 3U,          /*!< Set current pin interrupt is rising-edge sensitive.*/
//    kGPIO_IntFallingEdge = 4U,         /*!< Set current pin interrupt is falling-edge sensitive.*/
//    kGPIO_IntRisingOrFallingEdge = 5U, /*!< Enable the edge select bit to override the ICR register's configuration.*/
//} gpio_interrupt_mode_t;
	
  /* 初始化 KEY GPIO. */
  GPIO_PinInit(MPU_EXTI_GPIO, MPU_EXTI_GPIO_PIN, &mpu_config);
}




/**
 * @brief  初始化按键中断相关的内容
 * @param  无
 * @retval 无
 */
static void MPU_Interrupt_Config(void)   
{
  /* 开启GPIO引脚的中断 */
  GPIO_PortEnableInterrupts(MPU_EXTI_GPIO, 1U << MPU_EXTI_GPIO_PIN);                           
  /*设置中断优先级,*/
  set_IRQn_Priority(MPU_EXTI_IRQ,Group4_PreemptPriority_6, Group4_SubPriority_0);
  /* 开启GPIO端口中断 */
  EnableIRQ(MPU_EXTI_IRQ);
}


 /**
  * @brief  初始化控制KEY的IO    
  * @param  无
  * @retval 无
  */
void MPU_IT_GPIO_Config(void)
{
		/* 初始化GPIO复用、属性、模式及中断*/
		MPU_IOMUXC_MUX_Config();
		MPU_IOMUXC_PAD_Config();
		MPU_GPIO_Mode_Config();
		MPU_Interrupt_Config();
}
void EXTI_MPU_Config()
{
	MPU_IT_GPIO_Config();
}



/********************中断服务函数**************************/
/**
 * @brief  GPIO 输入中断服务函数
 *         CORE_BOARD_WAUP_KEY_IRQHandler只是一个宏，
 *         在本例中它指代函数名GPIO5_Combined_0_15_IRQHandler，
 *         中断服务函数名是固定的，可以在启动文件中找到。
 * @param  中断服务函数不能有输入参数
 * @note   中断函数一般只使用标志位进行指示，完成后尽快退出，
 *         具体操作或延时尽量不放在中断服务函数中
 * @retval 中断服务函数不能有返回值
 */
extern void gyro_data_ready_cb(void);
void MPU_EXTI_IRQHandler(void)
{ 
    /* 清除中断标志位 */
    GPIO_PortClearInterruptFlags(MPU_EXTI_GPIO,1U << MPU_EXTI_GPIO_PIN);  
  
		/* Handle new gyro*/
		gyro_data_ready_cb();
	
    /* 以下部分是为 ARM 的勘误838869添加的, 
       该错误影响 Cortex-M4, Cortex-M4F内核，       
       立即存储覆盖重叠异常，导致返回操作可能会指向错误的中断
        CM7不受影响，此处保留该代码
    */
  
    /* 原注释：Add for ARM errata 838869, affects Cortex-M4,
       Cortex-M4F Store immediate overlapping
       exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}










/*********************************************END OF FILE**********************/
