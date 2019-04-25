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
    
    
#define BUFFER_SIZE (100U)

/*Card结构描述符*/
extern sd_card_t g_sd;
 FATFS g_fileSystem; /* File system object */
 
 

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



/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
  
//  FRESULT error = FR_OK;
//  char ch = '0';
//  BYTE work[FF_MAX_SS];
//  const TCHAR driverNumberBuffer[3U] = {SDDISK + '0', ':', '/'};
  
  
  volatile FIL file_object;   //定义文件描述符，
  volatile DIR dir_object;    //目录对象结构体
  volatile FILINFO file_info; //文件信息描述结构体

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
  
  f_mount_test(&g_fileSystem);
////  f_mount_test(&FATFS_fileSystem);
//  error = f_mount(&g_fileSystem, driverNumberBuffer, 0U);
//  if (error)
//  {
//    /*错误类型判断*/
//    if(error == FR_NO_FILESYSTEM)//还没有文件系统
//    {
//      PRINTF("SD卡还没有文件系统，创建文件系统将会格式化您的SD卡。\r\n 确定是否继续？\r\n");
//      PRINTF("输入‘y’确定格式化，输入‘n’取消\r\n");
//      while(true)
//      {
//        ch = GETCHAR();
//        PUTCHAR(ch);
//        if(ch =='y'|| ch == 'Y')
//        {
//          /*为SD卡创建文件系统*/
//#if FF_USE_MKFS
//          PRINTF("\r\n制作文件系统...... SD卡容量越大，该过程持续时间越长。\r\n");
//          if (f_mkfs(driverNumberBuffer, FM_ANY, 0U, work, sizeof work))
//          {
//            PRINTF("知足文件系统失败.\r\n");
//            return -1;
//            
//          }
//          else
//          {
//            PRINTF("制作文件系统成功.\r\n");
//          }
//#endif /* FF_USE_MKFS */
//        }
//        else if(ch =='n'|| ch == 'N')
//        {
//          /*程序停止*/
//          return -1;
//        }
//        
//        PRINTF("输入‘y’确定格式化，输入‘n’取消\r\n");
//      }  
//    }
//    else//其他错误，暂时不处理，直接退出函数
//    {
//      PRINTF("挂载文件系统失败\r\n");
//      return -1;
//    }
//  }
//  else 
//  {
//    PRINTF("Mount volume success.\r\n");
//  }
//  /*判断是否允许使用相对路径*/
//#if (FF_FS_RPATH >= 2U)
//  error = f_chdrive((char const *)&driverNumberBuffer[0U]);
//  if (error)
//  {
//    PRINTF("Change drive failed.\r\n");
//    return -1;
//  }
//  else
//  {
//    PRINTF("Change drive success.\r\n");
//  }
//#endif
  
  /*在SD卡根目录创建一个目录*/
  f_mkdir_test("/dir_1");
  
  /*创建“/dir_1/f_1.txt”*/
  f_touch_test("/dir_1/he.txt"); 
  
  /*打开文件*/
  f_open_test("/dir_1/he.txt",&file_object);
  
   /*关闭文件*/
  f_close_test(&file_object);
  
  /*创建目录*/
  f_mkdir_test("/dir_1/dir_2");
  
  /*获取路径下的文件也文件夹*/
  f_readdir_test("/dir_1",&dir_object,&file_info);
 
 /*初始化数据缓冲区，为文件的读写做准备*/
  memset(g_bufferWrite, 'a', sizeof(g_bufferWrite));
  g_bufferWrite[BUFFER_SIZE - 2U] = '\r';
  g_bufferWrite[BUFFER_SIZE - 1U] = '\n';
  
  PRINTF("\r\n开始文件读写测试......  \r\n");
  
  f_write_read_test("/dir15/he.txt", g_bufferWrite, g_bufferRead);  
  
  while (true)
  {
    
  }
}




  


/****************************END OF FILE**********************/

typedef enum {
  FR_OK = 0,	  /* (0) 成功*/
  FR_DISK_ERR,	  /* (1) 硬件错误，错误发生功能在底层驱动代码 */
  FR_INT_ERR,	  /* (2) 断言失败 */
  FR_NOT_READY,   /* (3) 物理设备不工作 */
  FR_NO_FILE,     /* (4) 找不到文件 */
  FR_NO_PATH,	  /* (5) 找不到文件 */
  FR_INVALID_NAME,	/* (6) 路径名格式无效 */
  FR_DENIED,		/* (7) 由于禁止访问或目录已满导致访问失败 */
  FR_EXIST,		/* (8) 由于禁止访问导致访问失败 */
  FR_INVALID_OBJECT,	/* (9) 文件/目录对象无效 */
  FR_WRITE_PROTECTED,	/* (10) 物理驱动器受写保护 */
  FR_INVALID_DRIVE,	/* (11) 逻辑驱动器号无效 */
  FR_NOT_ENABLED,		/* (12) 该卷没有工作区 */
  FR_NO_FILESYSTEM,/* (13) 没有找到FatFs文件系统*/
  FR_MKFS_ABORTED,/* (14) 执行 f_mkfs()函数过程中由于某些原因异常终止 */
  FR_TIMEOUT,	/* (15) 等待超时错误 */
  FR_LOCKED,	/* (16) 访问了受保护的文件或目录 */
  FR_NOT_ENOUGH_CORE,/* (17) 无法分配LFN工作缓冲区 */
  FR_TOO_MANY_OPEN_FILES,/* (18) 打开文件数量大于 > FF_FS_LOCK宏定义的值 */
  FR_INVALID_PARAMETER	/* (19) 给定参数无效 */
} FRESULT;

