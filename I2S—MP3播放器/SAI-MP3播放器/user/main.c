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
    


extern codec_config_t boardCodecConfig;

extern edma_handle_t dmaTxHandle ;
extern edma_handle_t dmaRxHandle ;


extern sai_transfer_format_t format;

AT_NONCACHEABLE_SECTION_INIT(sai_edma_handle_t txHandle) = {0};

AT_NONCACHEABLE_SECTION_INIT(sai_edma_handle_t rxHandle) = {0};


AT_NONCACHEABLE_SECTION_ALIGN(uint8_t audioBuff[BUFFER_SIZE * BUFFER_NUM], 4);
codec_handle_t codecHandle = {0};

extern codec_config_t boardCodecConfig;

volatile bool istxFinished = false;
volatile bool isrxFinished = false;
volatile uint32_t beginCount = 0;
volatile uint32_t sendCount = 0;
volatile uint32_t receiveCount = 0;
volatile bool sdcard = false;
volatile uint32_t fullBlock = 0;
volatile uint32_t emptyBlock = BUFFER_NUM;


/*�ļ�ϵͳ�����ṹ��*/
FATFS g_fileSystem; /* File system object */
FIL g_fileObject;


/* @brief decription about the read/write buffer
* The size of the read/write buffer should be a multiple of 512, since SDHC/SDXC card uses 512-byte fixed
* block length and this driver example is enabled with a SDHC/SDXC card.If you are using a SDSC card, you
* can define the block length by yourself if the card supports partial access.
* The address of the read/write buffer should align to the specific DMA data buffer address align value if
* DMA transfer is used, otherwise the buffer address is not important.
* At the same time buffer address/size should be aligned to the cache line size if cache is supported.
*/
SDK_ALIGN(uint8_t g_bufferWrite[SDK_SIZEALIGN(100, SDMMC_DATA_BUFFER_ALIGN_CACHE)],
          MAX(SDMMC_DATA_BUFFER_ALIGN_CACHE, SDMMCHOST_DMA_BUFFER_ADDR_ALIGN));
SDK_ALIGN(uint8_t g_bufferRead[SDK_SIZEALIGN(100, SDMMC_DATA_BUFFER_ALIGN_CACHE)],
          MAX(SDMMC_DATA_BUFFER_ALIGN_CACHE, SDMMCHOST_DMA_BUFFER_ADDR_ALIGN));


/**
 * @brief ��ʱһ��ʱ��
 */
void delay(uint32_t count);

/**
 * @note �������ڲ�ͬ���Ż�ģʽ����ʱʱ�䲻ͬ��
 *       ��flexspi_nor_debug��flexspi_nor_release�汾�ĳ����У�
 *       flexspi_nor_release�汾����ʱҪ�̵ö�  
 */ 
void delay(uint32_t count)
{   
    volatile uint32_t i = 0;
    for (i = 0; i < count; ++i)
    {
        __asm("NOP"); /* ����nop��ָ�� */
    }
}




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


void RecordPlayback(I2S_Type *base, uint32_t time_s);



void delay2(uint32_t count)
{
  volatile uint32_t i = 0;
  for (i = 0; i < count; ++i)
  {
    __asm("NOP"); /* ����nop��ָ�� */
  }
}
extern sai_config_t config;           //����SAI�ṹ��

extern edma_config_t dmaConfig;


/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
  
//  volatile FIL file_object;   //�����ļ���������
//  volatile DIR dir_object;    //Ŀ¼����ṹ��
//  volatile FILINFO file_info; //�ļ���Ϣ�����ṹ��
  int error = -1;
  int bytesRead = 0;
  sai_transfer_t xfer = {0};
  
  uint32_t mclkSourceClockHz = 0U;
  
  /************��ֲ��������*******************/
  
  edma_config_t dmaConfig = {0};

  /* ��ʼ���ڴ汣����Ԫ */      
  BOARD_ConfigMPU();
  /* ��ʼ������������ */
  BOARD_InitPins();
  /* ��ʼ��������ʱ�� */
  BOARD_BootClockRUN();
  /* ��ʼ�����Դ��� */
  BOARD_InitDebugConsole();
  
  
  
  SAI1_Init();
  
  /*******��ʼ����Ƶ���ʱ��****************/
  


  /*Enable MCLK clock*/
  BOARD_EnableSaiMclkOutput(true);
  BOARD_Codec_I2C_Init();
    
    
  
  /* ��ӡϵͳʱ�� */
  PRINTF("\r\n");
  PRINTF("*****��ӭʹ�� Ұ��i.MX RT1052 ������*****\r\n");
  PRINTF("CPU:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_CpuClk));
  PRINTF("AHB:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_AhbClk));
  PRINTF("SEMC:            %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SemcClk));
  PRINTF("SYSPLL:          %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllClk));
  PRINTF("SYSPLLPFD0:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd0Clk));
  PRINTF("SYSPLLPFD1:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd1Clk));
  PRINTF("SYSPLLPFD2:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd2Clk));
  PRINTF("SYSPLLPFD3:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd3Clk));
  

  
  /*��ʼ������*/
  
/* Create EDMA handle */
  /*
   * dmaConfig.enableRoundRobinArbitration = false;
   * dmaConfig.enableHaltOnError = true;
   * dmaConfig.enableContinuousLinkMode = false;
   * dmaConfig.enableDebugMode = false;
   */


  SAI1_DMAConfig();
  





    /* Use default setting to init codec */
    CODEC_Init(&codecHandle, &boardCodecConfig);
    CODEC_SetFormat(&codecHandle, format.masterClockHz, format.sampleRate_Hz, format.bitWidth);
    
#if defined CODEC_USER_CONFIG
    BOARD_Codec_Config(&codecHandle);
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

  /*�����ļ�ϵͳ*/
   f_mount_test(&g_fileSystem);
   
   
   istxFinished = 0;
       while(1)
    {
      /*���ļ�*/
      error = f_open(&g_fileObject, _T("/record/22.wav"), (FA_READ));
      if (error == 0)
      {
        PRINTF("���ļ��ɹ� \r\n");
      }
      else
      {
        PRINTF("���ļ�ʧ�� \r\n");
      }
      /*�ƶ��ļ��Ķ�дָ��*/
      if (f_lseek(&g_fileObject, 30044U))
      {
        PRINTF("Set file pointer position failed. \r\n");
      }


      
      while(1)
      {
        
          error = f_read(&g_fileObject, audioBuff, BUFFER_SIZE * BUFFER_NUM, &bytesRead);
          if ((error) || ((BUFFER_SIZE * BUFFER_NUM) != bytesRead))
          {
            PRINTF("read error %d\r\n", error);
            while (1);
          }
          xfer.data = audioBuff;
          xfer.dataSize = BUFFER_SIZE * BUFFER_NUM;
          
          if (SAI_TransferSendEDMA(DEMO_SAI, &txHandle, &xfer) == kStatus_Success)
          {

          }
          delay2(2000000);
//          while(!istxFinished);
//          istxFinished = false;
            
      } 
    }
   
   PlaybackSine(DEMO_SAI, 100, 5);

  while (true)
  {
    
  }
}
















/****************************END OF FILE**********************/

