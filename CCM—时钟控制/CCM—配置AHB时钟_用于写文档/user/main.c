/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   用V2.3.1版本库建的工程模板
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
#include "fsl_gpio.h"
#include "fsl_gpt.h"

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"

#include "./led/bsp_led.h"   
#include "./SysTick/bsp_SysTick.h"



/*******************************************************************
 * Prototypes
 *******************************************************************/
/*!
 * @brief delay a while.
 */
void delay(uint32_t count);

/*******************************************************************
 * Code
 *******************************************************************/
void delay(uint32_t count)
{
    volatile uint32_t i = 0;
    for (i = 0; i < count; ++i)
    {
        __asm("NOP"); /* delay */
    }
}

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
    /* 初始化内存管理单元 */
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
  
    PRINTF("SysTick—系统滴答定时器\r\n");
  
    /* 初始化LED引脚 */
    LED_GPIO_Config();  
    
    /* 启动SysTick */
    SysTick_Init();
    
    while(1)
    {
      /* LED亮 */
      CORE_BOARD_LED_ON;
      RGB_LED_COLOR_GREEN;
      /* 延时 */

      //SysTick_Delay_Ms(500);
      Delay_ms(500);
      
      /* LED灭 */
      CORE_BOARD_LED_OFF;
      RGB_LED_COLOR_OFF;
      /* 延时 */

      //SysTick_Delay_Ms(500);
      Delay_ms(500);
      
    }     

}
/****************************END OF FILE**********************/




/*-------------------------------------------------------华丽的分割线------------------------------------------------*/


