/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "sai.h"
#include "fsl_codec_common.h"
#include "fsl_wm8960.h"
/******************************/
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


sai_config_t config;           //����SAI�ṹ��


void BOARD_EnableSaiMclkOutput(bool enable)
{
    if (enable)
    {
        IOMUXC_GPR->GPR1 |= IOMUXC_GPR_GPR1_SAI1_MCLK_DIR_MASK;
    }
    else
    {
        IOMUXC_GPR->GPR1 &= (~IOMUXC_GPR_GPR1_SAI1_MCLK_DIR_MASK);
    }
}



void	SAI1_Init(void)
{
  CLOCK_InitAudioPll(&audioPllConfig); //����
  BOARD_InitDebugConsole();
  
  /*Clock setting for SAI1*/
  CLOCK_SetMux(kCLOCK_Sai1Mux, DEMO_SAI1_CLOCK_SOURCE_SELECT);
  CLOCK_SetDiv(kCLOCK_Sai1PreDiv, DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER);
  CLOCK_SetDiv(kCLOCK_Sai1Div, DEMO_SAI1_CLOCK_SOURCE_DIVIDER);
  
  
    /* Init SAI module */
  /*
   * config.masterSlave = kSAI_Master;
   * config.mclkSource = kSAI_MclkSourceSysclk;
   * config.protocol = kSAI_BusLeftJustified;
   * config.syncMode = kSAI_ModeAsync;
   * config.mclkOutputEnable = true;
   */
  SAI_TxGetDefaultConfig(&config);
  SAI_TxInit(DEMO_SAI, &config);

  /* Initialize SAI Rx */
  SAI_RxGetDefaultConfig(&config);
  SAI_RxInit(DEMO_SAI, &config);
}










uint32_t mclkSourceClockHz = 0U;

edma_config_t dmaConfig = {0};    //dam���ýṹ��
edma_handle_t dmaTxHandle = {0};  //dma���;��
edma_handle_t dmaRxHandle = {0};  //dma���վ��

sai_transfer_format_t format = {0};// sai�����ʽ

AT_NONCACHEABLE_SECTION_INIT(sai_edma_handle_t txHandle) = {0};  //sai edma ���;��
AT_NONCACHEABLE_SECTION_INIT(sai_edma_handle_t rxHandle) = {0};  //sai edma ���վ��


extern codec_config_t boardCodecConfig;  //WM8960 ��ʼ���ṹ��
codec_handle_t codecHandle = {0};        //��������ö���


