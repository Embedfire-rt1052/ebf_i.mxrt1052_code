/**
  ******************************************************************
  * @file    nand_flash_test.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   NAND���Գ���
  ******************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  i.MXRT1052������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
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
* @brief  ��������
* @param  ��
* @retval ��
*/
void ErrorHandler(void)
{
    PRINTF("\n\r��д������������\n\r");
    while (1)
    {
    }
}
/**
* @brief  nand flash ��д���Գ���
* @param  ��
* @retval kStatus_Success���ɹ�
*/
void nand_flash_test(void)
{
    status_t status;
    char Manufacturer[12];

    PRINTF("\r\nNAND Flash���Գ���!\r\n");

    PRINTF("\r\nNAND Flash��ʼ��!\r\n");
    status = Nand_Flash_Init(&nandConfig, &nandHandle);
    if (status != kStatus_Success)
    {
        PRINTF("\r\nNAND Flash��ʼ��ʧ��!\r\n");
        PRINTF("\r\n error code: %d \r\n", status);
        ErrorHandler();
    }
    PRINTF("\r\nNAND Flash��ʼ���ɹ�!\r\n");
    //��ȡID
    PRINTF("\r\nNAND FlashID:0x%x\r\n", NAND_ReadID());
    memset(Manufacturer, 0, 12);
    strncpy(Manufacturer, (const char *)s_nandDeviceManufacturerList[nandHandle.vendorType], 8);
    PRINTF("\r\nNAND Flash  ���̣�%s\r\n", Manufacturer);
    PRINTF("\r\nNAND Flashÿҳ���������С��%d�ֽ�\r\n", nandHandle.bytesInPageDataArea);
    PRINTF("\r\nNAND Flashÿҳ���������С��%d�ֽ�\r\n", nandHandle.bytesInPageSpareArea);
    PRINTF("\r\nNAND Flashÿһ�������ҳ����%dҳ\r\n", nandHandle.pagesInBlock);
    PRINTF("\r\nNAND Flashÿһ�������ҳ����%dҳ\r\n", nandHandle.blocksInPlane);
    PRINTF("\r\nNAND Flash�豸����������%d��\r\n", nandHandle.planesInDevice);

    //		PRINTF("\r\n***NAND Flash ID��%d\r\n",nandHandle.JEDECid);
    //    SEMC->IPCR1 = 1;

    /* Erase Block */
    PRINTF("\r\nNAND Flash Erase������һ����!\r\n");
    status = Nand_Flash_Erase_Block(&nandHandle, 0);
    if (status != kStatus_Success)
    {
        PRINTF("\r\nNAND Flash������ʧ��!\r\n");
        ErrorHandler();
    }

    /* Read and check if it is blank. */
    PRINTF("\r\nNAND Flash����У��!\r\n");
    for (uint32_t pageIndex = 0; pageIndex < nandHandle.pagesInBlock; pageIndex++)
    {
        status = Nand_Flash_Read_Page(&nandHandle, pageIndex, mem_readBuffer, nandHandle.bytesInPageDataArea);
        if (status != kStatus_Success)
        {
            PRINTF("\r\nNAND Flash��ȡҳʧ��!\r\n");
            ErrorHandler();
        }

        for (uint32_t bytesIndex = 0; bytesIndex < nandHandle.bytesInPageDataArea; bytesIndex++)
        {
            if (mem_readBuffer[bytesIndex] != 0xFF)
            {
                PRINTF("\r\nNAND Flash������ʧ��!\r\n");
                ErrorHandler();
            }
        }
    }

    PRINTF("\r\nNAND Flash������ɹ�!\r\n");

    /* Program the page data. */
    PRINTF("\r\nNAND Flashҳд��!\r\n");
    /* Initialize the write buffers. */
    memset(mem_writeBuffer, 0xaa, sizeof(mem_writeBuffer));
    status = Nand_Flash_Page_Program(&nandHandle, 0, mem_writeBuffer, nandHandle.bytesInPageDataArea);
    if (status != kStatus_Success)
    {
        PRINTF("\r\nNAND Flashҳд��ʧ��!\r\n");
        ErrorHandler();
    }

    /* Read page data and check if the data read is equal to the data programed. */
    PRINTF("\r\nNAND Flashҳ��ȡ!\r\n");
    status = Nand_Flash_Read_Page(&nandHandle, 0, mem_readBuffer, nandHandle.bytesInPageDataArea);
    if (status != kStatus_Success)
    {
        PRINTF("\r\nNAND Flashҳ��ȡʧ��!\r\n");
        ErrorHandler();
    }

    if (memcmp(mem_writeBuffer, mem_readBuffer, nandHandle.bytesInPageDataArea) != 0)
    {
        PRINTF("\r\nNAND Flashҳ��ȡʧ��!\r\n");
        ErrorHandler();
    }

    PRINTF("\r\nNAND Flashҳ��д�ɹ�!\r\n");
}
