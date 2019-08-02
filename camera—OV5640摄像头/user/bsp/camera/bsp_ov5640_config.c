/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   摄像头驱动
  ******************************************************************
  * @attention
  *
  * 实验平台:野火  i.MXRT1052开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************
  */
#include "fsl_video_common.h"
#include "fsl_camera.h"
#include "fsl_camera_device.h"
#include "bsp_ov5640_config.h"
#include "./delay/core_delay.h"  

extern OV5640_MODE_PARAM cam_temp_mode;
//摄像头初始化配置
//注意：使用这种方式初始化结构体，要在c/c++选项中选择 C99 mode
OV5640_MODE_PARAM cam_mode_800_480 =
{
/************配置1***800*480******横屏显示*****************************/
	
	.frame_rate = FRAME_RATE_30FPS,	
	
	//ISP窗口
	.cam_isp_sx = 0,
	.cam_isp_sy = 0,	
	
	.cam_isp_width = 1920,
	.cam_isp_height = 1080,
	
	//输出窗口
	.scaling = 0,      //使能自动缩放
	.cam_out_sx = 16,	//使能自动缩放后，一般配置成16即可
	.cam_out_sy = 4,	  //使能自动缩放后，一般配置成4即可
	.cam_out_width = 800,
	.cam_out_height = 480,
	
	//LCD位置
	.lcd_sx = 0,
	.lcd_sy = 0,
	.lcd_scan = 7, //LCD扫描模式，本横屏配置可用1、3、5、7模式
	
	//以下可根据自己的需要调整，参数范围见结构体类型定义	
	.light_mode = 0,//自动光照模式
	.saturation = 0,	
	.brightness = 0,
	.contrast = 0,
	.effect = 0,		//正常模式
	.exposure = 0,		

	.auto_focus = 1,
};

//摄像头初始化配置
//注意：使用这种方式初始化结构体，要在c/c++选项中选择 C99 mode
OV5640_MODE_PARAM cam_mode_640_480 =
{
//	/*******配置3********640*480****小分辨率****************************/	
  .frame_rate = FRAME_RATE_30FPS,	
	
	//ISP窗口
	.cam_isp_sx = 0,
	.cam_isp_sy = 0,	
	
	.cam_isp_width = 1920,
	.cam_isp_height = 1080,
	
	//输出窗口
	.scaling = 1,      //使能自动缩放
	.cam_out_sx = 16,	//使能自动缩放后，一般配置成16即可
	.cam_out_sy = 4,	  //使能自动缩放后，一般配置成4即可
	.cam_out_width = 640,
	.cam_out_height = 480,
	
	//LCD位置
	.lcd_sx = 0,
	.lcd_sy = 0,
	.lcd_scan = 5, //LCD扫描模式，
	
	//以下可根据自己的需要调整，参数范围见结构体类型定义	
	.light_mode = 0,//自动光照模式
	.saturation = 0,	
	.brightness = 0,
	.contrast = 0,
	.effect = 0,		//正常模式
	.exposure = 0,		

	.auto_focus = 1,//自动对焦
};

OV5640_MODE_PARAM cam_mode_240_320 =
{
/**********配置2*****240*320****竖屏显示****************************/	
	.frame_rate = FRAME_RATE_30FPS,	
	
	//ISP窗口
	.cam_isp_sx = 0,
	.cam_isp_sy = 0,	
	
	.cam_isp_width = 1920,
	.cam_isp_height = 1080,
	
	//输出窗口
	.scaling = 1,      //使能自动缩放
	.cam_out_sx = 16,	//使能自动缩放后，一般配置成16即可
	.cam_out_sy = 4,	  //使能自动缩放后，一般配置成4即可
	.cam_out_width =240,	//240   320
	.cam_out_height =320,	//320   240
	
	//LCD位置
	.lcd_sx = 100,
	.lcd_sy = 100,
	.lcd_scan = 5, //LCD扫描模式，
	
	//以下可根据自己的需要调整，参数范围见结构体类型定义	
	.light_mode = 0,//自动光照模式
	.saturation = 0,	
	.brightness = 0,
	.contrast = 0,
	.effect = 0,		//正常模式
	.exposure = 0,		

	.auto_focus = 1,//自动对焦
};
OV5640_MODE_PARAM cam_mode_480_320 =
{
	.frame_rate = FRAME_RATE_30FPS,	
	
	//ISP窗口
	.cam_isp_sx = 0,
	.cam_isp_sy = 0,	
	
	.cam_isp_width = 1920,
	.cam_isp_height = 1080,
	
	//输出窗口
	.scaling = 1,      //使能自动缩放
	.cam_out_sx = 16,	//使能自动缩放后，一般配置成16即可
	.cam_out_sy = 4,	  //使能自动缩放后，一般配置成4即可
	.cam_out_width =480,	//240   320
	.cam_out_height =320,	//320   240
	
	//LCD位置
	.lcd_sx = 100,
	.lcd_sy = 100,
	.lcd_scan = 5, //LCD扫描模式，
	
	//以下可根据自己的需要调整，参数范围见结构体类型定义	
	.light_mode = 0,//自动光照模式
	.saturation = 0,	
	.brightness = 0,
	.contrast = 0,
	.effect = 0,		//正常模式
	.exposure = 0,		

	.auto_focus = 1,//自动对焦
};



