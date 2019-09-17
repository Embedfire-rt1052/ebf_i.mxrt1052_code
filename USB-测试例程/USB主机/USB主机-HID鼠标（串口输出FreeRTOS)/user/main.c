/**
 ******************************************************************
 * @file    main.c
 * @author  fire
 * @version V1.0
 * @date    2018-xx-xx
 * @brief   USB����-HID��꣨�������FreeRTOS)
 ******************************************************************
 * @attention
 *
 * ʵ��ƽ̨:Ұ��  i.MXRT1052������ 
 * ��̳    :http://www.firebbs.cn
 * �Ա�    :http://firestm32.taobao.com
 *
 ******************************************************************
 */
#include "bsp_host_mouse.h"
#include "board.h"
#include "host_mouse.h"

#if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
#include "fsl_sysmpu.h"
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */
 
#include "board.h"                
#include "pin_mux.h"
#include "usb_phy.h"
#include "clock_config.h"

/******************************* ȫ�ֱ������� ************************************/
/*
* ��������дӦ�ó����ʱ�򣬿�����Ҫ�õ�һЩȫ�ֱ�����
*/
extern usb_host_handle g_HostHandle;  //"usb host task" ������ں������� 
extern usb_host_mouse_instance_t g_HostHidMouse; //"app task" ������ں�������


/*
*************************************************************************
*                             ��������
*************************************************************************
*/
static void BSP_Init(void);  //���ڳ�ʼ�����������Դ

static void USB_HostTask(void *param); //"usb host task" ������
static void USB_HostApplicationTask(void *param);//"app task" ������



 int main(void)
 {
     BaseType_t xReturn = pdPASS; /* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */

     /* ������Ӳ����ʼ�� */
     BSP_Init();
     PRINTF("����һ��[Ұ��]-ȫϵ�п�����-USB host ���ʵ��!\r\n");
     /* ����LED_Task���� */
     xReturn = xTaskCreate((TaskFunction_t)USB_HostTask,          /* ������ں��� */
                           (const char *)"usb host task",          /* �������� */
                           (uint16_t)2000L / sizeof(portSTACK_TYPE),                      /* ����ջ��С */
                           (void *)g_HostHandle,                       /* ������ں������� */
                           (UBaseType_t)4,                     /* ��������ȼ� */
                           (TaskHandle_t *)NULL); /* ������ƿ�ָ�� */
     if (pdPASS == xReturn)
         PRINTF("����\"usb host task\"����ɹ�!\r\n");

     /* ����LED_Task���� */
     xReturn = xTaskCreate((TaskFunction_t)USB_HostApplicationTask,          /* ������ں��� */
                           (const char *)"app task",          /* �������� */
                           (uint16_t)2000L / sizeof(portSTACK_TYPE),                      /* ����ջ��С */
                           (void *)&g_HostHidMouse,                       /* ������ں������� */
                           (UBaseType_t)3,                     /* ��������ȼ� */
                           (TaskHandle_t *)NULL); /* ������ƿ�ָ�� */
     if (pdPASS == xReturn)
         PRINTF("����\"app task\"����ɹ�!\r\n");

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
    USB_HostApplicationInit();

    /* Ӳ��BSP��ʼ��ͳͳ�����������LED�����ڣ�LCD�� */
}


/**********************************************************************
 * @ ����˵���� "usb host task" ��������
 ********************************************************************/
static void USB_HostTask(void *param)
{
    while (1)
    {
        USB_HostTaskFn(param);
    }
}

/**********************************************************************
 * @ ����˵���� "app task"��������
 ********************************************************************/
static void USB_HostApplicationTask(void *param)
{
    while (1)
    {
        USB_HostHidMouseTask(param);
    }
}