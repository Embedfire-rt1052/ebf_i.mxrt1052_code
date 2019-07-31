/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   读写SD卡测试例程
  ******************************************************************
  * @attention
  *
  * 实验平台:野火  i.MXRT1052开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************
  */
#include "fsl_debug_console.h"
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"

#include "./bsp/sd/bsp_sd.h"  



#include "ff.h"
#include "diskio.h"
#include "fsl_sd_disk.h"
#include "./bsp/sd_fatfs_test/bsp_sd_fatfs_test.h"
    
    
    
    
    
#include "sai.h"
#include "fsl_sai.h"
#include "fsl_edma.h"
#include "fsl_clock.h"
#include "fsl_wm8960.h"
#include "fsl_codec_common.h"
#include "fsl_lpi2c.h"
#include "fsl_wm8960.h"
#include "music.h"
    


                                                        //extern codec_config_t boardCodecConfig;

                                                        ///******************************/
                                                        ///*
                                                        // * AUDIO PLL setting: Frequency = Fref * (DIV_SELECT + NUM / DENOM)
                                                        // *                              = 24 * (32 + 77/100)
                                                        // *                              = 786.48 MHz
                                                        // */
                                                        //const clock_audio_pll_config_t audioPllConfig = {
                                                        //    .loopDivider = 32,  /* PLL loop divider. Valid range for DIV_SELECT divider value: 27~54. */
                                                        //    .postDivider = 1,   /* Divider after the PLL, should only be 1, 2, 4, 8, 16. */
                                                        //    .numerator = 77,    /* 30 bit numerator of fractional loop divider. */
                                                        //    .denominator = 100, /* 30 bit denominator of fractional loop divider */
                                                        //};



                                                        //AT_NONCACHEABLE_SECTION_INIT(sai_edma_handle_t txHandle) = {0};
                                                        //edma_handle_t dmaTxHandle = {0};
                                                        //AT_NONCACHEABLE_SECTION_INIT(sai_edma_handle_t rxHandle) = {0};
                                                        //edma_handle_t dmaRxHandle = {0};
                                                        //sai_transfer_format_t format = {0};
                                                        ////AT_NONCACHEABLE_SECTION_ALIGN(uint8_t audioBuff[BUFFER_SIZE * BUFFER_NUM], 4);

                                                        //SDK_ALIGN(uint8_t audioBuff[SDK_SIZEALIGN(BUFFER_SIZE * BUFFER_NUM, SDMMC_DATA_BUFFER_ALIGN_CACHE)],
                                                        //          MAX(SDMMC_DATA_BUFFER_ALIGN_CACHE, SDMMCHOST_DMA_BUFFER_ADDR_ALIGN));


                                                        //codec_handle_t codecHandle = {0};

                                                        //extern codec_config_t boardCodecConfig;

                                                        //volatile bool istxFinished = false;
                                                        //volatile bool isrxFinished = false;
                                                        //volatile uint32_t beginCount = 0;
                                                        //volatile uint32_t sendCount = 0;
                                                        //volatile uint32_t receiveCount = 0;
                                                        //volatile bool sdcard = false;
                                                        //volatile uint32_t fullBlock = 0;
                                                        //volatile uint32_t emptyBlock = BUFFER_NUM;


                                                        ///* static values for fatfs */
                                                        //AT_NONCACHEABLE_SECTION(FATFS g_fileSystem); /* File system object */
                                                        //AT_NONCACHEABLE_SECTION(FIL g_fileObject);   /* File object */
                                                        //AT_NONCACHEABLE_SECTION(BYTE work[FF_MAX_SS]);
                                                        ///*! @brief SDMMC host detect card configuration */

                                                        /////*文件系统描述结构体*/
                                                        ////FATFS g_fileSystem; /* File system object */
                                                        // 

                                                        ///* @brief decription about the read/write buffer
                                                        //* The size of the read/write buffer should be a multiple of 512, since SDHC/SDXC card uses 512-byte fixed
                                                        //* block length and this driver example is enabled with a SDHC/SDXC card.If you are using a SDSC card, you
                                                        //* can define the block length by yourself if the card supports partial access.
                                                        //* The address of the read/write buffer should align to the specific DMA data buffer address align value if
                                                        //* DMA transfer is used, otherwise the buffer address is not important.
                                                        //* At the same time buffer address/size should be aligned to the cache line size if cache is supported.
                                                        //*/
                                                        //SDK_ALIGN(uint8_t g_bufferWrite[SDK_SIZEALIGN(100, SDMMC_DATA_BUFFER_ALIGN_CACHE)],
                                                        //          MAX(SDMMC_DATA_BUFFER_ALIGN_CACHE, SDMMCHOST_DMA_BUFFER_ADDR_ALIGN));
                                                        //SDK_ALIGN(uint8_t g_bufferRead[SDK_SIZEALIGN(100, SDMMC_DATA_BUFFER_ALIGN_CACHE)],
                                                        //          MAX(SDMMC_DATA_BUFFER_ALIGN_CACHE, SDMMCHOST_DMA_BUFFER_ADDR_ALIGN));


                                                        ///**
                                                        // * @brief 延时一段时间
                                                        // */
                                                        //void delay(uint32_t count);

                                                        ///**
                                                        // * @note 本函数在不同的优化模式下延时时间不同，
                                                        // *       如flexspi_nor_debug和flexspi_nor_release版本的程序中，
                                                        // *       flexspi_nor_release版本的延时要短得多  
                                                        // */ 
                                                        //void delay(uint32_t count)
                                                        //{   
                                                        //    volatile uint32_t i = 0;
                                                        //    for (i = 0; i < count; ++i)
                                                        //    {
                                                        //        __asm("NOP"); /* 调用nop空指令 */
                                                        //    }
                                                        //}




                                                        //static void txCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
                                                        //{
                                                        //    sendCount++;
                                                        //    emptyBlock++;

                                                        //    if (sendCount == beginCount)
                                                        //    {
                                                        //        istxFinished = true;
                                                        //        SAI_TransferTerminateSendEDMA(base, handle);
                                                        //        sendCount = 0;
                                                        //    }
                                                        //}


                                                        //static void rxCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
                                                        //{
                                                        //    receiveCount++;
                                                        //    fullBlock++;

                                                        //    if (receiveCount == beginCount)
                                                        //    {
                                                        //        isrxFinished = true;
                                                        //        SAI_TransferTerminateReceiveEDMA(base, handle);
                                                        //        receiveCount = 0;
                                                        //    }
                                                        //}


                                                        //void RecordPlayback(I2S_Type *base, uint32_t time_s);
                                                        //void RecordSDCard(I2S_Type *base, uint32_t time_s);
                                                        //void my_RecordSDCard(void);
                                                        //void PlaybackSine(I2S_Type *base, uint32_t SineWaveFreqHz, uint32_t time_s);

