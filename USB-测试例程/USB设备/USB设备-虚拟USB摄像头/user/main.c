/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
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
