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
    


extern codec_config_t boardCodecConfig;




extern sai_transfer_format_t format;
extern sai_edma_handle_t txHandle;
extern sai_edma_handle_t rxHandle;

extern codec_config_t boardCodecConfig;


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







//void RecordPlayback(I2S_Type *base, uint32_t time_s);






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
  
  
  
  /************移植新增内容*******************/
  
  edma_config_t dmaConfig = {0};

  /* 初始化内存保护单元 */      
  BOARD_ConfigMPU();
  /* 初始化开发板引脚 */
  BOARD_InitPins();
  /* 初始化开发板时钟 */
  BOARD_BootClockRUN();
  /* 初始化调试串口 */
  BOARD_InitDebugConsole();
  
  
  
  SAI1_Init();
  
  /*******初始化音频相关时钟****************/
  


  /*Enable MCLK clock*/
  BOARD_EnableSaiMclkOutput(true);
  
  BOARD_Codec_I2C_Init();
    
    
  
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
  

  
  /*初始化部分*/
  
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
          delay(2000000);
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

