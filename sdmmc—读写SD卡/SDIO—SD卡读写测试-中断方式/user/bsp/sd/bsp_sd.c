/**
******************************************************************
* @file    main.c
* @author  fire
* @version V1.0
* @date    2018-xx-xx
* @brief   SD卡驱动
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
#include "fsl_sd.h"
#include "./bsp/sd/bsp_sd.h"
#include "fsl_sdmmc_host.h"
/*******************************************************************
* Prototypes
*******************************************************************/
/*! @brief Data block count accessed in card */
#define DATA_BLOCK_COUNT (5U)
/*! @brief Start data block number accessed in card */
#define DATA_BLOCK_START (2U)
/*! @brief Data buffer size.块大小乘以块数 */
#define DATA_BUFFER_SIZE (FSL_SDMMC_DEFAULT_BLOCK_SIZE * DATA_BLOCK_COUNT)

/*! @brief Card结构描述符. */
sd_card_t g_sd;
/*******************************************************************
* Code
*******************************************************************/

 
void USDHC1_gpio_init(void)
{
  /*定义GPIO引脚配置结构体*/
  gpio_pin_config_t gpt_config;
  
    /*配置初始化结构体*/
  gpt_config.direction = kGPIO_DigitalOutput; //输出模式
  gpt_config.outputLogic =  1;                //默认高电平
  gpt_config.interruptMode = kGPIO_NoIntmode; //不使用中断
  
  /*SD1_D0*/
  IOMUXC_SetPinMux(USDHC1_DATA0_IOMUXC, 0U);
  IOMUXC_SetPinConfig(USDHC1_DATA0_IOMUXC, USDHC1_DATA_PAD_CONFIG_DATA);
  GPIO_PinInit(USDHC1_DATA0_GPIO, USDHC1_DATA0_GPIO_PIN, &gpt_config);
  
  /*SD1_D1*/
  IOMUXC_SetPinMux(USDHC1_DATA1_IOMUXC, 0U);
  IOMUXC_SetPinConfig(USDHC1_DATA1_IOMUXC, USDHC1_DATA_PAD_CONFIG_DATA);
  GPIO_PinInit(USDHC1_DATA1_GPIO, USDHC1_DATA1_GPIO_PIN, &gpt_config);

  /*SD1_D2*/
  IOMUXC_SetPinMux(USDHC1_DATA2_IOMUXC, 0U);
  IOMUXC_SetPinConfig(USDHC1_DATA2_IOMUXC, USDHC1_DATA_PAD_CONFIG_DATA);
  GPIO_PinInit(USDHC1_DATA2_GPIO, USDHC1_DATA2_GPIO_PIN, &gpt_config);
  
  /*SD1_D3*/
  IOMUXC_SetPinMux(USDHC1_DATA3_IOMUXC, 0U);
  IOMUXC_SetPinConfig(USDHC1_DATA3_IOMUXC, USDHC1_DATA_PAD_CONFIG_DATA);
  GPIO_PinInit(USDHC1_DATA3_GPIO, USDHC1_DATA3_GPIO_PIN, &gpt_config);  
  
  /*SD1_CMD*/
  IOMUXC_SetPinMux(USDHC1_CMD_IOMUXC, 0U);
  IOMUXC_SetPinConfig(USDHC1_CMD_IOMUXC, USDHC1_DATA_PAD_CONFIG_DATA);
  GPIO_PinInit(USDHC1_CMD_GPIO, USDHC1_CMD_GPIO_PIN, &gpt_config);
  
  /*SD1_CLK*/
  IOMUXC_SetPinMux(USDHC1_CLK_IOMUXC, 0U);
  IOMUXC_SetPinConfig(USDHC1_CLK_IOMUXC, USDHC1_CLK_PAD_CONFIG_DATA);
  GPIO_PinInit(USDHC1_CLK_GPIO, USDHC1_CLK_GPIO_PIN, &gpt_config);
 
}







/**
* @brief  BOARD_USDHCClockConfiguration
* @param  无
* @retval 无
*/
static void BOARD_USDHCClockConfiguration(void)
{
  /*设置系统PLL PFD2 系数为 18*/
  CLOCK_InitSysPfd(kCLOCK_Pfd0, 0x12U);
  /* 配置USDHC时钟源和分频系数 */
  CLOCK_SetDiv(kCLOCK_Usdhc1Div, 0U);
  CLOCK_SetMux(kCLOCK_Usdhc1Mux, 1U);
}

