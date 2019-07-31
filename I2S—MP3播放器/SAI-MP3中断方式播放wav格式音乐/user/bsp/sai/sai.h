#ifndef __BSP_SD_H
#define __BSP_SD_H

#include "fsl_clock.h"
#include "fsl_wm8960.h"
#include "fsl_sai.h"
#include "fsl_codec_common.h"

/* SAI instance and clock */
#define DEMO_CODEC_WM8960
#define DEMO_SAI SAI1
#define DEMO_SAI_CHANNEL (0)
#define DEMO_SAI_BITWIDTH (kSAI_WordWidth16bits)

//#define DEMO_SAI_IRQ SAI1_IRQn
//#define SAI_TxIRQHandler SAI1_IRQHandler

/* Select Audio/Video PLL (786.48 MHz) as sai1 clock source */
#define DEMO_SAI1_CLOCK_SOURCE_SELECT (2U)
/* Clock pre divider for sai1 clock source */
#define DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER (0U)
/* Clock divider for sai1 clock source */
#define DEMO_SAI1_CLOCK_SOURCE_DIVIDER (63U)
/* Get frequency of sai1 clock */
#define DEMO_SAI_CLK_FREQ                                                      \
  (CLOCK_GetFreq(kCLOCK_AudioPllClk) / (DEMO_SAI1_CLOCK_SOURCE_DIVIDER + 1U) / \
   (DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER + 1U))

/* I2C instance and clock */
#define DEMO_I2C LPI2C1
/* Select USB1 PLL (480 MHz) as master lpi2c clock source */
#define DEMO_LPI2C_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for master lpi2c clock source */
#define DEMO_LPI2C_CLOCK_SOURCE_DIVIDER (5U)
/* Get frequency of lpi2c clock */
#define DEMO_I2C_CLK_FREQ ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (DEMO_LPI2C_CLOCK_SOURCE_DIVIDER + 1U))

#define OVER_SAMPLE_RATE (384U)

void sai_init(void);









#endif /* __BSP_SD_H */   













