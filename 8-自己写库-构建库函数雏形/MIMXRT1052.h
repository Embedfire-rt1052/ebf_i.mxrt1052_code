
/* 寄存器的值常常是芯片外设自动更改的，
 * 即使CPU没有执行程序，也有可能发生变化
 * 编译器有可能会对没有执行程序的变量进行优化
*/

/* volatile表示易变的变量，防止编译器优化，*/
#define  __IO  volatile        /* 可读写，一般用于定义有可读写权限的寄存器*/
#define  __I   volatile const  /* 只读，一般用于定义只读权限的寄存器 */


/* 使用更简短直观的方式来定义无符号32、16、8位变量 */	
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

/* GPIO 寄存器结构体 */
typedef struct {
  __IO uint32_t DR;         /* DR数据寄存器，						地址偏移: 0x0 */
  __IO uint32_t GDIR;       /* GDIR方向寄存器，					地址偏移: 0x4 */
  __I  uint32_t PSR;        /* PSR状态寄存器，						地址偏移: 0x8 */
  __IO uint32_t ICR1;       /* ICR1中断配置寄存器1,			地址偏移: 0xC */
  __IO uint32_t ICR2;       /* ICR2中断配置寄存器2,			地址偏移: 0x10 */
  __IO uint32_t IMR;        /* IMR中断掩码寄存器,				地址偏移: 0x14 */
  __IO uint32_t ISR;        /* ISR中断状态寄存器,				地址偏移: 0x18 */
  __IO uint32_t EDGE_SEL;   /* EDGE_SEL边沿选择寄存器，	地址偏移: 0x1C */
} GPIO_Type;

/* 地址可查阅 参考手册-GPIO章节-GPIO Memory Map */
/* GPIO外设基地址 */
#define GPIO1_BASE        (unsigned int)(0x401B8000u)
#define GPIO2_BASE        (unsigned int)(0x401BC000u)
#define GPIO3_BASE        (unsigned int)(0x401C0000u)
#define GPIO4_BASE        (unsigned int)(0x401C4000u)
#define GPIO5_BASE        (unsigned int)(0x400C0000u)

/* 定义GPIO1-5 寄存器结构体指针 */
#define GPIO1          ((GPIO_Type *)GPIO1_BASE)
#define GPIO2          ((GPIO_Type *)GPIO2_BASE)
#define GPIO3          ((GPIO_Type *)GPIO3_BASE)
#define GPIO4          ((GPIO_Type *)GPIO4_BASE)
#define GPIO5          ((GPIO_Type *)GPIO5_BASE)
	
/* 使用结构体指针后，不需要这样定义了 GPIO1寄存器地址,强制转换成指针 */	
//#define GPIO1_DR						*(unsigned int*)(GPIO1_BASE+0x00)
//#define GPIO1_GDIR					*(unsigned int*)(GPIO1_BASE+0x04)
//#define GPIO1_PSR						*(unsigned int*)(GPIO1_BASE+0x08)
//#define GPIO1_ICR1					*(unsigned int*)(GPIO1_BASE+0x0C)
//#define GPIO1_ICR2					*(unsigned int*)(GPIO1_BASE+0x10)
//#define GPIO1_IMR						*(unsigned int*)(GPIO1_BASE+0x14)
//#define GPIO1_ISR						*(unsigned int*)(GPIO1_BASE+0x18)
//#define GPIO1_EDGE_SEL			*(unsigned int*)(GPIO1_BASE+0x1C)
	

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
  

/* MUX寄存器各个配置域的掩码、偏移及设置宏 */
/* MUX_MODE配置 */
#define IOMUXC_SW_MUX_CTL_PAD_MUX_MODE_MASK      (0x7U)
#define IOMUXC_SW_MUX_CTL_PAD_MUX_MODE_SHIFT     (0U)
#define IOMUXC_SW_MUX_CTL_PAD_MUX_MODE(x)        (((uint32_t)(((uint32_t)(x)) << IOMUXC_SW_MUX_CTL_PAD_MUX_MODE_SHIFT)) & IOMUXC_SW_MUX_CTL_PAD_MUX_MODE_MASK)

