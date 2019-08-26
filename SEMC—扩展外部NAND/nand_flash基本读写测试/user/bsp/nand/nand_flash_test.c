/**
  ******************************************************************
  * @file    nand_flash_test.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   NAND测试程序
  ******************************************************************
  * @attention
  *
  * 实验平台:野火  i.MXRT1052开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************
  */

#include "board.h"
#include "./nand/bsp_nand.h"
#include "fsl_debug_console.h"
#include "./nand/fsl_nand_flash.h"

uint8_t mem_writeBuffer[FLASH_PAGE_SIZE];
uint8_t mem_readBuffer[FLASH_PAGE_SIZE] = {0};
nand_handle_t nandHandle;

extern const uint8_t s_nandDeviceManufacturerList[][12];
extern nand_config_t nandConfig;
extern uint32_t NAND_ReadID(void);
/**
* @brief  错误处理函数
* @param  无
* @retval 无
*/
void ErrorHandler(void)
{
    PRINTF("\n\r读写出错，请检查配置\n\r");
    while (1)
    {
    }
}
/**
* @brief  nand flash 读写测试程序
* @param  无
* @retval kStatus_Success：成功
*/
void nand_flash_test(void)
{
    status_t status;
    char Manufacturer[12];

    PRINTF("\r\nNAND Flash测试程序!\r\n");

    PRINTF("\r\nNAND Flash初始化!\r\n");
    status = Nand_Flash_Init(&nandConfig, &nandHandle);
    if (status != kStatus_Success)
    {
        PRINTF("\r\nNAND Flash初始化失败!\r\n");
        PRINTF("\r\n error code: %d \r\n", status);
        ErrorHandler();
    }
    PRINTF("\r\nNAND Flash初始化成功!\r\n");
    //读取ID
    PRINTF("\r\nNAND FlashID:0x%x\r\n", NAND_ReadID());
    memset(Manufacturer, 0, 12);
    strncpy(Manufacturer, (const char *)s_nandDeviceManufacturerList[nandHandle.vendorType], 8);
    PRINTF("\r\nNAND Flash  厂商：%s\r\n", Manufacturer);
    PRINTF("\r\nNAND Flash每页数据区域大小：%d字节\r\n", nandHandle.bytesInPageDataArea);
    PRINTF("\r\nNAND Flash每页备用区域大小：%d字节\r\n", nandHandle.bytesInPageSpareArea);
    PRINTF("\r\nNAND Flash每一块包含的页数：%d页\r\n", nandHandle.pagesInBlock);
    PRINTF("\r\nNAND Flash每一层包含的页数：%d页\r\n", nandHandle.blocksInPlane);
    PRINTF("\r\nNAND Flash设备包含层数：%d层\r\n", nandHandle.planesInDevice);

    //		PRINTF("\r\n***NAND Flash ID：%d\r\n",nandHandle.JEDECid);
    //    SEMC->IPCR1 = 1;

    /* Erase Block */
    PRINTF("\r\nNAND Flash Erase擦除第一个块!\r\n");
    status = Nand_Flash_Erase_Block(&nandHandle, 0);
    if (status != kStatus_Success)
    {
        PRINTF("\r\nNAND Flash擦除块失败!\r\n");
        ErrorHandler();
    }

    /* Read and check if it is blank. */
    PRINTF("\r\nNAND Flash擦除校验!\r\n");
    for (uint32_t pageIndex = 0; pageIndex < nandHandle.pagesInBlock; pageIndex++)
    {
        status = Nand_Flash_Read_Page(&nandHandle, pageIndex, mem_readBuffer, nandHandle.bytesInPageDataArea);
        if (status != kStatus_Success)
        {
            PRINTF("\r\nNAND Flash读取页失败!\r\n");
            ErrorHandler();
        }

        for (uint32_t bytesIndex = 0; bytesIndex < nandHandle.bytesInPageDataArea; bytesIndex++)
        {
            if (mem_readBuffer[bytesIndex] != 0xFF)
            {
                PRINTF("\r\nNAND Flash擦除块失败!\r\n");
                ErrorHandler();
            }
        }
    }

    PRINTF("\r\nNAND Flash擦除块成功!\r\n");

    /* Program the page data. */
    PRINTF("\r\nNAND Flash页写入!\r\n");
    /* Initialize the write buffers. */
    memset(mem_writeBuffer, 0xaa, sizeof(mem_writeBuffer));
    status = Nand_Flash_Page_Program(&nandHandle, 0, mem_writeBuffer, nandHandle.bytesInPageDataArea);
    if (status != kStatus_Success)
    {
        PRINTF("\r\nNAND Flash页写入失败!\r\n");
        ErrorHandler();
    }

    /* Read page data and check if the data read is equal to the data programed. */
    PRINTF("\r\nNAND Flash页读取!\r\n");
    status = Nand_Flash_Read_Page(&nandHandle, 0, mem_readBuffer, nandHandle.bytesInPageDataArea);
    if (status != kStatus_Success)
    {
        PRINTF("\r\nNAND Flash页读取失败!\r\n");
        ErrorHandler();
    }

    if (memcmp(mem_writeBuffer, mem_readBuffer, nandHandle.bytesInPageDataArea) != 0)
    {
        PRINTF("\r\nNAND Flash页读取失败!\r\n");
        ErrorHandler();
    }

    PRINTF("\r\nNAND Flash页读写成功!\r\n");
}
