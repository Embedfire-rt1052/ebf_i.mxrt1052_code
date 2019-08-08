#ifndef __BSP_OV5640_H
#define __BSP_OV5640_H

#include "fsl_common.h"
#include "fsl_csi.h"
#include "bsp_ov2640_config.h"
#include "fsl_elcdif.h"
#include "fsl_camera.h"
#include "fsl_camera_receiver.h"
#include "fsl_camera_device.h"
#include "fsl_gpio.h"
#include "pin_mux.h"
#include "fsl_csi_camera_adapter.h"
#include ".\lcd\bsp_camera_lcd.h"

/*******************************************************************************
 * 相关宏定义
 ******************************************************************************/
#define APP_CAMERA_CONTROL_FLAGS (kCAMERA_HrefActiveHigh | kCAMERA_DataLatchOnRisingEdge)

/* 帧缓冲区数据对齐 . */
#define FRAME_BUFFER_ALIGN 64
#define APP_FRAME_BUFFER_COUNT 4
/* 像素格式RGB565，bytesPerPixel为2. */
#define APP_BPP 2
/* IIC . */
#define OV2640_I2C LPI2C1

/* 摄像头复位引脚 */
#define CAMERA_RST_GPIO 			GPIO1
#define CAMERA_RST_GPIO_PIN 	1

/* 摄像头电源引脚 */
#define CAMERA_PWR_GPIO 			GPIO1
#define CAMERA_PWR_GPIO_PIN 	0

#define Delay(ms)  Delay_ms(ms)

AT_NONCACHEABLE_SECTION_ALIGN(static uint16_t s_frameBuffer[APP_FRAME_BUFFER_COUNT][APP_IMG_HEIGHT][APP_IMG_WIDTH],
                              FRAME_BUFFER_ALIGN);

#define OV2640_DSP_RA_DLMT      0xFF
#define OV2640_SENSOR_GAIN       0x00
#define OV2640_SENSOR_COM1       0x03
#define OV2640_SENSOR_REG04      0x04
#define OV2640_SENSOR_REG08      0x08
#define OV2640_SENSOR_COM2       0x09

#define OV2640_SENSOR_PIDH       0x0A
#define OV2640_SENSOR_PIDL       0x0B

#define OV2640_SENSOR_COM3       0x0C
#define OV2640_SENSOR_COM4       0x0D
#define OV2640_SENSOR_AEC        0x10
#define OV2640_SENSOR_CLKRC      0x11
#define OV2640_SENSOR_COM7       0x12
#define OV2640_SENSOR_COM8       0x13
#define OV2640_SENSOR_COM9       0x14
#define OV2640_SENSOR_COM10      0x15
#define OV2640_SENSOR_HREFST     0x17
#define OV2640_SENSOR_HREFEND    0x18
#define OV2640_SENSOR_VSTART     0x19
#define OV2640_SENSOR_VEND       0x1A

#define OV2640_SENSOR_MIDH       0x1C
#define OV2640_SENSOR_MIDL       0x1D


#define OV2640_SENSOR_AEW        0x24
#define OV2640_SENSOR_AEB        0x25
#define OV2640_SENSOR_W          0x26
#define OV2640_SENSOR_REG2A      0x2A
#define OV2640_SENSOR_FRARL      0x2B
#define OV2640_SENSOR_ADDVSL     0x2D
#define OV2640_SENSOR_ADDVHS     0x2E
#define OV2640_SENSOR_YAVG       0x2F
#define OV2640_SENSOR_REG32      0x32
#define OV2640_SENSOR_ARCOM2     0x34
#define OV2640_SENSOR_REG45      0x45
#define OV2640_SENSOR_FLL        0x46
#define OV2640_SENSOR_FLH        0x47
#define OV2640_SENSOR_COM19      0x48
#define OV2640_SENSOR_ZOOMS      0x49
#define OV2640_SENSOR_COM22      0x4B
#define OV2640_SENSOR_COM25      0x4E
#define OV2640_SENSOR_BD50       0x4F
#define OV2640_SENSOR_BD60       0x50
#define OV2640_SENSOR_REG5D      0x5D
#define OV2640_SENSOR_REG5E      0x5E
#define OV2640_SENSOR_REG5F      0x5F
#define OV2640_SENSOR_REG60      0x60
#define OV2640_SENSOR_HISTO_LOW  0x61
#define OV2640_SENSOR_HISTO_HIGH 0x62
#define OV5640_SENSOR_GAIN       0x00
#define OV5640_SENSOR_COM1       0x03
#define OV5640_SENSOR_REG04      0x04
#define OV5640_SENSOR_REG08      0x08
#define OV5640_SENSOR_COM2       0x09

