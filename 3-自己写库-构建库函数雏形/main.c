
/**
	自己写库，构建库函数雏形
  */
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"


/*
  * 本章节代码是从 使用寄存器点亮LED灯章节代码修改过来
  * 属于循序渐进的一章，每一个步骤都对应一部分代码，所以
  * 该章节代码有好多个部分，不同部分都是使用条件编译来控制
  * 最后只保留了最后的代码部分。
  *
  * 条件编译跟C语言的 if else 类似，只有当条件为真的情况下的
  * 代码才会被编译，所以不要看到有多个main函数就以为不正确，
  * 其实最终编译的只有一个
 */

/* 原寄存器版本，条件为假表示不加入编译*/
#if 0

/* 旧方式需要一个个定义寄存器地址 */	
#define GPIO1_DR						*(unsigned int*)(GPIO1_BASE+0x00)
#define GPIO1_GDIR					*(unsigned int*)(GPIO1_BASE+0x04)
#define GPIO1_PSR						*(unsigned int*)(GPIO1_BASE+0x08)
#define GPIO1_ICR1					*(unsigned int*)(GPIO1_BASE+0x0C)
#define GPIO1_ICR2					*(unsigned int*)(GPIO1_BASE+0x10)
#define GPIO1_IMR						*(unsigned int*)(GPIO1_BASE+0x14)
#define GPIO1_ISR						*(unsigned int*)(GPIO1_BASE+0x18)
#define GPIO1_EDGE_SEL			*(unsigned int*)(GPIO1_BASE+0x1C)

/**
  *   主函数
  */
int main(void)
{	
	/* 开启GPIO1端口的时钟 */
	
	/* 清空控制GPIO1端口时钟的bit26、bit27 */
	
	CCM_CCGR1 &= ~(unsigned int)(3<<26);
	
	/* 把bit26、bit27设置为0b01，即开启GPIO1时钟 */
	CCM_CCGR1 |= (unsigned int)(1<<26);
		
	/* 设置MUX寄存器为0x05，表示把引脚用于普通GPIO */
	IOMUXC_MUX_GPIO_AD_B0_09 = (unsigned int)0x05;
	
	/* 设置PAD寄存器控制引脚的属性 */
	IOMUXC_PAD_GPIO_AD_B0_09 = (unsigned int)0x000B0;
	
	/*  属性配置说明:
		十六进制数0x000B0 = 二进制数0000 0000 0000 1011 0000 b
	*/
	
	/* 	bit0:       	SRE:	0 b   		压摆率: 慢压摆率
			bit1~bit2:  	-:		00 b    	寄存器保留项
			bit3~bit5:  	DSE:	110 b 		驱动强度: R0/6 （仅作为输出时有效 ）
			bit6~bit7:		SPEED:10 b 			带宽 : medium(100MHz)
			bit8~bit10:		-:		000 b			寄存器保留项
			bit11:				ODE:	0 b				开漏配置: 关闭 
																		（开漏高阻态常用于总线配置，如I2C ）
			bit12:				PKE:	0 b				拉/保持器配置: 关闭
			bit13:				PUE:	0 b				拉/保持器选择: 关闭了上下拉及保持器，任意值无效
			bit14~bit15:	PUS:	00 b			上拉/下拉选择: 关闭了上下拉及保持器，任意值无效
			bit16:				HYS:	0 b				滞回器配置: 关闭 
																	（仅输入时有效，施密特触发器，使能后可以过滤输入噪声）
			bit17~bit31: 	-:		0 b 			寄存器保留项
	*/
		
	/* 设置GPIO1_09引脚的方向，使用输出模式 */
	GPIO1_GDIR |= (unsigned int)(1<<9);

	/* 控制引脚为低电平，点亮LED灯 */
	GPIO1_DR &= ~(unsigned int)(1<<9);
	
	while(1);

}

