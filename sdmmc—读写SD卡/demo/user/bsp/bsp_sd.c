/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   SD������
  ******************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  i.MXRT1052������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
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

/*! @brief Card�ṹ������. */
sd_card_t g_sd;
/*******************************************************************
 * Code
 *******************************************************************/
 /**
  * @brief  BOARD_USDHCClockConfiguration
  * @param  ��
  * @retval ��
  */
 static void BOARD_USDHCClockConfiguration(void)
{
    /*����ϵͳPLL PFD2 ϵ��Ϊ 18*/
    CLOCK_InitSysPfd(kCLOCK_Pfd0, 0x12U);
    /* ����USDHCʱ��Դ�ͷ�Ƶϵ�� */
    CLOCK_SetDiv(kCLOCK_Usdhc1Div, 0U);
    CLOCK_SetMux(kCLOCK_Usdhc1Mux, 1U);
}
/**
  * @brief  SDCard_Init
  * @param  ��
  * @retval 0���ɹ���1��ʧ��
  */
int SDCard_Init(void)
{
		sd_card_t *card = &g_sd;
	
		/* ��ʼ��SD����ʱ�� */
		BOARD_USDHCClockConfiguration();
	
	  card->host.base = SD_HOST_BASEADDR;
    card->host.sourceClock_Hz = SD_HOST_CLK_FREQ;

    /* SD������ʼ������ */
    if (SD_HostInit(card) != kStatus_Success)
    {
        PRINTF("\r\nSD������ʼ��ʧ��\r\n");
        return -1;
    }  
		/* ��ʼ��SD�� */
		if (SD_CardInit(card))
		{
				PRINTF("\r\nSD��ʼ��ʧ��\r\n");
				return -1;
		}

		return 0;		
}
/**
  * @brief  CardInformationLog
  * @param  card��sd���ṹ��ָ��
  * @retval ��
  */
