/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   RTC驱动
  ******************************************************************
  * @attention
  *
  * 实验平台:野火  i.MXRT1052开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************
  */
#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_snvs_hp.h"

#include "pin_mux.h"
#include "stdio.h"
#include "clock_config.h"
#include "./bsp/rtc/bsp_snvs_hp_rtc.h" 
#include "./lcd/bsp_lcd.h" 





//中断相关，IRQ中断号及IRQHandler中断服务函数
#define kCLOCK_SnvsHp0 kCLOCK_SnvsHp
#define EXAMPLE_SNVS_IRQn SNVS_HP_WRAPPER_IRQn
#define EXAMPLE_SNVS_IRQHandler SNVS_HP_WRAPPER_IRQHandler




/* 等待闹钟报警发生标志 */
volatile bool busyWait;




/********************中断服务函数**************************/
/**
 * @brief  RTC 中断服务函数 
 *         EXAMPLE_SNVS_IRQHandler是一个宏，
 *         在本例中它指代函数名SNVS_HP_WRAPPER_IRQHandler，
 *         中断服务函数名是固定的，可以在启动文件中找到。
 * @param  中断服务函数不能有输入参数
 * @note   中断函数一般只使用标志位进行指示，完成后尽快退出，
 *         具体操作或延时尽量不放在中断服务函数中，
 * @retval 中断服务函数不能有返回值
 */
void EXAMPLE_SNVS_IRQHandler(void)
{
	  /* 判断中断条件是否满足 */
    if (SNVS_HP_RTC_GetStatusFlags(SNVS) & kSNVS_RTC_AlarmInterruptFlag)
    {
			  /* 设置报警中断标志 */
        busyWait = false;

        /* 清除报警标志 */
        SNVS_HP_RTC_ClearStatusFlags(SNVS, kSNVS_RTC_AlarmInterruptEnable);
    }
    /* 添加为ARM勘误表838869，影响Cortex-M4，Cortex-M4F存储立即重叠
       异常返回操作可能会导致错误的中断 */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}




/**
  * @brief  初始化RTC相关配置
  * @param  无
  * @retval 无
  */
void RTC_Config(void)
{
		snvs_hp_rtc_datetime_t rtcDate;/* 定义 rtc 日期配置结构体 */
    snvs_hp_rtc_config_t snvsRtcConfig;/* 定义 snvsRtc 配置结构体 */

    /* 初始化SNVS */
    /*
     * snvsConfig->rtccalenable = false;
     * snvsConfig->rtccalvalue = 0U;
     * snvsConfig->srtccalenable = false;
     * snvsConfig->srtccalvalue = 0U;
     * snvsConfig->PIFreq = 0U;
     */
    /* 获取默认配置 */
    SNVS_HP_RTC_GetDefaultConfig(&snvsRtcConfig);
    /* 初始化RTC */
    SNVS_HP_RTC_Init(SNVS, &snvsRtcConfig);

    /* 设置日期 */
    rtcDate.year = 2018U;
    rtcDate.month = 04U;
    rtcDate.day = 21U;
    rtcDate.hour = 8U;
    rtcDate.minute = 0;
    rtcDate.second = 0;

    /* 将RTC时间设置为默认时间和日期并启动RTC */
    SNVS_HP_RTC_SetDatetime(SNVS, &rtcDate);
    SNVS_HP_RTC_StartTimer(SNVS);
}


/**
  * @brief  显示时间和日期
  * @param  无
  * @retval 无
  */
