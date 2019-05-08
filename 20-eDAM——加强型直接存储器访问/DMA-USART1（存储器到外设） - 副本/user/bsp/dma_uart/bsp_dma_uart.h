#ifndef __BSP_DMA_UART_H
#define __BSP_DMA_UART_H

#include "fsl_common.h"
#include "fsl_edma.h"
#include "fsl_dmamux.h"
#include "fsl_lpuart.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_lpuart_edma.h"


#define DEMO_LPUART LPUART1      //����ʹ�õĴ���
#define DEMO_LPUART_CLK_FREQ BOARD_DebugConsoleSrcFreq()
#define EXAMPLE_DMAMUX DMAMUX  //DMAMUX ��ַ
#define EXAMPLE_DMA DMA0
#define ECHO_BUFFER_LENGTH 8

/*�궨�壬ָ��DMA ͨ��*/
#define LPUART_TX_DMA_CHANNEL 0U
#define LPUART_RX_DMA_CHANNEL 1U

/*�궨�壬 ָ������DMA����Ĵ���Դ*/
#define LPUART_TX_DMA_REQUEST kDmaRequestMuxLPUART1Tx
#define LPUART_RX_DMA_REQUEST kDmaRequestMuxLPUART1Rx



//#define EXAMPLE_DMAMUX DMAMUX  //DMAMUX ��ַ
//#define EXAMPLE_DMA DMA0
//#define eDAM_Channel 0
//#define BUFF_LENGTH 4U

void DMA_UART_Config(void);
void EDMA_Callback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds);
void LPUART_UserCallback(LPUART_Type *base, lpuart_edma_handle_t *handle, status_t status, void *userData);

#endif /* __BSP_DMA_UART_H */