//typedef struct _usdhc_host
//{
//    USDHC_Type *base;                   /*!< USDHC peripheral base address */
//    uint32_t sourceClock_Hz;            /*!< USDHC source clock frequency united in Hz */
//    usdhc_config_t config;              /*!< USDHC configuration */
//    usdhc_capability_t capability;      /*!< USDHC capability information */
//    usdhc_transfer_function_t transfer; /*!< USDHC transfer function */
//} usdhc_host_t;

//typedef struct _sd_card
//{
//    SDMMCHOST_CONFIG host; /*!< Host information */
//
//    sdcard_usr_param_t usrParam;    /*!< user parameter */
//    bool isHostReady;               /*!< use this flag to indicate if need host re-init or not*/
//    bool noInteralAlign;            /*!< use this flag to disable sdmmc align. If disable, sdmmc will not make sure the
//                                    data buffer address is word align, otherwise all the transfer are align to low level driver */
//    uint32_t busClock_Hz;           /*!< SD bus clock frequency united in Hz */
//    uint32_t relativeAddress;       /*!< Relative address of the card */
//    uint32_t version;               /*!< Card version */
//    uint32_t flags;                 /*!< Flags in _sd_card_flag */
//    uint32_t rawCid[4U];            /*!< Raw CID content */
//    uint32_t rawCsd[4U];            /*!< Raw CSD content */
//    uint32_t rawScr[2U];            /*!< Raw CSD content */
//    uint32_t ocr;                   /*!< Raw OCR content */
//    sd_cid_t cid;                   /*!< CID */
//    sd_csd_t csd;                   /*!< CSD */
//    sd_scr_t scr;                   /*!< SCR */
//    uint32_t blockCount;            /*!< Card total block number */
//    uint32_t blockSize;             /*!< Card block size */
//    sd_timing_mode_t currentTiming; /*!< current timing mode */
//    sd_driver_strength_t driverStrength;        /*!< driver strength */
//    sd_max_current_t maxCurrent;                /*!< card current limit */
//    sdmmc_operation_voltage_t operationVoltage; /*!< card operation voltage */
//} sd_card_t;




