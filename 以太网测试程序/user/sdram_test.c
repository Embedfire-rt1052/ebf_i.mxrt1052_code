/**
  ******************************************************************
  * @file    sdram_test.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   对SDRAM进行读写测试的驱动
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
#include "./delay/core_delay.h"   

#define SDRAM_START_ADDRESS       (0x80000000U)
#define SDRAM_MEM_LENGTH          (32*1024*1024)

#define SEMC_EXAMPLE_DATALEN      (0x1000U)
#define SEMC_EXAMPLE_WRITETIMES   (1000U)


uint32_t sdram_writeBuffer[SEMC_EXAMPLE_DATALEN];
uint32_t sdram_readBuffer[SEMC_EXAMPLE_DATALEN];

/**
  * @brief  使用32位指针进行访问SDRAM测试
  * @param  无
  * @retval 测试结果，true为正常，false为失败
  */
bool SEMC_SDRAMReadWrite32Bit(void)
{
    uint32_t index;
    uint32_t datalen = SEMC_EXAMPLE_DATALEN;
    /* 指向 SDRAM 测试的起始地址
       对该指针指向的地址赋值会触发向SDRAM的写入 
       对该指针指向的地址读取会触发从SDRAM的读取 */
    uint32_t *sdram = (uint32_t *)SDRAM_START_ADDRESS; 
    bool result = true;

    PRINTF("\r\n 使用32位的方式向SDRAM写入, 起始地址：0x%x, 写入长度：%d \r\n", 
           sdram, datalen);
    
    /* 向SDRAM写入数据 */
    for (index = 0; index < datalen; index++)
    {
        sdram_writeBuffer[index] = index;
        sdram[index] = sdram_writeBuffer[index];
    }

    PRINTF("\r\n 使用32位的方式向读取SDRAM, 起始地址：0x%x, 写入长度：%d \r\n", 
           sdram, datalen);
    
    /* 从SDRAM中读取数据 */
    for (index = 0; index < datalen; index++)
    {
        sdram_readBuffer[index] = sdram[index];
    }

    PRINTF("\r\n 开始比较读写的内容...\r\n");
    /* 比较两个缓冲区的内容 */
    while (datalen--)
    {
        if (sdram_writeBuffer[datalen] != sdram_readBuffer[datalen])
        {
            result = false;
            break;
        }
    }

    if (!result)
    {
        PRINTF("\r\n 错误！使用32位读写SDRAM的比较结果不一致\r\n");
    }
    else
    {
        PRINTF("\r\n 成功！使用32位读写SDRAM的比较结果一致\r\n");
    }
    
    return result;
}

/**
  * @brief  使用16位指针进行访问SDRAM测试
  * @param  无
  * @retval 测试结果，true为正常，false为失败
  */
static bool SEMC_SDRAMReadWrite16Bit(void)
{
    uint32_t index;
    uint32_t datalen = SEMC_EXAMPLE_DATALEN;
    /* 指向 SDRAM 测试的起始地址
     对该指针指向的地址赋值会触发向SDRAM的写入 
     对该指针指向的地址读取会触发从SDRAM的读取 */
    uint16_t *sdram = (uint16_t *)SDRAM_START_ADDRESS; 
    bool result = true;

    PRINTF("\r\n 使用16位的方式向SDRAM写入, 起始地址：0x%x, 写入长度：%d \r\n", 
           sdram, datalen);
  
    memset(sdram_writeBuffer, 0, sizeof(sdram_writeBuffer));
    memset(sdram_readBuffer, 0, sizeof(sdram_readBuffer));

    /* 向SDRAM写入数据 */
    for (index = 0; index < datalen; index++)
    {
        sdram_writeBuffer[index] = index % 0xFFFF;
        sdram[index] = sdram_writeBuffer[index];
    }

    PRINTF("\r\n 使用16位的方式向读取SDRAM, 起始地址：0x%x, 写入长度：%d \r\n", 
           sdram, datalen);

    
    /* 从SDRAM中读取数据 */
    for (index = 0; index < datalen; index++)
    {
        sdram_readBuffer[index] = sdram[index];
    }

    PRINTF("\r\n 开始比较读写的内容...\r\n");
    /* 比较两个缓冲区的内容 */
    while (datalen--)
    {
        if (sdram_writeBuffer[datalen] != sdram_readBuffer[datalen])
        {
            result = false;
            break;
        }
    }

    if (!result)
    {
        PRINTF("\r\n 错误！使用16位读写SDRAM的比较结果不一致!\r\n");
    }
    else
    {
        PRINTF("\r\n 成功！使用16位读写SDRAM的比较结果一致\r\n");
    }
    
    return result;
}

/**
  * @brief  使用8位指针进行访问SDRAM测试
  * @param  无
  * @retval 测试结果，true为正常，false为失败
  */
