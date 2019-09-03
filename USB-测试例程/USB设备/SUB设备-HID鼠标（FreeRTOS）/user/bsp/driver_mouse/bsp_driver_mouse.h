#ifndef __BSP_driver_mouse_H
#define __BSP_driver_mouse_H




#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"


#include "usb_phy.h"
#include "usb_device_class.h"
#include "usb_device_hid.h"
#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"
#if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
#include "fsl_sysmpu.h"
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */

#if ((defined FSL_FEATURE_SOC_USBPHY_COUNT) && (FSL_FEATURE_SOC_USBPHY_COUNT > 0U))
#include "usb_phy.h"
#endif


#include "FreeRTOS.h"
#include "semphr.h"
#include "event_groups.h"

#include "clock_config.h"
#include "board.h"
#include "pin_mux.h"








/*******************************************************************************
 * Definitions
 ******************************************************************************/

#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
#define CONTROLLER_ID kUSB_ControllerEhci0
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
#define CONTROLLER_ID kUSB_ControllerKhci0
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)
#define CONTROLLER_ID kUSB_ControllerLpcIp3511Fs0
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)
#define CONTROLLER_ID kUSB_ControllerLpcIp3511Hs0
#endif

#if defined(__GIC_PRIO_BITS)
#define USB_DEVICE_INTERRUPT_PRIORITY (25U)
#elif defined(__NVIC_PRIO_BITS) && (__NVIC_PRIO_BITS >= 3)
#define USB_DEVICE_INTERRUPT_PRIORITY (6U)
#else
#define USB_DEVICE_INTERRUPT_PRIORITY (3U)
#endif

#define USB_HID_MOUSE_REPORT_LENGTH (0x04U)
typedef struct _usb_hid_mouse_struct
{
    usb_device_handle deviceHandle;
    class_handle_t hidHandle;
    TaskHandle_t applicationTaskHandle;//USBÈÎÎñ¾ä±ú£¬
    TaskHandle_t deviceTaskHandle;
    uint8_t *buffer;
    uint8_t currentConfiguration;
    uint8_t currentInterfaceAlternateSetting[USB_HID_MOUSE_INTERFACE_COUNT];
    uint8_t speed;
    uint8_t attach;
#if (defined(USB_DEVICE_CHARGER_DETECT_ENABLE) && (USB_DEVICE_CHARGER_DETECT_ENABLE > 0U))
    volatile uint8_t vReginInterruptDetected;
    volatile uint8_t vbusValid;
    volatile usb_device_dcd_port_type_t dcdPortType;
    volatile usb_device_dcd_dev_status_t dcdDevStatus;
#endif
} usb_hid_mouse_struct_t;
/*******************************************************************************
 * API
 ******************************************************************************/
void USB_DeviceApplicationInit(void);
void APP_task(void *handle);
#endif /* __USB_HID_MOUSE_H__ */
