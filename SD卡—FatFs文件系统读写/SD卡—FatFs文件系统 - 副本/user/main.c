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

#define BUFFER_SIZE (100U)

/*Card结构描述符*/
extern sd_card_t g_sd;

/* @brief decription about the read/write buffer
* The size of the read/write buffer should be a multiple of 512, since SDHC/SDXC card uses 512-byte fixed
* block length and this driver example is enabled with a SDHC/SDXC card.If you are using a SDSC card, you
* can define the block length by yourself if the card supports partial access.
* The address of the read/write buffer should align to the specific DMA data buffer address align value if
* DMA transfer is used, otherwise the buffer address is not important.
* At the same time buffer address/size should be aligned to the cache line size if cache is supported.
*/
SDK_ALIGN(uint8_t g_bufferWrite[SDK_SIZEALIGN(BUFFER_SIZE, SDMMC_DATA_BUFFER_ALIGN_CACHE)],
          MAX(SDMMC_DATA_BUFFER_ALIGN_CACHE, SDMMCHOST_DMA_BUFFER_ADDR_ALIGN));
SDK_ALIGN(uint8_t g_bufferRead[SDK_SIZEALIGN(BUFFER_SIZE, SDMMC_DATA_BUFFER_ALIGN_CACHE)],
          MAX(SDMMC_DATA_BUFFER_ALIGN_CACHE, SDMMCHOST_DMA_BUFFER_ADDR_ALIGN));









/**
 * @brief 延时一段时间
 */
void delay(uint32_t count);

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



 FATFS g_fileSystem; /* File system object */
 FIL g_fileObject;   /* File object */


/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
  FRESULT error;
  DIR directory; /* Directory object */
  FILINFO fileInformation;
  UINT bytesWritten;
  UINT bytesRead;
  const TCHAR driverNumberBuffer[3U] = {SDDISK + '0', ':', '/'};
  volatile bool failedFlag = false;
  char ch = '0';
  BYTE work[FF_MAX_SS];
  
  
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
  

  /*挂载文件系统*/
  //FR_NO_FILESYSTEM
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
  
  
  /*判断是否允许使用相对路径*/
#if (FF_FS_RPATH >= 2U)
    error = f_chdrive((char const *)&driverNumberBuffer[0U]);
    if (error)
    {
        PRINTF("Change drive failed.\r\n");
        return -1;
    }
#endif
    


  /*---------------------------创建 "/dir_1"目录-------------------------------*/
  PRINTF("\r\n 创建目录 “/dir_1”......\r\n");
  error = f_mkdir(_T("/dir_1"));
  if (error)
  {
    if (error == FR_EXIST)
    {
      PRINTF("目录已经存在\r\n");
    }
    else
    {
      PRINTF("创建目录失败.\r\n");
      return -1;
    }
  }
  else
  {
    PRINTF("创建目录成功\r\n");
  }
  
  /*-------------------------在"/dir_1"目录里创建一个文件------------*/
  PRINTF("\r\n在“/dir_1”目录下创建一个“f_1.txt”文件......\r\n");
  error = f_open(&g_fileObject, _T("/dir_1/f_1.txt"), (FA_WRITE | FA_READ | FA_CREATE_ALWAYS));
  if (error)
  {
    if (error == FR_EXIST)
    {
      PRINTF("文件已经存在.\r\n");
    }
    else
    {
      PRINTF("创建文件失败\r\n");
      return -1;
    }
  }
  else
  {
    PRINTF("创建文件成功...... \r\n");
  }
  
  
