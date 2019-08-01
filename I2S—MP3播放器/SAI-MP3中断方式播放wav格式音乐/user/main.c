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

#include "fsl_lpi2c.h"
#include "fsl_wm8960.h"
#include "music.h"

#include "sai.h"

#define MUSIC_LEN (3000)

/*******************************************************************************
 * Definitions
 ******************************************************************************/

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

void delay(uint32_t count)
{
  volatile uint32_t i = 0;
  for (i = 0; i < count; ++i)
  {
    __asm("NOP"); /* 调用nop空指令 */
  }
}

AT_NONCACHEABLE_SECTION(FATFS g_fileSystem); /* File system object */
AT_NONCACHEABLE_SECTION(FIL g_fileObject);   /* File object */

volatile int section_1 = 0;
volatile int section_2 = 0;
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{

  int error = -1; //错误返回值
  int bytesRead;  //文件读取得到的数据量

  int value = 0;

  /************移植新增内容*******************/

  sai_transfer_t xfer;
  uint32_t temp = 0;
  uint32_t delayCycle = 500000;

  /* 初始化内存保护单元 */
  BOARD_ConfigMPU();
  /* 初始化开发板引脚 */
  BOARD_InitPins();
  /* 初始化开发板时钟 */
  BOARD_BootClockRUN();
  /* 初始化调试串口*/
  BOARD_InitDebugConsole();

  BOARD_ConfigMPU();
  BOARD_InitPins();
  BOARD_BootClockRUN();
  BOARD_InitDebugConsole();
  /*******初始化音频相关时钟****************/
  sai_init();

  /*挂载文件系统*/
  f_mount_test(&g_fileSystem);

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

  while (delayCycle)
  {
    __ASM("nop");
    delayCycle--;
  }

  do
  {
    if (section_1 == 0)
    {
      error = f_read(&g_fileObject, music1, MUSIC_LEN, &bytesRead);
      if ((error) || (MUSIC_LEN != bytesRead))
      {
        PRINTF("read error %d\r\n", error);
        while (1)
          ;
      }
      section_1 = 1;
    }
    else if (section_2 == 0)
    {
      /* code */
      error = f_read(&g_fileObject, music2, MUSIC_LEN, &bytesRead);
      if ((error) || (MUSIC_LEN != bytesRead))
      {
        PRINTF("read error %d\r\n", error);
        while (1)
          ;
      }
      section_2 = 1;
    }
  } while (0);

  //
  //  WM8960_GetVolume(codec_handle_t *handle, wm8960_module_t module);
  /*循环读取文件，并写入*/
  while (1)
  {
    do
    {
      /* music1缓冲区读完成 */
      if (section_1 == 1)
      {
        /*执行播放，*/
        temp = (uint32_t)music1;
        xfer.data = (uint8_t *)temp;
        xfer.dataSize = MUSIC_LEN;
        SAI_TransferSendNonBlocking(DEMO_SAI, &txHandle, &xfer);
        
        /*判断music2 是否为空，如果为空则读取SD卡数据到music2*/
        if (section_2 == 0)
        {
          error = f_read(&g_fileObject, music2, MUSIC_LEN, &bytesRead);
          if ((error) || (MUSIC_LEN != bytesRead))
          {
            PRINTF("read error %d\r\n", error);
            while (1)
              ;
          }
          section_2 = 1;
        }
        /* Wait until finished */
        while (isFinished != true)
        {
        }
        isFinished = false;
        section_1 = 0;
      }
      else if (section_2 == 1)  //music2缓冲区读完成
      {
        /* 执行播放 */
        temp = (uint32_t)music2;
        xfer.data = (uint8_t *)temp;
        xfer.dataSize = MUSIC_LEN;
        SAI_TransferSendNonBlocking(DEMO_SAI, &txHandle, &xfer);
        
        /*判断music1 是否为空，如果为空则读取SD卡数据到music2*/
        if (section_1 == 0)
        {
          error = f_read(&g_fileObject, music1, MUSIC_LEN, &bytesRead);
          if ((error) || (MUSIC_LEN != bytesRead))
          {
            PRINTF("read error %d\r\n", error);
            while (1);
          }
          section_1 = 1;
        }
        /* Wait until finished */
        while (isFinished != true)
        {
        }
        isFinished = false;
        section_2 = 0;
      }
    } while (0);

  }
}
