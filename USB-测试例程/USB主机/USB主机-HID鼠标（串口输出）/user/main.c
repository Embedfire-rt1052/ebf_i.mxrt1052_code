/**
 ******************************************************************
 * @file    main.c
 * @author  fire
 * @version V1.0
 * @date    2018-xx-xx
 * @brief   USB主机-HID鼠标（串口输出）
 ******************************************************************
 * @attention
 *
 * 实验平台:野火  i.MXRT1052开发板 
 * 论坛    :http://www.firebbs.cn
 * 淘宝    :http://firestm32.taobao.com
 *
 ******************************************************************
 */

#include "usb_host_config.h"
#include "usb_host.h"


#include "board.h"
#include "host_mouse.h"
#include "fsl_common.h"
#if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
#include "fsl_sysmpu.h"
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */
#include "bsp_hid_mouse.h"
#include "board.h"

#if ((!USB_HOST_CONFIG_KHCI) && (!USB_HOST_CONFIG_EHCI) && (!USB_HOST_CONFIG_OHCI) && (!USB_HOST_CONFIG_IP3516HS))
#error Please enable USB_HOST_CONFIG_KHCI, USB_HOST_CONFIG_EHCI, USB_HOST_CONFIG_OHCI, or USB_HOST_CONFIG_IP3516HS in file usb_host_config.
#endif

#include "pin_mux.h"
#include "usb_phy.h"
#include "clock_config.h"



/*******************************************************************************
 * Definitions
 ******************************************************************************/
extern usb_host_handle g_HostHandle;
extern usb_host_mouse_instance_t g_HostHidMouse;


int main(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    USB_HostApplicationInit();

    while (1)
    {
        USB_HostTaskFn(g_HostHandle);
        USB_HostHidMouseTask(&g_HostHidMouse);
    }
}
