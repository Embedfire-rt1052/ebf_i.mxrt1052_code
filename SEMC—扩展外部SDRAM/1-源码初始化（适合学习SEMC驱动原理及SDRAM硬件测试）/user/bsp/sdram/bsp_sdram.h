#ifndef __BSP_SDRAM_H
#define	__BSP_SDRAM_H

#include "fsl_common.h"

/* SEMC_CLK_ROOTÆµÂÊ */
#define EXAMPLE_SEMC_CLK_FREQ         CLOCK_GetFreq(kCLOCK_SemcClk)

/*******************************************************************************
 * º¯ÊýÉùÃ÷
 ******************************************************************************/
status_t SDRAM_Init(void);

void SEMC_SDRAMReadWrite32Bit(void);
void SEMC_SDRAMReadWrite16Bit(void);
void SEMC_SDRAMReadWrite8Bit(void);

#endif /* __BSP_SDRAM_H */