#define OV5640_SENSOR_PIDH       0x300A
#define OV5640_SENSOR_PIDL       0x300B

#define OV5640_SENSOR_COM3       0x0C
#define OV5640_SENSOR_COM4       0x0D
#define OV5640_SENSOR_AEC        0x10
#define OV5640_SENSOR_CLKRC      0x11
#define OV5640_SENSOR_COM7       0x12
#define OV5640_SENSOR_COM8       0x13
#define OV5640_SENSOR_COM9       0x14
#define OV5640_SENSOR_COM10      0x15
#define OV5640_SENSOR_HREFST     0x17
#define OV5640_SENSOR_HREFEND    0x18
#define OV5640_SENSOR_VSTART     0x19
#define OV5640_SENSOR_VEND       0x1A


#define OV5640_SENSOR_MIDH       0x1C
#define OV5640_SENSOR_MIDL       0x1D
   
#define OV5640_SENSOR_AEW        0x24
#define OV5640_SENSOR_AEB        0x25
#define OV5640_SENSOR_W          0x26
#define OV5640_SENSOR_REG2A      0x2A
#define OV5640_SENSOR_FRARL      0x2B
#define OV5640_SENSOR_ADDVSL     0x2D
#define OV5640_SENSOR_ADDVHS     0x2E
#define OV5640_SENSOR_YAVG       0x2F
#define OV5640_SENSOR_REG32      0x32
#define OV5640_SENSOR_ARCOM2     0x34
#define OV5640_SENSOR_REG45      0x45
#define OV5640_SENSOR_FLL        0x46
#define OV5640_SENSOR_FLH        0x47
#define OV5640_SENSOR_COM19      0x48
#define OV5640_SENSOR_ZOOMS      0x49
#define OV5640_SENSOR_COM22      0x4B
#define OV5640_SENSOR_COM25      0x4E
#define OV5640_SENSOR_BD50       0x4F
#define OV5640_SENSOR_BD60       0x50
#define OV5640_SENSOR_REG5D      0x5D
#define OV5640_SENSOR_REG5E      0x5E
#define OV5640_SENSOR_REG5F      0x5F
#define OV5640_SENSOR_REG60      0x60
#define OV5640_SENSOR_HISTO_LOW  0x61
#define OV5640_SENSOR_HISTO_HIGH 0x62

#define CAMERA_DEBUG_ON          1
#define CAMERA_DEBUG_ARRAY_ON   1
#define CAMERA_DEBUG_FUNC_ON    1

#define CAMERA_INFO(fmt,arg...)           PRINTF("<<-CAMERA-INFO->> "fmt"\n",##arg)
#define CAMERA_ERROR(fmt,arg...)          PRINTF("<<-CAMERA-ERROR->> "fmt"\n",##arg)
#define CAMERA_DEBUG(fmt,arg...)          do{\
                                         if(CAMERA_DEBUG_ON)\
                                         PRINTF("<<-CAMERA-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                       }while(0)
#define CAMERA_DEBUG_ARRAY(array, num)    do{\
                                         int32_t i;\
                                         uint8_t* a = array;\
                                         if(CAMERA_DEBUG_ARRAY_ON)\
                                         {\
                                            PRINTF("<<-CAMERA-DEBUG-ARRAY->>\n");\
                                            for (i = 0; i < (num); i++)\
                                            {\
                                                PRINTF("%02x   ", (a)[i]);\
                                                if ((i + 1 ) %10 == 0)\
                                                {\
                                                    printf("\n");\
                                                }\
                                            }\
                                            PRINTF("\n");\
                                        }\
                                       }while(0)
#define CAMERA_DEBUG_FUNC()               do{\
                                         if(CAMERA_DEBUG_FUNC_ON)\
                                         PRINTF("<<-CAMERA-FUNC->> Func:%s@Line:%d\n",__func__,__LINE__);\
                                       }while(0)


uint8_t OV5640_WriteFW(uint8_t *pBuffer ,uint16_t BufferSize);
extern void BOARD_InitCameraResource(void);
extern camera_device_handle_t cameraDevice;
extern camera_receiver_handle_t cameraReceiver;
extern void Camera_Init(void);
extern int index_num;
extern void Cam_Config_Switch(void);
#endif /* __BSP_OV2640_H */
