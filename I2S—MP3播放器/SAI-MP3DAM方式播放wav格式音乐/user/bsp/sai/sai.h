#ifndef __SAI_H
#define __SAI_H

#include "board.h"
#include "fsl_dmamux.h"
#include "fsl_sai_edma.h"
#include "fsl_debug_console.h"
#include "arm_math.h"


/* SAI instance and clock */
#define DEMO_CODEC_WM8960
#define DEMO_SAI SAI1
#define DEMO_SAI_CHANNEL (0)
#define DEMO_SAI_BITWIDTH (kSAI_WordWidth16bits)
#define DEMO_SAI_IRQ SAI1_IRQn
#define SAI_UserIRQHandler SAI1_IRQHandler

/* IRQ */
#define DEMO_SAI_TX_IRQ SAI1_IRQn
#define DEMO_SAI_RX_IRQ SAI1_IRQn

/* DMA */
#define EXAMPLE_DMA DMA0
#define EXAMPLE_DMAMUX DMAMUX
#define EXAMPLE_TX_CHANNEL (0U)
#define EXAMPLE_RX_CHANNEL (1U)
#define EXAMPLE_SAI_TX_SOURCE kDmaRequestMuxSai1Tx
#define EXAMPLE_SAI_RX_SOURCE kDmaRequestMuxSai1Rx

/* Select Audio/Video PLL (786.48 MHz) as sai1 clock source */ 
#define DEMO_SAI1_CLOCK_SOURCE_SELECT (2U)
/* Clock pre divider for sai1 clock source */
#define DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER (0U)
/* Clock divider for sai1 clock source */
#define DEMO_SAI1_CLOCK_SOURCE_DIVIDER (63U)

/* Get frequency of sai1 clock */
#define DEMO_SAI_CLK_FREQ                                                        \
    (CLOCK_GetFreq(kCLOCK_AudioPllClk) / (DEMO_SAI1_CLOCK_SOURCE_DIVIDER + 1U) / \
     (DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER + 1U))


/* Select USB1 PLL (480 MHz) as master lpi2c clock source */
#define DEMO_LPI2C_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for master lpi2c clock source */
#define DEMO_LPI2C_CLOCK_SOURCE_DIVIDER (5U)

/*定义缓冲区大小*/
#define BUFFER_SIZE (512)





void BOARD_EnableSaiMclkOutput(bool enable);
void SAI1_IIC_init(void);
void	SAI1_Init(void);
void	SAI1_DMAConfig(void);

static void txCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData);
static void rxCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData);



#endif /* __SAI_H */ 
