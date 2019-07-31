/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   ��дSD����������
  ******************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  i.MXRT1052������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************
  */
// #define    CPU_MIMXRT1052CVL5B
// #define    XIP_EXTERNAL_FLASH = 1
// #define    XIP_BOOT_HEADER_ENABLE = 1
// #define    XIP_BOOT_HEADER_DCD_ENABLE = 1
// #define    PRINTF_FLOAT_ENABLE = 1
// #define    SCANF_FLOAT_ENABLE = 1
// #define    PRINTF_ADVANCED_ENABLE = 1
// #define    SCANF_ADVANCED_ENABLE = 1
// #define    USE_RAM_VECTOR_TABLE
// #define    ARM_MATH_CM7
// #define    SDK_I2C_BASED_COMPONENT_USED
// #define    FSL_SDK_ENABLE_DRIVER_CACHE_CONTROL = 1
// #define    SKIP_SYSCLK_INIT
// #define    DEBUG
// #define    ARM_MATH_CM7
// #define    ARM_MATH_CM4
// #define    CPU_MIMXRT1052DVL6B
// #define    PRINTF_FLOAT_ENABLE = 1
// #define    SDK_I2C_BASED_COMPONENT_USED = 1
// #define    BOARD_USE_CODEC = 1

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

#include "fsl_clock.h"
#include "fsl_wm8960.h"
#include "fsl_codec_common.h"
#include "fsl_lpi2c.h"
#include "fsl_wm8960.h"
#include "music.h"

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




extern volatile bool isFinished;
extern sai_handle_t txHandle;


/*******************************************************************************
 * Code
 ******************************************************************************/



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
    __asm("NOP"); /* ����nop��ָ�� */
  }
}

AT_NONCACHEABLE_SECTION(FATFS g_fileSystem); /* File system object */
AT_NONCACHEABLE_SECTION(FIL g_fileObject);   /* File object */


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

//void sai_init(void)
//{
//  sai_config_t config;          //����SAI�ṹ��
//  sai_transfer_format_t format; //����SAI����ṹ��
//  uint32_t mclkSourceClockHz = 0U;

//  /*�ⲿGPIO��ʼ��*/
//  CLOCK_InitAudioPll(&audioPllConfig);
//  /*����IICʱ��*/
//  CLOCK_SetMux(kCLOCK_Lpi2cMux, DEMO_LPI2C_CLOCK_SOURCE_SELECT);
//  CLOCK_SetDiv(kCLOCK_Lpi2cDiv, DEMO_LPI2C_CLOCK_SOURCE_DIVIDER);

//  /*����SAI1ʱ��*/
//  CLOCK_SetMux(kCLOCK_Sai1Mux, DEMO_SAI1_CLOCK_SOURCE_SELECT);
//  CLOCK_SetDiv(kCLOCK_Sai1PreDiv, DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER);
//  CLOCK_SetDiv(kCLOCK_Sai1Div, DEMO_SAI1_CLOCK_SOURCE_DIVIDER);

//  /*����IIC��IISʱ��*/
//  BOARD_EnableSaiMclkOutput(true);
//  BOARD_Codec_I2C_Init();

//  /*����SAI*/
//  /*
//     * config.masterSlave = kSAI_Master;
//     * config.mclkSource = kSAI_MclkSourceSysclk;
//     * config.protocol = kSAI_BusLeftJustified;
//     * config.syncMode = kSAI_ModeAsync;
//     * config.mclkOutputEnable = true;
//     */
//  SAI_TxGetDefaultConfig(&config);
//  SAI_TxInit(DEMO_SAI, &config);

//  /* Configure the audio format */
//  memset(&format, 0U, sizeof(sai_transfer_format_t)); //

//  format.bitWidth = kSAI_WordWidth16bits;
//  format.channel = 0U;
//  format.sampleRate_Hz = kSAI_SampleRate48KHz;

//  format.masterClockHz = DEMO_SAI_CLK_FREQ;

//  format.protocol = config.protocol;
//  format.stereo = kSAI_Stereo;
//  format.isFrameSyncCompact = true;

//  /* Use default setting to init codec */
//  CODEC_Init(&codecHandle, &boardCodecConfig);
//  CODEC_SetFormat(&codecHandle, format.masterClockHz, format.sampleRate_Hz, format.bitWidth);

//  SAI_TransferTxCreateHandle(DEMO_SAI, &txHandle, callback, NULL);
//  mclkSourceClockHz = DEMO_SAI_CLK_FREQ;
//  SAI_TransferTxSetFormat(DEMO_SAI, &txHandle, &format, mclkSourceClockHz, format.masterClockHz);
//}

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
  
  int error = -1; //���󷵻�ֵ
  int bytesRead;  //�ļ���ȡ�õ���������

  /************��ֲ��������*******************/


  sai_transfer_t xfer;
  uint32_t temp = 0;
  uint32_t delayCycle = 500000;

  /* ��ʼ���ڴ汣����Ԫ */
  BOARD_ConfigMPU();
  /* ��ʼ������������ */
  BOARD_InitPins();
  /* ��ʼ��������ʱ�� */
  BOARD_BootClockRUN();
  /* ��ʼ�����Դ���*/
  BOARD_InitDebugConsole();

  BOARD_ConfigMPU();
  BOARD_InitPins();
  BOARD_BootClockRUN();
  BOARD_InitDebugConsole();
  /*******��ʼ����Ƶ���ʱ��****************/
  sai_init();
  /*Clock setting for LPI2C*/

  /*�����ļ�ϵͳ*/
  f_mount_test(&g_fileSystem);

  /*���ļ�*/
  error = f_open(&g_fileObject, _T("/record/48.wav"), (FA_READ));
  if (error == 0)
  {
    PRINTF("���ļ��ɹ� \r\n");
  }
  else
  {
    PRINTF("���ļ�ʧ�� \r\n");
  }

  /*�ƶ��ļ��Ķ�дָ��*/
  if (f_lseek(&g_fileObject, 44U))
  {
    PRINTF("Set file pointer position failed. \r\n");
  }

  while (delayCycle)
  {
    __ASM("nop");
    delayCycle--;
  }

  /*ѭ����ȡ�ļ�����д��*/
  while (1)
  {
    error = f_read(&g_fileObject, music, MUSIC_LEN, &bytesRead);
    PRINTF("the data is %d\r\n", error);
    if ((error) || (MUSIC_LEN != bytesRead))
    {
      PRINTF("the data is %d\r\n", error);
      PRINTF("��ȡ�ļ�ʧ�� \r\n");
      while (1)
      {
        ;
      }
    }
    else
    {
      PRINTF("��ȡ�ļ��ɹ�. \r\n");
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

  //   PRINTF("\n\r SAI example finished!\n\r ");
  while (1)
  {
    
  }
}

