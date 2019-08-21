
#include "fsl_edma.h"
#include "bsp_cache.h"
#include "fsl_dmamux.h"


edma_handle_t g_EDMA_Handle;            //定义eDAM传输句柄
edma_transfer_config_t transferConfig;//定义 edma 传输配置结构体
	
void Cache_DMA_Config(void)
{

  edma_config_t userConfig;//定义 edma 全局配置结构体
  /* 初始化DMAMUX */
  DMAMUX_Init(EXAMPLE_DMAMUX);
  /*设置DMA 通道一直处于活动状态*/
  DMAMUX_EnableAlwaysOn(EXAMPLE_DMAMUX, eDAM_Channel, true); 
  /*使能通道*/
  DMAMUX_EnableChannel(EXAMPLE_DMAMUX, eDAM_Channel);
  
  /*获得默认配置
  * userConfig.enableRoundRobinArbitration = false; 使用固定优先级模式，
  * userConfig.enableHaltOnError = true;            使能错误停止
  * userConfig.enableContinuousLinkMode = false;    不使用次循环通道连接
  * userConfig.enableDebugMode = false;             禁止DEBUG模式（在Debug 模式下DMA正常运行）
   */
  EDMA_GetDefaultConfig(&userConfig);
  /*初始化eDMA*/
  EDMA_Init(EXAMPLE_DMA,&userConfig);

  /*创建eDMA句柄*/
  EDMA_CreateHandle(&g_EDMA_Handle, EXAMPLE_DMA, eDAM_Channel);
  /*设置传输完成回调函数*/
  EDMA_SetCallback(&g_EDMA_Handle, EDMA_Callback, NULL);
}


/*!
 * @brief Prepares the eDMA transfer structure.
 *
 * This function prepares the transfer configuration structure according to the user input.
 *
 * @param config The user configuration structure of type edma_transfer_t.
 * @param srcAddr eDMA transfer source address.
 * @param srcWidth eDMA transfer source address width(bytes).
 * @param destAddr eDMA transfer destination address.
 * @param destWidth eDMA transfer destination address width(bytes).
 * @param bytesEachRequest eDMA transfer bytes per channel request.
 * @param transferBytes eDMA transfer bytes to be transferred.
 * @param type eDMA transfer type.
 * @note The data address and the data width must be consistent. For example, if the SRC
 *       is 4 bytes, the source address must be 4 bytes aligned, or it results in
 *       source address error (SAE).
 */
void APP_DMAMem2memTransfer(uint8_t *srcAddr, uint32_t srcWidth, uint8_t *dstAddr, uint32_t dstWidth, uint32_t size)
{
    edma_transfer_config_t transferConfig;

    EDMA_PrepareTransfer(&transferConfig, srcAddr, srcWidth, dstAddr, dstWidth, srcWidth, size, kEDMA_MemoryToMemory);
    EDMA_SubmitTransfer(&g_EDMA_Handle, &transferConfig);
    EDMA_StartTransfer(&g_EDMA_Handle);
}



void APP_CacheConfig(bool enable)
{
    if (enable)
    {
        /* Enable the l1 data cache. */
        L1CACHE_EnableDCache();
    }
    else
    {
        L1CACHE_DisableDCache();
    }
}


uint32_t APP_MemoryInit(void)
{
    return 0x20200000;
}



extern bool g_Transfer_Done;
/*  EDMA 传输完成回调函数. */
void EDMA_Callback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds)
{
    if (transferDone)
    {
        g_Transfer_Done = true;
    }
}
