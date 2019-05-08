#include "./bsp/dma_uart/bsp_dma_uart.h"
#include "fsl_lpuart_edma.h"

/*定义，DAM 传输句柄*/
edma_handle_t g_lpuartTxEdmaHandle;
edma_handle_t g_lpuartRxEdmaHandle;

/*定义 uart 句柄*/
lpuart_edma_handle_t g_lpuartEdmaHandle;


AT_NONCACHEABLE_SECTION_INIT(uint8_t g_tipString[]) =
    "LPUART EDMA example\r\nSend back received data\r\nEcho every 8 characters\r\n";
     

AT_NONCACHEABLE_SECTION_INIT(uint8_t g_txBuffer[ECHO_BUFFER_LENGTH]) = {0};
AT_NONCACHEABLE_SECTION_INIT(uint8_t g_rxBuffer[ECHO_BUFFER_LENGTH]) = {0};

volatile bool rxBufferEmpty = true;
volatile bool txBufferFull = false;
volatile bool txOnGoing = false;
volatile bool rxOnGoing = false;

void DMA_UART_Config(void)
{
  lpuart_config_t lpuartConfig;//定义串口初始化结构体
  edma_config_t userConfig;    //定义 edma 全局配置结构体
  
  lpuart_transfer_t xfer;
  lpuart_transfer_t sendXfer;
  lpuart_transfer_t receiveXfer;//定义串口数据传输结构体
  
  /* Initialize the LPUART. */
  /*
  * lpuartConfig.baudRate_Bps = 115200U;
  * lpuartConfig.parityMode = kLPUART_ParityDisabled;
  * lpuartConfig.stopBitCount = kLPUART_OneStopBit;
  * lpuartConfig.txFifoWatermark = 0;
  * lpuartConfig.rxFifoWatermark = 0;
  * lpuartConfig.enableTx = false;
  * lpuartConfig.enableRx = false;
  */
  LPUART_GetDefaultConfig(&lpuartConfig);
  
  /*修改默认的配置参数*/
  lpuartConfig.baudRate_Bps = BOARD_DEBUG_UART_BAUDRATE;
  lpuartConfig.enableTx = true;
  lpuartConfig.enableRx = true;
  
  LPUART_Init(DEMO_LPUART, &lpuartConfig, DEMO_LPUART_CLK_FREQ);
  
  
  /* 初始化DMAMUX */
  DMAMUX_Init(EXAMPLE_DMAMUX);
  /*设置DAM触发源*/
  DMAMUX_SetSource(EXAMPLE_DMAMUX, LPUART_TX_DMA_CHANNEL, LPUART_TX_DMA_REQUEST);
  DMAMUX_SetSource(EXAMPLE_DMAMUX, LPUART_RX_DMA_CHANNEL, LPUART_RX_DMA_REQUEST);
  /*使能通道*/
  DMAMUX_EnableChannel(EXAMPLE_DMAMUX, LPUART_TX_DMA_CHANNEL);
  DMAMUX_EnableChannel(EXAMPLE_DMAMUX, LPUART_RX_DMA_CHANNEL);
  
  /*初始化DMA模式*/
  EDMA_GetDefaultConfig(&userConfig);
  EDMA_Init(EXAMPLE_DMA, &userConfig);
  
  EDMA_CreateHandle(&g_lpuartTxEdmaHandle, EXAMPLE_DMA, LPUART_TX_DMA_CHANNEL);
  EDMA_CreateHandle(&g_lpuartRxEdmaHandle, EXAMPLE_DMA, LPUART_RX_DMA_CHANNEL);
  
  /* Create LPUART DMA handle. */
  LPUART_TransferCreateHandleEDMA(DEMO_LPUART, \
                                  &g_lpuartEdmaHandle, \
                                  LPUART_UserCallback, \
                                  NULL, \
                                  &g_lpuartTxEdmaHandle,\
                                  &g_lpuartRxEdmaHandle);
  xfer.data = g_tipString;
  xfer.dataSize = sizeof(g_tipString) - 1;
  txOnGoing = true; 
  
  LPUART_SendEDMA(DEMO_LPUART, &g_lpuartEdmaHandle, &xfer);
  
  /* 等待传输结束 */
  while (txOnGoing)
  {
    
  }
  while (1)
  {
    /* If RX is idle and g_rxBuffer is empty, start to read data to g_rxBuffer. */
    if ((!rxOnGoing) && rxBufferEmpty)
    {
      rxOnGoing = true;
      LPUART_ReceiveEDMA(DEMO_LPUART, &g_lpuartEdmaHandle, &receiveXfer);
    }
    
    /* If TX is idle and g_txBuffer is full, start to send data. */
    if ((!txOnGoing) && txBufferFull)
    {
      txOnGoing = true;
      LPUART_SendEDMA(DEMO_LPUART, &g_lpuartEdmaHandle, &sendXfer);
    }
    
    /* If g_txBuffer is empty and g_rxBuffer is full, copy g_rxBuffer to g_txBuffer. */
    if ((!rxBufferEmpty) && (!txBufferFull))
    {
      memcpy(g_txBuffer, g_rxBuffer, ECHO_BUFFER_LENGTH);
      rxBufferEmpty = true;
      txBufferFull = true;
    }
  }
}


/* LPUART 回调函数 */
void LPUART_UserCallback(LPUART_Type *base, lpuart_edma_handle_t *handle, status_t status, void *userData)
{
    userData = userData;

    if (kStatus_LPUART_TxIdle == status)
    {
        txBufferFull = false;
        txOnGoing = false;
       
    }

    if (kStatus_LPUART_RxIdle == status)
    {
        rxBufferEmpty = false;
        rxOnGoing = false;
    }
}





































