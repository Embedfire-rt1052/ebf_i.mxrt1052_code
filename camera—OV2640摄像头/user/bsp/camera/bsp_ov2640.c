/**
  ******************************************************************
  * @file    bsp_ov2640.c
  * @author  fire
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   OV2640驱动
  ******************************************************************
  * @attention
  *
  * 实验平台:野火  i.MXRT1052开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************
  */

#include "bsp_ov2640.h"
#include "bsp_ov2640_config.h"
#include "./lcd/bsp_lcd.h" 
#include "./key/bsp_key.h"
/*定义相关变量*/
uint32_t activeFrameAddr;
uint32_t inactiveFrameAddr;
static csi_private_data_t csiPrivateData;

OV2640_MODE_PARAM cam_temp_mode;//全局结构体
/* OV2640连接到CSI */
static csi_resource_t csiResource = {
    .csiBase = CSI,
};

camera_receiver_handle_t cameraReceiver = {
    .resource = &csiResource, .ops = &csi_ops, .privateData = &csiPrivateData,
};
/**
  * @brief  摄像头中断处理函数
  * @param  无
  * @retval 无
  */
extern uint8_t fps;
void CSI_IRQHandler(void)
{
    CSI_DriverIRQHandler();
		fps++; //帧率计数
}


/**
  * @brief  摄像头复位引脚控制函数
  * @param  pullUp：1：高电平，0：低电平
  * @retval 无
  */
static void BOARD_PullCameraResetPin(bool pullUp)
{
    if (pullUp)
    {
        GPIO_PinWrite(CAMERA_RST_GPIO, CAMERA_RST_GPIO_PIN, 1);
    }
    else
    {
        GPIO_PinWrite(CAMERA_RST_GPIO, CAMERA_RST_GPIO_PIN, 0);
    }
}
/**
  * @brief  摄像头电源引脚控制函数
  * @param  pullUp：1：高电平，0：低电平
  * @retval 无
  */
static void BOARD_PullCameraPowerDownPin(bool pullUp)
{
    if (pullUp)
    {
        GPIO_PinWrite(CAMERA_PWR_GPIO, CAMERA_PWR_GPIO_PIN, 1);
    }
    else
    {
        GPIO_PinWrite(CAMERA_PWR_GPIO, CAMERA_PWR_GPIO_PIN, 0);
    }
}
/* 摄像头资源结构体*/
static ov2640_resource_t ov2640Resource = {
    .sccbI2C = OV2640_I2C,
    .pullResetPin = BOARD_PullCameraResetPin,
    .pullPowerDownPin = BOARD_PullCameraPowerDownPin,
    .inputClockFreq_Hz = 24000000,
};
/*摄像头设备句柄*/
camera_device_handle_t cameraDevice = {
    .resource = &ov2640Resource, .ops = &ov2640_ops,
};
/**
* @brief  初始化板载摄像头资源	
  * @param  无
  * @retval 无
  */
void BOARD_InitCameraResource(void)
{
    lpi2c_master_config_t masterConfig;
    uint32_t sourceClock;

    LPI2C_MasterGetDefaultConfig(&masterConfig);
		/*设置I2C时钟为400KHz*/
    masterConfig.baudRate_Hz = 400000;
    masterConfig.debugEnable = true;
    masterConfig.ignoreAck = true;

    /*LPI2C时钟配置 */
    /*
     * LPI2C 时钟源选择:
     *  0: pll3_60m
     *  1: OSC clock
     */
    CLOCK_SetMux(kCLOCK_Lpi2cMux, 1);
    /*
     * LPI2C 分频系数.
     *  0b000000: Divide by 1
     *  0b111111: Divide by 2^6
     */
    CLOCK_SetDiv(kCLOCK_Lpi2cDiv, 0);

    /* LPI2C 时钟源为 OSC */
    sourceClock = CLOCK_GetOscFreq();

    LPI2C_MasterInit(OV2640_I2C, &masterConfig, sourceClock);
		/* 初始化摄像头的PDN和RST引脚 */
    gpio_pin_config_t pinConfig = {
        kGPIO_DigitalOutput, 1,
    };
    GPIO_PinInit(CAMERA_PWR_GPIO, CAMERA_PWR_GPIO_PIN, &pinConfig);
		GPIO_PinInit(CAMERA_RST_GPIO, CAMERA_RST_GPIO_PIN, &pinConfig);
}

/**
  * @brief  配置OV2640
  * @param  None
  * @retval None
  */
