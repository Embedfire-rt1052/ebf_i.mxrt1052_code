
/* �Ĵ�����ֵ������оƬ�����Զ����ĵģ�
 * ��ʹCPUû��ִ�г���Ҳ�п��ܷ����仯
 * �������п��ܻ��û��ִ�г���ı��������Ż�
*/

/* volatile��ʾ�ױ�ı�������ֹ�������Ż���*/
#define  __IO  volatile        /* �ɶ�д��һ�����ڶ����пɶ�дȨ�޵ļĴ���*/
#define  __I   volatile const  /* ֻ����һ�����ڶ���ֻ��Ȩ�޵ļĴ��� */


/* ʹ�ø����ֱ�۵ķ�ʽ�������޷���32��16��8λ���� */	
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

/* GPIO �Ĵ����ṹ�� */
typedef struct {
  __IO uint32_t DR;         /* DR���ݼĴ�����						��ַƫ��: 0x0 */
  __IO uint32_t GDIR;       /* GDIR����Ĵ�����					��ַƫ��: 0x4 */
  __I  uint32_t PSR;        /* PSR״̬�Ĵ�����						��ַƫ��: 0x8 */
  __IO uint32_t ICR1;       /* ICR1�ж����üĴ���1,			��ַƫ��: 0xC */
  __IO uint32_t ICR2;       /* ICR2�ж����üĴ���2,			��ַƫ��: 0x10 */
  __IO uint32_t IMR;        /* IMR�ж�����Ĵ���,				��ַƫ��: 0x14 */
  __IO uint32_t ISR;        /* ISR�ж�״̬�Ĵ���,				��ַƫ��: 0x18 */
  __IO uint32_t EDGE_SEL;   /* EDGE_SEL����ѡ��Ĵ�����	��ַƫ��: 0x1C */
} GPIO_Type;

/* ��ַ�ɲ��� �ο��ֲ�-GPIO�½�-GPIO Memory Map */
/* GPIO�������ַ */
#define GPIO1_BASE        (unsigned int)(0x401B8000u)
#define GPIO2_BASE        (unsigned int)(0x401BC000u)
#define GPIO3_BASE        (unsigned int)(0x401C0000u)
#define GPIO4_BASE        (unsigned int)(0x401C4000u)
#define GPIO5_BASE        (unsigned int)(0x400C0000u)

/* ����GPIO1-5 �Ĵ����ṹ��ָ�� */
#define GPIO1          ((GPIO_Type *)GPIO1_BASE)
#define GPIO2          ((GPIO_Type *)GPIO2_BASE)
#define GPIO3          ((GPIO_Type *)GPIO3_BASE)
#define GPIO4          ((GPIO_Type *)GPIO4_BASE)
#define GPIO5          ((GPIO_Type *)GPIO5_BASE)
	
/* ʹ�ýṹ��ָ��󣬲���Ҫ���������� GPIO1�Ĵ�����ַ,ǿ��ת����ָ�� */	
//#define GPIO1_DR						*(unsigned int*)(GPIO1_BASE+0x00)
//#define GPIO1_GDIR					*(unsigned int*)(GPIO1_BASE+0x04)
//#define GPIO1_PSR						*(unsigned int*)(GPIO1_BASE+0x08)
//#define GPIO1_ICR1					*(unsigned int*)(GPIO1_BASE+0x0C)
//#define GPIO1_ICR2					*(unsigned int*)(GPIO1_BASE+0x10)
//#define GPIO1_IMR						*(unsigned int*)(GPIO1_BASE+0x14)
//#define GPIO1_ISR						*(unsigned int*)(GPIO1_BASE+0x18)
//#define GPIO1_EDGE_SEL			*(unsigned int*)(GPIO1_BASE+0x1C)
	

/* ��ַ�ɲ��� �ο��ֲ�-IOMUXC�½�-IOMUXC Memory Map/Register Definition */
/* IOMUXC����ַ */
#define IOMUXC_BASE 				(unsigned int)(0x401F8000u)

/* MUXģʽѡ��Ĵ��� IOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B0_09 ,ǿ��ת����ָ��*/
#define IOMUXC_MUX_GPIO_AD_B0_09  	*(unsigned int*)(IOMUXC_BASE + 0xE0 )

#define MUX_MODE_ALT0_JTAG_TDI                 0x0
#define MUX_MODE_ALT1_FLEXPWM2_PWMA03          0x1
#define MUX_MODE_ALT2_ENET_RX_DATA02           0x2
#define MUX_MODE_ALT3_SAI2_TX_DATA             0x3
#define MUX_MODE_ALT4_CSI_DATA04               0x4
#define MUX_MODE_ALT5_GPIO1_IO09               0x5
#define MUX_MODE_ALT6_XBAR1_IN21               0x6
#define MUX_MODE_ALT7_GPT2_CLK                 0x7

