#include "fsl_edma.h"
#include "fsl_dmamux.h"
#include "./dma/bsp_dma.h"   


edma_handle_t g_EDMA_Handle;            //定义eDAM传输句柄
volatile bool g_Transfer_Done = false;//定义传输完成标志
uint32_t srcAddr[BUFF_LENGTH] = {0x01, 0x02, 0x03, 0x04};//源数据缓冲区
uint32_t destAddr[BUFF_LENGTH] = {0x00, 0x00, 0x00, 0x00};//目的数据缓冲区



void DMA_Config(void)
{
  edma_transfer_config_t transferConfig;//定义 edma 传输配置结构体
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
  
  /*初始化传输配置结构体*/
  transferConfig.srcAddr = (uint32_t)srcAddr;  //源地址       
  transferConfig.srcOffset = 4;                //源地址偏移
  transferConfig.srcTransferSize = kEDMA_TransferSize4Bytes;//源数据宽度
  
  transferConfig.destAddr = (uint32_t)destAddr;//目的地址
  transferConfig.destOffset = 4;               //目的地址偏移
  transferConfig.destTransferSize = kEDMA_TransferSize4Bytes;//目的数据宽度
  
  transferConfig.minorLoopBytes = 16;          //次循环传输字节数
  transferConfig.majorLoopCounts = 1;          //次循环执行次数
  
  /*创建eDMA句柄*/
  EDMA_CreateHandle(&g_EDMA_Handle, EXAMPLE_DMA, eDAM_Channel);
  /*设置传输完成回调函数*/
  EDMA_SetCallback(&g_EDMA_Handle, EDMA_Callback, NULL);
  /*提交eDAM传输请求*/
  EDMA_SubmitTransfer(&g_EDMA_Handle, &transferConfig);        
  /*启动传输*/
  EDMA_StartTransfer(&g_EDMA_Handle);  
}


/*  EDMA 传输完成回调函数. */
void EDMA_Callback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds)
{
    if (transferDone)
    {
        g_Transfer_Done = true;
    }
}