void	SAI1_DMAConfig(void)
{
  
    EDMA_GetDefaultConfig(&dmaConfig);
  EDMA_Init(EXAMPLE_DMA, &dmaConfig);
  EDMA_CreateHandle(&dmaTxHandle, EXAMPLE_DMA, EXAMPLE_TX_CHANNEL);
  EDMA_CreateHandle(&dmaRxHandle, EXAMPLE_DMA, EXAMPLE_RX_CHANNEL);

  DMAMUX_Init(EXAMPLE_DMAMUX);
  DMAMUX_SetSource(EXAMPLE_DMAMUX, EXAMPLE_TX_CHANNEL, (uint8_t)EXAMPLE_SAI_TX_SOURCE);
  DMAMUX_EnableChannel(EXAMPLE_DMAMUX, EXAMPLE_TX_CHANNEL);
  DMAMUX_SetSource(EXAMPLE_DMAMUX, EXAMPLE_RX_CHANNEL, (uint8_t)EXAMPLE_SAI_RX_SOURCE);
  DMAMUX_EnableChannel(EXAMPLE_DMAMUX, EXAMPLE_RX_CHANNEL);
  
  
  
//  /*! @brief Audio word width */
//typedef enum _sai_word_width
//{
//    kSAI_WordWidth8bits = 8U,   /*!< Audio data width 8 bits */
//    kSAI_WordWidth16bits = 16U, /*!< Audio data width 16 bits */
//    kSAI_WordWidth24bits = 24U, /*!< Audio data width 24 bits */
//    kSAI_WordWidth32bits = 32U  /*!< Audio data width 32 bits */
//} sai_word_width_t;
  /* Configure the audio format */
    format.bitWidth = kSAI_WordWidth16bits;
    format.channel = 0U;
    //      kSAI_SampleRate8KHz = 8000U,     /*!< Sample rate 8000 Hz */
//    kSAI_SampleRate11025Hz = 11025U, /*!< Sample rate 11025 Hz */
//    kSAI_SampleRate12KHz = 12000U,   /*!< Sample rate 12000 Hz */
//    kSAI_SampleRate16KHz = 16000U,   /*!< Sample rate 16000 Hz */
//    kSAI_SampleRate22050Hz = 22050U, /*!< Sample rate 22050 Hz */
//    kSAI_SampleRate24KHz = 24000U,   /*!< Sample rate 24000 Hz */
//    kSAI_SampleRate32KHz = 32000U,   /*!< Sample rate 32000 Hz */
//    kSAI_SampleRate44100Hz = 44100U, /*!< Sample rate 44100 Hz */
//    kSAI_SampleRate48KHz = 48000U,   /*!< Sample rate 48000 Hz */
//    kSAI_SampleRate96KHz = 96000U    /*!< Sample rate 96000 Hz */
    format.sampleRate_Hz = kSAI_SampleRate22050Hz;
#if (defined FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER && FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER) || \
    (defined FSL_FEATURE_PCC_HAS_SAI_DIVIDER && FSL_FEATURE_PCC_HAS_SAI_DIVIDER)
    format.masterClockHz = OVER_SAMPLE_RATE * format.sampleRate_Hz;
#else
    format.masterClockHz = DEMO_SAI_CLK_FREQ;
#endif
    format.protocol = config.protocol;
    format.stereo = kSAI_Stereo;
    format.isFrameSyncCompact = true;
#if defined(FSL_FEATURE_SAI_FIFO_COUNT) && (FSL_FEATURE_SAI_FIFO_COUNT > 1)
    format.watermark = FSL_FEATURE_SAI_FIFO_COUNT / 2U;
#endif
  
  
  
    SAI_TransferTxCreateHandleEDMA(DEMO_SAI, &txHandle, txCallback, NULL, &dmaTxHandle);
    SAI_TransferRxCreateHandleEDMA(DEMO_SAI, &rxHandle, rxCallback, NULL, &dmaRxHandle);

    mclkSourceClockHz = DEMO_SAI_CLK_FREQ;
    SAI_TransferTxSetFormatEDMA(DEMO_SAI, &txHandle, &format, mclkSourceClockHz, format.masterClockHz);
    SAI_TransferRxSetFormatEDMA(DEMO_SAI, &rxHandle, &format, mclkSourceClockHz, format.masterClockHz);

    /* Enable interrupt to handle FIFO error */
    SAI_TxEnableInterrupts(DEMO_SAI, kSAI_FIFOErrorInterruptEnable);
    SAI_RxEnableInterrupts(DEMO_SAI, kSAI_FIFOErrorInterruptEnable);
    EnableIRQ(DEMO_SAI_TX_IRQ);
    EnableIRQ(DEMO_SAI_RX_IRQ);
    
//        /* Use default setting to init codec */
//    CODEC_Init(&codecHandle, &boardCodecConfig);
//    CODEC_SetFormat(&codecHandle, format.masterClockHz, format.sampleRate_Hz, format.bitWidth);



//codec_config_t boardCodecConfig = {.I2C_SendFunc = BOARD_Codec_I2C_Send,
//                                   .I2C_ReceiveFunc = BOARD_Codec_I2C_Receive,
//                                   .op.Init = WM8960_Init,
//                                   .op.Deinit = WM8960_Deinit,
//                                   .op.SetFormat = WM8960_ConfigDataFormat};


    /* Use default setting to init codec */
    CODEC_Init(&codecHandle, &boardCodecConfig);
    CODEC_SetFormat(&codecHandle, format.masterClockHz, format.sampleRate_Hz, format.bitWidth);
    
//    WM8960_Init(&boardCodecConfig,NULL);

//    WM8960_SetMasterSlave(false);
    

//	 WM8960_SetVolume(&codecHandle,kWM8960_ModuleDAC,0x00);        //0-FF
//	 WM8960_SetVolume(&codecHandle,kWM8960_ModuleHP,0x7F);        //0-7F
//	 WM8960_SetVolume(&codecHandle,kWM8960_ModuleSpeaker,0x7F);   //0-7F

//	 WM8960_SetModule(&codecHandle,kWM8960_ModuleVREF,true);
//	 WM8960_SetModule(&codecHandle,kWM8960_ModuleDAC, true);
//	 WM8960_SetModule(&codecHandle,kWM8960_ModuleLineOut, true);
//	 WM8960_SetModule(&codecHandle,kWM8960_ModuleHP,true);
//	 WM8960_SetModule(&codecHandle,kWM8960_ModuleSpeaker,true);

//	 WM8960_SetProtocol(&codecHandle,kWM8960_BusLeftJustified);
  
}



extern volatile bool istxFinished;
extern volatile int tx_success_tount;

static void txCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
//    sendCount++;
//    emptyBlock++;
      tx_success_tount++;

//    if (sendCount == beginCount)
//    {
//        
//        SAI_TransferTerminateSendEDMA(base, handle);
//        sendCount = 0;
//    }
  istxFinished = true;
}


static void rxCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
//    receiveCount++;
//    fullBlock++;

//    if (receiveCount == beginCount)
//    {
//        isrxFinished = true;
//        SAI_TransferTerminateReceiveEDMA(base, handle);
//        receiveCount = 0;
//    }
}