/*******************************************************************************
 * Code
 ******************************************************************************/

static void OV5640_DelayMs(uint32_t ms)
{

	while (ms--)
	{
		for (volatile int i = 0U; i < 10000000U; i++)
		{
			__ASM("nop");
		}
	}
}

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
status_t OV5640_Init(camera_device_handle_t *handle, const camera_config_t *config);

status_t OV5640_InitExt(camera_device_handle_t *handle, const camera_config_t *config, const void *specialConfig);

status_t OV5640_Start(camera_device_handle_t *handle);

status_t OV5640_Stop(camera_device_handle_t *handle);

/*******************************************************************************
 * Variables
 ******************************************************************************/
const camera_device_operations_t ov5640_ops = {
    .init = OV5640_Init,
    .start = OV5640_Start,
    .stop = OV5640_Stop,
    .init_ext = OV5640_InitExt,
};

static const ov5640_reg_t ov5640InitRegs[] = {
    //15fps VGA RGB565 output
    // 24MHz input clock, 24MHz PCLK
    0x3103, 0x11, // system clock from pad, bit[1]
    0x3008, 0x82, // software reset, bit[7]
    // OV5640_DelayMs 5ms
    0x3008, 0x42, // software power down, bit[6]
    0x3103, 0x03, // system clock from PLL, bit[1]
    0x3017, 0xff, // FREX, Vsync, HREF, PCLK, D[9:6] output enable
    0x3018, 0xff, // D[5:0], GPIO[1:0] output enable
    0x3034, 0x1a, // MIPI 10-bit
    0x3037, 0x13, // PLL root divider, bit[4], PLL pre-divider, bit[3:0]
    0x3108, 0x01, // PCLK root divider, bit[5:4], SCLK2x root divider, bit[3:2]
    // SCLK root divider, bit[1:0]
    0x3630, 0x36,
    0x3631, 0x0e,
    0x3632, 0xe2,
    0x3633, 0x12,
    0x3621, 0xe0,
    0x3704, 0xa0,
    0x3703, 0x5a,
    0x3715, 0x78,
    0x3717, 0x01,
    0x370b, 0x60,
    0x3705, 0x1a,
    0x3905, 0x02,
    0x3906, 0x10,
    0x3901, 0x0a,
    0x3731, 0x12,
    0x3600, 0x08, // VCM control
    0x3601, 0x33, // VCM control
    0x302d, 0x60, // system control
    0x3620, 0x52,
    0x371b, 0x20,
    0x471c, 0x50,
    0x3a13, 0x43, // pre-gain = 1.047x
    0x3a18, 0x00, // gain ceiling
    0x3a19, 0xf8, // gain ceiling = 15.5x
    0x3635, 0x13,
    0x3636, 0x03,
    0x3634, 0x40,
    0x3622, 0x01,
    // 50/60Hz detection 50/60Hz 灯光条纹过滤
    0x3c01, 0x34, // Band auto, bit[7]
    0x3c04, 0x28, // threshold low sum
    0x3c05, 0x98, // threshold high sum
    0x3c06, 0x00, // light meter 1 threshold[15:8]
    0x3c07, 0x08, // light meter 1 threshold[7:0]
    0x3c08, 0x00, // light meter 2 threshold[15:8]
    0x3c09, 0x1c, // light meter 2 threshold[7:0]
    0x3c0a, 0x9c, // sample number[15:8]
    0x3c0b, 0x40, // sample number[7:0]
    0x3810, 0x00, // Timing Hoffset[11:8]
    0x3811, 0x10, // Timing Hoffset[7:0]
    0x3812, 0x00, // Timing Voffset[10:8]
    0x3708, 0x64,
    0x4001, 0x02, // BLC start from line 2
    0x4005, 0x1a, // BLC always update
    0x3000, 0x00, // enable blocks
    0x3004, 0xff, // enable clocks
    0x300e, 0x58, // MIPI power down, DVP enable
    0x302e, 0x00,

    0x4300, 0x6f, // RGB565
    0x501f, 0x01, // RGB565

    0x440e, 0x00,
    0x5000, 0xa7, // Lenc on, raw gamma on, BPC on, WPC on, CIP on
    // AEC target 自动曝光控制
    0x3a0f, 0x30, // stable range in high
    0x3a10, 0x28, // stable range in low
    0x3a1b, 0x30, // stable range out high
    0x3a1e, 0x26, // stable range out low
    0x3a11, 0x60, // fast zone high
    0x3a1f, 0x14, // fast zone low
    // Lens correction for ? 镜头补偿
    0x5800, 0x23,
    0x5801, 0x14,
    0x5802, 0x0f,
    0x5803, 0x0f,
    0x5804, 0x12,
    0x5805, 0x26,
    0x5806, 0x0c,
    0x5807, 0x08,
    0x5808, 0x05,
    0x5809, 0x05,
    0x580a, 0x08,
    0x580b, 0x0d,
    0x580c, 0x08,
    0x580d, 0x03,
    0x580e, 0x00,
    0x580f, 0x00,
    0x5810, 0x03,
    0x5811, 0x09,
    0x5812, 0x07,
    0x5813, 0x03,
    0x5814, 0x00,
    0x5815, 0x01,
    0x5816, 0x03,
    0x5817, 0x08,
    0x5818, 0x0d,
    0x5819, 0x08,
    0x581a, 0x05,
    0x581b, 0x06,
    0x581c, 0x08,
    0x581d, 0x0e,
    0x581e, 0x29,
    0x581f, 0x17,
    0x5820, 0x11,
    0x5821, 0x11,
    0x5822, 0x15,
    0x5823, 0x28,
    0x5824, 0x46,
    0x5825, 0x26,
    0x5826, 0x08,
    0x5827, 0x26,
    0x5828, 0x64,
    0x5829, 0x26,
    0x582a, 0x24,
    0x582b, 0x22,
    0x582c, 0x24,
    0x582d, 0x24,
    0x582e, 0x06,
    0x582f, 0x22,
    0x5830, 0x40,
    0x5831, 0x42,
    0x5832, 0x24,
    0x5833, 0x26,
    0x5834, 0x24,
    0x5835, 0x22,
    0x5836, 0x22,
    0x5837, 0x26,
    0x5838, 0x44,
    0x5839, 0x24,
    0x583a, 0x26,
    0x583b, 0x28,
    0x583c, 0x42,
    0x583d, 0xce, // lenc BR offset
    // AWB 自动白平衡
    0x5180, 0xff, // AWB B block
    0x5181, 0xf2, // AWB control
    0x5182, 0x00, // [7:4] max local counter, [3:0] max fast counter
    0x5183, 0x14, // AWB advanced
    0x5184, 0x25,
    0x5185, 0x24,
    0x5186, 0x09,
    0x5187, 0x09,
    0x5188, 0x09,
    0x5189, 0x75,
    0x518a, 0x54,
    0x518b, 0xe0,
    0x518c, 0xb2,
    0x518d, 0x42,
    0x518e, 0x3d,
    0x518f, 0x56,
    0x5190, 0x46,
    0x5191, 0xf8, // AWB top limit
    0x5192, 0x04, // AWB bottom limit
    0x5193, 0x70, // red limit
    0x5194, 0xf0, // green limit
    0x5195, 0xf0, // blue limit
    0x5196, 0x03, // AWB control
    0x5197, 0x01, // local limit
    0x5198, 0x04,
    0x5199, 0x12,
    0x519a, 0x04,
    0x519b, 0x00,
    0x519c, 0x06,
    0x519d, 0x82,
    0x519e, 0x38, // AWB control
    // Gamma 伽玛曲线
    0x5480, 0x01, // Gamma bias plus on, bit[0]
    0x5481, 0x08,
    0x5482, 0x14,
    0x5483, 0x28,
    0x5484, 0x51,
    0x5485, 0x65,
    0x5486, 0x71,
    0x5487, 0x7d,
    0x5488, 0x87,
    0x5489, 0x91,
    0x548a, 0x9a,
    0x548b, 0xaa,
    0x548c, 0xb8,
    0x548d, 0xcd,
    0x548e, 0xdd,
    0x548f, 0xea,
    0x5490, 0x1d,
    // color matrix 色彩矩阵
    0x5381, 0x1e, // CMX1 for Y
    0x5382, 0x5b, // CMX2 for Y
    0x5383, 0x08, // CMX3 for Y
    0x5384, 0x0a, // CMX4 for U
    0x5385, 0x7e, // CMX5 for U
    0x5386, 0x88, // CMX6 for U
    0x5387, 0x7c, // CMX7 for V
    0x5388, 0x6c, // CMX8 for V
    0x5389, 0x10, // CMX9 for V
    0x538a, 0x01, // sign[9]
    0x538b, 0x98, // sign[8:1]
    // UV adjust UV 色彩饱和度调整
    0x5580, 0x06, // saturation on, bit[1]
    0x5583, 0x40,
    0x5584, 0x10,
    0x5589, 0x10,
    0x558a, 0x00,
    0x558b, 0xf8,
    0x501d, 0x40, // enable manual offset of contrast
    // CIP 锐化和降噪
    0x5300, 0x08, // CIP sharpen MT threshold 1
    0x5301, 0x30, // CIP sharpen MT threshold 2
    0x5302, 0x10, // CIP sharpen MT offset 1
    0x5303, 0x00, // CIP sharpen MT offset 2
    0x5304, 0x08, // CIP DNS threshold 1
    0x5305, 0x30, // CIP DNS threshold 2
    0x5306, 0x08, // CIP DNS offset 1
    0x5307, 0x16, // CIP DNS offset 2
    0x5309, 0x08, // CIP sharpen TH threshold 1
    0x530a, 0x30, // CIP sharpen TH threshold 2
    0x530b, 0x04, // CIP sharpen TH offset 1
    0x530c, 0x06, // CIP sharpen TH offset 2
    0x5025, 0x00,
    0x3008, 0x02, // wake up from standby, bit[6]
   
//    0x503d, 0x80,//测试彩条
//    0x4741, 0x00,


    // 30fps, night mode 5fps
    // input clock 24Mhz, PCLK 45.6Mhz
		//30fps
    0x3035, 0x21, // PLL
    0x3036, 0x72, // PLL
	
		//15fps
//	  0x3035, 0x41, // PLL
//    0x3036, 0x72, // PLL
	
    0x3c07, 0x08, // light meter 1 threshold[7:0]
//    0x3820, 0x42, // flip		上下镜像
//    0x3821, 0x07, // mirror	左右镜像
    0x3814, 0x31, // timing X inc
    0x3815, 0x31, // timing Y inc
    0x3800, 0x00, // HS
    0x3801, 0x00, // HS
    0x3802, 0x00, // VS
    0x3803, 0xbe, // VS
    0x3804, 0x0a, // HW (HE)
    0x3805, 0x3f, // HW (HE)
    0x3806, 0x06, // VH (VE)
    0x3807, 0xe4, // VH (VE)
    0x3808, 0x03, // DVPHO
    0x3809, 0x56, // DVPHO
    0x380a, 0x01, // DVPVO
    0x380b, 0xe0, // DVPVO
    0x380c, 0x07, // HTS
    0x380d, 0x69, // HTS
    0x380e, 0x03, // VTS
    0x380f, 0x21, // VTS
    0x3813, 0x06, // timing V offset
    0x3618, 0x00,
    0x3612, 0x29,
    0x3709, 0x52,
    0x370c, 0x03,
    0x3a02, 0x09, // 60Hz max exposure, night mode 5fps
    0x3a03, 0x63, // 60Hz max exposure
    // banding filters are calculated automatically in camera driver
    //0x3a08, 0x00, // B50 step
    //0x3a09, 0x78, // B50 step
    //0x3a0a, 0x00, // B60 step
    //0x3a0b, 0x64, // B60 step
    //0x3a0e, 0x06, // 50Hz max band
    //0x3a0d, 0x08, // 60Hz max band
    0x3a14, 0x09, // 50Hz max exposure, night mode 5fps
    0x3a15, 0x63, // 50Hz max exposure
    0x4004, 0x02, // BLC line number
    0x3002, 0x1c, // reset JFIFO, SFIFO, JPG
    0x3006, 0xc3, // disable clock of JPEG2x, JPEG
    0x4713, 0x03, // JPEG mode 3
    0x4407, 0x04, // Quantization sacle
    0x460b, 0x35,
    0x460c, 0x22,
    0x4837, 0x22, // MIPI global timing
    0x3824, 0x02, // PCLK manual divider
    0x5001, 0xa3, // SDE on, CMX on, AWB on
    0x3503, 0x00, // AEC/AGC on//	  /* Initialize OV5640 */
    
    
//    0x3016, 0x01,
//    0x301C, 0x00,
//    0x3016, 0x01,
//    0x301C, 0x01,
//    0x3019, 0x01,

//    0x503d, 0x80,//测试彩条
//    0x4741, 0x00,
	  0x4740, 0x23,
};