//      if (f_close(&g_fileObject))
//    {
//        PRINTF("\r\nClose file failed.\r\n");
//        return -1;
//    }
//    else
//    {
//      PRINTF("\r\nClose file success.\r\n");
//    }
  
  /*-------------------在"/dir_1"目录里创建一个目录------------------------*/
  PRINTF("\r\n在“/dir_1”目录下创建一个“dir_2”文件夹......\r\n");
  error = f_mkdir(_T("/dir_1/dir_2"));
  if (error)
  {
    if (error == FR_EXIST)
    {
      PRINTF("文件夹已存在.\r\n");
    }
    else
    {
      PRINTF("创建“dir_2”文件夹失败\r\n");
      return -1;
    }
  }
  else
  {
    PRINTF("创建“dir_2”文件夹成功......\r\n");
  }
  
  /*---------------------输出"/dir_1"目录下的文件与文件-------------------*/
  PRINTF("\r\n列出“/dir_1”目录下的内容......\r\n");
  if (f_opendir(&directory, "/dir_1"))
  {
    PRINTF("打开路径失败\r\n");
    return -1;
  }
  
  for (;;)
  {
    error = f_readdir(&directory, &fileInformation);
    
    /* To the end. */
    if ((error != FR_OK) || (fileInformation.fname[0U] == 0U))
    {
      break;
    }
    if (fileInformation.fname[0] == '.')
    {
      continue;
    }
    if (fileInformation.fattrib & AM_DIR)
    {
      PRINTF("文件 : %s.\r\n", fileInformation.fname);
    }
    else
    {
      PRINTF("文件夹 : %s.\r\n", fileInformation.fname);
    }
  }
  
  /*初始化数据缓冲区，为文件的读写做准备*/
  memset(g_bufferWrite, 'a', sizeof(g_bufferWrite));
  g_bufferWrite[BUFFER_SIZE - 2U] = '\r';
  g_bufferWrite[BUFFER_SIZE - 1U] = '\n';
  
  PRINTF("\r\n  开始文件读写测试......  \r\n");
  
  
  
   while (true)
    {
        if (failedFlag || (ch == 'q'))
        {
            break;
        }
        /* 向 "/dir_1/f_1.dat" 文件内写入g_bufferWrite缓冲区的内容 */
        PRINTF("\r\n写入内容到“/dir_1/f_1.txt”文件\r\n");
        error = f_write(&g_fileObject, g_bufferWrite, sizeof(g_bufferWrite), &bytesWritten);
        if ((error) || (bytesWritten != sizeof(g_bufferWrite)))
        {
            PRINTF("写入文件失败. \r\n");
            failedFlag = true;
            continue;
        }
        else
        {
          PRINTF("写入文件成功 \r\n");
        }

        /* 移动文件读写指针到文件开始处 */
        if (f_lseek(&g_fileObject, 0U))
        {
            PRINTF("设置文件读写指针失败 \r\n");
            failedFlag = true;
            continue;
        }
        else
        {
          PRINTF("设置文件读写指针成功 \r\n");
        }
        
        /*读取"/dir_1/f_1.dat"文件的内容到 g_bufferRead 缓冲区*/
        PRINTF("读取“/dir_1/f_1.txt”文件的内容\r\n");
        memset(g_bufferRead, 0U, sizeof(g_bufferRead));
        error = f_read(&g_fileObject, g_bufferRead, sizeof(g_bufferRead), &bytesRead);
        if ((error) || (bytesRead != sizeof(g_bufferRead)))
        {
            PRINTF("读取文件失败 \r\n");
            failedFlag = true;
            continue;
        }
        else
        {
            PRINTF("读取文件成功. \r\n");
        }
        
        
        /*比较读写内容是否一致*/
        PRINTF("比较读写内容......\r\n");
        if (memcmp(g_bufferWrite, g_bufferRead, sizeof(g_bufferWrite)))
        {
            PRINTF("文件读写内容不一致\r\n");
            failedFlag = true;
            continue;
        }
        else
        {
          PRINTF("文件读写内容一致\r\n");
        }
        
        
        PRINTF("\r\n输入 'q' 退出读写测试.\r\n输入其他字符，再次进行读写测试\r\n");
        ch = GETCHAR();
        PUTCHAR(ch);
    }
    PRINTF("\r\n关闭文件，并结束测试.\r\n");

    if (f_close(&g_fileObject))
    {
        PRINTF("\r\nClose file failed.\r\n");
        return -1;
    }
    else
    {
      PRINTF("\r\nClose file success.\r\n");
    }
  
    while(1);		
}

//void 



/****************************END OF FILE**********************/


/************************第一部分*************************/