/* SION配置 */
#define IOMUXC_SW_MUX_CTL_PAD_SION_MASK          (0x10U)
#define IOMUXC_SW_MUX_CTL_PAD_SION_SHIFT         (4U)
#define IOMUXC_SW_MUX_CTL_PAD_SION(x)            (((uint32_t)(((uint32_t)(x)) << IOMUXC_SW_MUX_CTL_PAD_SION_SHIFT)) & IOMUXC_SW_MUX_CTL_PAD_SION_MASK)


/* PAD属性寄存器各个配置域的掩码、偏移及设置宏 */
/* SRE 压摆率配置*/
#define IOMUXC_SW_PAD_CTL_PAD_SRE_MASK           (0x1U)
#define IOMUXC_SW_PAD_CTL_PAD_SRE_SHIFT          (0U)
#define IOMUXC_SW_PAD_CTL_PAD_SRE(x)             (((uint32_t)(((uint32_t)(x)) << IOMUXC_SW_PAD_CTL_PAD_SRE_SHIFT)) & IOMUXC_SW_PAD_CTL_PAD_SRE_MASK)

/* DSE 驱动能力配置 */
#define IOMUXC_SW_PAD_CTL_PAD_DSE_MASK           (0x38U)
#define IOMUXC_SW_PAD_CTL_PAD_DSE_SHIFT          (3U)
#define IOMUXC_SW_PAD_CTL_PAD_DSE(x)             (((uint32_t)(((uint32_t)(x)) << IOMUXC_SW_PAD_CTL_PAD_DSE_SHIFT)) & IOMUXC_SW_PAD_CTL_PAD_DSE_MASK)

/* SPEED 带宽配置 */
#define IOMUXC_SW_PAD_CTL_PAD_SPEED_MASK         (0xC0U)
#define IOMUXC_SW_PAD_CTL_PAD_SPEED_SHIFT        (6U)
#define IOMUXC_SW_PAD_CTL_PAD_SPEED(x)           (((uint32_t)(((uint32_t)(x)) << IOMUXC_SW_PAD_CTL_PAD_SPEED_SHIFT)) & IOMUXC_SW_PAD_CTL_PAD_SPEED_MASK)

/* ODE 开漏输出配置 */
#define IOMUXC_SW_PAD_CTL_PAD_ODE_MASK           (0x800U)
#define IOMUXC_SW_PAD_CTL_PAD_ODE_SHIFT          (11U)
#define IOMUXC_SW_PAD_CTL_PAD_ODE(x)             (((uint32_t)(((uint32_t)(x)) << IOMUXC_SW_PAD_CTL_PAD_ODE_SHIFT)) & IOMUXC_SW_PAD_CTL_PAD_ODE_MASK)

/* PKE 上下拉、保持器使能配置 */
#define IOMUXC_SW_PAD_CTL_PAD_PKE_MASK           (0x1000U)
#define IOMUXC_SW_PAD_CTL_PAD_PKE_SHIFT          (12U)
#define IOMUXC_SW_PAD_CTL_PAD_PKE(x)             (((uint32_t)(((uint32_t)(x)) << IOMUXC_SW_PAD_CTL_PAD_PKE_SHIFT)) & IOMUXC_SW_PAD_CTL_PAD_PKE_MASK)

/* PUE 选择使用上下拉还是保持器 */
#define IOMUXC_SW_PAD_CTL_PAD_PUE_MASK           (0x2000U)
#define IOMUXC_SW_PAD_CTL_PAD_PUE_SHIFT          (13U)
#define IOMUXC_SW_PAD_CTL_PAD_PUE(x)             (((uint32_t)(((uint32_t)(x)) << IOMUXC_SW_PAD_CTL_PAD_PUE_SHIFT)) & IOMUXC_SW_PAD_CTL_PAD_PUE_MASK)

/* PUS 选择上拉还是下拉以及电阻 */
#define IOMUXC_SW_PAD_CTL_PAD_PUS_MASK           (0xC000U)
#define IOMUXC_SW_PAD_CTL_PAD_PUS_SHIFT          (14U)
#define IOMUXC_SW_PAD_CTL_PAD_PUS(x)             (((uint32_t)(((uint32_t)(x)) << IOMUXC_SW_PAD_CTL_PAD_PUS_SHIFT)) & IOMUXC_SW_PAD_CTL_PAD_PUS_MASK)