/* 改进版1，使用结构体访问寄存器，条件为假表示不加入编译*/
#elif 0
/**
  *   主函数
  */
int main(void)
{	
	/* 开启GPIO1端口的时钟 */
	
	/* 清空控制GPIO1端口时钟的bit26、bit27 */
	
	CCM_CCGR1 &= ~(unsigned int)(3<<26);
	
	/* 把bit26、bit27设置为0b01，即开启GPIO1时钟 */
	CCM_CCGR1 |= (unsigned int)(1<<26);
		
	/* 设置MUX寄存器为0x05，表示把引脚用于普通GPIO */
	IOMUXC_MUX_GPIO_AD_B0_09 = (unsigned int)0x05;
	
	/* 设置PAD寄存器控制引脚的属性 */
	IOMUXC_PAD_GPIO_AD_B0_09 = (unsigned int)0x000B0;
	
	/*  属性配置说明:
		十六进制数0x000B0 = 二进制数0000 0000 0000 1011 0000 b
	*/
	
	/* 	bit0:       	SRE:	0 b   		压摆率: 慢压摆率
			bit1~bit2:  	-:		00 b    	寄存器保留项
			bit3~bit5:  	DSE:	110 b 		驱动强度: R0/6 （仅作为输出时有效 ）
			bit6~bit7:		SPEED:10 b 			带宽 : medium(100MHz)
			bit8~bit10:		-:		000 b			寄存器保留项
			bit11:				ODE:	0 b				开漏配置: 关闭 
																		（开漏高阻态常用于总线配置，如I2C ）
			bit12:				PKE:	0 b				拉/保持器配置: 关闭
			bit13:				PUE:	0 b				拉/保持器选择: 关闭了上下拉及保持器，任意值无效
			bit14~bit15:	PUS:	00 b			上拉/下拉选择: 关闭了上下拉及保持器，任意值无效
			bit16:				HYS:	0 b				滞回器配置: 关闭 
																	（仅输入时有效，施密特触发器，使能后可以过滤输入噪声）
			bit17~bit31: 	-:		0 b 			寄存器保留项
	*/
		
	/* 设置GPIO1_09引脚的方向，使用输出模式 */
	GPIO1->GDIR |= (unsigned int)(1<<9);

	/* 控制引脚为低电平，点亮LED灯 */
	GPIO1->DR &= ~(unsigned int)(1<<9);
	
	while(1);
}


/* 改进版2，使用GPIO函数，条件为假表示不加入编译 */
#elif 0
/**
  *   主函数
  */