/*******************************************************************************
 * Definitions
 ******************************************************************************/
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

#define OVER_SAMPLE_RATE (384U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
sai_handle_t txHandle = {0};
static volatile bool isFinished = false;
#if !defined(DEMO_CODEC_WM8524)
codec_handle_t codecHandle = {0};
extern codec_config_t boardCodecConfig;
#endif
/*******************************************************************************
 * Code
 ******************************************************************************/

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

static void callback(I2S_Type *base, sai_handle_t *handle, status_t status, void *userData)
{
    isFinished = true;
}



void delay(uint32_t count)
{   
    volatile uint32_t i = 0;
    for (i = 0; i < count; ++i)
    {
        __asm("NOP"); /* 调用nop空指令 */
    }
}


AT_NONCACHEABLE_SECTION(FATFS g_fileSystem); /* File system object */
AT_NONCACHEABLE_SECTION(FIL g_fileObject);   /* File object */


/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
  uint32_t mclkSourceClockHz = 0U;
  int error = -1;//错误返回值
  int bytesRead; //文件读取得到的数据量
  
  /************移植新增内容*******************/
  sai_config_t config;           //配置SAI结构体
  edma_config_t dmaConfig = {0};
  
  sai_transfer_format_t format;
  sai_transfer_t xfer;
  uint32_t temp = 0;
  uint32_t delayCycle = 500000;


  /* 初始化内存保护单元 */      
  BOARD_ConfigMPU();
  /* 初始化开发板引脚 */
  BOARD_InitPins();
  /* 初始化开发板时钟 */
  BOARD_BootClockRUN();
  /* 初始化调试串口 */
  BOARD_InitDebugConsole();
  
  BOARD_ConfigMPU();
  BOARD_InitPins();
  BOARD_BootClockRUN();
  CLOCK_InitAudioPll(&audioPllConfig);
  BOARD_InitDebugConsole();  
  /*******初始化音频相关时钟****************/
  
  
  
  
    /*Clock setting for LPI2C*/
    CLOCK_SetMux(kCLOCK_Lpi2cMux, DEMO_LPI2C_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Lpi2cDiv, DEMO_LPI2C_CLOCK_SOURCE_DIVIDER);

    /*Clock setting for SAI1*/
    CLOCK_SetMux(kCLOCK_Sai1Mux, DEMO_SAI1_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Sai1PreDiv, DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER);
    CLOCK_SetDiv(kCLOCK_Sai1Div, DEMO_SAI1_CLOCK_SOURCE_DIVIDER);  
  
    /*Enable MCLK clock*/
    BOARD_EnableSaiMclkOutput(true);  
  
  
