/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   用V2.3.1版本库建的工程模板
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

#define SDRAM_START_ADDRESS (0x80000000U)
#define SEMC_CLK_FREQ CLOCK_GetFreq(kCLOCK_SemcClk)

#define SEMC_EXAMPLE_DATALEN (0x1000U)
#define SEMC_EXAMPLE_WRITETIMES (1000U)

#define SDRAM_MEM_LENGTH        32*1024*1024

uint32_t sdram_writeBuffer[SEMC_EXAMPLE_DATALEN];
uint32_t sdram_readBuffer[SEMC_EXAMPLE_DATALEN];

void SEMC_SDRAMReadWrite32Bit(void)
{
    uint32_t index;
    uint32_t datalen = SEMC_EXAMPLE_DATALEN;
    uint32_t *sdram = (uint32_t *)SDRAM_START_ADDRESS; /* SDRAM start address. */
    bool result = true;

    PRINTF("\r\n SEMC SDRAM Memory 32 bit Write Start, Start Address 0x%x, Data Length %d !\r\n", sdram, datalen);
    /* Prepare data and write to SDRAM. */
    for (index = 0; index < datalen; index++)
    {
        sdram_writeBuffer[index] = index;
        sdram[index] = sdram_writeBuffer[index];
    }

    PRINTF("\r\n SEMC SDRAM Read 32 bit Data Start, Start Address 0x%x, Data Length %d !\r\n", sdram, datalen);
    /* Read data from the SDRAM. */
    for (index = 0; index < datalen; index++)
    {
        sdram_readBuffer[index] = sdram[index];
    }

    PRINTF("\r\n SEMC SDRAM 32 bit Data Write and Read Compare Start!\r\n");
    /* Compare the two buffers. */
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
        PRINTF("\r\n SEMC SDRAM 32 bit Data Write and Read Compare Failed!\r\n");
    }
    else
    {
        PRINTF("\r\n SEMC SDRAM 32 bit Data Write and Read Compare Succeed!\r\n");
    }
}

static void SEMC_SDRAMReadWrite16Bit(void)
{
    uint32_t index;
    uint32_t datalen = SEMC_EXAMPLE_DATALEN;
    uint16_t *sdram = (uint16_t *)SDRAM_START_ADDRESS; /* SDRAM start address. */
    bool result = true;

    PRINTF("\r\n SEMC SDRAM Memory 16 bit Write Start, Start Address 0x%x, Data Length %d !\r\n", sdram, datalen);

    memset(sdram_writeBuffer, 0, sizeof(sdram_writeBuffer));
    memset(sdram_readBuffer, 0, sizeof(sdram_readBuffer));

    /* Prepare data and write to SDRAM. */
    for (index = 0; index < datalen; index++)
    {
        sdram_writeBuffer[index] = index % 0xFFFF;
        sdram[index] = sdram_writeBuffer[index];
    }

    PRINTF("\r\n SEMC SDRAM Read 16 bit Data Start, Start Address 0x%x, Data Length %d !\r\n", sdram, datalen);
    /* Read data from the SDRAM. */
    for (index = 0; index < datalen; index++)
    {
        sdram_readBuffer[index] = sdram[index];
    }

    PRINTF("\r\n SEMC SDRAM 16 bit Data Write and Read Compare Start!\r\n");
    /* Compare the two buffers. */
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
        PRINTF("\r\n SEMC SDRAM 16 bit Data Write and Read Compare Failed!\r\n");
    }
    else
    {
        PRINTF("\r\n SEMC SDRAM 16 bit Data Write and Read Compare Succeed!\r\n");
    }
}

static void SEMC_SDRAMReadWrite8Bit(void)
{
    uint32_t index;
    uint32_t datalen = SEMC_EXAMPLE_DATALEN;
    uint8_t *sdram = (uint8_t *)SDRAM_START_ADDRESS; /* SDRAM start address. */
    bool result = true;

    PRINTF("\r\n SEMC SDRAM Memory 8 bit Write Start, Start Address 0x%x, Data Length %d !\r\n", sdram, datalen);

    memset(sdram_writeBuffer, 0, sizeof(sdram_writeBuffer));
    memset(sdram_readBuffer, 0, sizeof(sdram_readBuffer));

    /* Prepare data and write to SDRAM. */
    for (index = 0; index < datalen; index++)
    {
        sdram_writeBuffer[index] = index % 0x100;
        sdram[index] = sdram_writeBuffer[index];
    }

    PRINTF("\r\n SEMC SDRAM Read 8 bit Data Start, Start Address 0x%x, Data Length %d !\r\n", sdram, datalen);
    /* Read data from the SDRAM. */
    for (index = 0; index < datalen; index++)
    {
        sdram_readBuffer[index] = sdram[index];
    }

    PRINTF("\r\n SEMC SDRAM 8 bit Data Write and Read Compare Start!\r\n");
    /* Compare the two buffers. */
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
        PRINTF("\r\n SEMC SDRAM 8 bit Data Write and Read Compare Failed!\r\n");
    }
    else
    {
        PRINTF("\r\n SEMC SDRAM 8 bit Data Write and Read Compare Succeed!\r\n");
    }
}

void SEMC_SDRAMReadWriteTest(void)
{
	/* 32Bit数据读写测试*/
	SEMC_SDRAMReadWrite32Bit();
	/* 16Bit数据读写测试 */
	SEMC_SDRAMReadWrite16Bit();
	/* 8Bit数据读写测试 */
	SEMC_SDRAMReadWrite8Bit();
}
/****************************END OF FILE**********************/