static bool SEMC_SDRAMReadWrite8Bit(void)
{
    uint32_t index;
    uint32_t datalen = SEMC_EXAMPLE_DATALEN;
    /* 指向 SDRAM 测试的起始地址
     对该指针指向的地址赋值会触发向SDRAM的写入 
     对该指针指向的地址读取会触发从SDRAM的读取 */
    uint8_t *sdram = (uint8_t *)SDRAM_START_ADDRESS; 
    bool result = true;

    PRINTF("\r\n 使用8位的方式向SDRAM写入, 起始地址：0x%x, 写入长度：%d \r\n", 
           sdram, datalen);

    memset(sdram_writeBuffer, 0, sizeof(sdram_writeBuffer));
    memset(sdram_readBuffer, 0, sizeof(sdram_readBuffer));

    /* 向SDRAM写入数据 */
    for (index = 0; index < datalen; index++)
    {
        sdram_writeBuffer[index] = index % 0x100;
        sdram[index] = sdram_writeBuffer[index];
    }

    PRINTF("\r\n 使用8位的方式向读取SDRAM, 起始地址：0x%x, 写入长度：%d \r\n", 
           sdram, datalen);
    
    /* 从SDRAM中读取数据 */
    for (index = 0; index < datalen; index++)
    {
        sdram_readBuffer[index] = sdram[index];
    }

    PRINTF("\r\n 开始比较读写的内容...\r\n");
    /* 比较两个缓冲区的内容 */
    while (datalen--)
    {
        if (sdram_writeBuffer[datalen] != sdram_readBuffer[datalen])
        {
            result = false;
            break;
        }
    }

    if (!result)
    {
        PRINTF("\r\n 错误！使用8位读写SDRAM的比较结果不一致!\r\n");
    }
    else
    {
        PRINTF("\r\n 成功！使用8位读写SDRAM的比较结果一致\r\n");
    }
   
    return result;
}

/**
  * @brief  以32、16、8位的方式对SDRAM进行读写测试
  * @param  无
  * @retval true：测试正常 false:测试失败
  */
bool SEMC_SDRAMReadWriteTest(void)
{
  bool result;
  
	/* 32Bit数据读写测试*/
	result = SEMC_SDRAMReadWrite32Bit();
  if(!result) return result;
  
	/* 16Bit数据读写测试 */
	result = SEMC_SDRAMReadWrite16Bit();
  if(!result) return result;

	/* 8Bit数据读写测试 */
	result = SEMC_SDRAMReadWrite8Bit();

  return result;
}


/**
  * @brief  整片SDRAM读写测试
  * @param  None
  * @retval 正常返回true，异常返回false
  */
bool SDRAM_FullChipTest(void)
{
  /*写入数据计数器*/
  uint32_t counter=0;
  
  /* 8位的数据 */
  uint8_t ubWritedata_8b = 0, ubReaddata_8b = 0;  
  
  /* 16位的数据 */
  uint16_t uhWritedata_16b = 0, uhReaddata_16b = 0; 
  
  PRINTF("\r\n 正在检测整片SDRAM，以8位、16位的方式读写整片SDRAM...\r\n");


  /*按8位格式读写数据，并校验*/
  
  /* 把SDRAM数据全部重置为0 ，SDRAM_MEM_LENGTH是以8位为单位的 */
  for (counter = 0x00; counter < SDRAM_MEM_LENGTH; counter++)
  {
    *(__IO uint8_t*) (SDRAM_START_ADDRESS + counter) = (uint8_t)0x0;
  }
  
  /* 向整个SDRAM写入数据  8位 */
  for (counter = 0; counter < SDRAM_MEM_LENGTH; counter++)
  {
    *(__IO uint8_t*) (SDRAM_START_ADDRESS + counter) = (uint8_t)(ubWritedata_8b + counter);
  }
  
  /* 读取 SDRAM 数据并检测*/
  for(counter = 0; counter<SDRAM_MEM_LENGTH;counter++ )
  {
		/* 从该地址读出数据 */
    ubReaddata_8b = *(__IO uint8_t*)(SDRAM_START_ADDRESS + counter);  
    
		/* 检测数据，若不相等，跳出函数,返回检测失败结果 */
    if(ubReaddata_8b != (uint8_t)(ubWritedata_8b + counter))      
    {
      PRINTF("\r\n 8位数据读写错误！\r\n");
      return false;
    }
  }	
  
  /*按16位格式读写数据，并检测*/  
  /* 把SDRAM数据全部重置为0 */
  for (counter = 0x00; counter < SDRAM_MEM_LENGTH/2; counter++)
  {
    *(__IO uint16_t*) (SDRAM_START_ADDRESS + 2*counter) = (uint16_t)0x00;
  }
  
  /* 向整个SDRAM写入数据  16位 */
  for (counter = 0; counter < SDRAM_MEM_LENGTH/2; counter++)
  {
    *(__IO uint16_t*) (SDRAM_START_ADDRESS + 2*counter) = (uint16_t)(uhWritedata_16b + counter);
  }
  
    /* 读取 SDRAM 数据并检测*/
  for(counter = 0; counter<SDRAM_MEM_LENGTH/2;counter++ )
  {
		/* 从该地址读出数据 */
    uhReaddata_16b = *(__IO uint16_t*)(SDRAM_START_ADDRESS + 2*counter);  
    
		/* 检测数据，若不相等，跳出函数,返回检测失败结果 */
    if(uhReaddata_16b != (uint16_t)(uhWritedata_16b + counter))      
    {
      PRINTF("\r\n 16位数据读写错误！\r\n");
      return false;
    }
  }  
  PRINTF("\r\n SDRAM整片读写测试正常！\r\n"); 

  return true;  
}

/****************************END OF FILE**********************/