#if !defined DEMO_CODEC_WM8524
    BOARD_Codec_I2C_Init();
#endif

    memset(&format, 0U, sizeof(sai_transfer_format_t));

    PRINTF("SAI example started!\n\r");

    /*
     * config.masterSlave = kSAI_Master;
     * config.mclkSource = kSAI_MclkSourceSysclk;
     * config.protocol = kSAI_BusLeftJustified;
     * config.syncMode = kSAI_ModeAsync;
     * config.mclkOutputEnable = true;
     */
    SAI_TxGetDefaultConfig(&config);  




#if defined DEMO_SAI_CLOCK_SOURCE
    config.bclkSource = (sai_bclk_source_t)DEMO_SAI_CLOCK_SOURCE;
#endif

#if defined DEMO_CODEC_WM8524
    config.protocol = kSAI_BusI2S;
#endif
    SAI_TxInit(DEMO_SAI, &config);

    /* Configure the audio format */
    format.bitWidth = kSAI_WordWidth16bits;
    format.channel = 0U;
    format.sampleRate_Hz = kSAI_SampleRate48KHz;
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

#if defined(DEMO_CODEC_WM8524)
    wm8524_config_t codecConfig = {0};
    wm8524_handle_t codecHandle = {0};
    codecConfig.busPinNum = CODEC_BUS_PIN_NUM;
    codecConfig.busPin = CODEC_BUS_PIN;
    codecConfig.mutePin = CODEC_MUTE_PIN;
    codecConfig.mutePinNum = CODEC_MUTE_PIN_NUM;
    codecConfig.protocol = kWM8524_ProtocolI2S;
    WM8524_Init(&codecHandle, &codecConfig);
#else
    /* Use default setting to init codec */
    CODEC_Init(&codecHandle, &boardCodecConfig);
    CODEC_SetFormat(&codecHandle, format.masterClockHz, format.sampleRate_Hz, format.bitWidth);
#endif

    
    
#if defined(CODEC_CYCLE)
    delayCycle = CODEC_CYCLE;
#endif
    
    
   /*挂载文件系统*/
  f_mount_test(&g_fileSystem);
    
  /*打开文件*/  
  error = f_open(&g_fileObject, _T("/record/48.wav"), (FA_READ));
  if(error == 0)
  {
    PRINTF("打开文件成功 \r\n");
  }
  else
  {
    PRINTF("打开文件失败 \r\n");
  }
  
  /*移动文件的读写指针*/
  if (f_lseek(&g_fileObject, 44U))
  {
     PRINTF("Set file pointer position failed. \r\n");
  }

  while (delayCycle)
  {
     __ASM("nop");
     delayCycle--;
  }

  SAI_TransferTxCreateHandle(DEMO_SAI, &txHandle, callback, NULL);
  mclkSourceClockHz = DEMO_SAI_CLK_FREQ;
  SAI_TransferTxSetFormat(DEMO_SAI, &txHandle, &format, mclkSourceClockHz, format.masterClockHz);
  
  
  /*循环读取文件，并写入*/
  while(1)
  { 
     error = f_read(&g_fileObject, music, MUSIC_LEN, &bytesRead);
     PRINTF("the data is %d\r\n",error);
     if ((error)||(MUSIC_LEN != bytesRead))
     {
       PRINTF("the data is %d\r\n",error);
       PRINTF("读取文件失败 \r\n");
       while(1)
       {
         ;
       }
     }
     else
     {
        PRINTF("读取文件成功. \r\n");
//         for(j=0; j<4027; j++)
//         {
//////           PRINTF("the data is %x\r\n",audioBuff[j]);
//         }
     }
     
     
         /*  xfer structure */
    temp = (uint32_t)music;
    xfer.data = (uint8_t *)temp;
    xfer.dataSize = MUSIC_LEN;
    SAI_TransferSendNonBlocking(DEMO_SAI, &txHandle, &xfer);
     
    /* Wait until finished */
    while (isFinished != true)
    {
    } 
    delay(40000000);
    

    PRINTF("\n\r SAI example finished!\n\r ");
    
  }

