/**
 ******************************************************************
 * @file    main.c
 * @author  fire
 * @version V1.0
 * @date    2018-xx-xx
 * @brief   USB设备-虚拟USB摄像头(bm)
 ******************************************************************
 * @attention
 *
 * 实验平台:野火  i.MXRT1052开发板 
 * 论坛    :http://www.firebbs.cn
 * 淘宝    :http://firestm32.taobao.com
 *
 ******************************************************************
 */
 #include "video_data.h"

 #include "fsl_device_registers.h"
 #include "clock_config.h"
 #include "board.h"
 #include "fsl_debug_console.h"

 #include <stdio.h>
 #include <stdlib.h>

 #if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
 #include "fsl_sysmpu.h"
 #endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */


#include "pin_mux.h"
#include "bsp_driver_virtual_camera.h"



void USB_DeviceApplicationInit(void);

#if defined(__CC_ARM) || (defined(__ARMCC_VERSION)) || defined(__GNUC__)
int main(void)
#else
void main(void)
#endif
{
    BOARD_ConfigMPU();

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    USB_DeviceApplicationInit();

    while (1U)
    {
#if USB_DEVICE_CONFIG_USE_TASK
        USB_DeviceTaskFn(g_UsbDeviceVideoVirtualCamera.deviceHandle);
#endif
    }
}