/* PAD�������üĴ��� IOMUXC_SW_PAD_CTL_PAD_GPIO_AD_B0_09 ,ǿ��ת����ָ��*/
#define IOMUXC_PAD_GPIO_AD_B0_09  	*(unsigned int*)(IOMUXC_BASE + 0x2D0 )
  

/* MUX�Ĵ�����������������롢ƫ�Ƽ����ú� */
/* MUX_MODE���� */
#define IOMUXC_SW_MUX_CTL_PAD_MUX_MODE_MASK      (0x7U)
#define IOMUXC_SW_MUX_CTL_PAD_MUX_MODE_SHIFT     (0U)
#define IOMUXC_SW_MUX_CTL_PAD_MUX_MODE(x)        (((uint32_t)(((uint32_t)(x)) << IOMUXC_SW_MUX_CTL_PAD_MUX_MODE_SHIFT)) & IOMUXC_SW_MUX_CTL_PAD_MUX_MODE_MASK)

/* SION���� */
#define IOMUXC_SW_MUX_CTL_PAD_SION_MASK          (0x10U)
#define IOMUXC_SW_MUX_CTL_PAD_SION_SHIFT         (4U)
#define IOMUXC_SW_MUX_CTL_PAD_SION(x)            (((uint32_t)(((uint32_t)(x)) << IOMUXC_SW_MUX_CTL_PAD_SION_SHIFT)) & IOMUXC_SW_MUX_CTL_PAD_SION_MASK)


/* PAD���ԼĴ�����������������롢ƫ�Ƽ����ú� */
/* SRE ѹ��������*/
#define IOMUXC_SW_PAD_CTL_PAD_SRE_MASK           (0x1U)
#define IOMUXC_SW_PAD_CTL_PAD_SRE_SHIFT          (0U)
#define IOMUXC_SW_PAD_CTL_PAD_SRE(x)             (((uint32_t)(((uint32_t)(x)) << IOMUXC_SW_PAD_CTL_PAD_SRE_SHIFT)) & IOMUXC_SW_PAD_CTL_PAD_SRE_MASK)

/* DSE ������������ */
#define IOMUXC_SW_PAD_CTL_PAD_DSE_MASK           (0x38U)
#define IOMUXC_SW_PAD_CTL_PAD_DSE_SHIFT          (3U)
#define IOMUXC_SW_PAD_CTL_PAD_DSE(x)             (((uint32_t)(((uint32_t)(x)) << IOMUXC_SW_PAD_CTL_PAD_DSE_SHIFT)) & IOMUXC_SW_PAD_CTL_PAD_DSE_MASK)

/* SPEED �������� */
#define IOMUXC_SW_PAD_CTL_PAD_SPEED_MASK         (0xC0U)
#define IOMUXC_SW_PAD_CTL_PAD_SPEED_SHIFT        (6U)
#define IOMUXC_SW_PAD_CTL_PAD_SPEED(x)           (((uint32_t)(((uint32_t)(x)) << IOMUXC_SW_PAD_CTL_PAD_SPEED_SHIFT)) & IOMUXC_SW_PAD_CTL_PAD_SPEED_MASK)

/* ODE ��©������� */
#define IOMUXC_SW_PAD_CTL_PAD_ODE_MASK           (0x800U)
#define IOMUXC_SW_PAD_CTL_PAD_ODE_SHIFT          (11U)
#define IOMUXC_SW_PAD_CTL_PAD_ODE(x)             (((uint32_t)(((uint32_t)(x)) << IOMUXC_SW_PAD_CTL_PAD_ODE_SHIFT)) & IOMUXC_SW_PAD_CTL_PAD_ODE_MASK)

/* PKE ��������������ʹ������ */
#define IOMUXC_SW_PAD_CTL_PAD_PKE_MASK           (0x1000U)
#define IOMUXC_SW_PAD_CTL_PAD_PKE_SHIFT          (12U)
#define IOMUXC_SW_PAD_CTL_PAD_PKE(x)             (((uint32_t)(((uint32_t)(x)) << IOMUXC_SW_PAD_CTL_PAD_PKE_SHIFT)) & IOMUXC_SW_PAD_CTL_PAD_PKE_MASK)

/* PUE ѡ��ʹ�����������Ǳ����� */
#define IOMUXC_SW_PAD_CTL_PAD_PUE_MASK           (0x2000U)
#define IOMUXC_SW_PAD_CTL_PAD_PUE_SHIFT          (13U)
#define IOMUXC_SW_PAD_CTL_PAD_PUE(x)             (((uint32_t)(((uint32_t)(x)) << IOMUXC_SW_PAD_CTL_PAD_PUE_SHIFT)) & IOMUXC_SW_PAD_CTL_PAD_PUE_MASK)

