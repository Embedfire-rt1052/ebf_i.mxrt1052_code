#include "fsl_gpio.h"
#include "pad_config.h"

#include "./bsp/nvic/bsp_nvic.h"
#include "./bsp/gpt/bsp_gpt.h"
#include "./bsp/led/bsp_led.h"

/**
* @brief  配置GPT工作模式
* @param  无
* @retval 无
*/
void GPT_Config(void)
{
    gpt_config_t gptConfig;

//    /*GPT的时钟设置*/
  CLOCK_SetMux(kCLOCK_PerclkMux, EXAMPLE_GPT_CLOCK_SOURCE_SELECT);
  CLOCK_SetDiv(kCLOCK_PerclkDiv, EXAMPLE_GPT_CLOCK_DIVIDER_SELECT);
//    CLOCK_SetMux(kCLOCK_PerclkMux, 1); /* 将PIT时钟源设置为OSC 24M */
//    CLOCK_SetDiv(kCLOCK_PerclkDiv, 0); /* 将PIT时钟分频器设置为1 */

    /*初始化GPT*/
    GPT_GetDefaultConfig(&gptConfig);
    gptConfig.clockSource = kGPT_ClockSource_LowFreq; /* 32K RTC OSC */
    // gptConfig.enableMode = false;                     /* 停止时保持计数器 */
    gptConfig.enableMode = true; /* 停止时不要保持计数器 */
    gptConfig.enableRunInDoze = true;
    /* 初始化GPT模块 */
    GPT_Init(WAKEUP_GPT_BASE, &gptConfig);
    /* 设置时钟分频 */
    GPT_SetClockDivider(WAKEUP_GPT_BASE, 1);
}


void SetCompValue()
{
//    /* 得GPT定时器时钟源频率 */
//    gptFreq = EXAMPLE_GPT_CLK_FREQ;

//    /* 设置GPT定时器的计数值*/
//    GPT_SetOutputCompareValue(EXAMPLE_GPT, kGPT_OutputCompare_Channel1, gptFreq);

//    /*设置输出模式*/
//    GPT_SetOutputOperationMode(EXAMPLE_GPT,kGPT_OutputCompare_Channel1,kGPT_OutputOperation_Toggle);
//    /* 开启GPT2定时器比较通道1的中断 */
//    GPT_EnableInterrupts(EXAMPLE_GPT, kGPT_OutputCompare1InterruptEnable);

//    /*设置中断优先级,*/
//    set_IRQn_Priority(GPT_IRQ_ID,Group4_PreemptPriority_6, Group4_SubPriority_0);
//    /*使能中断*/
//    EnableIRQ(GPT_IRQ_ID);

//    /* 开启定时器 */
//    GPT_StartTimer(EXAMPLE_GPT);

}

///*定义中断服务函数*/
//void WAKEUP_GPT_IRQn_HANDLER(void)
//{
//  /* 清除中断标志位.*/
//  GPT_ClearStatusFlags(EXAMPLE_GPT, kGPT_OutputCompare1Flag);
//  i++;
//
//  if(i%2)
//  {
//     RGB_BLUE_LED_OFF;
//  }
//  else
//  {
//     RGB_BLUE_LED_ON;
//  }
//}

