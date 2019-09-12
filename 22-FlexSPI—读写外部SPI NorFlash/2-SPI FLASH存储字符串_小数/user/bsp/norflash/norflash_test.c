/**
  ******************************************************************
  * @file    norflash_test.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   nor flash测试
  ******************************************************************
  * @attention
  *
  * 实验平台:野火  i.MXRT1052开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************
  */
#include "fsl_flexspi.h"

#include "fsl_debug_console.h"
#include "./norflash/bsp_norflash.h"  
#include "./delay/core_delay.h"
#include "./led/bsp_led.h"  


#define EXAMPLE_SECTOR      4096      /* 要进行读写测试的扇区号 */
#define EXAMPLE_SIZE        (4*1024)  /* 读写测试的数据量，单位为字节*/

const uint8_t DATA_VALID_FLAG = 0x5A;                     /* 数据有效的标志值 */
#define DATA_VALID_ADDR     (EXAMPLE_SECTOR*SECTOR_SIZE) /* 数据有效标志存储的地址 */

/* 字符串示例存储的地址 */
#define EXAMPLE_STRING_ADDR   (DATA_VALID_ADDR+FLASH_PAGE_SIZE*1) 
/* double数据示例存储的地址 */
#define EXAMPLE_DOUBLE_ADDR   (DATA_VALID_ADDR+FLASH_PAGE_SIZE*2) 
/* int数据示例存储的地址 */
#define EXAMPLE_INT_ADDR      (DATA_VALID_ADDR+FLASH_PAGE_SIZE*3) 

/* 读写测试使用的缓冲区 */
static uint8_t s_nor_program_buffer[EXAMPLE_SIZE];
static uint8_t s_nor_read_buffer[EXAMPLE_SIZE];

/**
* @brief  使用IP命令的方式进行读写测试
* @param  无
* @retval 测试结果，正常为0
*/
int NorFlash_IPCommand_Test(void)
{    
    uint32_t i = 0;
    status_t status;
    uint32_t JedecDeviceID = 0;

    PRINTF("\r\nNorFlash IP命令访问测试\r\n");

    /***************************读ID测试****************************/
    /* 获取JedecDevice ID. */
    FlexSPI_NorFlash_Get_JedecDevice_ID(FLEXSPI, &JedecDeviceID);
    
    if(JedecDeviceID != FLASH_WINBOND_JEDECDEVICE_ID && 
			 JedecDeviceID != FLASH_ISSI_JEDECDEVICE_ID)
    {
      RGB_RED_LED_ON;
      PRINTF("FLASH检测错误，读取到的JedecDeviceID值为: 0x%x\r\n", JedecDeviceID);
      return -1;
    }
    
    PRINTF("检测到FLASH芯片，JedecDeviceID值为: 0x%x\r\n", JedecDeviceID);    
   
    /***************************擦除测试****************************/
    PRINTF("擦除扇区测试\r\n");
    
    /* 擦除指定扇区 */
    status = FlexSPI_NorFlash_Erase_Sector(FLEXSPI, EXAMPLE_SECTOR * SECTOR_SIZE);
    if (status != kStatus_Success)
    {
        RGB_RED_LED_ON;
        PRINTF("擦除flash扇区失败 !\r\n");
        return -1;
    }
    
    /* 读取内容 */
    status = FlexSPI_NorFlash_Buffer_Read(FLEXSPI, 
                                         EXAMPLE_SECTOR * SECTOR_SIZE, 
                                         s_nor_read_buffer,
                                         EXAMPLE_SIZE);
    
    if (status != kStatus_Success)
    {
        RGB_RED_LED_ON;
        PRINTF("读取数据失败 !\r\n");
        return -1;
    }    
	
    /* 擦除后FLASH中的内容应为0xFF，
       设置比较用的s_nor_program_buffer值全为0xFF */    
    memset(s_nor_program_buffer, 0xFF, EXAMPLE_SIZE);
    /* 把读出的数据与0xFF比较 */
    if (memcmp(s_nor_program_buffer, s_nor_read_buffer, EXAMPLE_SIZE))
    {
        RGB_RED_LED_ON;
        PRINTF("擦除数据，读出数据不正确 !\r\n ");
        return -1;
    }
    else
    {
        PRINTF("擦除数据成功. \r\n");
    }
    
    /***************************写入数据测试****************************/
    PRINTF("写入数据测试 \r\n");
    
    for (i = 0; i < EXAMPLE_SIZE; i++)
    {
        s_nor_program_buffer[i] = (uint8_t)i;
    }

    /* 写入数据 */
    status = FlexSPI_NorFlash_Buffer_Program(FLEXSPI,
                                             EXAMPLE_SECTOR * SECTOR_SIZE,
                                             (void *)s_nor_program_buffer,
                                             EXAMPLE_SIZE);
    if (status != kStatus_Success)
    {
        RGB_RED_LED_ON;
        PRINTF("写入失败 !\r\n");
        return -1;
    }
    
    /* 读取数据 */
    status = FlexSPI_NorFlash_Buffer_Read(FLEXSPI, 
                                           EXAMPLE_SECTOR * SECTOR_SIZE, 
                                           s_nor_read_buffer,
                                           EXAMPLE_SIZE);
    
    if (status != kStatus_Success)
    {
        RGB_RED_LED_ON;
        PRINTF("读取数据失败 !\r\n");
        return -1;
    }
    
    /* 把读出的数据与写入的比较 */
    if (memcmp(s_nor_program_buffer, s_nor_read_buffer, EXAMPLE_SIZE))
    {
        RGB_RED_LED_ON;
        PRINTF("写入数据，读出数据不正确 !\r\n ");
        return -1;
    }
    else
    {
        PRINTF("写入数据成功. \r\n");
    }    
    
    RGB_GREEN_LED_ON;
    CORE_BOARD_LED_ON;
    PRINTF("NorFlash IP命令访问测试完成。\r\n");
    
    return 0;
}