void RTC_TimeAndDate_Show(void)
{
    uint8_t Rtctmp=0;//定义临时变量用于刷新屏幕显示
    char LCDTemp[100];//定义字符串缓存数组
    snvs_hp_rtc_datetime_t rtcDate;//定义全局RTC时间结构体	
    while(1)
    {
        /* 获取日期 */
        SNVS_HP_RTC_GetDatetime(SNVS, &rtcDate);
        /* 每秒打印一次 */ 
        if(Rtctmp != rtcDate.second)
        {
        
            /* 打印日期 */ 
            PRINTF("The Date :  Y:%0.2d - M:%0.2d - D:%0.2d\r\n", 
            rtcDate.year,
            rtcDate.month, 
            rtcDate.day
            );
            
            /*液晶显示日期*/
            /*先把要显示的数据用sprintf函数转换为字符串，然后才能用液晶显示函数显示*/
            sprintf(LCDTemp,"The Date :  Y:%0.2d - M:%0.2d - D:%0.2d", 
            rtcDate.year,
            rtcDate.month, 
            rtcDate.day
            );
            
            LCD_SetColors(CL_RED,CL_BLACK);/*设置字体的颜色及字体的背景颜色*/
            LCD_DisplayStringLine(10,(uint8_t *)LCDTemp); /*将字符串显示到屏幕上*/
            
            /*打印时间*/
            PRINTF("The Time :  %0.2d:%0.2d:%0.2d \r\n\r\n", 
            rtcDate.hour, 
            rtcDate.minute, 
            rtcDate.second);
            
            /*液晶显示时间*/
            /*先把要显示的数据用sprintf函数转换为字符串，然后才能用液晶显示函数显示*/
            sprintf(LCDTemp,"The Time :  %0.2d:%0.2d:%0.2d", 
            rtcDate.hour, 
            rtcDate.minute, 
            rtcDate.second);
            /*将字符串显示到屏幕上*/
            LCD_DisplayStringLine(50,(uint8_t *)LCDTemp);
        
        }
        Rtctmp = rtcDate.second;
    }
}



/**
  * @brief  RTC闹钟测试
  * @param  无
  * @retval 无
  */
void RTC_AlarmTest(void)
{
	  uint32_t sec;//用户所输入的等待报警时间
    uint8_t index;//用于接收串口数据
		snvs_hp_rtc_datetime_t rtcDate;//定义全局RTC时间结构体

	  /* 启用 SNVS 闹钟中断 */
    SNVS_HP_RTC_EnableInterrupts(SNVS, kSNVS_RTC_AlarmInterruptEnable);

    /* 使能 SNVS 中断 */
    EnableIRQ(EXAMPLE_SNVS_IRQn);
    PRINTF("设置闹钟时间.\r\n");
    /* 大循环内设置闹钟时间 */
    while (1)
    {
			/* 设置临时变量的初始值 */
        busyWait = true;
        index = 0;
        sec = 0;

        /* 获取日期 */
        SNVS_HP_RTC_GetDatetime(SNVS, &rtcDate);

        /* 打印默认时间 */
        PRINTF("当前时间: %04hd-%02hd-%02hd %02hd:%02hd:%02hd\r\n", rtcDate.year, rtcDate.month, rtcDate.day,
               rtcDate.hour, rtcDate.minute, rtcDate.second);

        /* 用户输入闹钟时间 */
        PRINTF("请输入秒数等待闹钟报警并按回车键 \r\n");
        PRINTF("秒数必须是正值\r\n");

        while (index != 0x0D)
        {
					/* 等待获取输入的时间 */
            index = GETCHAR();
            if((index >= '0') && (index <= '9'))
            {
                PUTCHAR(index);
							/* 提取实际输入的时间秒数 */
                sec = sec * 10 + (index - 0x30U);
            }
        }
        PRINTF("\r\n");

        SNVS_HP_RTC_GetDatetime(SNVS, &rtcDate);
				
				/* 不满足60秒时，直接将输入时间累加到 秒数位 */
        if ((rtcDate.second + sec) < 60)
        {
            rtcDate.second += sec;
        }
        else
        {
				/* 将用户输入时间累加到实际时钟，并计算出应该报警的实际时间 */
            rtcDate.minute += (rtcDate.second + sec) / 60U;
            rtcDate.second = (rtcDate.second + sec) % 60U;
        }

				/* 设置闹钟时间 */				
        SNVS_HP_RTC_SetAlarm(SNVS, &rtcDate);

        /* 获取闹钟时间 */
        SNVS_HP_RTC_GetAlarm(SNVS, &rtcDate);

        /* 打印闹钟报警时间 */
        PRINTF("闹钟报警时间: %04hd-%02hd-%02hd %02hd:%02hd:%02hd\r\n", rtcDate.year, rtcDate.month, rtcDate.day,
               rtcDate.hour, rtcDate.minute, rtcDate.second);

        /* 等待闹钟警报发生 */
        while (busyWait)
        {
        }

        PRINTF("\r\n 闹钟警报发生 !!!! ");
    }
}
