

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



sai_config_t config;           //????SAI????


/*SAI IIC ?????????*/
void SAI1_IIC_init(void)
{
  /*Clock setting for LPI2C*/
  CLOCK_SetMux(kCLOCK_Lpi2cMux, DEMO_LPI2C_CLOCK_SOURCE_SELECT);
  CLOCK_SetDiv(kCLOCK_Lpi2cDiv, DEMO_LPI2C_CLOCK_SOURCE_DIVIDER);
  BOARD_Codec_I2C_Init();
}


void	SAI1_Init(void)
{
  CLOCK_InitAudioPll(&audioPllConfig); //????
  BOARD_InitDebugConsole();
  
  /*Clock setting for SAI1*/
  CLOCK_SetMux(kCLOCK_Sai1Mux, DEMO_SAI1_CLOCK_SOURCE_SELECT);
  CLOCK_SetDiv(kCLOCK_Sai1PreDiv, DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER);
  CLOCK_SetDiv(kCLOCK_Sai1Div, DEMO_SAI1_CLOCK_SOURCE_DIVIDER);
  
  /*Enable MCLK clock*/
  BOARD_EnableSaiMclkOutput(true);
  
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


volatile uint32_t mclkSourceClockHz = 0U;
edma_config_t dmaConfig = {0};    //dam???????
edma_handle_t dmaTxHandle = {0};  //dma??????
edma_handle_t dmaRxHandle = {0};  //dma??????
sai_transfer_format_t format = {0};// sai??????


AT_NONCACHEABLE_SECTION_INIT(sai_edma_handle_t txHandle) = {0};  //sai edma ??????
AT_NONCACHEABLE_SECTION_INIT(sai_edma_handle_t rxHandle) = {0};  //sai edma ??????


extern codec_config_t boardCodecConfig;  //WM8960 ?????????
codec_handle_t codecHandle = {0};        //????????????


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
    format.sampleRate_Hz = kSAI_SampleRate44100Hz;
#if (defined FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER && FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER) || \
    (defined FSL_FEATURE_PCC_HAS_SAI_DIVIDER && FSL_FEATURE_PCC_HAS_SAI_DIVIDER)
    format.masterClockHz = OVER_SAMPLE_RATE * format.sampleRate_Hz;
#else
    format.masterClockHz = DEMO_SAI_CLK_FREQ;
#endif



///*! @brief Define the SAI bus type */
//typedef enum _sai_protocol
//{
//    kSAI_BusLeftJustified = 0x0U, /*!< Uses left justified format.*/
//    kSAI_BusRightJustified,       /*!< Uses right justified format. */
//    kSAI_BusI2S,                  /*!< Uses I2S format. */
//    kSAI_BusPCMA,                 /*!< Uses I2S PCM A format.*/
//    kSAI_BusPCMB                  /*!< Uses I2S PCM B format. */
//} sai_protocol_t;



//typedef struct _sai_transfer_format
//{
//    uint32_t sampleRate_Hz;   /*!< Sample rate of audio data */
//    uint32_t bitWidth;        /*!< Data length of audio data, usually 8/16/24/32 bits */
//    sai_mono_stereo_t stereo; /*!< Mono or stereo */
//    uint32_t masterClockHz;   /*!< Master clock frequency in Hz */
//#if defined(FSL_FEATURE_SAI_FIFO_COUNT) && (FSL_FEATURE_SAI_FIFO_COUNT > 1)
//    uint8_t watermark;       /*!< Watermark value */
//#endif                       /* FSL_FEATURE_SAI_FIFO_COUNT */
//    uint8_t channel;         /*!< Data channel used in transfer.*/
//    sai_protocol_t protocol; /*!< Which audio protocol used */
//    bool isFrameSyncCompact; /*!< True means Frame sync length is configurable according to bitWidth, false means frame
//                                sync length is 64 times of bit clock. */
//} sai_transfer_format_t;

    format.protocol = kSAI_BusI2S;
//    kSAI_BusI2S   protocol
    format.stereo = kSAI_Stereo;//双声道
    format.isFrameSyncCompact = true;
#if defined(FSL_FEATURE_SAI_FIFO_COUNT) && (FSL_FEATURE_SAI_FIFO_COUNT > 1)//  FIFO count ????????????????
    format.watermark = FSL_FEATURE_SAI_FIFO_COUNT / 2U;
#endif
  
  
    SAI_TransferTxCreateHandleEDMA(DEMO_SAI, &txHandle, txCallback, NULL, &dmaTxHandle);
    SAI_TransferRxCreateHandleEDMA(DEMO_SAI, &rxHandle, rxCallback, NULL, &dmaRxHandle);

    mclkSourceClockHz = DEMO_SAI_CLK_FREQ;
    SAI_TransferTxSetFormatEDMA(DEMO_SAI, &txHandle, &format, mclkSourceClockHz, format.masterClockHz);
    SAI_TransferRxSetFormatEDMA(DEMO_SAI, &rxHandle, &format, mclkSourceClockHz, format.masterClockHz);


   
//codec_config_t boardCodecConfig = {.I2C_SendFunc = BOARD_Codec_I2C_Send,
//                                   .I2C_ReceiveFunc = BOARD_Codec_I2C_Receive,
//                                   .op.Init = WM8960_Init,
//                                   .op.Deinit = WM8960_Deinit,
//                                   .op.SetFormat = WM8960_ConfigDataFormat};


    /* Use default setting to init codec */
    CODEC_Init(&codecHandle, &boardCodecConfig);
    CODEC_SetFormat(&codecHandle, format.masterClockHz, format.sampleRate_Hz, format.bitWidth);
}

/*???saiMclk ???*/
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





extern volatile int curretn_outbuffer;
extern volatile bool outbuffer1_full;
extern volatile bool outbuffer2_full;



/*I2S DAM ?????????????*/
static void txCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
  if(curretn_outbuffer == 0)
  {
    /*buffer 1发送完成，设置outbuffer1为空*/
    outbuffer1_full = false;
  }
  if(curretn_outbuffer == 1)
  {
    /*buffer 2 发送完成，设置outbuffer2为空*/
    outbuffer2_full = false;
  }

}


static void rxCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
  ;
}



