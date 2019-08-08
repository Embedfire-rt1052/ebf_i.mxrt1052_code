#ifndef _FSL_OV5640_H_
#define _FSL_OV5640_H_

#include "fsl_common.h"
#include "fsl_sccb.h"
#include "fsl_camera_device.h"

/*******************************************************************************
 * ��غ궨��
 ******************************************************************************/
#define OV5640_SCCB_ADDR 0x3cU
#define OV5640_REVISION 0x5640U

#define OV5640_WriteReg(i2c, reg, val) SCCB_WriteReg(i2c, OV5640_SCCB_ADDR, kSCCB_RegAddr16Bit, (reg), (val))

#define OV5640_ReadReg(i2c, reg, val) SCCB_ReadReg(i2c, OV5640_SCCB_ADDR, kSCCB_RegAddr16Bit, (reg), (val))

#define OV5640_ModifyReg(i2c, reg, clrMask, val) \
    SCCB_ModifyReg(i2c, OV5640_SCCB_ADDR, kSCCB_RegAddr16Bit, (reg), (clrMask), (val))
	
#define OV5640_PID_REG 0x300AU    
#define OV5640_VER_REG 0x300BU  
#define FRAME_RATE_30FPS	0 //30֡
#define FRAME_RATE_15FPS	1 //15֡

/*******************************************************************************
 * ��ؽṹ��
 ******************************************************************************/
typedef struct _ov5640_reg
{
    uint16_t reg;
    uint8_t val;
} ov5640_reg_t;

/*!
* @brief OV5640 ��Դ�ṹ��
 */
typedef struct _ov5640_resource
{
    sccb_i2c_t sccbI2C;                    /* SCCB ͨ��. */
    void (*pullResetPin)(bool pullUp);     /*��λ���ſ��ƺ���. */
    void (*pullPowerDownPin)(bool pullUp); /*PDN���ſ��ƺ���. */
    uint32_t inputClockFreq_Hz;            /*����ʱ��Ƶ��. */
} ov5640_resource_t;

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
