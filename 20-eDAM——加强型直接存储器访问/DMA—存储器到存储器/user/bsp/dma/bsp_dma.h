#ifndef __BSP_DMA_H
#define __BSP_DMA_H

#include "fsl_common.h"
#include "fsl_edma.h"
#include "fsl_debug_console.h"

#define EXAMPLE_DMAMUX DMAMUX  //DMAMUX 基址
#define EXAMPLE_DMA DMA0       //DMA基址
#define eDAM_Channel 0         //DMA通道
#define BUFF_LENGTH 4U         //输出缓冲区长度

void DMA_Config(void);
void EDMA_Callback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds);


#endif /* __BSP_DMA_H */



