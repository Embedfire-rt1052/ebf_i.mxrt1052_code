/*
 * Copyright (c) 2017, NXP Semiconductors, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _FSL_OV5640_H_
#define _FSL_OV5640_H_

#include "fsl_common.h"
#include "fsl_sccb.h"
#include "fsl_camera_device.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define OV5640_SCCB_ADDR 0x3cU
#define OV5640_REVISION 0x5640U

#define OV5640_WriteReg(i2c, reg, val) SCCB_WriteReg(i2c, OV5640_SCCB_ADDR, kSCCB_RegAddr16Bit, (reg), (val))

#define OV5640_ReadReg(i2c, reg, val) SCCB_ReadReg(i2c, OV5640_SCCB_ADDR, kSCCB_RegAddr16Bit, (reg), (val))

#define OV5640_ModifyReg(i2c, reg, clrMask, val) \
    SCCB_ModifyReg(i2c, OV5640_SCCB_ADDR, kSCCB_RegAddr16Bit, (reg), (clrMask), (val))
	
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define OV5640_PID_REG 0x300AU        /*!< Product ID Number MSB */
#define OV5640_VER_REG 0x300BU        /*!< Product ID Number LSB */

typedef struct _ov5640_reg
{
    uint16_t reg;
    uint8_t val;
} ov5640_reg_t;

/*!
 * @brief OV5640 resource.
 *
 * Before initialize the OV5640, the resource must be initialized that the SCCB
 * I2C could start to work.
 */
typedef struct _ov5640_resource
{
    sccb_i2c_t sccbI2C;                    /*!< I2C for SCCB transfer. */
    void (*pullResetPin)(bool pullUp);     /*!< Function to pull reset pin high or low. */
    void (*pullPowerDownPin)(bool pullUp); /*!< Function to pull the power down pin high or low. */
    uint32_t inputClockFreq_Hz;            /*!< Input clock frequency. */
} ov5640_resource_t;

extern uint32_t activeFrameAddr;
extern uint32_t inactiveFrameAddr;

/*! @brief OV5640 operation functions. */
extern const camera_device_operations_t ov5640_ops;
extern void CSI_DriverIRQHandler(void);

#define FRAME_RATE_30FPS	0 //30帧
#define FRAME_RATE_15FPS	1 //15帧

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
	
	uint16_t lcd_sx;//图像显示在液晶屏的X起始位置
	uint16_t lcd_sy;//图像显示在液晶屏的Y起始位置
	uint8_t lcd_scan;//液晶屏的扫描模式（0-7）
	
	uint8_t light_mode;//光照模式，参数范围[0~4]
	int8_t saturation;//饱和度,参数范围[-3 ~ +3]   
	int8_t brightness;//光照度，参数范围[-4~+4]
	int8_t contrast;//对比度，参数范围[-3~+3]
	uint8_t effect;	//特殊效果，参数范围[0~9]:	
	int8_t exposure;//曝光补偿，参数范围[-3~+3]

	
	uint8_t auto_focus;//是否使用自动对焦功能 1:使用，0:不使用

}OV5640_MODE_PARAM;
extern void Set_Cam_mode(int temp);
extern OV5640_MODE_PARAM cam_mode;
extern OV5640_MODE_PARAM cam_mode_240_320;
extern OV5640_MODE_PARAM cam_mode_640_480;
extern OV5640_MODE_PARAM cam_mode_800_480;
/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(__cplusplus)
}
#endif

#endif /* _FSL_OV5640_H_ */
