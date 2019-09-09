/**
 ******************************************************************
 * @file    main.c
 * @author  fire
 * @version V1.0
 * @date    2018-xx-xx
 * @brief   SRAM动态创建多任务
 ******************************************************************
 * @attention
 *
 * 实验平台:野火  i.MXRT1052开发板 
 * 论坛    :http://www.firebbs.cn
 * 淘宝    :http://firestm32.taobao.com
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





/**************************** 任务句柄 ********************************/
/* 
* 任务句柄是一个指针，用于指向一个任务，当任务创建好之后，它就具有了一个任务句柄
* 以后我们要想操作这个任务都需要通过这个任务句柄，如果是自身的任务操作自己，那么
* 这个句柄可以为NULL。
*/

/* LED1任务句柄 */
static TaskHandle_t LED1_Task_Handle = NULL;

/********************************** 内核对象句柄 *********************************/
/*
* 信号量，消息队列，事件标志组，软件定时器这些都属于内核的对象，要想使用这些内核
* 对象，必须先创建，创建成功之后会返回一个相应的句柄。实际上就是一个指针，后续我
* 们就可以通过这个句柄操作这些内核对象。
*
* 内核对象说白了就是一种全局的数据结构，通过这些数据结构我们可以实现任务间的通信，
* 任务间的事件同步等各种功能。至于这些功能的实现我们是通过调用这些内核对象的函数
* 来完成的
* 
*/


/******************************* 全局变量声明 ************************************/
/*
* 当我们在写应用程序的时候，可能需要用到一些全局变量。
*/



/*
*************************************************************************
*                             函数声明
*************************************************************************
*/


static void LED1_Task(void *pvParameters); /* LED1_Task任务实现 */

static void BSP_Init(void); /* 用于初始化板载相关资源 */





/**********************************************************************
 * @ 函数名  ： LED_Task
 * @ 功能说明： LED_Task任务主体
 * @ 参数    ：   
 * @ 返回值  ： 无
 ********************************************************************/
static void LED1_Task(void *parameter)
{
    while (1)
    {
        LED1_ON;
        vTaskDelay(500); /* 延时500个tick */
        PRINTF("LED1_Task Running,LED1_ON\r\n");

        LED1_OFF;
        vTaskDelay(500); /* 延时500个tick */
        PRINTF("LED1_Task Running,LED1_OFF\r\n");
    }
}




/*定义device-USB-mouse*/
extern usb_hid_mouse_struct_t g_UsbDeviceHidMouse;

/*****************************************************************
 * @brief  主函数
 * @param  无
 * @retval 无
 * @note   第一步：开发板硬件初始化 
           第二步：创建APP应用任务
           第三步：启动FreeRTOS，开始多任务调度
 ****************************************************************/
int main(void)
{
    BaseType_t xReturn = pdPASS; /* 定义一个创建信息返回值，默认为pdPASS */

    /* 开发板硬件初始化 */
    BSP_Init();
    PRINTF("这是一个[野火]-全系列开发板-FreeRTOS-动态创建多任务实验!\r\n");
    /* 创建LED_Task任务 */
    xReturn = xTaskCreate((TaskFunction_t)LED1_Task,          /* 任务入口函数 */
                          (const char *)"LED1_Task",          /* 任务名字 */
                          (uint16_t)512,                      /* 任务栈大小 */
                          (void *)NULL,                       /* 任务入口函数参数 */
                          (UBaseType_t)4,                     /* 任务的优先级 */
                          (TaskHandle_t *)&LED1_Task_Handle); /* 任务控制块指针 */
    if (pdPASS == xReturn)
        PRINTF("创建LED1_Task任务成功!\r\n");

    /* 创建LED_Task任务 */
    xReturn = xTaskCreate((TaskFunction_t)APP_task,          /* 任务入口函数 */
                          (const char *)"app task",          /* 任务名字 */
                          (uint16_t)5000L / sizeof(portSTACK_TYPE),                      /* 任务栈大小 */
                          (void *)&g_UsbDeviceHidMouse,                       /* 任务入口函数参数 */
                          (UBaseType_t)3,                     /* 任务的优先级 */
                          (TaskHandle_t *)&g_UsbDeviceHidMouse.applicationTaskHandle); /* 任务控制块指针 */
    if (pdPASS == xReturn)
        PRINTF("创建LED2_Task任务成功!\r\n");

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

    /* 硬件BSP初始化统统放在这里，比如LED，串口，LCD等 */

    /* LED 端口初始化 */
    LED_GPIO_Config();
}