/**
  * @brief  设置帧率
  * @param  可使用宏：FRAME_RATE_30FPS，FRAME_RATE_15FPS
  * @retval 无
  */
void OV5640_FrameRate_Set(uint8_t frame_rate)
{ 
		if(frame_rate == FRAME_RATE_30FPS)
		{
			OV5640_WriteReg(LPI2C1,0x3035,0x21);//X_ADDR_ST高字节
			OV5640_WriteReg(LPI2C1,0x3036,0x72);//X_ADDR_ST低字节	
		}
		else if(frame_rate == FRAME_RATE_15FPS)
		{
			OV5640_WriteReg(LPI2C1,0x3035,0x41);//X_ADDR_ST高字节
			OV5640_WriteReg(LPI2C1,0x3036,0x72);//X_ADDR_ST低字节	
		}
	
}

/**
  * @brief  设置ISP图像大小，即ISP图像在传感器中的窗口
  *         较小的ISP图像可以提高图像采集频率(还要设置时钟)
  * @param  x_st,y_st：起始位置偏移
	* @param  width，height: 宽度，高度
  * @note   x_st+width <= 2592,y_st+height <= 1944
  * @retval 无
  */
void OV5640_ISPSize_Set(uint16_t x_st,uint16_t y_st,uint16_t width,uint16_t height)
{ 
		//  OV5640_WriteReg(LPI2C1,0X3212,0X03);  	

		OV5640_WriteReg(LPI2C1,0x3800,x_st>>8);		//X_ADDR_ST高字节
		OV5640_WriteReg(LPI2C1,0x3801,x_st&0xff);//X_ADDR_ST低字节	
		OV5640_WriteReg(LPI2C1,0x3802,y_st>>8);		//Y_ADDR_ST高字节
		OV5640_WriteReg(LPI2C1,0x3803,y_st&0xff);//Y_ADDR_ST低字节

			OV5640_WriteReg(LPI2C1,0x3804,(x_st+width)>>8);		//X_ADDR_END高字节
		OV5640_WriteReg(LPI2C1,0x3805,(x_st+width)&0xff);	//X_ADDR_END低字节	
		OV5640_WriteReg(LPI2C1,0x3806,(y_st+height)>>8);		//Y_ADDR_END高字节
		OV5640_WriteReg(LPI2C1,0x3807,(y_st+height)&0xff);//Y_ADDR_END低字节

		//  OV5640_WriteReg(LPI2C1,0X3212,0X13);		
		//  OV5640_WriteReg(LPI2C1,0X3212,0Xa3);		
}





