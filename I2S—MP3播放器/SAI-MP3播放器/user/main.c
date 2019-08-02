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
    

extern sai_transfer_format_t format;
extern sai_edma_handle_t txHandle;
extern sai_edma_handle_t rxHandle;

__attribute__((section("NonCacheable.init")))
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t audioBuff[BUFFER_SIZE * BUFFER_NUM], 4);
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t audioBuff2[BUFFER_SIZE * BUFFER_NUM], 4);


/*���͡�����״̬*/
volatile bool istxFinished = false;
volatile bool isrxFinished = false;


volatile uint32_t beginCount = 0;
volatile uint32_t sendCount = 0;
volatile uint32_t receiveCount = 0;


/*�� �� ���Ļ��������������ĸ�*/
volatile uint32_t fullBlock = 0;
volatile uint32_t emptyBlock = BUFFER_NUM;


volatile bool sdcard = false;


volatile int tx_success_tount = 0;







/*�ļ�ϵͳ�����ṹ��*/
FATFS g_fileSystem; /* File system object */
FIL g_fileObject;



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


/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
  

  int error = -1;
  int bytesRead = 0;
  sai_transfer_t xfer = {0};
  
  /* ��ʼ���ڴ汣����Ԫ */      
  BOARD_ConfigMPU();
  /* ��ʼ������������ */
  BOARD_InitPins();
  /* ��ʼ��������ʱ�� */
  BOARD_BootClockRUN();
  /* ��ʼ�����Դ��� */
  BOARD_InitDebugConsole();
  
  
   /*Clock setting for LPI2C*/
  CLOCK_SetMux(kCLOCK_Lpi2cMux, DEMO_LPI2C_CLOCK_SOURCE_SELECT);
  CLOCK_SetDiv(kCLOCK_Lpi2cDiv, DEMO_LPI2C_CLOCK_SOURCE_DIVIDER);
  BOARD_Codec_I2C_Init();
  
  
  SAI1_Init();

  /*Enable MCLK clock*/
  BOARD_EnableSaiMclkOutput(true);
  
    
    
  
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
  

   SAI1_DMAConfig();
  
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
      if (f_lseek(&g_fileObject, 44U))
      {
        PRINTF("Set file pointer position failed. \r\n");
      }

      error = f_read(&g_fileObject, audioBuff, BUFFER_SIZE*BUFFER_NUM, (UINT*)&bytesRead);
      if ((error) || ((BUFFER_SIZE * BUFFER_NUM) != bytesRead))
      {
        PRINTF("read error %d\r\n", error);
        while (1);
      }
//      
//      error = f_read(&g_fileObject, audioBuff2, BUFFER_SIZE*BUFFER_NUM, (UINT*)&bytesRead);
//      if ((error) || ((BUFFER_SIZE * BUFFER_NUM) != bytesRead))
//      {
//        PRINTF("read error %d\r\n", error);
//        while (1);
//      }
      
      while(1)
      {
        
        
          PRINTF("su_counter is:%d \r\n",tx_success_tount);
          error = f_read(&g_fileObject, audioBuff, BUFFER_SIZE*BUFFER_NUM, (UINT*)&bytesRead);
          if ((error) || ((BUFFER_SIZE * BUFFER_NUM) != bytesRead))
          {
            PRINTF("read error %d\r\n", error);
            while (1);
          }
          delay(2000000);
          xfer.data = (audioBuff);
          xfer.dataSize = BUFFER_SIZE * BUFFER_NUM;
          
          while(1)
          {
            if (SAI_TransferSendEDMA(DEMO_SAI, &txHandle, &xfer) == kStatus_Success)
            {
              break;
            }
          }
          memset(audioBuff,0,BUFFER_SIZE*BUFFER_NUM);
          delay(2000000);
          

          
          

      } 
    }
   
//   PlaybackSine(DEMO_SAI, 100, 5);

  while (true)
  {
    
  }
}
















/****************************END OF FILE**********************/

