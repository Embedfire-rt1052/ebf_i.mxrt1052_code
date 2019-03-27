#ifndef __BSP_SD_H
#define __BSP_SD_H


#include "pad_config.h"
#include "fsl_iomuxc.h"
#include "fsl_common.h"
#include "fsl_sd.h"

/*********************************************************
 * TMR3 输入端口、引脚号及IOMUXC复用宏定义
 
使用i.MX RT1052-Pro底板：
 *  TMR3_CH0-----GPIO_AD_B1_00-----(CN5, 21)
 *  TMR3_CH1-----GPIO_AD_B1_01-----(CN5, 23)
 *  TMR3_CH2-----GPIO_AD_B1_02-----(CN5, 10)
 *  TMR3_CH3-----GPIO_AD_B1_03-----(CN5, 8)
 *引脚功能         引脚标号     引脚在开发板上对应位置
 (CN5, 21) 含义是引脚连接到开发板CN5排针的第21脚


使用i.MX RT1052-Mini底板

 *  TMR3_CH0-----GPIO_AD_B1_00-----(CN4, 21)
 *  TMR3_CH1-----GPIO_AD_B1_01-----(CN4, 23)
 *  TMR3_CH2-----GPIO_AD_B1_02-----(CN4, 10)
 *  TMR3_CH3-----GPIO_AD_B1_03-----(CN4, 8)
 *引脚功能         引脚标号     引脚在开发板上对应位置
 (CN4, 21) 含义是引脚连接到开发板CN4排针的第21脚 

注意：TMR定时器的一个通道可以对应到一个或者多个外部引脚，详细信息请参考
      《IMXRT1050RM》第4章External Signals and Pin Multiplexing
 *********************************************************/


/*SD1_CMD*/
#define USDHC1_CMD_GPIO             GPIO3
#define USDHC1_CMD_GPIO_PIN         (12U)
#define USDHC1_CMD_IOMUXC           IOMUXC_GPIO_SD_B0_00_USDHC1_CMD

/*SD1_CLK*/
#define USDHC1_CLK_GPIO             GPIO2
#define USDHC1_CLK_GPIO_PIN         (30U)
#define USDHC1_CLK_IOMUXC           IOMUXC_GPIO_SD_B0_01_USDHC1_CLK

/*SD1_D0*/
#define USDHC1_DATA0_GPIO             GPIO3
#define USDHC1_DATA0_GPIO_PIN         (14U)
#define USDHC1_DATA0_IOMUXC           IOMUXC_GPIO_SD_B0_02_USDHC1_DATA0

/*SD1_D1*/
#define USDHC1_DATA1_GPIO             GPIO3
#define USDHC1_DATA1_GPIO_PIN         (15U)
#define USDHC1_DATA1_IOMUXC           IOMUXC_GPIO_SD_B0_03_USDHC1_DATA1

/*SD1_D2*/
#define USDHC1_DATA2_GPIO             GPIO3
#define USDHC1_DATA2_GPIO_PIN         (16U)
#define USDHC1_DATA2_IOMUXC           IOMUXC_GPIO_SD_B0_04_USDHC1_DATA2

/*SD1_D3*/
#define USDHC1_DATA3_GPIO             GPIO3
#define USDHC1_DATA3_GPIO_PIN         (17U)
#define USDHC1_DATA3_IOMUXC           IOMUXC_GPIO_SD_B0_05_USDHC1_DATA3

/**********************特殊功能引脚********************************/
/**/
//#define GPT1_COMPARE3_GPIO             GPIO1
//#define GPT1_COMPARE3_GPIO_PIN         (5U)
//#define GPT1_COMPARE3_IOMUXC           IOMUXC_GPIO_B1_14_USDHC1_VSELECT
//
///**/
//#define GPT1_COMPARE2_GPIO             GPIO2
//#define GPT1_COMPARE2_GPIO_PIN         (28U)
//#define GPT1_COMPARE2_IOMUXC           IOMUXC_GPIO_B1_12_GPIO2_IO28
//
//
///**/
//#define GPT1_COMPARE3_GPIO             GPIO1
//#define GPT1_COMPARE3_GPIO_PIN         (5U)
//#define GPT1_COMPARE3_IOMUXC           IOMUXC_GPIO_AD_B0_05_GPIO1_IO05




/* USDHC1 DATA引脚 CMD引脚 PAD属性配置 IO28,*/
#define USDHC1_DATA_PAD_CONFIG_DATA     (SRE_1_FAST_SLEW_RATE| \
                                        DSE_1_R0_1| \
                                        SPEED_2_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_1_PULL_KEEPER_ENABLED| \
                                        PUE_1_PULL_SELECTED| \
                                        PUS_1_47K_OHM_PULL_UP| \
                                        HYS_1_HYSTERESIS_ENABLED)

    /* 配置说明 : */
    /* 转换速率: 转换速率快
        驱动强度: R0 
        带宽配置 : medium(100MHz)
        开漏配置: 关闭 
        拉/保持器配置: 使能
        拉/保持器选择: 上下拉
        上拉/下拉选择: 4.7K欧姆上拉(选择了保持器此配置无效)
        滞回器配置: 禁止 */ 

/* USDHC1 CLK引脚PAD属性配置 */
#define USDHC1_CLK_PAD_CONFIG_DATA       (SRE_1_FAST_SLEW_RATE| \
                                        DSE_1_R0_1| \
                                        SPEED_2_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_1_PULL_KEEPER_ENABLED| \
                                        PUE_0_KEEPER_SELECTED| \
                                        PUS_1_47K_OHM_PULL_UP| \
                                        HYS_1_HYSTERESIS_ENABLED)
    /* 配置说明 : */
    /* 转换速率: 转换速率快
        驱动强度: R0 
        带宽配置 : medium(100MHz)
        开漏配置: 关闭 
        拉/保持器配置: 使能
        拉/保持器选择: 保持器
        上拉/下拉选择: 4.7K欧姆上拉(选择了保持器此配置无效)
        滞回器配置: 禁止 */

///* USDHC1 VSELECT引脚PAD属性配置 */
//#define TMR_PWM_OUTPUT_PAD_CONFIG_DATA       (SRE_1_FAST_SLEW_RATE| \
//                                        DSE_4_R0_4| \
//                                        SPEED_2_MEDIUM_100MHz| \
//                                        ODE_0_OPEN_DRAIN_DISABLED| \
//                                        PKE_1_PULL_KEEPER_ENABLED| \
//                                        PUE_1_PULL_SELECTED| \
//                                        PUS_1_47K_OHM_PULL_UP| \
//                                        HYS_1_HYSTERESIS_ENABLED)
//
//    /* 配置说明 : */
//    /* 转换速率: 转换速率快
//        驱动强度: R0/4
//        带宽配置 : medium(100MHz)
//        开漏配置: 关闭 
//        拉/保持器配置: 使能
//        拉/保持器选择: 保持器
//        上拉/下拉选择: 4.7K欧姆上拉(选择了保持器此配置无效)
//        滞回器配置: 禁止 */ 



void USDHC1_gpio_init(void);
static status_t AccessCard(sd_card_t *card);
static void BOARD_USDHCClockConfiguration(void);

void SDCardTest(void);
int SDCard_Init(void);

#endif /* __BSP_SD_H */                             