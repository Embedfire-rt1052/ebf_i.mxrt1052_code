
#include "fsl_edma.h"
#include "bsp_cache.h"
#include "fsl_dmamux.h"


edma_handle_t g_EDMA_Handle;            //����eDAM������
edma_transfer_config_t transferConfig;//���� edma �������ýṹ��
	
void Cache_DMA_Config(void)
{

  edma_config_t userConfig;//���� edma ȫ�����ýṹ��
  /* ��ʼ��DMAMUX */
  DMAMUX_Init(EXAMPLE_DMAMUX);
  /*����DMA ͨ��һֱ���ڻ״̬*/
  DMAMUX_EnableAlwaysOn(EXAMPLE_DMAMUX, eDAM_Channel, true); 
  /*ʹ��ͨ��*/
  DMAMUX_EnableChannel(EXAMPLE_DMAMUX, eDAM_Channel);
  
  /*���Ĭ������
  * userConfig.enableRoundRobinArbitration = false; ʹ�ù̶����ȼ�ģʽ��
  * userConfig.enableHaltOnError = true;            ʹ�ܴ���ֹͣ
  * userConfig.enableContinuousLinkMode = false;    ��ʹ�ô�ѭ��ͨ������
  * userConfig.enableDebugMode = false;             ��ֹDEBUGģʽ����Debug ģʽ��DMA�������У�
   */
  EDMA_GetDefaultConfig(&userConfig);
  /*��ʼ��eDMA*/
  EDMA_Init(EXAMPLE_DMA,&userConfig);

  /*����eDMA���*/
  EDMA_CreateHandle(&g_EDMA_Handle, EXAMPLE_DMA, eDAM_Channel);
  /*���ô�����ɻص�����*/
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
/*  EDMA ������ɻص�����. */
void EDMA_Callback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds)
{
    if (transferDone)
    {
        g_Transfer_Done = true;
    }
}
