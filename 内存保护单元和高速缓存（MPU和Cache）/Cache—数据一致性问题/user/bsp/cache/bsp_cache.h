#ifndef __BSP_CACHE_H
#define __BSP_CACHE_H

#include "fsl_common.h"
#include "fsl_edma.h"
#include "fsl_cache.h"

#include "fsl_debug_console.h"

#define EXAMPLE_DMAMUX DMAMUX  //DMAMUX 基址
#define EXAMPLE_DMA DMA0       //DMA基址
#define eDAM_Channel 0         //DMA通道
#define BUFF_LENGTH 4U         //输出缓冲区长度

void Cache_DMA_Config(void);
uint32_t APP_MemoryInit(void);
void APP_CacheConfig(bool enable);
void EDMA_Callback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds);
void APP_DMAMem2memTransfer(uint8_t *srcAddr, uint32_t srcWidth, uint8_t *dstAddr, uint32_t dstWidth, uint32_t size);

#endif /* __BSP_CACHE_H */



