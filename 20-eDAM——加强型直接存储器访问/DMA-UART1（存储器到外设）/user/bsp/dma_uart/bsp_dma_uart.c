#include "./bsp/dma_uart/bsp_dma_uart.h"
#include "fsl_lpuart_edma.h"

/*����������main.c�ļ�*/
extern volatile bool rxBufferEmpty;
extern volatile bool txBufferFull;
extern volatile bool txOnGoing;
extern volatile bool rxOnGoing;


/*�������*/
extern lpuart_edma_handle_t g_lpuartEdmaHandle;
extern edma_handle_t g_lpuartTxEdmaHandle;
extern edma_handle_t g_lpuartRxEdmaHandle;


/*
*�������ܣ���ʼ����������
ע��:��ʵ��ʹ�õ���UART1,UART1��ϵͳ���ڣ���ϵͳ�������Ѿ���������ŵĳ�ʼ��
�������������������UART1���ų�ʼ�������ʹ��������������������ų�ʼ�������Ǳ�����ӵġ�
*/
void UART_GPIO_Init(void)
{
  /* ����gpio��ʼ�����ýṹ�� */
  gpio_pin_config_t adc_config;
  /*ADC��GPIO����*/   
  adc_config.direction = kGPIO_DigitalInput; //����ģʽ
//adc_config.outputLogic =  1;                //Ĭ�ϸߵ�ƽ��������ģʽ�����ø�ѡ����Ч
  adc_config.interruptMode = kGPIO_NoIntmode; //��ʹ���ж�
  
  
  IOMUXC_SetPinMux(UART_RX_IOMUXC, 0U);                                      
  IOMUXC_SetPinMux(UART_TX_IOMUXC, 0U);
  
  IOMUXC_SetPinConfig(UART_RX_IOMUXC, UART_RX_PAD_CONFIG_DATA);
  IOMUXC_SetPinConfig(UART_TX_IOMUXC, UART_TX_PAD_CONFIG_DATA);
  
  GPIO_PinInit(UART_RX_GPIO, UART_RX_GPIO_PIN, &adc_config);
  
  adc_config.direction = kGPIO_DigitalOutput; //����ģʽ
  adc_config.outputLogic =  1;                //Ĭ�ϸߵ�ƽ
  GPIO_PinInit(UART_TX_GPIO, UART_TX_GPIO_PIN, &adc_config);
  
}


/*
*�������ܣ���ʼ������
*/
void UART_Init(void)
{
  lpuart_config_t lpuartConfig;//����LUART��ʼ���ṹ��
  
  /*��ʼ��UART����*/
  UART_GPIO_Init();
  /* LPUART.Ĭ������ */
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
  lpuartConfig.baudRate_Bps = BOARD_DEBUG_UART_BAUDRATE;
  lpuartConfig.enableTx = true;
  lpuartConfig.enableRx = true;
  
  LPUART_Init(DEMO_LPUART, &lpuartConfig, DEMO_LPUART_CLK_FREQ);
}


/*
*�������ܣ���ʼ��������ʹ�õ�DMA
*/
void UART_DMA_Init(void)
{
  edma_config_t config;//����eDMA��ʼ���ṹ��
  
  /*��ʼ��DMAMUX */
  DMAMUX_Init(EXAMPLE_LPUART_DMAMUX_BASEADDR);
  /* ΪLPUART����DMA����ͨ��*/
  DMAMUX_SetSource(EXAMPLE_LPUART_DMAMUX_BASEADDR, LPUART_TX_DMA_CHANNEL, LPUART_TX_DMA_REQUEST);
  DMAMUX_SetSource(EXAMPLE_LPUART_DMAMUX_BASEADDR, LPUART_RX_DMA_CHANNEL, LPUART_RX_DMA_REQUEST);
  DMAMUX_EnableChannel(EXAMPLE_LPUART_DMAMUX_BASEADDR, LPUART_TX_DMA_CHANNEL);
  DMAMUX_EnableChannel(EXAMPLE_LPUART_DMAMUX_BASEADDR, LPUART_RX_DMA_CHANNEL);
  
  /* ��ʼ��DMA */
  EDMA_GetDefaultConfig(&config);
  EDMA_Init(EXAMPLE_LPUART_DMA_BASEADDR, &config);
  /*����eDMA�����*/
  EDMA_CreateHandle(&g_lpuartTxEdmaHandle, EXAMPLE_LPUART_DMA_BASEADDR, LPUART_TX_DMA_CHANNEL);
  EDMA_CreateHandle(&g_lpuartRxEdmaHandle, EXAMPLE_LPUART_DMA_BASEADDR, LPUART_RX_DMA_CHANNEL);
  
  /* ���� LPUART DMA ��� */
  LPUART_TransferCreateHandleEDMA(DEMO_LPUART, &g_lpuartEdmaHandle, LPUART_UserCallback, NULL, &g_lpuartTxEdmaHandle,
                                  &g_lpuartRxEdmaHandle);
}




/* LPUART �ص����� */
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
