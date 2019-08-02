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
    

extern sai_transfer_format_t format;
extern sai_edma_handle_t txHandle;
extern sai_edma_handle_t rxHandle;

__attribute__((section("NonCacheable.init")))
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t audioBuff[BUFFER_SIZE * BUFFER_NUM], 4);
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t audioBuff2[BUFFER_SIZE * BUFFER_NUM], 4);


/*发送、接收状态*/
volatile bool istxFinished = false;
volatile bool isrxFinished = false;


volatile uint32_t beginCount = 0;
volatile uint32_t sendCount = 0;
volatile uint32_t receiveCount = 0;


/*空 和 满的缓冲区个数，共四个*/
volatile uint32_t fullBlock = 0;
volatile uint32_t emptyBlock = BUFFER_NUM;


volatile bool sdcard = false;


volatile int tx_success_tount = 0;







/*文件系统描述结构体*/
FATFS g_fileSystem; /* File system object */
FIL g_fileObject;



/**
 * @note 本函数在不同的优化模式下延时时间不同，
 *       如flexspi_nor_debug和flexspi_nor_release版本的程序中，
 *       flexspi_nor_release版本的延时要短得多  
 */ 
void delay(uint32_t count)
{   
    volatile uint32_t i = 0;
    for (i = 0; i < count; ++i)
    {
        __asm("NOP"); /* 调用nop空指令 */
    }
}


/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
  

  int error = -1;
  int bytesRead = 0;
  sai_transfer_t xfer = {0};
  
  /* 初始化内存保护单元 */      
  BOARD_ConfigMPU();
  /* 初始化开发板引脚 */
  BOARD_InitPins();
  /* 初始化开发板时钟 */
  BOARD_BootClockRUN();
  /* 初始化调试串口 */
  BOARD_InitDebugConsole();
  
  
   /*Clock setting for LPI2C*/
  CLOCK_SetMux(kCLOCK_Lpi2cMux, DEMO_LPI2C_CLOCK_SOURCE_SELECT);
  CLOCK_SetDiv(kCLOCK_Lpi2cDiv, DEMO_LPI2C_CLOCK_SOURCE_DIVIDER);
  BOARD_Codec_I2C_Init();
  
  
  SAI1_Init();

  /*Enable MCLK clock*/
  BOARD_EnableSaiMclkOutput(true);
  
    
    
  
  /* 打印系统时钟 */
  PRINTF("\r\n");
  PRINTF("*****欢迎使用 野火i.MX RT1052 开发板*****\r\n");
  PRINTF("CPU:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_CpuClk));
  PRINTF("AHB:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_AhbClk));
  PRINTF("SEMC:            %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SemcClk));
  PRINTF("SYSPLL:          %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllClk));
  PRINTF("SYSPLLPFD0:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd0Clk));
  PRINTF("SYSPLLPFD1:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd1Clk));
  PRINTF("SYSPLLPFD2:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd2Clk));
  PRINTF("SYSPLLPFD3:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd3Clk));
  

   SAI1_DMAConfig();
  
  /*挂载文件系统*/
   f_mount_test(&g_fileSystem);
   
   istxFinished = 0;
       while(1)
    {
      /*打开文件*/
      error = f_open(&g_fileObject, _T("/record/22.wav"), (FA_READ));
      if (error == 0)
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

