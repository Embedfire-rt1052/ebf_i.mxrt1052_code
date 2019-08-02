/**
  ******************************************************************
  * @file    bsp_ov5640.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   OV5640����
  ******************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  i.MXRT1052������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "bsp_ov5640.h"
#include "bsp_ov5640_AF.h"
#include "bsp_ov5640_config.h"

#include "./key/bsp_key.h"

uint32_t activeFrameAddr;
uint32_t inactiveFrameAddr;
static csi_private_data_t csiPrivateData;

OV5640_MODE_PARAM cam_temp_mode;//ȫ�ֽṹ��

/* OV5640���ӵ�CSI */
static csi_resource_t csiResource = {
    .csiBase = CSI,
};

camera_receiver_handle_t cameraReceiver = {
    .resource = &csiResource, .ops = &csi_ops, .privateData = &csiPrivateData,
};
/**
  * @brief  ����ͷ�жϴ�����
  * @param  ��
  * @retval ��
  */

void CSI_IRQHandler(void)
{
    CSI_DriverIRQHandler();

}


/**
  * @brief  ����ͷ��λ���ſ��ƺ���
  * @param  pullUp��1���ߵ�ƽ��0���͵�ƽ
  * @retval ��
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
  * @brief  ����ͷ��Դ���ſ��ƺ���
  * @param  pullUp��1���ߵ�ƽ��0���͵�ƽ
  * @retval ��
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

static ov5640_resource_t ov5640Resource = {
    .sccbI2C = OV5640_I2C,
    .pullResetPin = BOARD_PullCameraResetPin,
    .pullPowerDownPin = BOARD_PullCameraPowerDownPin,
    .inputClockFreq_Hz = 24000000,
};

camera_device_handle_t cameraDevice = {
    .resource = &ov5640Resource, .ops = &ov5640_ops,
};

void BOARD_InitCameraResource(void)
{
    lpi2c_master_config_t masterConfig;
    uint32_t sourceClock;

    LPI2C_MasterGetDefaultConfig(&masterConfig);
		/*����I2Cʱ��Ϊ400KHz*/
    masterConfig.baudRate_Hz = 400000;
    masterConfig.debugEnable = true;
    masterConfig.ignoreAck = true;

    /*LPI2Cʱ������ */
    /*
     * LPI2C ʱ��Դѡ��:
     *  0: pll3_60m
     *  1: OSC clock
     */
    CLOCK_SetMux(kCLOCK_Lpi2cMux, 1);
    /*
     * LPI2C ��Ƶϵ��.
     *  0b000000: Divide by 1
     *  0b111111: Divide by 2^6
     */
    CLOCK_SetDiv(kCLOCK_Lpi2cDiv, 0);

    /* LPI2C ʱ��ԴΪ OSC */
    sourceClock = CLOCK_GetOscFreq();

    LPI2C_MasterInit(OV5640_I2C, &masterConfig, sourceClock);

    /* ��ʼ������ͷ��PDN��RST���� */
    gpio_pin_config_t pinConfig = {
        kGPIO_DigitalOutput, 1,
    };
    GPIO_PinInit(CAMERA_PWR_GPIO, CAMERA_PWR_GPIO_PIN, &pinConfig);
		GPIO_PinInit(CAMERA_RST_GPIO, CAMERA_RST_GPIO_PIN, &pinConfig);
}
		

/**
  * @brief  ����OV5640
  * @param  None
  * @retval None
  */
