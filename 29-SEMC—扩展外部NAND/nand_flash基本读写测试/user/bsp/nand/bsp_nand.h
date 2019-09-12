#ifndef __BSP_NAND_H
#define	__BSP_NAND_H

#include "fsl_common.h"

#define EXAMPLE_SEMC SEMC
#define EXAMPLE_SEMC_START_ADDRESS (0x80000000U)
#define EXAMPLE_SEMC_CLK_FREQ CLOCK_GetFreq(kCLOCK_SemcClk)
#define EXAMPLE_SEMC_NAND_AXI_START_ADDRESS (0x9E000000U)
#define EXAMPLE_SEMC_NAND_IPG_START_ADDRESS (0x00000000U)
#define FLASH_PAGE_SIZE (4096)

/*******************************************************************************
 * º¯ÊýÉùÃ÷
 ******************************************************************************/
void BOARD_InitNand(void);
#endif /* __BSP_NAND_H */
