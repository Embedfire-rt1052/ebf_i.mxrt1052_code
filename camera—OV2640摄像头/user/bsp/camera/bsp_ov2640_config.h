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

#ifndef _FSL_OV2640_WriteRegs_H_
#define _FSL_OV2640_WriteRegs_H_

#include "fsl_common.h"
#include "fsl_sccb.h"
#include "fsl_camera_device.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define OV2640_SCCB_ADDR 0x3cU
#define OV2640_REVISION 0x5640U

#define OV2640_WriteReg(i2c, reg, val) SCCB_WriteReg(i2c, OV2640_SCCB_ADDR, kSCCB_RegAddr16Bit, (reg), (val))

#define OV2640_ReadReg(i2c, reg, val) SCCB_ReadReg(i2c, OV2640_SCCB_ADDR, kSCCB_RegAddr16Bit, (reg), (val))

#define OV2640_ModifyReg(i2c, reg, clrMask, val) \
    SCCB_ModifyReg(i2c, OV2640_SCCB_ADDR, kSCCB_RegAddr16Bit, (reg), (clrMask), (val))

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define OV2640_WriteRegs_PID_REG 0x300AU        /*!< Product ID Number MSB */
#define OV2640_WriteRegs_VER_REG 0x300BU        /*!< Product ID Number LSB */

typedef struct _OV2640_WriteRegs_reg
{
    uint16_t reg;
    uint8_t val;
} OV2640_WriteRegs_reg_t;

/*!
 * @brief 2640 resource.
 *
 * Before initialize the OV2640_WriteRegs, the resource must be initialized that the SCCB
 * I2C could start to work.
 */

typedef struct _ov2640_resource
{
    sccb_i2c_t sccbI2C;                    /*!< I2C for SCCB transfer. */
    void (*pullResetPin)(bool pullUp);     /*!< ���ڽ���λ�������߻����͵Ĺ���. */
    void (*pullPowerDownPin)(bool pullUp); /*!< ���ڽ���Դ�������߻����͵Ĺ���. */
    uint32_t inputClockFreq_Hz;            /*!< ����ʱ��Ƶ��. */
} ov2640_resource_t;

extern uint32_t activeFrameAddr;
extern uint32_t inactiveFrameAddr;

/*! @brief OV2640_WriteRegs operation functions. */
extern const camera_device_operations_t ov2640_ops;
extern void CSI_DriverIRQHandler(void);


#define FRAME_RATE_30FPS	0 //30֡
#define FRAME_RATE_15FPS	1 //15֡

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
	
	int lcd_sx;//ͼ����ʾ��Һ������X��ʼλ��
	int lcd_sy;//ͼ����ʾ��Һ������Y��ʼλ��
	uint8_t lcd_scan;//Һ������ɨ��ģʽ��0-7��
	
	uint8_t light_mode;//����ģʽ��������Χ[0~4]
	int8_t saturation;//���Ͷ�,������Χ[-3 ~ +3]   
	int8_t brightness;//���նȣ�������Χ[-4~+4]
	int8_t contrast;//�Աȶȣ�������Χ[-3~+3]
	uint8_t effect;	//����Ч����������Χ[0~9]:	
	int8_t exposure;//�عⲹ����������Χ[-3~+3]

	
	uint8_t auto_focus;//�Ƿ�ʹ���Զ��Խ����� 1:ʹ�ã�0:��ʹ��

}OV2640_WriteRegs_MODE_PARAM;
extern OV2640_WriteRegs_MODE_PARAM Set_Cam_mode(int temp);
extern OV2640_WriteRegs_MODE_PARAM cam_mode;
extern OV2640_WriteRegs_MODE_PARAM cam_mode_240_320;
extern OV2640_WriteRegs_MODE_PARAM cam_mode_640_480;
extern OV2640_WriteRegs_MODE_PARAM cam_mode_800_480;
/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(__cplusplus)
}
#endif

#endif /* _FSL_OV2640_WriteRegs_H_ */
