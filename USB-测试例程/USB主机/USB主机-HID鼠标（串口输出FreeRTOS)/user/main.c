/**
 ******************************************************************
 * @file    main.c
 * @author  fire
 * @version V1.0
 * @date    2018-xx-xx
 * @brief   USB主机-HID鼠标（串口输出FreeRTOS)
 ******************************************************************
 * @attention
 *
 * 实验平台:野火  i.MXRT1052开发板 
 * 论坛    :http://www.firebbs.cn
 * 淘宝    :http://firestm32.taobao.com
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

/******************************* 全局变量声明 ************************************/
/*
* 当我们在写应用程序的时候，可能需要用到一些全局变量。
*/
extern usb_host_handle g_HostHandle;  //"usb host task" 任务入口函数参数 
extern usb_host_mouse_instance_t g_HostHidMouse; //"app task" 任务入口函数参数


/*
*************************************************************************
*                             函数声明
*************************************************************************
*/
static void BSP_Init(void);  //用于初始化板载相关资源

static void USB_HostTask(void *param); //"usb host task" 任务函数
static void USB_HostApplicationTask(void *param);//"app task" 任务函数



 int main(void)
 {
     BaseType_t xReturn = pdPASS; /* 定义一个创建信息返回值，默认为pdPASS */

     /* 开发板硬件初始化 */
     BSP_Init();
     PRINTF("这是一个[野火]-全系列开发板-USB host 鼠标实验!\r\n");
     /* 创建LED_Task任务 */
     xReturn = xTaskCreate((TaskFunction_t)USB_HostTask,          /* 任务入口函数 */
                           (const char *)"usb host task",          /* 任务名字 */
                           (uint16_t)2000L / sizeof(portSTACK_TYPE),                      /* 任务栈大小 */
                           (void *)g_HostHandle,                       /* 任务入口函数参数 */
                           (UBaseType_t)4,                     /* 任务的优先级 */
                           (TaskHandle_t *)NULL); /* 任务控制块指针 */
     if (pdPASS == xReturn)
         PRINTF("创建\"usb host task\"任务成功!\r\n");

     /* 创建LED_Task任务 */
     xReturn = xTaskCreate((TaskFunction_t)USB_HostApplicationTask,          /* 任务入口函数 */
                           (const char *)"app task",          /* 任务名字 */
                           (uint16_t)2000L / sizeof(portSTACK_TYPE),                      /* 任务栈大小 */
                           (void *)&g_HostHidMouse,                       /* 任务入口函数参数 */
                           (UBaseType_t)3,                     /* 任务的优先级 */
                           (TaskHandle_t *)NULL); /* 任务控制块指针 */
     if (pdPASS == xReturn)
         PRINTF("创建\"app task\"任务成功!\r\n");

     /* 启动任务调度 */
     if (pdPASS == xReturn)
         vTaskStartScheduler(); /* 启动任务，开启调度 */
     else
         return -1;

     while (1); /* 正常不会执行到这里 */
 }




/***********************************************************************
 * @ 函数名  ： BSP_Init
 * @ 功能说明： 板级外设初始化，所有板子上的初始化均可放在这个函数里面
 * @ 参数    ：   
 * @ 返回值  ： 无
 *********************************************************************/
static void BSP_Init(void)
{

    /* 初始化内存保护单元 */
    BOARD_ConfigMPU();
    /* 初始化开发板引脚 */
    BOARD_InitPins();
    /* 初始化开发板时钟 */
    BOARD_BootClockRUN();
    /* 初始化调试串口 */
    BOARD_InitDebugConsole();
    /* 打印系统时钟 */
    PRINTF("\r\n");
    PRINTF("*****欢迎使用 野火i.MX RT1052 开发板*****\r\n");
    PRINTF("CPU:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_CpuClk));
    PRINTF("AHB:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_AhbClk));
    PRINTF("SEMC:            %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SemcClk));
    PRINTF("SYSPLL:          %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllClk));
    PRINTF("SYSPLLPFD0:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd0Clk));
    PRINTF("SYSPLLPFD1:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd1Clk));
    PRINTF("SYSPLLPFD2:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd2Clk));
    PRINTF("SYSPLLPFD3:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd3Clk));

    /* 初始化SysTick */
    SysTick_Config(SystemCoreClock / configTICK_RATE_HZ);
    USB_HostApplicationInit();

    /* 硬件BSP初始化统统放在这里，比如LED，串口，LCD等 */
}


/**********************************************************************
 * @ 功能说明： "usb host task" 任务主体
 ********************************************************************/
static void USB_HostTask(void *param)
{
    while (1)
    {
        USB_HostTaskFn(param);
    }
}

/**********************************************************************
 * @ 功能说明： "app task"任务主体
 ********************************************************************/
static void USB_HostApplicationTask(void *param)
{
    while (1)
    {
        USB_HostHidMouseTask(param);
    }
}