/* PUS ѡ���������������Լ����� */
#define IOMUXC_SW_PAD_CTL_PAD_PUS_MASK           (0xC000U)
#define IOMUXC_SW_PAD_CTL_PAD_PUS_SHIFT          (14U)
#define IOMUXC_SW_PAD_CTL_PAD_PUS(x)             (((uint32_t)(((uint32_t)(x)) << IOMUXC_SW_PAD_CTL_PAD_PUS_SHIFT)) & IOMUXC_SW_PAD_CTL_PAD_PUS_MASK)

/* HYS �ͺ�ʹ�� */
#define IOMUXC_SW_PAD_CTL_PAD_HYS_MASK           (0x10000U)
#define IOMUXC_SW_PAD_CTL_PAD_HYS_SHIFT          (16U)
#define IOMUXC_SW_PAD_CTL_PAD_HYS(x)             (((uint32_t)(((uint32_t)(x)) << IOMUXC_SW_PAD_CTL_PAD_HYS_SHIFT)) & IOMUXC_SW_PAD_CTL_PAD_HYS_MASK)

/* PAD�������üĴ���������*/
/* SRE ѹ����ѡ�� */
#define SRE_0_SLOW_SLEW_RATE                IOMUXC_SW_PAD_CTL_PAD_SRE(0)
#define SRE_1_FAST_SLEW_RATE                IOMUXC_SW_PAD_CTL_PAD_SRE(1)

/* �����������ã�������ֵ�Ĵ�С */
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

/* SPEED �������� */
#define SPEED_0_LOW_50MHz                   IOMUXC_SW_PAD_CTL_PAD_SPEED(0)
#define SPEED_1_MEDIUM_100MHz               IOMUXC_SW_PAD_CTL_PAD_SPEED(1)
#define SPEED_2_MEDIUM_100MHz               IOMUXC_SW_PAD_CTL_PAD_SPEED(2)
#define SPEED_3_MAX_200MHz                  IOMUXC_SW_PAD_CTL_PAD_SPEED(3)

/* ODE �Ƿ�ʹ�ÿ�©ģʽ */
#define ODE_0_OPEN_DRAIN_DISABLED           IOMUXC_SW_PAD_CTL_PAD_ODE(0)     
#define ODE_1_OPEN_DRAIN_ENABLED            IOMUXC_SW_PAD_CTL_PAD_ODE(1)     

/* PKE �Ƿ�ʹ�ܱ����������������� */
#define PKE_0_PULL_KEEPER_DISABLED          IOMUXC_SW_PAD_CTL_PAD_PKE(0)      
#define PKE_1_PULL_KEEPER_ENABLED           IOMUXC_SW_PAD_CTL_PAD_PKE(1)      

/* PUE ѡ��ʹ�ñ��������������� */
#define PUE_0_KEEPER_SELECTED               IOMUXC_SW_PAD_CTL_PAD_PUE(0)   
#define PUE_1_PULL_SELECTED                 IOMUXC_SW_PAD_CTL_PAD_PUE(1)   

/* PUS ���������� */
#define PUS_0_100K_OHM_PULL_DOWN            IOMUXC_SW_PAD_CTL_PAD_PUS(0)     
#define PUS_1_47K_OHM_PULL_UP               IOMUXC_SW_PAD_CTL_PAD_PUS(1)   
#define PUS_2_100K_OHM_PULL_UP              IOMUXC_SW_PAD_CTL_PAD_PUS(2)   
#define PUS_3_22K_OHM_PULL_UP               IOMUXC_SW_PAD_CTL_PAD_PUS(3)   

/* HYS �ͺ��� */
#define HYS_0_HYSTERESIS_DISABLED           IOMUXC_SW_PAD_CTL_PAD_HYS(0)  
#define HYS_1_HYSTERESIS_ENABLED            IOMUXC_SW_PAD_CTL_PAD_HYS(1)  


/* ��ַ�ɲ��� �ο��ֲ�-CCM�½�-CCM Memory Map/Register Definition */
/* CCMʱ�ӿ����������ַ */
#define CCM_BASE												(unsigned int)(0x400FC000u)

/* ʱ�����üĴ���CCM_CCGR1��ַ */
#define CCM_CCGR1	 											*(unsigned int*)(CCM_BASE + 0x6C)

/* GPIO1ʱ���������ڼĴ���CCM_CCGR1�е�λ�� */
#define CCM_CCGR1_CG13_SHIFT              (26U)

/* ����LED������ĺ� */
#define CORE_BOARD_LED_ON			GPIO1_DR &=~ (1<<9);
#define CORE_BOARD_LED_OFF		GPIO1_DR |= (1<<9);
