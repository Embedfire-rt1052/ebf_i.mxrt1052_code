/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   Cache-数据一致性实验
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
#include "./led/bsp_led.h"
#include "bsp_cache.h"

/*数据缓冲区长度*/
#define MEM_DMATRANSFER_LEN 10U * FSL_FEATURE_L1DCACHE_LINESIZE_BYTE
/* DMA 传输超时时间 */
#define DMA_TRANSFER_TIMEOUT 0xFFFFU

volatile bool g_Transfer_Done;                                 //定义传输完成标志
AT_NONCACHEABLE_SECTION(uint8_t g_data[MEM_DMATRANSFER_LEN]);  //定义数据缓冲区，
uint32_t g_count = 0;                                          //用于DAM传输等待计算


/*******************************************************************
 * Prototypes
 *******************************************************************/
/**
 * @brief 延时一段时间
 */
void delay(uint32_t count);

/*******************************************************************
 * Code
 *******************************************************************/
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

void InvalidateDCache_test(void); //缓存无效化测试函数
void CleanDCache_test(void);      //缓存清理测试函数

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
  /* 初始化内存保护单元*/
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

  /*开启Cache 并初始化DMA */
  Cache_DMA_Config();

//  /*缓存清理测试*/
//  CleanDCache_test();

  /*缓存无效化测试*/
  InvalidateDCache_test();

  while (1)
    ;
}



/*缓存清除*/
void CleanDCache_test(void)
{
  uint32_t count;              //用于循环技术
  volatile uint32_t readDummy; //仅用于读，无实际意义
  uint32_t startAddr;                                            //保存本实验使用的内存起始地址

  startAddr = APP_MemoryInit();
  /*关闭Cache*/
  APP_CacheConfig(false);

  /*使用初始化缓冲区g_data和startAddr内存空间*/
  for (count = 0; count < MEM_DMATRANSFER_LEN; count++)
  {
    g_data[count] = 0xaa;
    *(uint8_t *)(startAddr + count) = 0xbb;
  }

  /*开启Cache 并初始化DMA */
  APP_CacheConfig(true);


  /*第一次从 从内存中(startAddr)读数据，执行之后Cache中会保留数据备份*/
  for (count = 0; count < MEM_DMATRANSFER_LEN; count++)
  {
    /* Access the memory first. */
    readDummy = *(uint8_t *)(startAddr + count);
    (void)readDummy;
  }

  /* 通过CPU传输数据,  */
  for (count = 0; count < MEM_DMATRANSFER_LEN; count++)
  {
    *(uint8_t *)(startAddr + count) = 0xff;
  }

  /* Get the real data in the memory . */
  APP_DMAMem2memTransfer((void *)startAddr, sizeof(g_data[0]), &g_data[0], sizeof(g_data[0]), sizeof(g_data));

  /* Wait for EDMA transfer finished. */
  while ((g_Transfer_Done != true) && (g_count < DMA_TRANSFER_TIMEOUT))
  {
    g_count++;
  }

  if (g_count != DMA_TRANSFER_TIMEOUT)
  {

    if (memcmp((void *)&g_data[0], (void *)startAddr, MEM_DMATRANSFER_LEN) != 0)
    {

      /*输出实际内存中的数据，DMA读取到的才是内存中的实际内容*/
      PRINTF("\r\n 执行缓存清理之前内存中的数据如下：\r\n");
      for (count = 0; count < MEM_DMATRANSFER_LEN; count++)
      {
        PRINTF("0X%02x, ", g_data[count]);
      }

      /*执行内存清理*/
      L1CACHE_CleanDCacheByRange(startAddr, MEM_DMATRANSFER_LEN);

      /* Transfer from the sdram to data[]. */
      APP_DMAMem2memTransfer((void *)startAddr, sizeof(g_data[0]), &g_data[0], sizeof(g_data[0]),
                             sizeof(g_data));

      /* Wait for EDMA transfer finished. */
      while ((g_Transfer_Done != true) && (g_count < DMA_TRANSFER_TIMEOUT))
      {
        g_count++;
      }
      /*传输完成*/
      if (g_count != DMA_TRANSFER_TIMEOUT)
      {
        if (memcmp((void *)&g_data[0], (void *)startAddr, MEM_DMATRANSFER_LEN) == 0)
        {
          /*输出实际内存中的数据，DMA读取到的才是内存中的实际内容*/
          PRINTF("\r\n 执行缓存清理之后内存中的数据如下：\r\n");
          for (count = 0; count < MEM_DMATRANSFER_LEN; count++)
          {
            PRINTF("0X%02x, ", g_data[count]);
          }
          PRINTF("\r\n 实验完成\r\n");
        }
      }
    }
  }
}