/* HYS 滞后使能 */
#define IOMUXC_SW_PAD_CTL_PAD_HYS_MASK           (0x10000U)
#define IOMUXC_SW_PAD_CTL_PAD_HYS_SHIFT          (16U)
#define IOMUXC_SW_PAD_CTL_PAD_HYS(x)             (((uint32_t)(((uint32_t)(x)) << IOMUXC_SW_PAD_CTL_PAD_HYS_SHIFT)) & IOMUXC_SW_PAD_CTL_PAD_HYS_MASK)

/* PAD属性配置寄存器的配置*/
/* SRE 压摆率选择 */
#define SRE_0_SLOW_SLEW_RATE                IOMUXC_SW_PAD_CTL_PAD_SRE(0)
#define SRE_1_FAST_SLEW_RATE                IOMUXC_SW_PAD_CTL_PAD_SRE(1)

/* 驱动能力配置，配置阻值的大小 */
#define DSE_0_OUTPUT_DRIVER_DISABLED        IOMUXC_SW_PAD_CTL_PAD_DSE(0)
/* R0 260 Ohm @ 3.3V, 150Ohm@1.8V, 240 Ohm for DDR */
#define DSE_1_R0_1                          IOMUXC_SW_PAD_CTL_PAD_DSE(1) 
/* R0/2 */
#define DSE_2_R0_2                          IOMUXC_SW_PAD_CTL_PAD_DSE(2)
/* R0/3 */
#define DSE_3_R0_3                          IOMUXC_SW_PAD_CTL_PAD_DSE(3)
/* R0/4 */
#define DSE_4_R0_4                          IOMUXC_SW_PAD_CTL_PAD_DSE(4)
/* R0/5 */
#define DSE_5_R0_5                          IOMUXC_SW_PAD_CTL_PAD_DSE(5)
/* R0/6 */
#define DSE_6_R0_6                          IOMUXC_SW_PAD_CTL_PAD_DSE(6)
/* R0/7 */
#define DSE_7_R0_7                          IOMUXC_SW_PAD_CTL_PAD_DSE(7)

/* SPEED 带宽配置 */
#define SPEED_0_LOW_50MHz                   IOMUXC_SW_PAD_CTL_PAD_SPEED(0)
#define SPEED_1_MEDIUM_100MHz               IOMUXC_SW_PAD_CTL_PAD_SPEED(1)
#define SPEED_2_MEDIUM_100MHz               IOMUXC_SW_PAD_CTL_PAD_SPEED(2)
#define SPEED_3_MAX_200MHz                  IOMUXC_SW_PAD_CTL_PAD_SPEED(3)

/* ODE 是否使用开漏模式 */
#define ODE_0_OPEN_DRAIN_DISABLED           IOMUXC_SW_PAD_CTL_PAD_ODE(0)     
#define ODE_1_OPEN_DRAIN_ENABLED            IOMUXC_SW_PAD_CTL_PAD_ODE(1)     

/* PKE 是否使能保持器或上下拉功能 */
#define PKE_0_PULL_KEEPER_DISABLED          IOMUXC_SW_PAD_CTL_PAD_PKE(0)      
#define PKE_1_PULL_KEEPER_ENABLED           IOMUXC_SW_PAD_CTL_PAD_PKE(1)      

/* PUE 选择使用保持器还是上下拉 */
#define PUE_0_KEEPER_SELECTED               IOMUXC_SW_PAD_CTL_PAD_PUE(0)   
#define PUE_1_PULL_SELECTED                 IOMUXC_SW_PAD_CTL_PAD_PUE(1)   

/* PUS 上下拉配置 */
#define PUS_0_100K_OHM_PULL_DOWN            IOMUXC_SW_PAD_CTL_PAD_PUS(0)     
#define PUS_1_47K_OHM_PULL_UP               IOMUXC_SW_PAD_CTL_PAD_PUS(1)   
#define PUS_2_100K_OHM_PULL_UP              IOMUXC_SW_PAD_CTL_PAD_PUS(2)   
#define PUS_3_22K_OHM_PULL_UP               IOMUXC_SW_PAD_CTL_PAD_PUS(3)   

/* HYS 滞后功能 */
#define HYS_0_HYSTERESIS_DISABLED           IOMUXC_SW_PAD_CTL_PAD_HYS(0)  
#define HYS_1_HYSTERESIS_ENABLED            IOMUXC_SW_PAD_CTL_PAD_HYS(1)  


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
