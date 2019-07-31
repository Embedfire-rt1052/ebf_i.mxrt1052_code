/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "sai.h"

codec_handle_t codecHandle = {0};
extern codec_config_t boardCodecConfig;
volatile bool isFinished = false;
sai_handle_t txHandle = {0};
/*
 * AUDIO PLL setting: Frequency = Fref * (DIV_SELECT + NUM / DENOM)
 *                              = 24 * (32 + 77/100)
 *                              = 786.48 MHz
 */
const clock_audio_pll_config_t audioPllConfig = {
    .loopDivider = 32,  /* PLL loop divider. Valid range for DIV_SELECT divider value: 27~54. */
    .postDivider = 1,   /* Divider after the PLL, should only be 1, 2, 4, 8, 16. */
    .numerator = 77,    /* 30 bit numerator of fractional loop divider. */
    .denominator = 100, /* 30 bit denominator of fractional loop divider */
};

static void callback(I2S_Type *base, sai_handle_t *handle, status_t status, void *userData)
{
  isFinished = true;
}

void sai_init(void)
{
  sai_config_t config;          //配置SAI结构体
  sai_transfer_format_t format; //定义SAI传输结构体
  uint32_t mclkSourceClockHz = 0U;

  /*外部GPIO初始化*/
  CLOCK_InitAudioPll(&audioPllConfig);
  /*设置IIC时钟*/
  CLOCK_SetMux(kCLOCK_Lpi2cMux, DEMO_LPI2C_CLOCK_SOURCE_SELECT);
  CLOCK_SetDiv(kCLOCK_Lpi2cDiv, DEMO_LPI2C_CLOCK_SOURCE_DIVIDER);

  /*设置SAI1时钟*/
  CLOCK_SetMux(kCLOCK_Sai1Mux, DEMO_SAI1_CLOCK_SOURCE_SELECT);
  CLOCK_SetDiv(kCLOCK_Sai1PreDiv, DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER);
  CLOCK_SetDiv(kCLOCK_Sai1Div, DEMO_SAI1_CLOCK_SOURCE_DIVIDER);

  /*开启IIC和IIS时钟*/
  BOARD_EnableSaiMclkOutput(true);
  BOARD_Codec_I2C_Init();

  /*配置SAI*/
  /*
     * config.masterSlave = kSAI_Master;
     * config.mclkSource = kSAI_MclkSourceSysclk;
     * config.protocol = kSAI_BusLeftJustified;
     * config.syncMode = kSAI_ModeAsync;
     * config.mclkOutputEnable = true;
     */
  SAI_TxGetDefaultConfig(&config);
  SAI_TxInit(DEMO_SAI, &config);

  /* Configure the audio format */
  memset(&format, 0U, sizeof(sai_transfer_format_t)); //

  format.bitWidth = kSAI_WordWidth16bits;
  format.channel = 0U;
  format.sampleRate_Hz = kSAI_SampleRate48KHz;

  format.masterClockHz = DEMO_SAI_CLK_FREQ;

  format.protocol = config.protocol;
  format.stereo = kSAI_Stereo;
  format.isFrameSyncCompact = true;

  /* Use default setting to init codec */
  CODEC_Init(&codecHandle, &boardCodecConfig);
  CODEC_SetFormat(&codecHandle, format.masterClockHz, format.sampleRate_Hz, format.bitWidth);

  SAI_TransferTxCreateHandle(DEMO_SAI, &txHandle, callback, NULL);
  mclkSourceClockHz = DEMO_SAI_CLK_FREQ;
  SAI_TransferTxSetFormat(DEMO_SAI, &txHandle, &format, mclkSourceClockHz, format.masterClockHz);
}