/**
* @brief  使用AHB命令的方式进行读写测试
* @param  无
* @retval 测试结果，正常为0
*/
int NorFlash_AHBCommand_Test(void)
{
    uint32_t i = 0;
    status_t status;
    uint32_t JedecDeviceID = 0;

    PRINTF("\r\nNorFlash AHB命令访问测试\r\n");

    /***************************读ID测试****************************/
    /* 获取JedecDevice ID. */
    FlexSPI_NorFlash_Get_JedecDevice_ID(FLEXSPI, &JedecDeviceID);
    
    if(JedecDeviceID != FLASH_WINBOND_JEDECDEVICE_ID && 
			 JedecDeviceID != FLASH_ISSI_JEDECDEVICE_ID)
    {
      PRINTF("FLASH检测错误，读取到的JedecDeviceID值为: 0x%x\r\n", JedecDeviceID);
      return -1;
    }
    
    PRINTF("检测到FLASH芯片，JedecDeviceID值为: 0x%x\r\n", JedecDeviceID);
    
    /***************************擦除测试****************************/
    PRINTF("擦除扇区测试\r\n");
    
    /* 擦除指定扇区 */
    status = FlexSPI_NorFlash_Erase_Sector(FLEXSPI, EXAMPLE_SECTOR * SECTOR_SIZE);
    if (status != kStatus_Success)
    {
        PRINTF("擦除flash扇区失败 !\r\n");
        return -1;
    }
    
    /* 读取数据 */
    memcpy(s_nor_read_buffer, 
           NORFLASH_AHB_POINTER(EXAMPLE_SECTOR * SECTOR_SIZE),
           EXAMPLE_SIZE);
    
    /* 擦除后FLASH中的内容应为0xFF，
       设置比较用的s_nor_program_buffer值全为0xFF */    
    memset(s_nor_program_buffer, 0xFF, EXAMPLE_SIZE);
    /* 把读出的数据与0xFF比较 */
    if (memcmp(s_nor_program_buffer, s_nor_read_buffer, EXAMPLE_SIZE))
    {
        PRINTF("擦除数据，读出数据不正确 !\r\n ");
        return -1;
    }
    else
    {
        PRINTF("擦除数据成功. \r\n");
    }
    
    /***************************一次写入少量数据测试****************************/
    PRINTF("8、16、32位写入数据测试：0x12、0x3456、0x789abcde \r\n");
    /*使用AHB命令方式写入数据*/
    *(uint8_t *)NORFLASH_AHB_POINTER(EXAMPLE_SECTOR * SECTOR_SIZE) = 0x12;
    *(uint16_t *)NORFLASH_AHB_POINTER(EXAMPLE_SECTOR * SECTOR_SIZE + 4) = 0x3456;
    *(uint32_t *)NORFLASH_AHB_POINTER(EXAMPLE_SECTOR * SECTOR_SIZE + 8) = 0x789abcde;

    /*使用AHB命令方式读取数据*/
    PRINTF("8位读写结果 = 0x%x\r\n",
           *(uint8_t *)NORFLASH_AHB_POINTER(EXAMPLE_SECTOR * SECTOR_SIZE));
    PRINTF("16位读写结果 = 0x%x\r\n",
           *(uint16_t *)NORFLASH_AHB_POINTER(EXAMPLE_SECTOR * SECTOR_SIZE+4));
    PRINTF("32位读写结果 = 0x%x\r\n",
           *(uint32_t *)NORFLASH_AHB_POINTER(EXAMPLE_SECTOR * SECTOR_SIZE+8));
    
    /***************************一次写入一个扇区数据测试****************************/
    PRINTF("\r\n大量数据写入和读取测试 \r\n");
    
    for (i = 0; i < EXAMPLE_SIZE; i++)
    {
        s_nor_program_buffer[i] = (uint8_t)i;
    }
    
    /* 擦除指定扇区 */
    status = FlexSPI_NorFlash_Erase_Sector(FLEXSPI, EXAMPLE_SECTOR * SECTOR_SIZE);
    if (status != kStatus_Success)
    {
        PRINTF("擦除flash扇区失败 !\r\n");
        return -1;
    }
        
#if 1
    /* 写入一个扇区的数据 */
    status = FlexSPI_NorFlash_Buffer_Program(FLEXSPI,
                                             EXAMPLE_SECTOR * SECTOR_SIZE,
                                             (void *)s_nor_program_buffer,
                                             EXAMPLE_SIZE);
    if (status != kStatus_Success)
    {
        PRINTF("写入失败 !\r\n");
        return -1;
    }
    
    /* 使用软件复位来重置 AHB 缓冲区. */
    FLEXSPI_SoftwareReset(FLEXSPI);

    /* 读取数据 */
    memcpy(s_nor_read_buffer, 
           NORFLASH_AHB_POINTER(EXAMPLE_SECTOR * SECTOR_SIZE),
           EXAMPLE_SIZE);
    
#else
    /*注意！！！！经测试本功能还不能正常使用！！！！！！！*/
    PRINTF("以下功能还不能正常使用，保留仅用于以后调试！\r\n");
    /* 使用软件复位来重置 AHB 缓冲区. */
    FLEXSPI_SoftwareReset(FLEXSPI);
    /* 写入一个扇区的数据 */
    memcpy(NORFLASH_AHB_POINTER(EXAMPLE_SECTOR * SECTOR_SIZE), 
           s_nor_program_buffer,
           EXAMPLE_SIZE);
    
    /* 使用软件复位来重置 AHB 缓冲区. */
    FLEXSPI_SoftwareReset(FLEXSPI);

    /* 读取数据 */
    memcpy(s_nor_read_buffer, 
           NORFLASH_AHB_POINTER(EXAMPLE_SECTOR * SECTOR_SIZE),
           EXAMPLE_SIZE);
    
    PRINTF("读取到的数据:\r\n");
    FLASH_DEBUG_ARRAY(s_nor_read_buffer, EXAMPLE_SIZE);
    
    PRINTF("本功能还不能正常使用，保留仅用于以后调试！下面会提示读取到的数据不正确\r\n");
#endif 
    
    /* 把读出的数据与写入的比较 */
    if (memcmp(s_nor_program_buffer, s_nor_read_buffer,EXAMPLE_SIZE))
    {
        PRINTF("写入数据，读出数据不正确 !\r\n ");
        return -1;
    }
    else
    {
        PRINTF("大量数据写入和读取测试成功. \r\n");
    }  
    
    PRINTF("NorFlash AHB命令访问测试完成。\r\n");
    
    return 0;  
}


