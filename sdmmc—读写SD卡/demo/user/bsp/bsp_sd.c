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
/*******************************************************************
 * Prototypes
 *******************************************************************/
/*! @brief Data block count accessed in card */
#define DATA_BLOCK_COUNT (5U)
/*! @brief Start data block number accessed in card */
#define DATA_BLOCK_START (2U)
/*! @brief Data buffer size. */
#define DATA_BUFFER_SIZE (FSL_SDMMC_DEFAULT_BLOCK_SIZE * DATA_BLOCK_COUNT)

/*! @brief Card结构描述符. */
sd_card_t g_sd;
/*******************************************************************
 * Code
 *******************************************************************/
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
