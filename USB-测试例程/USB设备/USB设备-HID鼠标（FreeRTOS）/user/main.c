/**
 ******************************************************************
 * @file    main.c
 * @author  fire
 * @version V1.0
 * @date    2018-xx-xx
 * @brief   SRAM��̬����������
 ******************************************************************
 * @attention
 *
 * ʵ��ƽ̨:Ұ��  i.MXRT1052������ 
 * ��̳    :http://www.firebbs.cn
 * �Ա�    :http://firestm32.taobao.com
 *
 ******************************************************************
 */




#include "fsl_device_registers.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"

#include <stdio.h>
#include <stdlib.h>

#include "pin_mux.h"


#include "bsp_led.h"
#include "bsp_driver_mouse.h" 





/**************************** ������ ********************************/
/* 
* ��������һ��ָ�룬����ָ��һ�����񣬵����񴴽���֮�����;�����һ��������
* �Ժ�����Ҫ��������������Ҫͨ�������������������������������Լ�����ô
* ����������ΪNULL��
*/

/* LED1������ */
static TaskHandle_t LED1_Task_Handle = NULL;

/********************************** �ں˶����� *********************************/
/*
* �ź�������Ϣ���У��¼���־�飬�����ʱ����Щ�������ں˵Ķ���Ҫ��ʹ����Щ�ں�
* ���󣬱����ȴ����������ɹ�֮��᷵��һ����Ӧ�ľ����ʵ���Ͼ���һ��ָ�룬������
* �ǾͿ���ͨ��������������Щ�ں˶���
*
* �ں˶���˵���˾���һ��ȫ�ֵ����ݽṹ��ͨ����Щ���ݽṹ���ǿ���ʵ��������ͨ�ţ�
* �������¼�ͬ���ȸ��ֹ��ܡ�������Щ���ܵ�ʵ��������ͨ��������Щ�ں˶���ĺ���
* ����ɵ�
* 
*/


/******************************* ȫ�ֱ������� ************************************/
/*
* ��������дӦ�ó����ʱ�򣬿�����Ҫ�õ�һЩȫ�ֱ�����
*/



/*
*************************************************************************
*                             ��������
*************************************************************************
*/


static void LED1_Task(void *pvParameters); /* LED1_Task����ʵ�� */

static void BSP_Init(void); /* ���ڳ�ʼ�����������Դ */





/**********************************************************************
 * @ ������  �� LED_Task
 * @ ����˵���� LED_Task��������
 * @ ����    ��   
 * @ ����ֵ  �� ��
 ********************************************************************/
static void LED1_Task(void *parameter)
{
    while (1)
    {
        LED1_ON;
        vTaskDelay(500); /* ��ʱ500��tick */
        PRINTF("LED1_Task Running,LED1_ON\r\n");

        LED1_OFF;
        vTaskDelay(500); /* ��ʱ500��tick */
        PRINTF("LED1_Task Running,LED1_OFF\r\n");
    }
}




/*����device-USB-mouse*/
extern usb_hid_mouse_struct_t g_UsbDeviceHidMouse;

/*****************************************************************
 * @brief  ������
 * @param  ��
 * @retval ��
 * @note   ��һ����������Ӳ����ʼ�� 
           �ڶ���������APPӦ������
           ������������FreeRTOS����ʼ���������
 ****************************************************************/
int main(void)
{
    BaseType_t xReturn = pdPASS; /* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */

    /* ������Ӳ����ʼ�� */
    BSP_Init();
    PRINTF("����һ��[Ұ��]-ȫϵ�п�����-FreeRTOS-��̬����������ʵ��!\r\n");
    /* ����LED_Task���� */
    xReturn = xTaskCreate((TaskFunction_t)LED1_Task,          /* ������ں��� */
                          (const char *)"LED1_Task",          /* �������� */
                          (uint16_t)512,                      /* ����ջ��С */
                          (void *)NULL,                       /* ������ں������� */
                          (UBaseType_t)4,                     /* ��������ȼ� */
                          (TaskHandle_t *)&LED1_Task_Handle); /* ������ƿ�ָ�� */
    if (pdPASS == xReturn)
        PRINTF("����LED1_Task����ɹ�!\r\n");

    /* ����LED_Task���� */
    xReturn = xTaskCreate((TaskFunction_t)APP_task,          /* ������ں��� */
                          (const char *)"app task",          /* �������� */
                          (uint16_t)5000L / sizeof(portSTACK_TYPE),                      /* ����ջ��С */
                          (void *)&g_UsbDeviceHidMouse,                       /* ������ں������� */
                          (UBaseType_t)3,                     /* ��������ȼ� */
                          (TaskHandle_t *)&g_UsbDeviceHidMouse.applicationTaskHandle); /* ������ƿ�ָ�� */
    if (pdPASS == xReturn)
        PRINTF("����LED2_Task����ɹ�!\r\n");

    /* ����������� */
    if (pdPASS == xReturn)
        vTaskStartScheduler(); /* �������񣬿������� */
    else
        return -1;

    while (1); /* ��������ִ�е����� */
}



/***********************************************************************
 * @ ������  �� BSP_Init
 * @ ����˵���� �弶�����ʼ�������а����ϵĳ�ʼ�����ɷ��������������
 * @ ����    ��   
 * @ ����ֵ  �� ��
 *********************************************************************/
static void BSP_Init(void)
{

    /* ��ʼ���ڴ汣����Ԫ */
    BOARD_ConfigMPU();
    /* ��ʼ������������ */
    BOARD_InitPins();
    /* ��ʼ��������ʱ�� */
    BOARD_BootClockRUN();
    /* ��ʼ�����Դ��� */
    BOARD_InitDebugConsole();
    /* ��ӡϵͳʱ�� */
    PRINTF("\r\n");
    PRINTF("*****��ӭʹ�� Ұ��i.MX RT1052 ������*****\r\n");
    PRINTF("CPU:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_CpuClk));
    PRINTF("AHB:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_AhbClk));
    PRINTF("SEMC:            %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SemcClk));
    PRINTF("SYSPLL:          %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllClk));
    PRINTF("SYSPLLPFD0:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd0Clk));
    PRINTF("SYSPLLPFD1:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd1Clk));
    PRINTF("SYSPLLPFD2:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd2Clk));
    PRINTF("SYSPLLPFD3:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd3Clk));

    /* ��ʼ��SysTick */
    SysTick_Config(SystemCoreClock / configTICK_RATE_HZ);

    /* Ӳ��BSP��ʼ��ͳͳ�����������LED�����ڣ�LCD�� */

    /* LED �˿ڳ�ʼ�� */
    LED_GPIO_Config();
}
