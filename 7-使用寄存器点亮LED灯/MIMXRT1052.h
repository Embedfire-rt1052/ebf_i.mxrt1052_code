
/* 地址可查阅 参考手册-GPIO章节-GPIO Memory Map */
/* GPIO1外设基地址 */
#define GPIO1_BASE        (unsigned int)(0x401B8000u)
	
/* GPIO1寄存器地址,强制转换成指针 */	
#define GPIO1_DR						*(unsigned int*)(GPIO1_BASE+0x00)
#define GPIO1_GDIR					*(unsigned int*)(GPIO1_BASE+0x04)
#define GPIO1_PSR						*(unsigned int*)(GPIO1_BASE+0x08)
#define GPIO1_ICR1					*(unsigned int*)(GPIO1_BASE+0x0C)
#define GPIO1_ICR2					*(unsigned int*)(GPIO1_BASE+0x10)
#define GPIO1_IMR						*(unsigned int*)(GPIO1_BASE+0x14)
#define GPIO1_ISR						*(unsigned int*)(GPIO1_BASE+0x18)
#define GPIO1_EDGE_SEL			*(unsigned int*)(GPIO1_BASE+0x1C)

/* 地址可查阅 参考手册-IOMUXC章节-IOMUXC Memory Map/Register Definition */
/* IOMUXC基地址 */
#define IOMUXC_BASE 				(unsigned int)(0x401F8000u)

/* MUX模式选择寄存器 IOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B0_09 ,强制转换成指针*/
#define IOMUXC_MUX_GPIO_AD_B0_09  	*(unsigned int*)(IOMUXC_BASE + 0xE0 )

#define MUX_MODE_ALT0_JTAG_TDI                 0x0
#define MUX_MODE_ALT1_FLEXPWM2_PWMA03          0x1
#define MUX_MODE_ALT2_ENET_RX_DATA02           0x2
#define MUX_MODE_ALT3_SAI2_TX_DATA             0x3
#define MUX_MODE_ALT4_CSI_DATA04               0x4
#define MUX_MODE_ALT5_GPIO1_IO09               0x5
#define MUX_MODE_ALT6_XBAR1_IN21               0x6
#define MUX_MODE_ALT7_GPT2_CLK                 0x7

/* PAD属性设置寄存器 IOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B0_09 ,强制转换成指针*/
#define IOMUXC_PAD_GPIO_AD_B0_09  	*(unsigned int*)(IOMUXC_BASE + 0x2D0 )

/* 地址可查阅 参考手册-CCM章节-CCM Memory Map/Register Definition */
/* CCM时钟控制外设基地址 */
#define CCM_BASE												(unsigned int)(0x400FC000u)

/* 时钟配置寄存器CCM_CCGR1地址 */
#define CCM_CCGR1	 											*(unsigned int*)(CCM_BASE + 0x6C)

/* GPIO1时钟配置域在寄存器CCM_CCGR1中的位号 */
#define CCM_CCGR1_CG13_SHIFT              (26U)

/* 控制LED灯亮灭的宏 */
#define CORE_BOARD_LED_ON			GPIO1_DR &=~ (1<<9);
#define CORE_BOARD_LED_OFF		GPIO1_DR |= (1<<9);
