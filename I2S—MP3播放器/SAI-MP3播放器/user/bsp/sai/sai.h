///*
// * Copyright (c) 2015, Freescale Semiconductor, Inc.
// * Copyright 2016-2017 NXP
// * All rights reserved.
// *
// * SPDX-License-Identifier: BSD-3-Clause
// */

#include "board.h"
#include "fsl_dmamux.h"
#include "fsl_sai_edma.h"
#include "fsl_debug_console.h"
#include "arm_math.h"

///*******************************************************************************
// * Definitions
// ******************************************************************************/
//#define OVER_SAMPLE_RATE (384U)
//#define SAMPLE_RATE (kSAI_SampleRate16KHz)
//#define BUFFER_SIZE (512)
//#define BUFFER_NUM (4)
//#if defined BOARD_HAS_SDCARD && (BOARD_HAS_SDCARD != 0)
//#define DEMO_SDCARD (1U)
//#endif
///*******************************************************************************
// * Prototypes
// ******************************************************************************/
//void PlaybackSine(I2S_Type *base, uint32_t SineWaveFreqHz, uint32_t time_s);
//void RecordSDCard(I2S_Type *base, uint32_t time_s);
//void RecordPlayback(I2S_Type *base, uint32_t time_s);
///*******************************************************************************
// * Variables
// ******************************************************************************/
/* SAI instance and clock */
#define DEMO_CODEC_WM8960
#define DEMO_SAI SAI1
#define DEMO_SAI_CHANNEL (0)
#define DEMO_SAI_BITWIDTH (kSAI_WordWidth16bits)
#define DEMO_SAI_IRQ SAI1_IRQn
#define SAI_UserIRQHandler SAI1_IRQHandler

/* IRQ */
#define DEMO_SAI_TX_IRQ SAI1_IRQn
#define DEMO_SAI_RX_IRQ SAI1_IRQn

/* DMA */
#define EXAMPLE_DMA DMA0
#define EXAMPLE_DMAMUX DMAMUX
#define EXAMPLE_TX_CHANNEL (0U)
#define EXAMPLE_RX_CHANNEL (1U)
#define EXAMPLE_SAI_TX_SOURCE kDmaRequestMuxSai1Tx
#define EXAMPLE_SAI_RX_SOURCE kDmaRequestMuxSai1Rx

/* Select Audio/Video PLL (786.48 MHz) as sai1 clock source */
#define DEMO_SAI1_CLOCK_SOURCE_SELECT (2U)
/* Clock pre divider for sai1 clock source */
#define DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER (0U)
/* Clock divider for sai1 clock source */
#define DEMO_SAI1_CLOCK_SOURCE_DIVIDER (63U)
/* Get frequency of sai1 clock */
#define DEMO_SAI_CLK_FREQ                                                        \
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
/* SD power control function */
#define DEMO_SDCARD_POWER_CTRL_FUNCTION_EXIST



/*与主函数中的宏定义同名，暂时设定为2*/
#define BUFFER_SIZE (512)


#define OVER_SAMPLE_RATE (384U)
#define SAMPLE_RATE (kSAI_SampleRate16KHz)
//#define BUFFER_SIZE (512)
#define BUFFER_NUM (4)














void BOARD_EnableSaiMclkOutput(bool enable);
void	SAI1_Init(void);
void	SAI1_DMAConfig(void);