//    /*  xfer structure */
//    temp = (uint32_t)music;
//    xfer.data = (uint8_t *)temp;
//    xfer.dataSize = MUSIC_LEN;
//    SAI_TransferSendNonBlocking(DEMO_SAI, &txHandle, &xfer);
//    
//    
//    
//    
//    /* Wait until finished */
//    while (isFinished != true)
//    {
//    }

//    PRINTF("\n\r SAI example finished!\n\r ");
    while (1)
    {
    }


}















//  
//  CLOCK_InitAudioPll(&audioPllConfig); //新增
//  BOARD_InitDebugConsole();
//  
// /*Clock setting for LPI2C*/
//  CLOCK_SetMux(kCLOCK_Lpi2cMux, DEMO_LPI2C_CLOCK_SOURCE_SELECT);
//  CLOCK_SetDiv(kCLOCK_Lpi2cDiv, DEMO_LPI2C_CLOCK_SOURCE_DIVIDER);

//  /*Clock setting for SAI1*/
//  CLOCK_SetMux(kCLOCK_Sai1Mux, DEMO_SAI1_CLOCK_SOURCE_SELECT);
//  CLOCK_SetDiv(kCLOCK_Sai1PreDiv, DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER);
//  CLOCK_SetDiv(kCLOCK_Sai1Div, DEMO_SAI1_CLOCK_SOURCE_DIVIDER);

//  /*Enable MCLK clock*/
//  BOARD_EnableSaiMclkOutput(true);
//  BOARD_Codec_I2C_Init();
//    
//    
//  
//  /* 打印系统时钟 */

//  

//  
//  /*初始化部分*/
//  
///* Create EDMA handle */
//  /*
//   * dmaConfig.enableRoundRobinArbitration = false;
//   * dmaConfig.enableHaltOnError = true;
//   * dmaConfig.enableContinuousLinkMode = false;
//   * dmaConfig.enableDebugMode = false;
//   */
//  EDMA_GetDefaultConfig(&dmaConfig);
//  EDMA_Init(EXAMPLE_DMA, &dmaConfig);
//  EDMA_CreateHandle(&dmaTxHandle, EXAMPLE_DMA, EXAMPLE_TX_CHANNEL);
//  EDMA_CreateHandle(&dmaRxHandle, EXAMPLE_DMA, EXAMPLE_RX_CHANNEL);

//  DMAMUX_Init(EXAMPLE_DMAMUX);
//  DMAMUX_SetSource(EXAMPLE_DMAMUX, EXAMPLE_TX_CHANNEL, (uint8_t)EXAMPLE_SAI_TX_SOURCE);
//  DMAMUX_EnableChannel(EXAMPLE_DMAMUX, EXAMPLE_TX_CHANNEL);
//  DMAMUX_SetSource(EXAMPLE_DMAMUX, EXAMPLE_RX_CHANNEL, (uint8_t)EXAMPLE_SAI_RX_SOURCE);
//  DMAMUX_EnableChannel(EXAMPLE_DMAMUX, EXAMPLE_RX_CHANNEL);


//  /* Init SAI module */
//  /*
//   * config.masterSlave = kSAI_Master;
//   * config.mclkSource = kSAI_MclkSourceSysclk;
//   * config.protocol = kSAI_BusLeftJustified;
//   * config.syncMode = kSAI_ModeAsync;
//   * config.mclkOutputEnable = true;
//   */
//  SAI_TxGetDefaultConfig(&config);
//  SAI_TxInit(DEMO_SAI, &config);

//  /* Initialize SAI Rx */
//  SAI_RxGetDefaultConfig(&config);
//  SAI_RxInit(DEMO_SAI, &config);


///* Configure the audio format */
//    format.bitWidth = kSAI_WordWidth16bits;
//    format.channel = 0U;
//    format.sampleRate_Hz = SAMPLE_RATE;
//#if (defined FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER && FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER) || \
//    (defined FSL_FEATURE_PCC_HAS_SAI_DIVIDER && FSL_FEATURE_PCC_HAS_SAI_DIVIDER)
//    format.masterClockHz = OVER_SAMPLE_RATE * format.sampleRate_Hz;
//#else
//    format.masterClockHz = DEMO_SAI_CLK_FREQ;
//#endif
//    format.protocol = config.protocol;
//    format.stereo = 0;
//    format.isFrameSyncCompact = true;
//#if defined(FSL_FEATURE_SAI_FIFO_COUNT) && (FSL_FEATURE_SAI_FIFO_COUNT > 1)
//    format.watermark = FSL_FEATURE_SAI_FIFO_COUNT / 2U;
//#endif