static void CardInformationLog(sd_card_t *card)
{
    assert(card);

    PRINTF("\r\n����С %d * %d bytes\r\n", card->blockCount, card->blockSize);
    PRINTF("\r\n��������:\r\n");
    if (card->operationVoltage == kCARD_OperationVoltage330V)
    {
        PRINTF("\r\n  SD��������ѹ : 3.3V\r\n");
    }
    else if (card->operationVoltage == kCARD_OperationVoltage180V)
    {
        PRINTF("\r\n  SD��������ѹ : 1.8V\r\n");
    }

    if (card->currentTiming == kSD_TimingSDR12DefaultMode)
    {
        if (card->operationVoltage == kCARD_OperationVoltage330V)
        {
            PRINTF("\r\n  ʱ��ģʽ: ����ģʽ\r\n");
        }
        else if (card->operationVoltage == kCARD_OperationVoltage180V)
        {
            PRINTF("\r\n  ʱ��ģʽ: SDR12 ģʽ\r\n");
        }
    }
    else if (card->currentTiming == kSD_TimingSDR25HighSpeedMode)
    {
        if (card->operationVoltage == kCARD_OperationVoltage180V)
        {
            PRINTF("\r\n  ʱ��ģʽ: SDR25\r\n");
        }
        else
        {
            PRINTF("\r\n  ʱ��ģʽ: High Speed\r\n");
        }
    }
    else if (card->currentTiming == kSD_TimingSDR50Mode)
    {
        PRINTF("\r\n  ʱ��ģʽ: SDR50\r\n");
    }
    else if (card->currentTiming == kSD_TimingSDR104Mode)
    {
        PRINTF("\r\n  ʱ��ģʽ: SDR104\r\n");
    }
    else if (card->currentTiming == kSD_TimingDDR50Mode)
    {
        PRINTF("\r\n  ʱ��ģʽ: DDR50\r\n");
    }

    PRINTF("\r\n  Freq : %d HZ\r\n", card->busClock_Hz);
}
static status_t AccessCard(sd_card_t *card)
{
		/* д�����ݻ��� */
		SDK_ALIGN(uint8_t g_dataWrite[SDK_SIZEALIGN(DATA_BUFFER_SIZE, SDMMC_DATA_BUFFER_ALIGN_CACHE)],
          MAX(SDMMC_DATA_BUFFER_ALIGN_CACHE, SDMMCHOST_DMA_BUFFER_ADDR_ALIGN));
		/* ��ȡ���ݻ��� */
		SDK_ALIGN(uint8_t g_dataRead[SDK_SIZEALIGN(DATA_BUFFER_SIZE, SDMMC_DATA_BUFFER_ALIGN_CACHE)],
          MAX(SDMMC_DATA_BUFFER_ALIGN_CACHE, SDMMCHOST_DMA_BUFFER_ADDR_ALIGN));	
		memset(g_dataWrite, 0x5aU, sizeof(g_dataWrite));

		PRINTF("\r\nд��/��ȡһ�����ݿ�......\r\n");
		if (kStatus_Success != SD_WriteBlocks(card, g_dataWrite, DATA_BLOCK_START, 1U))
		{
				PRINTF("д��һ�����ݿ�ʧ��.\r\n");
				return kStatus_Fail;
		}

		memset(g_dataRead, 0U, sizeof(g_dataRead));
		if (kStatus_Success != SD_ReadBlocks(card, g_dataRead, DATA_BLOCK_START, 1U))
		{
				PRINTF("��ȡһ�����ݿ�.\r\n");
				return kStatus_Fail;
		}

		PRINTF("�Ƚ϶�ȡ/д������......\r\n");
		if (memcmp(g_dataRead, g_dataWrite, FSL_SDMMC_DEFAULT_BLOCK_SIZE))
		{
				PRINTF("��ȡ/д�����ݲ�һ��.\r\n");
				return kStatus_Fail;
		}
		PRINTF("��ȡ/д������һ��\r\n");

		PRINTF("д��/��ȡ������ݿ�......\r\n");
		if (kStatus_Success != SD_WriteBlocks(card, g_dataWrite, DATA_BLOCK_START, DATA_BLOCK_COUNT))
		{
				PRINTF("д�������ݿ�ʧ��.\r\n");
				return kStatus_Fail;
		}

		memset(g_dataRead, 0U, sizeof(g_dataRead));

		if (kStatus_Success != SD_ReadBlocks(card, g_dataRead, DATA_BLOCK_START, DATA_BLOCK_COUNT))
		{
				PRINTF("��ȡ������ݿ�ʧ��.\r\n");
				return kStatus_Fail;
		}

		PRINTF("�Ƚ϶�ȡ/д������......\r\n");
		if (memcmp(g_dataRead, g_dataWrite, FSL_SDMMC_DEFAULT_BLOCK_SIZE))
		{
				PRINTF("��ȡ/д�����ݲ�һ��.\r\n");
				return kStatus_Fail;
		}
		PRINTF("��ȡ/д������һ��.\r\n");

		PRINTF("����������ݿ�......\r\n");
		if (kStatus_Success != SD_EraseBlocks(card, DATA_BLOCK_START, DATA_BLOCK_COUNT))
		{
				PRINTF("����������ݿ�ʧ��.\r\n");
				return kStatus_Fail;
		}
		return kStatus_Success;
}
/**
  * @brief  SDCardTest
  * @param  ��
  * @retval ��
  */
void SDCardTest(void)
{
		PRINTF("\r\nSDCARD ��д��������.\r\n");
		SDCard_Init();
		/* ��ӡ��Ƭ������Ϣ */
    CardInformationLog(&g_sd);
		/* ��д���� */
		if(AccessCard(&g_sd)==kStatus_Success)
			PRINTF("\r\nSDCARD �������.\r\n");
		else
			PRINTF("\r\nSDCARD ����ʧ��.\r\n");
	
}
/****************************END OF FILE**********************/
