//#ifndef __BSP_driver_mouse_H
//#define __BSP_driver_mouse_H

//#include "usb_device_config.h"
//#include "usb.h"
//#include "usb_device.h"

//#include "usb_device_class.h"
//#include "usb_device_hid.h"
//#include "usb_device_ch9.h"
//#include "usb_device_descriptor.h"
//#include "usb_mouse.h" 
//#include "usb_phy.h"

//#include "clock_config.h"
//#include "board.h"
//#include "pin_mux.h"



///*根据宏定义选择设备 */
//#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
//#define CONTROLLER_ID kUSB_ControllerEhci0
//#endif
//#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
//#define CONTROLLER_ID kUSB_ControllerKhci0
//#endif
//#if defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)
//#define CONTROLLER_ID kUSB_ControllerLpcIp3511Fs0
//#endif
//#if defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)
//#define CONTROLLER_ID kUSB_ControllerLpcIp3511Hs0
//#endif

//#define USB_DEVICE_INTERRUPT_PRIORITY (3U)
//#define USB_HID_MOUSE_REPORT_LENGTH (0x04U)

///*定义USB-MOUSE 结构体，*/
//typedef struct _usb_hid_mouse_struct
//{
//    usb_device_handle deviceHandle;  //USB_DeviceClassInit函数初始化后得到的句柄
//    class_handle_t hidHandle;  
//    uint8_t *buffer;
//    uint8_t currentConfiguration;
//    uint8_t currentInterfaceAlternateSetting[USB_HID_MOUSE_INTERFACE_COUNT];
//    uint8_t speed;
//    uint8_t attach;
//#if (defined(USB_DEVICE_CHARGER_DETECT_ENABLE) && (USB_DEVICE_CHARGER_DETECT_ENABLE > 0U))
//    volatile uint8_t vReginInterruptDetected;
//    volatile uint8_t vbusValid;
//    volatile usb_device_dcd_port_type_t dcdPortType;
//    volatile usb_device_dcd_dev_status_t dcdDevStatus;
//#endif
//} usb_hid_mouse_struct_t;

///*******************************************************************************
// * API
// ******************************************************************************/
//void USB_DeviceApplicationInit(void);
//#endif /* __USB_HID_MOUSE_H__ */