int main(void)
{	
	/* 使用GPIO初始化结构体定义一个变量用于配置GPIO */
	gpio_pin_config_t led_config;
	/* 开启GPIO1端口的时钟 */
	
	/* 清空控制GPIO1端口时钟的bit26、bit27 */
	
	CCM_CCGR1 &= ~(unsigned int)(3<<26);
	
	/* 把bit26、bit27设置为0b01，即开启GPIO1时钟 */
	CCM_CCGR1 |= (unsigned int)(1<<26);
		
	/* 设置MUX寄存器为0x05，表示把引脚用于普通GPIO */
	IOMUXC_MUX_GPIO_AD_B0_09 = (unsigned int)0x05;
	
	/* 设置PAD寄存器控制引脚的属性 */
	IOMUXC_PAD_GPIO_AD_B0_09 = (unsigned int)0x000B0;
	
	/*  属性配置说明:
		十六进制数0x000B0 = 二进制数0000 0000 0000 1011 0000 b
	*/
	
	/* 	bit0:       	SRE:	0 b   		压摆率: 慢压摆率
			bit1~bit2:  	-:		00 b    	寄存器保留项
			bit3~bit5:  	DSE:	110 b 		驱动强度: R0/6 （仅作为输出时有效 ）
			bit6~bit7:		SPEED:10 b 			带宽 : medium(100MHz)
			bit8~bit10:		-:		000 b			寄存器保留项
			bit11:				ODE:	0 b				开漏配置: 关闭 
																		（开漏高阻态常用于总线配置，如I2C ）
			bit12:				PKE:	0 b				拉/保持器配置: 关闭
			bit13:				PUE:	0 b				拉/保持器选择: 关闭了上下拉及保持器，任意值无效
			bit14~bit15:	PUS:	00 b			上拉/下拉选择: 关闭了上下拉及保持器，任意值无效
			bit16:				HYS:	0 b				滞回器配置: 关闭 
																	（仅输入时有效，施密特触发器，使能后可以过滤输入噪声）
			bit17~bit31: 	-:		0 b 			寄存器保留项
	*/
		
//	/* 设置GPIO1_09引脚的方向，使用输出模式 */
//	GPIO1_GDIR |= (unsigned int)(1<<9);

//	/* 控制引脚为低电平，点亮LED灯 */
//	GPIO1_DR &= ~(unsigned int)(1<<9);

	led_config.direction = kGPIO_DigitalOutput;	//输出模式
	led_config.outputLogic = 1;									//默认高电平
	led_config.interruptMode = kGPIO_NoIntmode;	//不使用中断
	
	/* 使用led_config 初始化GPIO1_IO09*/
	GPIO_PinInit(GPIO1,9,&led_config);
	
	/* 控制GPIO1_IO09为低电平，点亮LED灯 */
	GPIO_PinWrite(GPIO1,9,0);
	
	while(1);

}

/* 改进版3，使用IOMUXC函数，条件为假表示不加入编译 */
#elif 1

/**
  *   主函数
  */
int main(void)
{	
	/* 使用GPIO初始化结构体定义一个变量用于配置GPIO */
	gpio_pin_config_t led_config;
  
	/* 开启GPIO1端口的时钟 */
	
	/* 清空控制GPIO1端口时钟的bit26、bit27 */
	
	CCM_CCGR1 &= ~(unsigned int)(3<<26);
	
	/* 把bit26、bit27设置为0b01，即开启GPIO1时钟 */
	CCM_CCGR1 |= (unsigned int)(1<<26);
		 
  /* 设置引脚为MUX_MODE及SION
      IOMUXC_GPIO_AD_B0_09_GPIO1_IO09宏表示的五个参数：
      0x401F80E0U,  GPIO_AD_B0_09的MUX寄存器的地址
      0x5U,         选择ATL5,即GPIO功能
      0,            可选的要同时配置的寄存器，为0表示不配置
      0,            可选寄存器要赋予的值
      0x401F82D0U   GPIO_AD_B0_09的PAD属性配置寄存器的地址
  */
  /*  设置复用为GPIO1_IO09功能
      不使用SION功能
  */  
	IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_09_GPIO1_IO09, 
										0 );		


	
	/* 设置引脚属性 */
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_09_GPIO1_IO09,
											SRE_0_SLOW_SLEW_RATE|
											DSE_6_R0_6|
											SPEED_2_MEDIUM_100MHz|
											ODE_0_OPEN_DRAIN_DISABLED|
											PKE_0_PULL_KEEPER_DISABLED|
											PUE_0_KEEPER_SELECTED|
											PUS_0_100K_OHM_PULL_DOWN|
											HYS_0_HYSTERESIS_DISABLED
											);

	led_config.direction = kGPIO_DigitalOutput;	//输出模式
	led_config.outputLogic = 1;									//默认高电平
	led_config.interruptMode = kGPIO_NoIntmode;	//不使用中断
	
	/* 使用led_config 初始化GPIO1_IO09*/
	GPIO_PinInit(GPIO1,9,&led_config);
	
	/* 控制GPIO1_IO09为低电平，点亮LED灯 */
	GPIO_PinWrite(GPIO1,9,0);
	
	while(1);

}

#endif



// 函数为空，目的是为了骗过编译器不报错
void SystemInit(void)
{	
}