/**
  * @brief  设置图像输出像大小，位置，以及是否使用自动缩放功能
  * @param  scaling:0,关闭自动缩放功能，1，开启自动缩放功能
  * @param  x_off,y_off:-关闭自动缩放功能时，这两个值为输出窗口在ISP图像中的偏移。
												 ！！不使用自动缩放功能时，非常容易出错。

                         ！！使用15fps相对不容易出错，但还是推荐使用自动缩放功能。

                        -开启自动缩放功能时，这两个值设置为 16,4 一般能正常使用，
                        也可根据输出窗口的宽高比例来调整，比较复杂，
                        请参考《OV5640 自动对焦摄像模组应用指南》第46页
  * @param  width,height:图像宽度和图像高度
  * @retval 无
  */
void OV5640_OutSize_Set(uint8_t scaling,uint16_t x_off,uint16_t y_off,uint16_t width,uint16_t height)
{ 
		uint8_t reg_temp;

		//读取原寄存器内容
		//		reg_temp = OV5640_ReadReg(0x5001);
		OV5640_ReadReg(LPI2C1,0x5001,&reg_temp);	
		//不使用自动缩放功能
		if(scaling == 0 )
		{
			OV5640_WriteReg(LPI2C1,0x5001,reg_temp &~(1<<5) );	//scaling off		
		}
		else
		{
			OV5640_WriteReg(LPI2C1,0x5001,reg_temp|(1<<5) );	//scaling on		
		}	

		OV5640_WriteReg(LPI2C1,0x3810,x_off>>8);	//X offset高字节
		OV5640_WriteReg(LPI2C1,0x3811,x_off&0xff);//X offset低字节	
		OV5640_WriteReg(LPI2C1,0x3812,y_off>>8);	//Y offset高字节
		OV5640_WriteReg(LPI2C1,0x3813,y_off&0xff);//Y offset低字节


		OV5640_WriteReg(LPI2C1,0X3212,0X03);  	

		OV5640_WriteReg(LPI2C1,0x3808,width>>8);	//输出宽度高字节
		OV5640_WriteReg(LPI2C1,0x3809,width&0xff);//输出宽度低字节  
		OV5640_WriteReg(LPI2C1,0x380a,height>>8);//输出高度高字节
		OV5640_WriteReg(LPI2C1,0x380b,height&0xff);//输出高度低字节

		OV5640_WriteReg(LPI2C1,0X3212,0X13);		
		OV5640_WriteReg(LPI2C1,0X3212,0Xa3);		
}
//环境光模式参数表
const static uint8_t OV5640_LightMode_reg[][7]=
{ 
		0x04,0X00,0X04,0X00,0X04,0X00,0X00,//Auto,自动 
		0x06,0X1C,0X04,0X00,0X04,0XF3,0X01,//Sunny,日光
		0x05,0X48,0X04,0X00,0X07,0XCF,0X01,//Office,办公室
		0x06,0X48,0X04,0X00,0X04,0XD3,0X01,//Cloudy,阴天 
		0x04,0X10,0X04,0X00,0X08,0X40,0X01,//Home,室内
}; 
/**
  * @brief  配置光线模式
  * @param  参数用于选择光线模式
  *         0x00 Auto     自动
  *         0x01 Sunny    日光
  *         0x02 Office 	办公室
  *         0x03 Cloudy		阴天 
  *         0x04 Home			室内

  * @retval None
  */
