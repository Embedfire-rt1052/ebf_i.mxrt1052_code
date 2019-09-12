
/**
	使用寄存器的方法点亮LED灯
  */
#include "MIMXRT1052.h"


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

// 函数为空，目的是为了骗过编译器不报错
void SystemInit(void)
{	
}

