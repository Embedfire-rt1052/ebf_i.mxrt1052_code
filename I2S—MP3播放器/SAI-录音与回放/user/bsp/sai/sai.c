

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

sai_config_t config; //配置SAI结构体

/*SAI IIC 初始化函数*/
void SAI1_IIC_init(void)
{
  /*Clock setting for LPI2C*/
  CLOCK_SetMux(kCLOCK_Lpi2cMux, DEMO_LPI2C_CLOCK_SOURCE_SELECT);
  CLOCK_SetDiv(kCLOCK_Lpi2cDiv, DEMO_LPI2C_CLOCK_SOURCE_DIVIDER);
  BOARD_Codec_I2C_Init();
}

void SAI1_Init(void)
{
  CLOCK_InitAudioPll(&audioPllConfig); //新增
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
edma_config_t dmaConfig = {0};      //dam配置结构体
edma_handle_t dmaTxHandle = {0};    //dma发送句柄
edma_handle_t dmaRxHandle = {0};    //dma接收句柄
sai_transfer_format_t format = {0}; // sai传输格式

AT_NONCACHEABLE_SECTION_INIT(sai_edma_handle_t txHandle) = {0}; //sai edma 发送句柄
AT_NONCACHEABLE_SECTION_INIT(sai_edma_handle_t rxHandle) = {0}; //sai edma 接收句柄

extern codec_config_t boardCodecConfig; //WM8960 初始化结构体
codec_handle_t codecHandle = {0};       //编解码配置定义

void SAI1_DMAConfig(void)
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
  format.sampleRate_Hz = kSAI_SampleRate16KHz;
#if (defined FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER && FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER) || \
    (defined FSL_FEATURE_PCC_HAS_SAI_DIVIDER && FSL_FEATURE_PCC_HAS_SAI_DIVIDER)
  format.masterClockHz = OVER_SAMPLE_RATE * format.sampleRate_Hz;
#else
  format.masterClockHz = DEMO_SAI_CLK_FREQ;
#endif
  format.protocol = config.protocol;
  format.stereo = kSAI_Stereo;
  format.isFrameSyncCompact = true;
#if defined(FSL_FEATURE_SAI_FIFO_COUNT) && (FSL_FEATURE_SAI_FIFO_COUNT > 1) //  FIFO count 定义是多少？？？？
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

/*使能saiMclk 输出*/
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

extern volatile bool istxFinished; //保存TX发送状态
extern volatile bool isrxFinished;

extern volatile uint32_t fullBlock;
extern volatile uint32_t emptyBlock;

extern volatile uint32_t sendCount;
extern volatile uint32_t receiveCount;

extern volatile uint32_t beginCount;


/*I2S DAM 发送完成回调函数*/
static void txCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    sendCount++;
    emptyBlock++;

    if (sendCount == beginCount)
    {
        istxFinished = true;
        SAI_TransferTerminateSendEDMA(base, handle);
        sendCount = 0;
    }
}


/*I2S DAM 接收完成回调函数*/
static void rxCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    receiveCount++;
    fullBlock++;

    if (receiveCount == beginCount)
    {
        isrxFinished = true;
        SAI_TransferTerminateReceiveEDMA(base, handle);
        receiveCount = 0;
    }
}