void OV5640_LightMode(uint8_t mode)
{
		OV5640_WriteReg(LPI2C1,0x3212, 0x03); // start group 3

		OV5640_WriteReg(LPI2C1,0x3406, OV5640_LightMode_reg[mode][0]);
		OV5640_WriteReg(LPI2C1,0x3400, OV5640_LightMode_reg[mode][1]);
		OV5640_WriteReg(LPI2C1,0x3401, OV5640_LightMode_reg[mode][2]);
		OV5640_WriteReg(LPI2C1,0x3402, OV5640_LightMode_reg[mode][3]);
		OV5640_WriteReg(LPI2C1,0x3403, OV5640_LightMode_reg[mode][4]);
		OV5640_WriteReg(LPI2C1,0x3404, OV5640_LightMode_reg[mode][5]);
		OV5640_WriteReg(LPI2C1,0x3405, OV5640_LightMode_reg[mode][6]);

		OV5640_WriteReg(LPI2C1,0x3212, 0x13); // end group 3
		OV5640_WriteReg(LPI2C1,0x3212, 0xa3); // lanuch group 3
}


//特效设置参数表
const static uint8_t OV5640_Effect_reg[][4]=
{ 
		0X06,0x40,0X10,0X08,//正常 
		0X1E,0xA0,0X40,0X08,//冷色
		0X1E,0x80,0XC0,0X08,//暖色
		0X1E,0x80,0X80,0X08,//黑白
		0X1E,0x40,0XA0,0X08,//泛黄 
		0X40,0x40,0X10,0X08,//反色
		0X1E,0x60,0X60,0X08,//偏绿
		0X1E,0xF0,0XF0,0X08,//过曝
		0X06,0x40,0X10,0X09,//正负片叠加
}; 
/**
  * @brief  特殊效果
  * @param  参数用于选择光线模式
	*   0x01  正常 
	*   0x02  冷色
	*   0x03  暖色
	*   0x04  黑白
	*   0x05  泛黄 
	*   0x06  反色
	*   0x07  偏绿
	*   0x08  过曝
	*   0x09  正负片叠加

  * @retval None
  */
