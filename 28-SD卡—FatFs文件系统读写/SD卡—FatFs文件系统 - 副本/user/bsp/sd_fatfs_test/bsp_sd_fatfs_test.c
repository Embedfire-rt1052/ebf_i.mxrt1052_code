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
#include "./bsp/sd_fatfs_test/bsp_sd_fatfs_test.h"
#include "fsl_sdmmc_host.h"


#include "ff.h"
#include "diskio.h"
#include "fsl_sd_disk.h"


extern  FATFS g_fileSystem; /* File system object */
extern  FIL g_fileObject;   /* File object */


void f_mount_test(void)
{
  FRESULT error;
  const TCHAR driverNumberBuffer[3U] = {SDDISK + '0', ':', '/'};
  error = f_mount(&g_fileSystem, driverNumberBuffer, 0U);
  if (error)
  {
    /*错误类型判断*/
    if(error == FR_NO_FILESYSTEM)//还没有文件系统
    {
      PRINTF("SD卡还没有文件系统，创建文件系统将会格式化您的SD卡。\r\n 确定是否继续？\r\n");
      PRINTF("输入‘y’确定格式化，输入‘n’取消\r\n");
      while(true)
      {
        ch = GETCHAR();
        PUTCHAR(ch);
        if(ch =='y'|| ch == 'Y')
        {
          /*为SD卡创建文件系统*/
#if FF_USE_MKFS
          PRINTF("\r\n制作文件系统...... SD卡容量越大，该过程持续时间越长。\r\n");
          if (f_mkfs(driverNumberBuffer, FM_ANY, 0U, work, sizeof work))
          {
            PRINTF("知足文件系统失败.\r\n");
            return -1;
          }
          else
          {
            PRINTF("制作文件系统成功.\r\n");
          }
#endif /* FF_USE_MKFS */
        }
        else if(ch =='n'|| ch == 'N')
        {
          /*退出函数*/
          return -1;
        }
        
        PRINTF("输入‘y’确定格式化，输入‘n’取消\r\n");
      }  
    }
    else//其他错误，暂时不处理，直接退出函数
    {
      PRINTF("挂载文件系统失败\r\n");
      return -1;
    }

  }
  else 
  {
    PRINTF("Mount volume success.\r\n");
  }
    
}



/****************************END OF FILE**********************/
