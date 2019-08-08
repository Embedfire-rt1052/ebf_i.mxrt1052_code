#ifndef _FSL_OV5640_H_
#define _FSL_OV5640_H_

#include "fsl_common.h"
#include "fsl_sccb.h"
#include "fsl_camera_device.h"

/*******************************************************************************
 * 相关宏定义
 ******************************************************************************/
#define OV5640_SCCB_ADDR 0x3cU
#define OV5640_REVISION 0x5640U

#define OV5640_WriteReg(i2c, reg, val) SCCB_WriteReg(i2c, OV5640_SCCB_ADDR, kSCCB_RegAddr16Bit, (reg), (val))

#define OV5640_ReadReg(i2c, reg, val) SCCB_ReadReg(i2c, OV5640_SCCB_ADDR, kSCCB_RegAddr16Bit, (reg), (val))

#define OV5640_ModifyReg(i2c, reg, clrMask, val) \
    SCCB_ModifyReg(i2c, OV5640_SCCB_ADDR, kSCCB_RegAddr16Bit, (reg), (clrMask), (val))
	
#define OV5640_PID_REG 0x300AU    
#define OV5640_VER_REG 0x300BU  
#define FRAME_RATE_30FPS	0 //30帧
#define FRAME_RATE_15FPS	1 //15帧

/*******************************************************************************
 * 相关结构体
 ******************************************************************************/
typedef struct _ov5640_reg
{
    uint16_t reg;
    uint8_t val;
} ov5640_reg_t;

/*!
* @brief OV5640 资源结构体
 */
typedef struct _ov5640_resource
{
    sccb_i2c_t sccbI2C;                    /* SCCB 通道. */
    void (*pullResetPin)(bool pullUp);     /*复位引脚控制函数. */
    void (*pullPowerDownPin)(bool pullUp); /*PDN引脚控制函数. */
    uint32_t inputClockFreq_Hz;            /*输入时钟频率. */
} ov5640_resource_t;

/*摄像头配置结构体*/
typedef struct
{	
	uint8_t frame_rate;	//输出帧率
	
	uint16_t cam_isp_sx; //摄像头ISP X起始位置
	uint16_t cam_isp_sy; //摄像头ISP Y起始位置

	uint16_t cam_isp_width; //摄像头ISP 宽
	uint16_t cam_isp_height; //摄像头ISP 高

	uint8_t scaling;				//是否使用自动缩放，推荐使用，1:使用，0:不使用
	
	uint16_t cam_out_sx; //摄像头输出窗口X起始位置
	uint16_t cam_out_sy; //摄像头输出窗口Y起始位置
	
	uint16_t cam_out_width;//输出图像分辨率，宽
	uint16_t cam_out_height;//输出图像分辨率，高
	
	int lcd_sx;//图像显示在液晶屏的X起始位置
	int lcd_sy;//图像显示在液晶屏的Y起始位置
	uint8_t lcd_scan;//液晶屏的扫描模式（0-7）
	
	uint8_t light_mode;//光照模式，参数范围[0~4]
	int8_t saturation;//饱和度,参数范围[-3 ~ +3]   
	int8_t brightness;//光照度，参数范围[-4~+4]
	int8_t contrast;//对比度，参数范围[-3~+3]
	uint8_t effect;	//特殊效果，参数范围[0~9]:	
	int8_t exposure;//曝光补偿，参数范围[-3~+3]

	
	uint8_t auto_focus;//是否使用自动对焦功能 1:使用，0:不使用

}OV5640_MODE_PARAM;
extern OV5640_MODE_PARAM Set_Cam_mode(int temp);
extern OV5640_MODE_PARAM cam_mode;
extern OV5640_MODE_PARAM cam_mode_240_320;
extern OV5640_MODE_PARAM cam_mode_640_480;
extern OV5640_MODE_PARAM cam_mode_800_480;
extern uint32_t activeFrameAddr;
extern uint32_t inactiveFrameAddr;
extern const camera_device_operations_t ov5640_ops;
extern void CSI_DriverIRQHandler(void);

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(__cplusplus)
}
#endif

#endif /* _BSP_OV5640_H_ */