void OV5640_SpecialEffects(uint8_t mode)
{
		OV5640_WriteReg(LPI2C1,0x3212, 0x03); // start group 3

		OV5640_WriteReg(LPI2C1,0x5580, OV5640_Effect_reg[mode][0]);
		OV5640_WriteReg(LPI2C1,0x5583, OV5640_Effect_reg[mode][1]); // sat U
		OV5640_WriteReg(LPI2C1,0x5584, OV5640_Effect_reg[mode][2]); // sat V
		OV5640_WriteReg(LPI2C1,0x5003, OV5640_Effect_reg[mode][3]);

		OV5640_WriteReg(LPI2C1,0x3212, 0x13); // end group 3
		OV5640_WriteReg(LPI2C1,0x3212, 0xa3); // launch group 3
}


/**
  * @brief  设置光照度
	* @param  bri:光照度，参数范围[-4~+4]
  * @retval 无
  */
void OV5640_BrightnessConfig(int8_t Brightness)
{	
		OV5640_WriteReg(LPI2C1,0x3212, 0x03); // start group 3

		OV5640_WriteReg(LPI2C1,0x5587, (Brightness<<4)&0xF0);

		if(Brightness >= 0)	
		{
			OV5640_WriteReg(LPI2C1,0x5588, 0x01);
		}
		else
		{
			OV5640_WriteReg(LPI2C1,0x5588, 0x09);
		}
			
		OV5640_WriteReg(LPI2C1,0x3212, 0x13); // end group 3
		OV5640_WriteReg(LPI2C1,0x3212, 0xa3); // launch group 3
}

//对比度参数表
const static uint8_t OV5640_Contrast_reg[][2]=
{
		0x2C,0x1C,
		0x28,0x18,
		0x24,0x10,
		0x20,0x00,
		0x1C,0x1C,
		0x18,0x18,
		0x14,0x14,
};

/**
  * @brief  设置对比度
	* @param  cnst:对比度，参数范围[-3~+3]
  * @retval 无
  */
void OV5640_ContrastConfig(int8_t cnst)
{	
		OV5640_WriteReg(LPI2C1,0x3212, 0x03); // start group 3
		OV5640_WriteReg(LPI2C1,0x5586, OV5640_Contrast_reg[cnst+3][0]);
		OV5640_WriteReg(LPI2C1,0x5585, OV5640_Contrast_reg[cnst+3][1]);
		OV5640_WriteReg(LPI2C1,0x3212, 0x13); // end group 3
		OV5640_WriteReg(LPI2C1,0x3212, 0xa3); // launch group 3
}

//色彩饱和度参数表
const static uint8_t OV5640_Saturation_reg[][6]=
{ 
		0X0C,0x30,0X3D,0X3E,0X3D,0X01,//-3 
		0X10,0x3D,0X4D,0X4E,0X4D,0X01,//-2  
		0X15,0x52,0X66,0X68,0X66,0X02,//-1  
		0X1A,0x66,0X80,0X82,0X80,0X02,//0  
		0X1F,0x7A,0X9A,0X9C,0X9A,0X02,//+1  
		0X24,0x8F,0XB3,0XB6,0XB3,0X03,//+2
		0X2B,0xAB,0XD6,0XDA,0XD6,0X04,//+3
}; 
/**
  * @brief  设置饱和度
  * @param  sat:饱和度,参数范围[-3 ~ +3]             	
  * @retval 无
  */
