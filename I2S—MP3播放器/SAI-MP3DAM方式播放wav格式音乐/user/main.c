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

/*sai dma 接收、发送句柄*/
extern sai_edma_handle_t txHandle;
extern sai_edma_handle_t rxHandle;

/*音乐缓冲区，四字节对齐*/
__attribute__((section("NonCacheable.init")))
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t audioBuff[BUFFER_SIZE], 4);
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t audioBuff2[BUFFER_SIZE], 4);
/*定义缓冲区标志位*/
volatile bool buffer1_full = false;
volatile bool buffer2_full = false;

/*发送、接收状态*/
volatile bool istxFinished = false;
volatile bool isrxFinished = false;

/*执行双缓冲读取函数*/
void double_buffer(void);

/*文件系统描述结构体*/
FATFS g_fileSystem; /* File system object */
FIL g_fileObject;
volatile bool file_read_finished = false;

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
  /* 初始化内存保护单元 */
  BOARD_ConfigMPU();
  /* 初始化开发板引脚 */
  BOARD_InitPins();
  /* 初始化开发板时钟 */
  BOARD_BootClockRUN();
  /* 初始化调试串口 */
  BOARD_InitDebugConsole();

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

  /*初始化SAI*/
  SAI1_IIC_init();
  SAI1_Init();
  SAI1_DMAConfig();

  /*挂载文件系统*/
  f_mount_test(&g_fileSystem);

  while (1)
  {
    /*打开文件*/
    error = f_open(&g_fileObject, _T("/record/ctl.wav"), (FA_READ));
    if (error == 0)
    {
      PRINTF("打开文件成功 \r\n");
      file_read_finished = false;
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

    /*播放音乐测试*/
    double_buffer();
    PRINTF("music play finished \r\n");
    while(1);
  }
}

/*执行双缓冲读取*/
void double_buffer(void)
{
  int error = -1;
  int bytesRead = 0;

  sai_transfer_t xfer_buffer1 = {0};
  sai_transfer_t xfer_buffer2 = {0};

  error = f_read(&g_fileObject, audioBuff, BUFFER_SIZE, (UINT *)&bytesRead);
  if (error)
  {
    PRINTF("read error %d\r\n", error);
    while (1)
      ;
  }
  else
  {
    xfer_buffer1.data = audioBuff;
    xfer_buffer1.dataSize = bytesRead;
    buffer1_full = true;
  }

  while (1)
  {

    if (buffer1_full)
    {
      istxFinished = false;

      /*执行发送buffer1*/
      SAI_TransferSendEDMA(DEMO_SAI, &txHandle, &xfer_buffer1);
      while (istxFinished == false)
      {
        /*buffer2为空，读数据到buffer2*/
        if (!buffer2_full)
        {
          error = f_read(&g_fileObject, audioBuff2, BUFFER_SIZE, (UINT *)&bytesRead);
          if (error)
          {
            PRINTF("read error %d\r\n", error);
            while (1)
              ;
          }
          else
          {
            xfer_buffer2.data = audioBuff2;
            xfer_buffer2.dataSize = bytesRead;
            if (bytesRead != BUFFER_SIZE)
            {
              file_read_finished = true;
            }

            /*设置buffer2满标志*/
            buffer2_full = true;
          }
        }
      }
      buffer1_full = false;
    }


    /*buffer2满，发送buffer2中的内容*/
    if (buffer2_full)
    {
      istxFinished = false;

      /*执行buffer2发送*/
      SAI_TransferSendEDMA(DEMO_SAI, &txHandle, &xfer_buffer2);
      
      /*等待发送完成*/
      while (istxFinished == false)
      {
        /*buffer1为空，读数据到buffer1*/
        if (!buffer1_full)
        {
          error = f_read(&g_fileObject, audioBuff, BUFFER_SIZE, (UINT *)&bytesRead);
          if (error)
          {
            PRINTF("read error %d\r\n", error);
            while (1)
              ;
          }
          else
          {
            xfer_buffer1.data = audioBuff;
            xfer_buffer1.dataSize = bytesRead;
            if (bytesRead != BUFFER_SIZE)
            {
              file_read_finished = true;
            }
            buffer1_full = true;
          }
        }
      }
      buffer2_full = false;
    }

    /*文件读写结束，清除缓存*/
    if(file_read_finished && (!buffer1_full) &&(!buffer2_full))
    {
      if (buffer1_full)
      {
        istxFinished = false;
        SAI_TransferSendEDMA(DEMO_SAI, &txHandle, &xfer_buffer1);
        while (istxFinished == false)
          ;
      }
      if (buffer2_full)
      {
        istxFinished = false;
        SAI_TransferSendEDMA(DEMO_SAI, &txHandle, &xfer_buffer2);
        while (istxFinished == false)
          ;
      }

      error = f_close(&g_fileObject);
      if (error)
      {
        PRINTF("close file filed  %d\r\n");
        while (1)
          ;
      }
      /*文件读取结束并且成功关闭文件*/
      PRINTF("file read finished  %d\r\n");
      break;
    }
  }
}

///****************************END OF FILE**********************/
