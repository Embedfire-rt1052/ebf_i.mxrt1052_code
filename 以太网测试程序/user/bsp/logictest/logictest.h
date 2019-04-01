#ifndef __LOGICTEST_H
#define __LOGICTEST_H

#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "./delay/core_delay.h"
/*********************************************************
 * LogicTest端口、引脚号及IOMUXC复用宏定义
 *********************************************************/
#define Logic_595_SER_GPIO             	GPIO2
#define Logic_595_SER_GPIO_PIN         (28U)
#define Logic_595_SER_IOMUXC           IOMUXC_GPIO_B1_12_GPIO2_IO28

#define Logic_595_LCLK_GPIO          		GPIO2
#define Logic_595_LCLK_GPIO_PIN        	(29U)
#define Logic_595_LCLK_IOMUXC          	IOMUXC_GPIO_B1_13_GPIO2_IO29

#define Logic_595_SCLK_GPIO            	GPIO1
#define Logic_595_SCLK_GPIO_PIN        	(24U)
#define Logic_595_SCLK_IOMUXC         	IOMUXC_GPIO_AD_B1_08_GPIO1_IO24


#define LogicIn0_GPIO               		GPIO3
#define LogicIn0_GPIO_PIN           		(26U)
#define LogicIn0_IOMUXC             		IOMUXC_GPIO_EMC_40_GPIO3_IO26

#define LogicIn1_GPIO               		GPIO3
#define LogicIn1_GPIO_PIN           		(27U)
#define LogicIn1_IOMUXC             		IOMUXC_GPIO_EMC_41_GPIO3_IO27

#define LogicIn2_GPIO               		GPIO5
#define LogicIn2_GPIO_PIN           		(2U)
#define LogicIn2_IOMUXC             		IOMUXC_SNVS_PMIC_STBY_REQ_GPIO5_IO02

#define LogicIn3_GPIO               		GPIO5
#define LogicIn3_GPIO_PIN           		(0U)
#define LogicIn3_IOMUXC             		IOMUXC_SNVS_WAKEUP_GPIO5_IO00

#define LogicIn4_GPIO               		GPIO1
#define LogicIn4_GPIO_PIN           		(31U)
#define LogicIn4_IOMUXC             		IOMUXC_GPIO_AD_B1_15_GPIO1_IO31

#define LogicIn5_GPIO               		GPIO1
#define LogicIn5_GPIO_PIN           		(30U)
#define LogicIn5_IOMUXC             		IOMUXC_GPIO_AD_B1_14_GPIO1_IO30

#define LogicIn6_GPIO               		GPIO1
#define LogicIn6_GPIO_PIN           		(29U)
#define LogicIn6_IOMUXC             		IOMUXC_GPIO_AD_B1_13_GPIO1_IO29

#define LogicIn7_GPIO               		GPIO1
#define LogicIn7_GPIO_PIN           		(28U)
#define LogicIn7_IOMUXC             		IOMUXC_GPIO_AD_B1_12_GPIO1_IO28

#define LogicIn8_GPIO               		GPIO1
#define LogicIn8_GPIO_PIN           		(27U)
#define LogicIn8_IOMUXC             		IOMUXC_GPIO_AD_B1_11_GPIO1_IO27

#define LogicIn9_GPIO               		GPIO1
#define LogicIn9_GPIO_PIN           		(26U)
#define LogicIn9_IOMUXC             		IOMUXC_GPIO_AD_B1_10_GPIO1_IO26

#define LogicIn10_GPIO               		GPIO1
#define LogicIn10_GPIO_PIN           		(25U)
#define LogicIn10_IOMUXC             		IOMUXC_GPIO_AD_B1_09_GPIO1_IO25

#define LogicIn11_GPIO               		GPIO1
#define LogicIn11_GPIO_PIN           		(24U)
#define LogicIn11_IOMUXC             		IOMUXC_GPIO_AD_B1_08_GPIO1_IO24

#define LogicIn12_GPIO               		GPIO1
#define LogicIn12_GPIO_PIN           		(23U)
#define LogicIn12_IOMUXC             		IOMUXC_GPIO_AD_B1_07_GPIO1_IO23

#define LogicIn13_GPIO               		GPIO1
#define LogicIn13_GPIO_PIN           		(22U)
#define LogicIn13_IOMUXC             		IOMUXC_GPIO_AD_B1_06_GPIO1_IO22

#define LogicIn14_GPIO               		GPIO1
#define LogicIn14_GPIO_PIN           		(21U)
#define LogicIn14_IOMUXC             		IOMUXC_GPIO_AD_B1_05_GPIO1_IO21

#define LogicIn15_GPIO               		GPIO1
#define LogicIn15_GPIO_PIN           		(20U)
#define LogicIn15_IOMUXC             		IOMUXC_GPIO_AD_B1_04_GPIO1_IO20

