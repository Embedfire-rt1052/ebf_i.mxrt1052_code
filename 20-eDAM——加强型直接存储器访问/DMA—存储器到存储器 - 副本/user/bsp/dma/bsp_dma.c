#include "fsl_edma.h"
#include "fsl_dmamux.h"

#include "./dma/bsp_dma.h"   

volatile uint32_t NBYTES;
edma_handle_t g_EDMA_Handle;//定义eDAM传输句柄
volatile bool g_Transfer_Done = false;//定义传输完成标志
AT_NONCACHEABLE_SECTION_INIT(uint32_t srcAddr[BUFF_LENGTH]) = {0x01, 0x02, 0x03, 0x04};//源数据缓冲区
AT_NONCACHEABLE_SECTION_INIT(uint32_t destAddr[BUFF_LENGTH]) = {0x00, 0x00, 0x00, 0x00};//目的数据缓冲区

uint8_t char_source[20] = {'1','2','3','4','5','g','h','i','j'};
uint8_t char_resive[20] = {'1'};


void DMA_Config(void)
{
  
  edma_transfer_config_t transferConfig;//定义 edma 传输配置结构体
  edma_config_t userConfig;//定义 edma 全局配置结构体
  //k = (void*)(0x40184000 + 0x1c);
  /* 初始化DMAMUX */
  DMAMUX_Init(EXAMPLE_DMAMUX);
  
  /*设置DMA 通道开启后一直工作*/
  DMAMUX_EnableAlwaysOn(EXAMPLE_DMAMUX, eDAM_Channel, true); 
  /*使能通道*/
  DMAMUX_EnableChannel(EXAMPLE_DMAMUX, eDAM_Channel);
  /*获得默认配置
  * userConfig.enableRoundRobinArbitration = false; 选择使用循环通道仲裁模式，
  * userConfig.enableHaltOnError = true;            使能错误停止
  * userConfig.enableContinuousLinkMode = false;    
  * userConfig.enableDebugMode = false;             禁止DEBUG模式
   */
  EDMA_GetDefaultConfig(&userConfig);
  /*初始化eDMA*/
  EDMA_Init(EXAMPLE_DMA,&userConfig);
  
  /*创建eDMA句柄*/
  EDMA_CreateHandle(&g_EDMA_Handle, EXAMPLE_DMA, eDAM_Channel);     
  /*设置传输完成回调函数*/
  EDMA_SetCallback(&g_EDMA_Handle, EDMA_Callback, NULL);
  
  /*eDMA预传输，根据配置参数完成传输配置结构体的初始化*/
  //从存储器到外设

  transferConfig.srcAddr = (uint32_t)char_source;//源地址       
  transferConfig.srcOffset = 1;
  transferConfig.srcTransferSize = kEDMA_TransferSize1Bytes;
  
//transferConfig.destAddr = (uint32_t)(0x40184000 + 0x1c);//目的地址
  transferConfig.destAddr = (uint32_t)char_resive;//目的地址
  transferConfig.destOffset = 1;
  transferConfig.destTransferSize = kEDMA_TransferSize1Bytes;
  
  transferConfig.minorLoopBytes = 8;
  transferConfig.majorLoopCounts = 1;
  

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

