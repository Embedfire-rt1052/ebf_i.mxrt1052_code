/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_OV5640_H
#define __BSP_OV5640_H

/* Includes ------------------------------------------------------------------*/
#include "fsl_common.h"
#include "fsl_csi.h"
#include "fsl_ov5640.h"
#include "fsl_elcdif.h"
#include "fsl_camera.h"
#include "fsl_camera_receiver.h"
#include "fsl_camera_device.h"
#include "fsl_gpio.h"
#include "pin_mux.h"
#include "fsl_csi_camera_adapter.h"
#include ".\lcd\lcd.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern camera_device_handle_t cameraDevice;
extern camera_receiver_handle_t cameraReceiver;

/* Camera definition. */
#define APP_CAMERA_HEIGHT 480
#define APP_CAMERA_WIDTH 800
#define APP_CAMERA_CONTROL_FLAGS (kCAMERA_HrefActiveLow | kCAMERA_DataLatchOnRisingEdge)

/* Frame buffer data alignment. */
#define FRAME_BUFFER_ALIGN 64


#define APP_FRAME_BUFFER_COUNT 4
/* Pixel format RGB565, bytesPerPixel is 2. */
#define APP_BPP 2

#define FRAME_RATE_30FPS	0 //30֡
#define FRAME_RATE_15FPS	1 //15֡

#define OV5640_I2C LPI2C1

#define Delay(ms)  Delay_ms(ms)

AT_NONCACHEABLE_SECTION_ALIGN(static uint16_t s_frameBuffer[APP_FRAME_BUFFER_COUNT][APP_IMG_HEIGHT][APP_IMG_WIDTH],
                              FRAME_BUFFER_ALIGN);

/* Exported constants --------------------------------------------------------*/
#define DCMI_DR_ADDRESS       (uint32_t)&DCMI->DR
#define FSMC_LCD_ADDRESS      FSMC_Addr_ILI9806G_DATA


typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;

/*����ͷ���ýṹ��*/
typedef struct
{	
	uint8_t frame_rate;	//���֡��
	
	uint16_t cam_isp_sx; //����ͷISP X��ʼλ��
	uint16_t cam_isp_sy; //����ͷISP Y��ʼλ��

	uint16_t cam_isp_width; //����ͷISP ��
	uint16_t cam_isp_height; //����ͷISP ��

	uint8_t scaling;				//�Ƿ�ʹ���Զ����ţ��Ƽ�ʹ�ã�1:ʹ�ã�0:��ʹ��
	
	uint16_t cam_out_sx; //����ͷ�������X��ʼλ��
	uint16_t cam_out_sy; //����ͷ�������Y��ʼλ��
	
	uint16_t cam_out_width;//���ͼ��ֱ��ʣ���
	uint16_t cam_out_height;//���ͼ��ֱ��ʣ���
	
	uint16_t lcd_sx;//ͼ����ʾ��Һ������X��ʼλ��
	uint16_t lcd_sy;//ͼ����ʾ��Һ������Y��ʼλ��
	uint8_t lcd_scan;//Һ������ɨ��ģʽ��0-7��
	
	uint8_t light_mode;//����ģʽ��������Χ[0~4]
	int8_t saturation;//���Ͷ�,������Χ[-3 ~ +3]   
	int8_t brightness;//���նȣ�������Χ[-4~+4]
	int8_t contrast;//�Աȶȣ�������Χ[-3~+3]
	uint8_t effect;	//����Ч����������Χ[0~9]:	
	int8_t exposure;//�عⲹ����������Χ[-3~+3]

	
	uint8_t auto_focus;//�Ƿ�ʹ���Զ��Խ����� 1:ʹ�ã�0:��ʹ��

}OV5640_MODE_PARAM;


extern OV5640_MODE_PARAM cam_mode;

/* Exported types ------------------------------------------------------------*/
//�洢����ͷID�Ľṹ��
typedef struct
{
  uint8_t PIDH;
  uint8_t PIDL;
}OV5640_IDTypeDef;

