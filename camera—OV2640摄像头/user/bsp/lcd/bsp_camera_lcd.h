#ifndef _LCD_H_
#define _LCD_H_

#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_pxp.h"
#include "fsl_elcdif.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_ELCDIF LCDIF
#define APP_PXP PXP

#define APP_IMG_HEIGHT 480
#define APP_IMG_WIDTH 800

#define APP_HSW 1				//HSYNC宽度
#define APP_HFP 20//10	//HSYNC前的无效像素
#define APP_HBP 46			//HSYNC后的无效像素
#define APP_VSW 1				//VSYNC宽度
#define APP_VFP 22			//VSYNC前的无效行数
#define APP_VBP 23			//VSYNC后的无效行数

#define APP_POL_FLAGS \
    (kELCDIF_DataEnableActiveHigh | kELCDIF_VsyncActiveLow | kELCDIF_HsyncActiveLow | kELCDIF_DriveDataOnRisingClkEdge)

/* 背光引脚 */
#define LCD_BL_GPIO GPIO1
#define LCD_BL_GPIO_PIN 15

#define APP_LCDIF_DATA_BUS kELCDIF_DataBus16Bit

/*
 * Frame buffer data alignment.
 * The PXP input buffer, output buffer, and LCDIF frame buffer address 64B align.
 */
#define FRAME_BUFFER_ALIGN 64

/* PS input buffer is square. */
#if APP_IMG_WIDTH > APP_IMG_HEIGHT
#define APP_PS_SIZE (APP_IMG_HEIGHT / 2U)
#else
#define APP_PS_SIZE (APP_IMG_WIDTH / 2U)
#endif

#define APP_PS_ULC_X 0U
#define APP_PS_ULC_Y 0U

//#define APP_BPP 4U /* Use 24-bit RGB888 format. */

#ifndef APP_LCDIF_DATA_BUS
#define APP_LCDIF_DATA_BUS kELCDIF_DataBus24Bit
#endif
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void APP_InitInputBuffer(void);
static void APP_InitLcdif(void);
static void APP_InitPxp(void);
static void APP_Scale(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
//AT_NONCACHEABLE_SECTION_ALIGN(static uint32_t s_psBufferLcd[2][APP_IMG_HEIGHT][APP_IMG_WIDTH], FRAME_BUFFER_ALIGN);
//AT_NONCACHEABLE_SECTION_ALIGN(static uint32_t s_psBufferPxp[APP_PS_SIZE][APP_PS_SIZE], FRAME_BUFFER_ALIGN);


/* PXP Output buffer config. */
static pxp_output_buffer_config_t outputBufferConfig;
void BOARD_InitLcdifPixelClock(void);
void BOARD_InitLcd(void);
void LCD_APP_Test(void);

#endif /* _LCD_H_ */
