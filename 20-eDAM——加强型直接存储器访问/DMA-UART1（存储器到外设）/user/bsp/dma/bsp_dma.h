#ifndef __BSP_DMA_H
#define __BSP_DMA_H

#include "fsl_common.h"
#include "fsl_edma.h"
#include "fsl_debug_console.h"

#define EXAMPLE_DMAMUX DMAMUX  //DMAMUX »ùÖ·
#define EXAMPLE_DMA DMA0
#define eDAM_Channel 0
#define BUFF_LENGTH 4U

void DMA_Config(void);
void EDMA_Callback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds);


#endif /* __BSP_DMA_H */