/*缓存无效化测试函数,，*/
void InvalidateDCache_test(void)
{
  uint32_t count;              //用于循环技术
  volatile uint32_t readDummy; //仅用于读，无实际意义
  uint32_t startAddr;                                            //保存本实验使用的内存起始地址

  startAddr = APP_MemoryInit();
  /*关闭Cache*/
  APP_CacheConfig(false);

  /*使用初始化缓冲区g_data和startAddr内存空间*/
  for (count = 0; count < MEM_DMATRANSFER_LEN; count++)
  {
    g_data[count] = 0xaa;
    *(uint8_t *)(startAddr + count) = 0xbb;
  }
  
  /*开启Cache 并初始化DMA */
  APP_CacheConfig(true);

  /*第一次从 从内存中(startAddr)读数据，执行之后Cache中会保留数据备份*/
  for (count = 0; count < MEM_DMATRANSFER_LEN; count++)
  {
    /* Access the memory first. */
    readDummy = *(uint8_t *)(startAddr + count);
    (void)readDummy;
  }

  /*使用DMA修改内存的数据*/
  /* 使用DMA 更新(startAddr) 中的数据,此时Cache不知道更新了*/
  APP_DMAMem2memTransfer(&g_data[0], sizeof(g_data[0]), (void *)startAddr, sizeof(g_data[0]), sizeof(g_data));

  /*等待 EDMA 传输完成*/
  while ((g_Transfer_Done != true) && (g_count < DMA_TRANSFER_TIMEOUT))
  {
    g_count++;
  }

  /*等待传输完成*/
  if (g_count != DMA_TRANSFER_TIMEOUT)
  {
    PRINTF("\r\n eDAM 存储器到存储器传输完成\r\n");
    /*比较第一次使用dma 写入的数据与原数据是否一致*/
    if (memcmp((void *)&g_data[0], (void *)startAddr, MEM_DMATRANSFER_LEN) != 0)
    {
      PRINTF("\r\n 执行缓存无效化之前使用CPU读取得到的数据如下：\r\n");
      for (count = 0; count < MEM_DMATRANSFER_LEN; count++)
      {
        PRINTF("0X%02x, ", *(uint8_t *)(startAddr + count));
      }

      /*不一致，执行缓存无效化，*/
      L1CACHE_InvalidateDCacheByRange(startAddr, MEM_DMATRANSFER_LEN);

      /* 执行缓存无效化后再次使用CPU读取内存中的数据 */
      if (memcmp((void *)&g_data[0], (void *)startAddr, MEM_DMATRANSFER_LEN) == 0)
      {
        /*数据一致*/
        PRINTF("\r\n 执行缓存无效化之后使用CPU读取得到的数据如下：\r\n");
        for (count = 0; count < MEM_DMATRANSFER_LEN; count++)
        {
          PRINTF("0X%02x, ", *(uint8_t *)(startAddr + count));
        }
        // invalidateResult = true;
        PRINTF("\r\n 实验完成\r\n");
      }
    }
    else
    {
      /*测试失败，没有出现数据一致性问题*/
      PRINTF("\r\n 测试失败，没有出现数据一致性问题！\r\n");
    }
  }
  else
  {
    /* DMA传输错误，测试失败 */
    PRINTF("\r\n DMA传输错误，测试失败！\r\n");
  }
}



/****************************END OF FILE**********************/