void Camera_Init(void) 
{
		/* ��ʼ������ͷ���� */
		BOARD_InitCSIPins();
		/* ��ʼ������ͷ��I2C���������� */
	
		BOARD_InitCameraResource();
		Set_Cam_mode(1);
	
    elcdif_rgb_mode_config_t lcdConfig = {
        .panelWidth = APP_IMG_WIDTH,		//��Ļ����� ͼ����	����Ϊ ʵ����Ļ��С��800*480��
        .panelHeight = APP_IMG_HEIGHT,	//��Ļ����� ͼ��߶�
        .hsw = APP_HSW,
        .hfp = APP_HFP,
        .hbp = APP_HBP,
        .vsw = APP_VSW,
        .vfp = APP_VFP,
        .vbp = APP_VBP,
        .polarityFlags = APP_POL_FLAGS,
        .pixelFormat = kELCDIF_PixelFormatRGB565,
        .dataBus = APP_LCDIF_DATA_BUS,
    };

    const camera_config_t cameraConfig = {
        .pixelFormat = kVIDEO_PixelFormatRGB565,
        .bytesPerPixel = APP_BPP,
			  .resolution = FSL_VIDEO_RESOLUTION(cam_temp_mode.cam_out_width, cam_temp_mode.cam_out_height),//��Ƶ�ֱ��ʵ� ��Ⱥ͸߶�	cam_mode.cam_out_width		cam_mode.cam_out_height
        .frameBufferLinePitch_Bytes = APP_IMG_WIDTH * APP_BPP,
        .interface = kCAMERA_InterfaceGatedClock,
        .controlFlags = APP_CAMERA_CONTROL_FLAGS,
        .framePerSec = 30,
    };
		
		
    memset(s_frameBuffer, 0, sizeof(s_frameBuffer));

    CAMERA_RECEIVER_Init(&cameraReceiver, &cameraConfig, NULL, NULL);

    CAMERA_DEVICE_Init(&cameraDevice, &cameraConfig);

    CAMERA_DEVICE_Start(&cameraDevice);

    /* ����֡�������ύ������������. */
    for (uint32_t i = 0; i < APP_FRAME_BUFFER_COUNT; i++)
    {
        CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, (uint32_t)(s_frameBuffer[i]));
    }

    CAMERA_RECEIVER_Start(&cameraReceiver);

    /*
     * LCDIF �л�������ͷǻ������, ������ڴ˴���ȡ����������.
     */
    /* �ȴ���ȡ����֡����������ʾ */
    while (kStatus_Success != CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &activeFrameAddr))
    {
    }

    /* �ȴ���ȡ����֡����������ʾ */
    while (kStatus_Success != CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &inactiveFrameAddr))
    {
    }
	
    lcdConfig.bufferAddr = (uint32_t)activeFrameAddr;

    ELCDIF_RgbModeInit(APP_ELCDIF, &lcdConfig);

    ELCDIF_SetNextBufferAddr(APP_ELCDIF, inactiveFrameAddr);
    ELCDIF_RgbModeStart(APP_ELCDIF);
	  /* �򿪱��� */
    GPIO_PinWrite(LCD_BL_GPIO, LCD_BL_GPIO_PIN, 1);
	
}
/**
  * @brief  ����ͷ����ѡ��
  * @param  None
  * @retval None
  */

void Cam_Config_Switch()
{
		static int index_num=1;
		/* ���WAUP���� */
		if(Key_Scan(CORE_BOARD_WAUP_KEY_GPIO, CORE_BOARD_WAUP_KEY_GPIO_PIN) == KEY_ON )
		{
			PRINTF("index_num = %d \r\n");
			index_num++;
			if(index_num>4)
			{
				index_num=1;
			}

			/*	��������ͷģʽ */
			Set_Cam_mode(index_num);

			const camera_config_t cameraConfig = {
					.pixelFormat = kVIDEO_PixelFormatRGB565,
					.bytesPerPixel = APP_BPP,
					.resolution = FSL_VIDEO_RESOLUTION(cam_temp_mode.cam_out_width, cam_temp_mode.cam_out_height),//��Ƶ�ֱ��ʵ� ��Ⱥ͸߶�
					.frameBufferLinePitch_Bytes = APP_IMG_WIDTH * APP_BPP,
					.interface = kCAMERA_InterfaceGatedClock,
					.controlFlags = APP_CAMERA_CONTROL_FLAGS,
					.framePerSec = 30,
			};
			/*	�ر�����ͷ���� */
			CAMERA_DEVICE_Stop(&cameraDevice);			
			memset(s_frameBuffer, 0, sizeof(s_frameBuffer));
			
			CAMERA_RECEIVER_Init(&cameraReceiver, &cameraConfig, NULL, NULL);

			CAMERA_DEVICE_Init(&cameraDevice, &cameraConfig);

			CAMERA_DEVICE_Start(&cameraDevice);
			
			/* ����֡�������ύ������������. */
			for (uint32_t i = 0; i < APP_FRAME_BUFFER_COUNT; i++)
			{
					CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, (uint32_t)(s_frameBuffer[i]));
			}
			CAMERA_RECEIVER_Start(&cameraReceiver);	
		}    
}
