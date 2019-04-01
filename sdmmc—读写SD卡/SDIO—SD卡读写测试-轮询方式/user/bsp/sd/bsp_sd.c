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
#include "./bsp/sd/bsp_sd.h"
#include "fsl_sdmmc_host.h"
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






///**
//* @brief  ��ʼ��uart���ò���
//* @param  ��
//* @retval ��
//*/
//void UART_ModeConfig(void)
//{
//
//  /*���崮�����ò����ṹ����������ڱ��洮�ڵ�������Ϣ*/
//  lpuart_config_t config;
//  
//  /*���ù̼��⺯���õ�Ĭ�ϵĴ������ò�������Ĭ�ϵ����ò����������޸�*/
//  LPUART_GetDefaultConfig(&config);
//  config.baudRate_Bps = DEBUG_UART_BAUDRATE;  //������
//  config.enableRx = DEBUG_UART_ENABLE_RESIVE; //�Ƿ������������
//  config.enableTx = DEBUG_UART_ENABLE_SEND;   //�Ƿ�����������
//  
//  /*���ù̼��⺯�������޸ĺõ�������Ϣд�뵽���ڵ����üĴ�����*/
//  LPUART_Init(DEBUG_UARTx, &config, BOARD_DEBUG_UART_CLK_FREQ);
//
//  
//  /*��������ж�*/
//  LPUART_EnableInterrupts(DEBUG_UARTx, kLPUART_RxDataRegFullInterruptEnable);
//  
//   /*�����ж����ȼ�,*/
//  set_IRQn_Priority(DEBUG_UART_IRQ,Group4_PreemptPriority_6, Group4_SubPriority_0);
//  /*ʹ���ж�*/
//  EnableIRQ(DEBUG_UART_IRQ);
//  
//
// }
// 
// /**
//* @brief  ��ʼ��uart���Ź���
//* @param  ��
//* @retval ��
//*/
//void UART_IOMUXC_MUX_Config(void)
//{
//  /* RX��TX���� */
//  IOMUXC_SetPinMux(UART_RX_IOMUXC, 0U);                                   
//  IOMUXC_SetPinMux(UART_TX_IOMUXC, 0U); 
//}
// 
// /**
//* @brief  ��ʼ��uart���IOMUXC��PAD��������
//* @param  ��
//* @retval ��
//*/
//void UART_IOMUXC_PAD_Config(void)
//{
//  IOMUXC_SetPinConfig(UART_RX_IOMUXC, UART_RX_PAD_CONFIG_DATA);
//  IOMUXC_SetPinConfig(UART_TX_IOMUXC, UART_TX_PAD_CONFIG_DATA);
  

void USDHC1_gpio_init(void)
{
  /*����GPIO�������ýṹ��*/
  gpio_pin_config_t gpt_config;
  
    /*���ó�ʼ���ṹ��*/
  gpt_config.direction = kGPIO_DigitalOutput; //���ģʽ
  gpt_config.outputLogic =  1;                //Ĭ�ϸߵ�ƽ
  gpt_config.interruptMode = kGPIO_NoIntmode; //��ʹ���ж�
  
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
* @retval 0���ɹ���-1��ʧ��
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
