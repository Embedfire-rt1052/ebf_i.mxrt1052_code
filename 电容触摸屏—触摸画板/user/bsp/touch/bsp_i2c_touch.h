#ifndef __I2C_TOUCH_H
#define	__I2C_TOUCH_H

#include "fsl_common.h"
#include "fsl_lpi2c.h"
#include "fsl_iomuxc.h"


extern lpi2c_master_handle_t g_m_handle;
extern volatile bool g_MasterCompletionFlag ;
extern volatile bool g_TouchPadInputSignal;

  

/* 选择 USB1 PLL (PLL3 480 MHz) 作为 lpi2c 时钟源 */
#define LPI2C_CLOCK_SOURCE_SELECT (0U)
/* 设置USB1 PLL时钟到LPI2C根时钟的时钟分频  */
#define LPI2C_CLOCK_SOURCE_DIVIDER (5U)
/* 读取 lpi2c 时钟频率 */
#define LPI2C_CLOCK_FREQUENCY ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (LPI2C_CLOCK_SOURCE_DIVIDER + 1U))
#define LPI2C_MASTER_CLOCK_FREQUENCY  LPI2C_CLOCK_FREQUENCY


/*LPI2C相关宏定义*/
#define GTP_I2C_MASTER_BASE   (LPI2C1_BASE)    //定义使用的I2C
//定义I2C控制寄存器
#define GTP_I2C_MASTER        ((LPI2C_Type *)GTP_I2C_MASTER_BASE)
#define GTP_I2C_BAUDRATE      400000U     //定义I2C波特率

/* 等待超时时间 */
#define I2CT_FLAG_TIMEOUT    ((uint32_t)0x1000)  //等待超时时间设定
//长等待时间设定
#define I2CT_LONG_TIMEOUT         ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))


/*! @brief 触摸芯片引脚定义 */
/*LPI2C时钟引脚*/ 
#define TOUCH_PAD_SCL_IOMUXC	IOMUXC_GPIO_AD_B1_00_LPI2C1_SCL
/*LPI2C数据引脚*/ 
#define TOUCH_PAD_SDA_IOMUXC	IOMUXC_GPIO_AD_B1_01_LPI2C1_SDA

/*定义触摸芯片的复位引脚*/
#define TOUCH_PAD_RST_GPIO 	     GPIO1
#define TOUCH_PAD_RST_GPIO_PIN 	    (2U)
#define TOUCH_PAD_RST_IOMUXC	    IOMUXC_GPIO_AD_B0_02_GPIO1_IO02

/*定义芯片的中断输出引脚*/
#define TOUCH_PAD_INT_GPIO 	    GPIO1
#define TOUCH_PAD_INT_GPIO_PIN 	    (11U)
#define TOUCH_PAD_INT_IOMUXC	    IOMUXC_GPIO_AD_B0_11_GPIO1_IO11

/*触摸中断定义*/
#define TOUCH_PAD_INT_IRQ 	    GPIO1_Combined_0_15_IRQn
#define TOUCH_PAD_IRQHANDLER        GPIO1_Combined_0_15_IRQHandler

/* I2C的SCL和SDA引脚使用同样的PAD配置 */
#define I2C_PAD_CONFIG_DATA            (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_1_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_1_PULL_KEEPER_ENABLED| \
                                        PUE_0_KEEPER_SELECTED| \
                                        PUS_3_22K_OHM_PULL_UP| \
                                        HYS_0_HYSTERESIS_DISABLED)   
    /* 配置说明 : */
    /* 转换速率: 转换速率慢
        驱动强度: R0/6 
        带宽配置 : medium(100MHz)
        开漏配置: 关闭 
        拉/保持器配置: 使能
        拉/保持器选择: 保持器
        上拉/下拉选择: 22K欧姆上拉(选择了保持器此配置无效)
        滞回器配置: 禁止 */ 
        

/* 触摸芯片的RST和INT引脚使用同样的PAD配置 */
#define GTP_RST_INT_PAD_CONFIG_DATA            (SRE_0_SLOW_SLEW_RATE| \
                                                DSE_6_R0_6| \
                                                SPEED_1_MEDIUM_100MHz| \
                                                ODE_0_OPEN_DRAIN_DISABLED| \
                                                PKE_1_PULL_KEEPER_ENABLED| \
                                                PUE_0_KEEPER_SELECTED| \
                                                PUS_2_100K_OHM_PULL_UP| \
                                                HYS_0_HYSTERESIS_DISABLED)   
    /* 配置说明 : */
    /* 转换速率: 转换速率慢
        驱动强度: R0/6 
        带宽配置 : medium(100MHz)
        开漏配置: 关闭 
        拉/保持器配置: 使能
        拉/保持器选择: 保持器
        上拉/下拉选择: 100K欧姆上拉(选择了保持器此配置无效)
        滞回器配置: 禁止 */ 

//函数接口
int32_t GTP_I2C_ReadBytes(uint8_t client_addr, uint8_t *buf, int32_t len);

void I2C_Touch_Init(void);
uint32_t I2C_WriteBytes(uint8_t ClientAddr,uint8_t* pBuffer,  uint8_t NumByteToWrite);
uint32_t I2C_ReadBytes(uint8_t ClientAddr,uint8_t* pBuffer, uint16_t NumByteToRead);
void GTP_ResetChip(void);
void GTP_IRQDisable(void);
void GTP_IRQEnable(void);



#endif /* __I2C_TOUCH_H */