void OV5640_Color_Saturation(int8_t sat)
{
		uint8_t i;

		OV5640_WriteReg(LPI2C1,0x3212, 0x03); // start group 3
		OV5640_WriteReg(LPI2C1,0x5381, 0x1c);	
		OV5640_WriteReg(LPI2C1,0x5382, 0x5a);
		OV5640_WriteReg(LPI2C1,0x5383, 0x06);

		for(i=0;i<6;i++)
		{
			OV5640_WriteReg(LPI2C1,0x5384+i, OV5640_Saturation_reg[sat+3][i]);
		}

		OV5640_WriteReg(LPI2C1,0x538b, 0x98);
		OV5640_WriteReg(LPI2C1,0x538a, 0x01);	
		OV5640_WriteReg(LPI2C1,0x3212, 0x13); // end group 3
		OV5640_WriteReg(LPI2C1,0x3212, 0xa3); // launch group 3
	
}			

//曝光补偿设置参数表
const static uint8_t OV5640_Exposure_reg[][6]=
{
    0x10,0x08,0x10,0x08,0x20,0x10,//-3  
    0x20,0x18,0x41,0x20,0x18,0x10,//-2
    0x30,0x28,0x61,0x30,0x28,0x10,//-1 
    0x38,0x30,0x61,0x38,0x30,0x10,//0  
    0x40,0x38,0x71,0x40,0x38,0x10,//+1 
    0x50,0x48,0x90,0x50,0x48,0x20,//+2   
    0x60,0x58,0xa0,0x60,0x58,0x20,//+3    
};

/**
  * @brief  设置曝光补偿
  * @param  ev:曝光补偿等级，参数范围[-3 ~ +3]             	
  * @retval 无
  */
void OV5640_Exposure(int8_t ev)
{
	OV5640_WriteReg(LPI2C1,0x3212, 0x03); // start group 3

	OV5640_WriteReg(LPI2C1,0x3a0f, OV5640_Exposure_reg[ev+3][0]);
	OV5640_WriteReg(LPI2C1,0x3a10, OV5640_Exposure_reg[ev+3][1]);
	OV5640_WriteReg(LPI2C1,0x3a11, OV5640_Exposure_reg[ev+3][2]);
	OV5640_WriteReg(LPI2C1,0x3a1b, OV5640_Exposure_reg[ev+3][3]);
	OV5640_WriteReg(LPI2C1,0x3a1e, OV5640_Exposure_reg[ev+3][4]);
	OV5640_WriteReg(LPI2C1,0x3a1f, OV5640_Exposure_reg[ev+3][5]);
	
	OV5640_WriteReg(LPI2C1,0x3212, 0x13); // end group 3
	OV5640_WriteReg(LPI2C1,0x3212, 0xa3); // launch group 3

}

/**未测试*/
/**
  * @brief  设置图像输出窗口
  * @param  sx,sy,起始地址
						width,height:宽度(对应:horizontal)和高度(对应:vertical)
  * @retval 0,设置成功， 其他,设置失败
  */
void OV5640_Window_Set(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height)
{
	uint16_t endx;
	uint16_t endy;
	uint8_t temp;
	endx=sx+width/2;	//V*2
 	endy=sy+height/2;

 	OV5640_WriteReg(LPI2C1,0XFF,0X01);
	//temp = OV5640_ReadReg(0X03);				//读取Vref之前的值
	OV5640_ReadReg(LPI2C1,0X03,&temp);
	
	temp&=0XF0;
	temp|=((endy&0X03)<<2)|(sy&0X03);
	OV5640_WriteReg(LPI2C1,0X03,temp);			//设置Vref的start和end的最低2位
	OV5640_WriteReg(LPI2C1,0X19,sy>>2);			//设置Vref的start高8位
	OV5640_WriteReg(LPI2C1,0X1A,endy>>2);		//设置Vref的end的高8位

	//temp = OV5640_ReadReg(0X32);					//读取Href之前的值
	OV5640_ReadReg(LPI2C1,0X32,&temp);
	
	temp&=0XC0;
	temp|=((endx&0X07)<<3)|(sx&0X07);
	OV5640_WriteReg(LPI2C1,0X32,temp);				//设置Href的start和end的最低3位
	OV5640_WriteReg(LPI2C1,0X17,sx>>3);			//设置Href的start高8位
	OV5640_WriteReg(LPI2C1,0X18,endx>>3);			//设置Href的end的高8位
}

/**
  * @brief  设置相机模式
  * @param  None        	
  * @retval None
  */
void Set_Cam_mode(int temp)
{
	/*	只需在这里修改传参，即可修改参数，详细参数详见结构体	*/
	switch(temp)
	{
		case 1:	cam_temp_mode=cam_mode_240_320;break;
		case 2:	cam_temp_mode=cam_mode_640_480;break;
		case 3:	cam_temp_mode=cam_mode_800_480;break;
	}
}

/**
  * @brief  使用cammode参数初始化各项配置
  * @param  None        	
  * @retval None
  */