#define LogicIn16_GPIO               		GPIO1
#define LogicIn16_GPIO_PIN           		(19U)
#define LogicIn16_IOMUXC             		IOMUXC_GPIO_AD_B1_03_GPIO1_IO19

#define LogicIn17_GPIO               		GPIO1
#define LogicIn17_GPIO_PIN           		(18U)
#define LogicIn17_IOMUXC             		IOMUXC_GPIO_AD_B1_02_GPIO1_IO18

#define LogicIn18_GPIO               		GPIO1
#define LogicIn18_GPIO_PIN           		(14U)
#define LogicIn18_IOMUXC             		IOMUXC_GPIO_AD_B0_14_GPIO1_IO14

#define LogicIn19_GPIO               		GPIO1
#define LogicIn19_GPIO_PIN           		(10U)
#define LogicIn19_IOMUXC             		IOMUXC_GPIO_AD_B0_10_GPIO1_IO10

#define LogicIn20_GPIO               		GPIO1
#define LogicIn20_GPIO_PIN           		(9U)
#define LogicIn20_IOMUXC             		IOMUXC_GPIO_AD_B0_09_GPIO1_IO09

#define LogicIn21_GPIO               		GPIO1
#define LogicIn21_GPIO_PIN           		(8U)
#define LogicIn21_IOMUXC             		IOMUXC_GPIO_AD_B0_08_GPIO1_IO08

#define LogicIn22_GPIO               		GPIO1
#define LogicIn22_GPIO_PIN           		(3U)
#define LogicIn22_IOMUXC             		IOMUXC_GPIO_AD_B0_03_GPIO1_IO03

#define LogicIn23_GPIO               		GPIO1
#define LogicIn23_GPIO_PIN           		(0U)
#define LogicIn23_IOMUXC             		IOMUXC_GPIO_AD_B0_00_GPIO1_IO00

#define LogicIn24_GPIO               		GPIO1
#define LogicIn24_GPIO_PIN           		(1U)
#define LogicIn24_IOMUXC             		IOMUXC_GPIO_AD_B0_01_GPIO1_IO01

#define LogicIn25_GPIO               		GPIO1
#define LogicIn25_GPIO_PIN           		(5U)
#define LogicIn25_IOMUXC             		IOMUXC_GPIO_AD_B0_05_GPIO1_IO05

#define LogicIn26_GPIO               		GPIO1
#define LogicIn26_GPIO_PIN           		(13U)
#define LogicIn26_IOMUXC             		IOMUXC_GPIO_AD_B0_13_GPIO1_IO13

#define LogicIn27_GPIO               		GPIO1
#define LogicIn27_GPIO_PIN           		(12U)
#define LogicIn27_IOMUXC             		IOMUXC_GPIO_AD_B0_12_GPIO1_IO12

#define LogicIn28_GPIO               		GPIO1
#define LogicIn28_GPIO_PIN           		(7U)
#define LogicIn28_IOMUXC             		IOMUXC_GPIO_AD_B0_07_GPIO1_IO07

#define LogicIn29_GPIO               		GPIO1
#define LogicIn29_GPIO_PIN           		(6U)
#define LogicIn29_IOMUXC             		IOMUXC_GPIO_AD_B0_06_GPIO1_IO06

#define LogicIn30_GPIO               		GPIO1
#define LogicIn30_GPIO_PIN           		(4U)
#define LogicIn30_IOMUXC             		IOMUXC_GPIO_AD_B0_04_GPIO1_IO04

#define LogicIn31_GPIO               		NULL
#define LogicIn31_GPIO_PIN           		NULL
#define LogicIn31_IOMUXC             		NULL

#define Logic_DELAY_COUNT               1000


//串行数据引脚
#define   Logic_SER(a) if (a)  \
          GPIO_PinWrite(Logic_595_SER_GPIO, Logic_595_SER_GPIO_PIN, 1U);\
          else    \
          GPIO_PinWrite(Logic_595_SER_GPIO, Logic_595_SER_GPIO_PIN, 0U);

//时钟控制引脚
#define   Logic_SCLK(a) if (a)  \
          GPIO_PinWrite(Logic_595_SCLK_GPIO, Logic_595_SCLK_GPIO_PIN, 1U);\
          else    \
          GPIO_PinWrite(Logic_595_SCLK_GPIO, Logic_595_SCLK_GPIO_PIN, 0U);

//移位控制引脚
#define   Logic_LCLK(a) if (a)  \
          GPIO_PinWrite(Logic_595_LCLK_GPIO, Logic_595_LCLK_GPIO_PIN, 1U);\
          else    \
          GPIO_PinWrite(Logic_595_LCLK_GPIO, Logic_595_LCLK_GPIO_PIN, 0U);

/*******************************************************************************
 * 函数声明
 ******************************************************************************/
void LogicTest_GPIO_Config(void);
void LogicOutByte(uint8_t data);
void LogicOutWord(uint32_t data);
uint8_t LogicInStatus(uint8_t InNum);
uint8_t logicTestInOut(void);
#endif /* __LOGICTEST_H */