void BOARD_BootClockRUN(void)
{
  /********************第一部分**********************/
    /* 保存 RTC OSC 时钟频率. */
    CLOCK_SetRtcXtalFreq(32768U);
    /* 保存 XTAL 24MHz时钟频率. */
    CLOCK_SetXtalFreq(24000000U);
    
  /****************************第二部分***************************/
    /* 设置时钟选择模块PeriphClk2Mux 和 PeriphMux，选择时钟*/
    CLOCK_SetMux(kCLOCK_PeriphClk2Mux, 1); /*PERIPH_CLK2 MUX选择OSC */
    CLOCK_SetMux(kCLOCK_PeriphMux, 1);     /* PERIPH_CLK MUX选择PERIPH_CLK2*/
    
  /******************************第三部分****************************/ 
    /* 设置VDD_SOC 为 1.5V*/
    DCDC->REG3 = (DCDC->REG3 & (~DCDC_REG3_TRG_MASK)) | DCDC_REG3_TRG(0x12);
    /*等待设置生效*/
    while (DCDC_REG0_STS_DC_OK_MASK !=(DCDC_REG0_STS_DC_OK_MASK & DCDC->REG0))
    {
    }
    
    /********************************第四部分************************/   
    /* 初始化 ARM PLL(PLL1). */
    CLOCK_InitArmPll(&armPllConfig_BOARD_BootClockRUN);
    /* 初始化System PLL. */
#ifndef SKIP_SYSCLK_INIT
    CLOCK_InitSysPll(&sysPllConfig_BOARD_BootClockRUN);
#endif
    /* Init Usb1 PLL. */
#if !(defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1))
    CLOCK_InitUsb1Pll(&usb1PllConfig_BOARD_BootClockRUN);
#endif
    /* Enbale Audio PLL output. */
    CCM_ANALOG->PLL_AUDIO |= CCM_ANALOG_PLL_AUDIO_ENABLE_MASK;
    /* Enbale Video PLL output. */
    CCM_ANALOG->PLL_VIDEO |= CCM_ANALOG_PLL_VIDEO_ENABLE_MASK;
    /* Enable ENET PLL output. */
    CCM_ANALOG->PLL_ENET |= CCM_ANALOG_PLL_ENET_ENABLE_MASK;
    CCM_ANALOG->PLL_ENET |= CCM_ANALOG_PLL_ENET_ENET_25M_REF_EN_MASK;
    
    /*******************************第五部分************************/
    /* Set periph clock2 clock source. */ 
    CLOCK_SetMux(kCLOCK_PeriphClk2Mux, 0); //CBCMR[PERIPH_CLK2_SEL] 
    /* Set PERIPH_CLK2_PODF. */
    CLOCK_SetDiv(kCLOCK_PeriphClk2Div, 0); //CBCDR[PERIPH_CLK2_PODF]
    
    /*******************************第六部分************************/
    /* Set periph clock source. */
    CLOCK_SetMux(kCLOCK_PeriphMux, 0); //CBCDR[PERIPH_CLK_SEL]
    /* Set AHB_PODF. */
    CLOCK_SetDiv(kCLOCK_AhbDiv, 0);    //CBCDR[AHB_PODF]
    /* Set IPG_PODF. */
    CLOCK_SetDiv(kCLOCK_IpgDiv, 3);    //CBCDR[IPG_PODF]
    /* Set ARM_PODF. */
    CLOCK_SetDiv(kCLOCK_ArmDiv, 1);    //CACRR[ARM_PODF]
    /* Set preperiph clock source. */
    CLOCK_SetMux(kCLOCK_PrePeriphMux, 3);//CBCMR[PRE_PERIPH_CLK_SEL]
    
    /*****************************第七部分****************************/
    /* Set PERCLK_PODF. */
    CLOCK_SetDiv(kCLOCK_PerclkDiv, 1);//CSCMR1[PERCLK_PODF]
    /* Set periph clock source. */
    CLOCK_SetMux(kCLOCK_PerclkMux, 0);//CSCMR1[PRECLK_CLK_SEL]
    
    /****************************第八部分****************************/
    /* Set Usdhc1 clock source. */
    CLOCK_SetMux(kCLOCK_Usdhc1Mux, 0);//CSCMR1[USDHCn_CLK_SEL]
    /* Set USDHC1_PODF. */
    CLOCK_SetDiv(kCLOCK_Usdhc1Div, 1);//CSCDR1[USDHCn_PODF]
    
    /******************************第九部****************************/
    /* Set Usdhc2 clock source. */
    CLOCK_SetMux(kCLOCK_Usdhc2Mux, 0);
    /* Set USDHC2_PODF. */
    CLOCK_SetDiv(kCLOCK_Usdhc2Div, 1);
   /***其余外设时钟设置与第七、八、九部分类似，详细请参考ckock_config.c********/
}


static inline void CLOCK_SetRtcXtalFreq(uint32_t freq)
{
    g_rtcXtalFreq = freq;
}

static inline void CLOCK_SetXtalFreq(uint32_t freq)
{
    g_xtalFreq = freq;
}





/******************************第一部分****************************/
const clock_arm_pll_config_t armPllConfig_BOARD_BootClockRUN = {
    .loopDivider = 88, /* PLL loop divider, Fout = Fin * 44 */
};
/*****************************第二部分****************************/
CLOCK_InitArmPll(&armPllConfig_BOARD_BootClockRUN);







    /* Set periph clock source. */
    CLOCK_SetMux(kCLOCK_PeriphMux, 0); //CBCDR[PERIPH_CLK_SEL]
    
    /*******************************第一部分**********************/
    /* Set AHB_PODF. */
    CLOCK_SetDiv(kCLOCK_AhbDiv, 0);    //CBCDR[AHB_PODF]
    
    /*******************************第二部分*********************/
    /* Set IPG_PODF. */
    CLOCK_SetDiv(kCLOCK_IpgDiv, 3);    //CBCDR[IPG_PODF]
    /********************************第*/
    /* Set ARM_PODF. */
    CLOCK_SetDiv(kCLOCK_ArmDiv, 1);    //CACRR[ARM_PODF]
    /* Set preperiph clock source. */
    CLOCK_SetMux(kCLOCK_PrePeriphMux, 3);//CBCMR[PRE_PERIPH_CLK_SEL]




