/**
* @brief  存储各种数据到FLASH的示例
* @param  无
* @retval 测试结果，正常为0
*/
int NorFlash_Data_Test(void)
{    
    uint32_t i = 0;
    status_t status;
    uint32_t JedecDeviceID = 0;
  
     /* 存储小数和整数的数组，各7个 */
    long double double_buffer[7] = {0};
    int int_buffer[7] = {0};
    /* 存储的字符串 */
    const char write_string[] = "欢迎使用野火开发板！";
    char string_buffer[40] ;    

    PRINTF("\r\nNorFlash存储字符串、小数、长整数示例\r\n");

    /***************************读ID测试****************************/
    /* 获取JedecDevice ID. */
    FlexSPI_NorFlash_Get_JedecDevice_ID(FLEXSPI, &JedecDeviceID);
    
    if(JedecDeviceID != FLASH_WINBOND_JEDECDEVICE_ID && 
			 JedecDeviceID != FLASH_ISSI_JEDECDEVICE_ID)
    {
      RGB_RED_LED_ON;
      PRINTF("FLASH检测错误，读取到的JedecDeviceID值为: 0x%x\r\n", JedecDeviceID);
      return -1;
    }
    
    PRINTF("检测到FLASH芯片，JedecDeviceID值为: 0x%x\r\n", JedecDeviceID);  
    
    /*若标志等于DATA_VALID_FLAG，表示之前已有写入数据*/
    if( NORFLASH_AHB_READ_BYTE(DATA_VALID_ADDR) == DATA_VALID_FLAG )  
    {
        PRINTF("\r\n检测到数据标志\r\n");

        /* 使用IP命令或AHB命令读取都是可以的 */
      
        /* 使用AHB命令读取字符串 */
        memcpy(string_buffer, 
               NORFLASH_AHB_POINTER(EXAMPLE_STRING_ADDR),
               sizeof(string_buffer));
      
        /* 使用AHB命令读取小数 */
        memcpy(double_buffer, 
               NORFLASH_AHB_POINTER(EXAMPLE_DOUBLE_ADDR),
               sizeof(double_buffer));
      
        /* 使用IP命令读取整数 */
        status = FlexSPI_NorFlash_Buffer_Read(FLEXSPI, 
                                             EXAMPLE_INT_ADDR, 
                                             (uint8_t *)int_buffer,
                                             sizeof(int_buffer));
      
        PRINTF("\r\n从芯片读到的数据：\r\n");    
        
        PRINTF("读取到的字符串：%s\r\n",string_buffer);
        
        for( i=0; i<7; i++ )
        {          
          PRINTF("小数 rx = %f \r\n",double_buffer[i]);
          PRINTF("整数 rx = %d \r\n",int_buffer[i]);        
        }
    }    
    else
    {
      PRINTF("\r\n没有检测到数据标志，FLASH没有存储数据，即将进行数据写入实验\r\n");
      
      /*擦除扇区*/
      FlexSPI_NorFlash_Erase_Sector(FLEXSPI, DATA_VALID_ADDR);  
      
      /* 写入标志位到第0页 */
      status = FlexSPI_NorFlash_Buffer_Program(FLEXSPI,
                                               DATA_VALID_ADDR,
                                               (uint8_t *)&DATA_VALID_FLAG,
                                               1);
      
      /*生成要写入的数据*/
      for( i=0; i<7; i++ )
      {
         double_buffer[i] = i +0.1;
         int_buffer[i]=i*500+1 ;
      }

      /* 写入字符串数据到第1页 */    
      status = FlexSPI_NorFlash_Buffer_Program(FLEXSPI,
                                               EXAMPLE_STRING_ADDR,
                                               (uint8_t *)write_string,
                                               sizeof(write_string));
      /* 第2页 */
      status = FlexSPI_NorFlash_Buffer_Program(FLEXSPI,
                                               EXAMPLE_DOUBLE_ADDR,
                                               (uint8_t *)double_buffer,
                                               sizeof(double_buffer));
      /* 第3页 */
      status = FlexSPI_NorFlash_Buffer_Program(FLEXSPI,
                                               EXAMPLE_INT_ADDR,
                                               (uint8_t *)int_buffer,
                                               sizeof(int_buffer));
            
      PRINTF("向芯片写入了数据：\r\n");
      
      PRINTF("写入的字符串：%s\r\n",write_string);
      /*打印到串口*/
      for( i=0; i<7; i++ )
      {
        PRINTF("小数tx = %f\r\n",double_buffer[i]);
        PRINTF("整数tx = %d\r\n",int_buffer[i]);
      }
      
      PRINTF("\r\n请复位开发板，以读取数据进行检验\r\n");
    }   
    
    return status;
}

/****************************END OF FILE**********************/