void OV5640_USER_Config(void)
{	

		OV5640_FrameRate_Set(cam_temp_mode.frame_rate);
		OV5640_DelayMs(1);

		OV5640_ISPSize_Set(cam_temp_mode.cam_isp_sx,       	//摄像头ISP X起始位置
												 cam_temp_mode.cam_isp_sy,     	//摄像头ISP Y起始位置
												 cam_temp_mode.cam_isp_width,   	//摄像头ISP 宽
												 cam_temp_mode.cam_isp_height);  //摄像头ISP 高
		OV5640_DelayMs(1);
		/*	设置图像输出像大小，位置，以及是否使用自动缩放功能	*/
		OV5640_OutSize_Set(cam_temp_mode.scaling,						//是否使用自动缩放
													cam_temp_mode.cam_out_sx,      //摄像头输出窗口X起始位置
													cam_temp_mode.cam_out_sy,      //摄像头输出窗口Y起始位置
													cam_temp_mode.cam_out_width,   //输出图像分辨率，宽
													cam_temp_mode.cam_out_height); //输出图像分辨率，高
	
		OV5640_DelayMs(1);
		OV5640_BrightnessConfig(cam_temp_mode.brightness);		//光照度，
		OV5640_DelayMs(1);

		OV5640_Color_Saturation(cam_temp_mode.saturation);		//饱和度
		OV5640_DelayMs(1);

		OV5640_ContrastConfig(cam_temp_mode.contrast);				//对比度
		OV5640_DelayMs(1);

		OV5640_Exposure(cam_temp_mode.exposure);							//曝光补偿
		OV5640_DelayMs(1);

		OV5640_LightMode(cam_temp_mode.light_mode);					//光照模式
		OV5640_DelayMs(1);

		OV5640_SpecialEffects(cam_temp_mode.effect);					//特殊效果
		OV5640_DelayMs(1);


}



static status_t OV5640_WriteRegs(sccb_i2c_t i2c, const ov5640_reg_t regs[], uint32_t num)
{
    status_t status = kStatus_Success;

    for (uint32_t i = 0; i < num; i++)
    {	
        status = OV5640_WriteReg(i2c, regs[i].reg, regs[i].val);

        if (kStatus_Success != status)
        {
            break;
        }
    }

    return status;
}

static status_t OV5640_SoftwareReset(sccb_i2c_t i2c)
{
    return OV5640_WriteReg(i2c, 0x3008, 0x80);
}


extern uint8_t OV5640_FOCUS_AD5820_Init(void);
status_t OV5640_Init(camera_device_handle_t *handle, const camera_config_t *config)
{
    status_t status;
    uint8_t pid, ver;
    uint16_t width, height;
    ov5640_resource_t *resource = (ov5640_resource_t *)(handle->resource);
    sccb_i2c_t i2c = resource->sccbI2C;

    if ((kCAMERA_InterfaceNonGatedClock != config->interface) && (kCAMERA_InterfaceGatedClock != config->interface) &&
        (kCAMERA_InterfaceCCIR656 != config->interface))
    {
        return kStatus_InvalidArgument;
    }

    width = FSL_VIDEO_EXTRACT_WIDTH(config->resolution);
    height = FSL_VIDEO_EXTRACT_HEIGHT(config->resolution);

    if ((width > 800) || (height > 480))
    {
        return kStatus_InvalidArgument;
    }

    resource->pullPowerDownPin(true);
		resource->pullResetPin(false);
    /* 延时 */
    OV5640_DelayMs(1);

    resource->pullPowerDownPin(false);

    /* 延时 */
    OV5640_DelayMs(2);

    resource->pullResetPin(true);

    /* 延时 */
    OV5640_DelayMs(2);

    /* 识别设备 */
    status = OV5640_ReadReg(i2c, OV5640_PID_REG, &pid);
    if (kStatus_Success != status)
    {
        return status;
    }

    status = OV5640_ReadReg(i2c, OV5640_VER_REG, &ver);
    if (kStatus_Success != status)
    {
        return status;
    }

    if (OV5640_REVISION != (((uint32_t)pid << 8U) | (uint32_t)ver))
    {
        return kStatus_Fail;
    }

    /* 设备识别确定, 执行软件复位 */
    OV5640_SoftwareReset(i2c);

    /* 延时 */
    OV5640_DelayMs(2);

    /* 配置摄像头寄存器 */
    status = OV5640_WriteRegs(i2c, ov5640InitRegs, ARRAY_SIZE(ov5640InitRegs));
    if (kStatus_Success != status)
    {
        return status;
    }
		/* 配置摄像头寄存器 */
		OV5640_USER_Config();
		/* 初始化摄像头自动对焦 */
		OV5640_FOCUS_AD5820_Init();	
		return kStatus_Success;
}

status_t OV5640_Start(camera_device_handle_t *handle)
{
    return kStatus_Success;
}

status_t OV5640_Stop(camera_device_handle_t *handle)
{
    return kStatus_Success;
}

status_t OV5640_InitExt(camera_device_handle_t *handle, const camera_config_t *config, const void *specialConfig)
{
    return OV5640_Init(handle, config);
}