//    /* Use default setting to init codec */
//    CODEC_Init(&codecHandle, &boardCodecConfig);
//    CODEC_SetFormat(&codecHandle, format.masterClockHz, format.sampleRate_Hz, format.bitWidth);
//#if defined CODEC_USER_CONFIG
//    BOARD_Codec_Config(&codecHandle);
//#endif

//    SAI_TransferTxCreateHandleEDMA(DEMO_SAI, &txHandle, txCallback, NULL, &dmaTxHandle);
//    SAI_TransferRxCreateHandleEDMA(DEMO_SAI, &rxHandle, rxCallback, NULL, &dmaRxHandle);

//    mclkSourceClockHz = DEMO_SAI_CLK_FREQ;
//    SAI_TransferTxSetFormatEDMA(DEMO_SAI, &txHandle, &format, mclkSourceClockHz, format.masterClockHz);
//    SAI_TransferRxSetFormatEDMA(DEMO_SAI, &rxHandle, &format, mclkSourceClockHz, format.masterClockHz);

//    /* Enable interrupt to handle FIFO error */
//    SAI_TxEnableInterrupts(DEMO_SAI, kSAI_FIFOErrorInterruptEnable);
//    SAI_RxEnableInterrupts(DEMO_SAI, kSAI_FIFOErrorInterruptEnable);
//    EnableIRQ(DEMO_SAI_TX_IRQ);
//    EnableIRQ(DEMO_SAI_RX_IRQ);




//  /*挂载文件系统*/
//   f_mount_test(&g_fileSystem);
  

//  error = f_open(&g_fileObject, _T("/record/123.txt"), (FA_READ));
//  if(error == 0)
//  {
//    PRINTF("打开文件成功 \r\n");
//  }
//  else
//  {
//    PRINTF("打开文件失败 \r\n");
//  }
   
//    RecordSDCard(DEMO_SAI, 5);
//   PlaybackSine(DEMO_SAI, 200, 2);
  
//  my_RecordSDCard();
//  while (true)
//  {
//    
//  }






















//void my_RecordSDCard(void)
//{
//  int error = -1;
//  int bytesRead = 0;
//  int i = 0;
//  memset(g_bufferRead, 1U, sizeof(g_bufferRead));
//      for(i=0; i < sizeof(g_bufferRead); i++)
//    {
//      PRINTF("the data is %0x \r\n",g_bufferRead[i]);
//    }
//  PRINTF("root data is  %s \r\n",g_bufferRead);
//  
//  error = f_open(&g_fileObject, _T("/record/music1.wav"), (FA_READ));
////   error = f_open(&g_fileObject, _T("/record/123.txt"), (FA_READ));
//  if(error == 0)
//  {
//    PRINTF("打开文件成功 \r\n");
//  }
//  else
//  {
//    PRINTF("打开文件失败 \r\n");
//  }
//    if (f_lseek(&g_fileObject, 10000U))
//    {
//        PRINTF("Set file pointer position failed. \r\n");
//    }
//  error = f_read(&g_fileObject, g_bufferRead, sizeof(g_bufferRead), &bytesRead);
//  if ((error) || (bytesRead != sizeof(g_bufferRead)))
//  {
//    PRINTF("读取文件失败 \r\n");
//  }
//  else
//  {
//    PRINTF("读取文件成功. \r\n");
//    for(i=0; i < sizeof(g_bufferRead); i++)
//    {
//      PRINTF("the data is %0x \r\n",g_bufferRead[i]);
//    }
////    PRINTF("the data is %0x \r\n",g_bufferRead);
//  }
//  
////    memset(g_bufferRead, 0U, sizeof(g_bufferRead));

////    f_open(&g_fileObject, _T("/record/music1.wav"), (FA_READ));
////    if (f_lseek(&g_fileObject, 44U))
////    {
////        PRINTF("Set file pointer position failed. \r\n");
////    }
////    else
////    {
////      PRINTF("Set file pointer position success. \r\n");
////    }
////    
////   
////        error = f_read(&g_fileObject, g_bufferRead, sizeof(g_bufferRead), &bytesRead);
////        if(error == 0)
////        {
////            PRINTF("\r\n read success \r\n");
////            PRINTF("the data is %s \r\n",g_bufferRead);
////        }
//    
//}
///****************************END OF FILE**********************/