void Camera_Init(void) 
{
		/* 初始化摄像头引脚 */
		BOARD_InitCSIPins();
		/* 初始化摄像头的I2C及控制引脚 */
		BOARD_InitCameraResource();
    /* 设置摄像头模式 */
		Set_Cam_mode(1);
	  /* 配置屏幕参数 */
    elcdif_rgb_mode_config_t lcdConfig = {
        .panelWidth = APP_IMG_WIDTH,		/*屏幕的面板 图像宽度	设置为 实际屏幕大小（800*480）*/
        .panelHeight = APP_IMG_HEIGHT,	/*屏幕的面板 图像高度*/
        .hsw = APP_HSW,
        .hfp = APP_HFP,
        .hbp = APP_HBP,
        .vsw = APP_VSW,
        .vfp = APP_VFP,
        .vbp = APP_VBP,
        .polarityFlags = APP_POL_FLAGS,
        .pixelFormat = kELCDIF_PixelFormatRGB565,/*像素格式*/
        .dataBus = APP_LCDIF_DATA_BUS,/*液晶输出位宽为16bit*/
    };
    /* 配置摄像头参数 */
    const camera_config_t cameraConfig = {
        .pixelFormat = kVIDEO_PixelFormatRGB565,/*输出像素格式*/
        .bytesPerPixel = APP_BPP,
				.resolution = FSL_VIDEO_RESOLUTION(cam_temp_mode.cam_out_width, cam_temp_mode.cam_out_height),//视频分辨率
        .frameBufferLinePitch_Bytes = APP_IMG_WIDTH * APP_BPP,
        .interface = kCAMERA_InterfaceGatedClock,
        .controlFlags = APP_CAMERA_CONTROL_FLAGS,
        .framePerSec = 30,
    };
				
    memset(s_frameBuffer, 0, sizeof(s_frameBuffer));

    CAMERA_RECEIVER_Init(&cameraReceiver, &cameraConfig, NULL, NULL);

    CAMERA_DEVICE_Init(&cameraDevice, &cameraConfig);

    CAMERA_DEVICE_Start(&cameraDevice);

    /* 将空帧缓冲区提交到缓冲区队列. */
    for (uint32_t i = 0; i < APP_FRAME_BUFFER_COUNT; i++)
    {
        CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, (uint32_t)(s_frameBuffer[i]));
    }

    CAMERA_RECEIVER_Start(&cameraReceiver);

    /*
     * LCDIF 有活动缓冲区和非活动缓冲区, 因此请在此处获取两个缓冲区.
     */
    /* 等待获取完整帧缓冲区以显示 */
    while (kStatus_Success != CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &activeFrameAddr))
    {
    }

    /* 等待获取完整帧缓冲区以显示 */
    while (kStatus_Success != CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &inactiveFrameAddr))
    {
    }

    lcdConfig.bufferAddr = (uint32_t)activeFrameAddr;

    ELCDIF_RgbModeInit(APP_ELCDIF, &lcdConfig);/*初始化液晶屏*/

    ELCDIF_SetNextBufferAddr(APP_ELCDIF, LCD_SetOpenWindows_Pos(Set_Cam_mode(index_num), inactiveFrameAddr));
    ELCDIF_RgbModeStart(APP_ELCDIF);/*启动显示*/
	  /* 打开背光 */
    GPIO_PinWrite(LCD_BL_GPIO, LCD_BL_GPIO_PIN, 1);
}
/**
  * @brief  摄像头配置选择
  * @param  None
  * @retval None
  */
int index_num=1;
void Cam_Config_Switch()
{
		/* 检测WAUP按键 */
		if(Key_Scan(CORE_BOARD_WAUP_KEY_GPIO, CORE_BOARD_WAUP_KEY_GPIO_PIN) == KEY_ON )
		{
			index_num++;
			if(index_num>3)
			{
				index_num=1;
			}
      /*	设置摄像头模式 */
			Set_Cam_mode(index_num);
      /* 配置摄像头参数 */
			const camera_config_t cameraConfig = {
					.pixelFormat = kVIDEO_PixelFormatRGB565,
					.bytesPerPixel = APP_BPP,
					.resolution = FSL_VIDEO_RESOLUTION(cam_temp_mode.cam_out_width, cam_temp_mode.cam_out_height),//视频分辨率的 宽度和高度
					.frameBufferLinePitch_Bytes = APP_IMG_WIDTH * APP_BPP,
					.interface = kCAMERA_InterfaceGatedClock,
					.controlFlags = APP_CAMERA_CONTROL_FLAGS,
					.framePerSec = 30,
			};
			/*	关闭摄像头配置 */
			CAMERA_DEVICE_Stop(&cameraDevice);			
			memset(s_frameBuffer, 0, sizeof(s_frameBuffer));
			/* 初始化摄像头接收 */
			CAMERA_RECEIVER_Init(&cameraReceiver, &cameraConfig, NULL, NULL);
      /* 初始化摄像头设备 */
			CAMERA_DEVICE_Init(&cameraDevice, &cameraConfig);
      /* 摄像头设备开始 */
			CAMERA_DEVICE_Start(&cameraDevice);
			/* 将空帧缓冲区提交到缓冲区队列. */
			for (uint32_t i = 0; i < APP_FRAME_BUFFER_COUNT; i++)
			{
					CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, (uint32_t)(s_frameBuffer[i]));
			}
			CAMERA_RECEIVER_Start(&cameraReceiver);	
			
			memset(s_frameBuffer, 0, sizeof(s_frameBuffer));//避免重影
		}    
}