/**
* @brief  SDCard_Init
* @param  无
* @retval 0：成功，1：失败
*/
int SDCard_Init(void)
{
  sd_card_t *card = &g_sd;
  
  /* 初始化SD外设时钟 */
  BOARD_USDHCClockConfiguration();

  card->host.base = SD_HOST_BASEADDR;
  card->host.sourceClock_Hz = SD_HOST_CLK_FREQ;
  
  /* SD主机初始化函数 */
  if (SD_HostInit(card) != kStatus_Success)
  {
    PRINTF("\r\nSD主机初始化失败\r\n");
    return -1;
  }  

  /* 初始化SD卡 */
  if (SD_CardInit(card))
  {
    PRINTF("\r\nSD初始化失败\r\n");
    return -1;
  }
  
  return 0;		
}
/**
* @brief  CardInformationLog
* @param  card：sd卡结构体指针
* @retval 无
*/
static void CardInformationLog(sd_card_t *card)
{
  assert(card);
  
  PRINTF("\r\n卡大小 %d * %d bytes\r\n", card->blockCount, card->blockSize);
  PRINTF("\r\n工作条件:\r\n");
  if (card->operationVoltage == kCARD_OperationVoltage330V)
  {
    PRINTF("\r\n  SD卡操作电压 : 3.3V\r\n");
  }
  else if (card->operationVoltage == kCARD_OperationVoltage180V)
  {
    PRINTF("\r\n  SD卡操作电压 : 1.8V\r\n");
  }
  
  if (card->currentTiming == kSD_TimingSDR12DefaultMode)
  {
    if (card->operationVoltage == kCARD_OperationVoltage330V)
    {
      PRINTF("\r\n  时序模式: 常规模式\r\n");
    }
    else if (card->operationVoltage == kCARD_OperationVoltage180V)
    {
      PRINTF("\r\n  时序模式: SDR12 模式\r\n");
    }
  }
  else if (card->currentTiming == kSD_TimingSDR25HighSpeedMode)
  {
    if (card->operationVoltage == kCARD_OperationVoltage180V)
    {
      PRINTF("\r\n  时序模式: SDR25\r\n");
    }
    else
    {
      PRINTF("\r\n  时序模式: High Speed\r\n");
    }
  }
  else if (card->currentTiming == kSD_TimingSDR50Mode)
  {
    PRINTF("\r\n  时序模式: SDR50\r\n");
  }
  else if (card->currentTiming == kSD_TimingSDR104Mode)
  {
    PRINTF("\r\n  时序模式: SDR104\r\n");
  }
  else if (card->currentTiming == kSD_TimingDDR50Mode)
  {
    PRINTF("\r\n  时序模式: DDR50\r\n");
  }
  
  PRINTF("\r\n  Freq : %d HZ\r\n", card->busClock_Hz);
}
static status_t AccessCard(sd_card_t *card)
{
  /* 写入数据缓存 */

/*
说明：
  1.宏SDK_SIZEALIGN(N(数据大小), x)该宏定义的作用是增加N的值直到能被x整除，
 例如 N=6,x=4.则宏定义的结果是8。N=7,x=2宏定义的结果为8.
  2.宏SDK_ALIGN用于实现数对齐
*/
  SDK_ALIGN(uint8_t g_dataWrite[SDK_SIZEALIGN(DATA_BUFFER_SIZE, SDMMC_DATA_BUFFER_ALIGN_CACHE)],
            MAX(SDMMC_DATA_BUFFER_ALIGN_CACHE, SDMMCHOST_DMA_BUFFER_ADDR_ALIGN));
  /* 读取数据缓存 */
  SDK_ALIGN(uint8_t g_dataRead[SDK_SIZEALIGN(DATA_BUFFER_SIZE, SDMMC_DATA_BUFFER_ALIGN_CACHE)],
            MAX(SDMMC_DATA_BUFFER_ALIGN_CACHE, SDMMCHOST_DMA_BUFFER_ADDR_ALIGN));	
  memset(g_dataWrite, 0x5aU, sizeof(g_dataWrite));
  
  PRINTF("\r\n写入/读取一个数据块......\r\n");
  if (kStatus_Success != SD_WriteBlocks(card, g_dataWrite, DATA_BLOCK_START, 1U))
  {
    PRINTF("写入一个数据块失败.\r\n");
    return kStatus_Fail;
  }
  
  memset(g_dataRead, 0U, sizeof(g_dataRead));
  if (kStatus_Success != SD_ReadBlocks(card, g_dataRead, DATA_BLOCK_START, 1U))
  {
    PRINTF("读取一个数据块.\r\n");
    return kStatus_Fail;
  }
  
  PRINTF("比较读取/写入内容......\r\n");
  if (memcmp(g_dataRead, g_dataWrite, FSL_SDMMC_DEFAULT_BLOCK_SIZE))
  {
    PRINTF("读取/写入内容不一致.\r\n");
    return kStatus_Fail;
  }
  PRINTF("读取/写入内容一致\r\n");
  
  PRINTF("写入/读取多个数据块......\r\n");
  if (kStatus_Success != SD_WriteBlocks(card, g_dataWrite, DATA_BLOCK_START, DATA_BLOCK_COUNT))
  {
    PRINTF("写入多个数据块失败.\r\n");
    return kStatus_Fail;
  }
  
  memset(g_dataRead, 0U, sizeof(g_dataRead));
  
  if (kStatus_Success != SD_ReadBlocks(card, g_dataRead, DATA_BLOCK_START, DATA_BLOCK_COUNT))
  {
    PRINTF("读取多个数据块失败.\r\n");
    return kStatus_Fail;
  }
  
  PRINTF("比较读取/写入内容......\r\n");
  if (memcmp(g_dataRead, g_dataWrite, FSL_SDMMC_DEFAULT_BLOCK_SIZE))
  {
    PRINTF("读取/写入内容不一致.\r\n");
    return kStatus_Fail;
  }
  PRINTF("读取/写入内容一致.\r\n");
  
  PRINTF("擦除多个数据块......\r\n");
  if (kStatus_Success != SD_EraseBlocks(card, DATA_BLOCK_START, DATA_BLOCK_COUNT))
  {
    PRINTF("擦除多个数据块失败.\r\n");
    return kStatus_Fail;
  }
  return kStatus_Success;
}
/**
* @brief  SDCardTest
* @param  无
* @retval 无
*/
void SDCardTest(void)
{
  PRINTF("\r\nSDCARD 读写测试例程.\r\n");
  SDCard_Init();
  /* 打印卡片工作信息 */
  CardInformationLog(&g_sd);
  /* 读写测试 */
  if(AccessCard(&g_sd)==kStatus_Success)
    PRINTF("\r\nSDCARD 测试完成.\r\n");
  else
    PRINTF("\r\nSDCARD 测试失败.\r\n");
  
}
/****************************END OF FILE**********************/