/* Exported constants --------------------------------------------------------*/

/* Use this define to set the maximum delay timeout for the I2C DCMI_OV5640_SingleRandomWrite()
   and DCMI_OV5640_SingleRandomRead() operations. Exeeding this timeout delay, 
   the read/write functions will be aborted and return error code (0xFF).
   The period of the delay will depend on the system operating frequency. The following
   value has been set for system running at 168 MHz. */
#define DCMI_TIMEOUT_MAX               10000

#define OV5640_DEVICE_ADDRESS    0x78

/* OV5640 Registers definition when DSP bank selected (0xFF = 0x00) */
#define OV5640_DSP_R_BYPASS     0x05
#define OV5640_DSP_Qs           0x44
#define OV5640_DSP_CTRL         0x50
#define OV5640_DSP_HSIZE1       0x51
#define OV5640_DSP_VSIZE1       0x52
#define OV5640_DSP_XOFFL        0x53
#define OV5640_DSP_YOFFL        0x54
#define OV5640_DSP_VHYX         0x55
#define OV5640_DSP_DPRP         0x56
#define OV5640_DSP_TEST         0x57
#define OV5640_DSP_ZMOW         0x5A
#define OV5640_DSP_ZMOH         0x5B
#define OV5640_DSP_ZMHH         0x5C
#define OV5640_DSP_BPADDR       0x7C
#define OV5640_DSP_BPDATA       0x7D
#define OV5640_DSP_CTRL2        0x86
#define OV5640_DSP_CTRL3        0x87
#define OV5640_DSP_SIZEL        0x8C
#define OV5640_DSP_HSIZE2       0xC0
#define OV5640_DSP_VSIZE2       0xC1
#define OV5640_DSP_CTRL0        0xC2
#define OV5640_DSP_CTRL1        0xC3
#define OV5640_DSP_R_DVP_SP     0xD3
#define OV5640_DSP_IMAGE_MODE   0xDA
#define OV5640_DSP_RESET        0xE0
#define OV5640_DSP_MS_SP        0xF0
#define OV5640_DSP_SS_ID        0x7F
#define OV5640_DSP_SS_CTRL      0xF8
#define OV5640_DSP_MC_BIST      0xF9
#define OV5640_DSP_MC_AL        0xFA
#define OV5640_DSP_MC_AH        0xFB
#define OV5640_DSP_MC_D         0xFC
#define OV5640_DSP_P_STATUS     0xFE
#define OV5640_DSP_RA_DLMT      0xFF

/* OV5640 Registers definition when sensor bank selected (0xFF = 0x01) */
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

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void OV5640_HW_Init(void);
void OV5640_Reset(void);
void OV5640_ReadID(OV5640_IDTypeDef *OV5640ID);
void OV5640_Init(void);
void OV5640_RGB565_Default_Config(void);
void OV5640_USER_Config(void);
void OV5640_Capture_Control(FunctionalState state);

void OV5640_FrameRate_Set(uint8_t frame_rate);

void OV5640_BrightnessConfig(int8_t Brightness);
void OV5640_ContrastConfig(int8_t cnst);
void OV5640_Color_Saturation(int8_t sat);
void OV5640_ColorEffectsConfig(uint8_t value1, uint8_t value2);
void OV5640_LightMode(uint8_t mode);
void OV5640_SpecialEffects(uint8_t mode);
void OV5640_Exposure(int8_t ev);


uint8_t OV5640_WriteFW(uint8_t *pBuffer ,uint16_t BufferSize);

void OV5640_ISPSize_Set(uint16_t x_st,uint16_t y_st,uint16_t width,uint16_t height);
void OV5640_OutSize_Set(uint8_t scaling,uint16_t x_off,uint16_t y_off,uint16_t width,uint16_t height);


extern void Camera_Init(void);

#endif /* __BSP_OV5640_H */
