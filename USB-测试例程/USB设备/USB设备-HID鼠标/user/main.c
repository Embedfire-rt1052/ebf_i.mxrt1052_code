/**
 ******************************************************************
 * @file    main.c
 * @author  fire
 * @version V1.0
 * @date    2018-xx-xx
 * @brief   USB�豸-HID���
 ******************************************************************
 * @attention
 *
 * ʵ��ƽ̨:Ұ��  i.MXRT1052������ 
 * ��̳    :http://www.firebbs.cn
 * �Ա�    :http://firestm32.taobao.com
 *
 ******************************************************************
 */

#include "board.h"
#include "fsl_debug_console.h"
#include "clock_config.h"
#include "pin_mux.h"

#include <stdio.h>
#include <stdlib.h>
#include "bsp_driver_mouse.h" 



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
			
    }
}
