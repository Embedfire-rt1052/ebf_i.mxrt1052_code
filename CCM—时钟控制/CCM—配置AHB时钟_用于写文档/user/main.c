/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   ��V2.3.1�汾�⽨�Ĺ���ģ��
  ******************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  i.MXRT1052������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
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
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
    /* ��ʼ���ڴ����Ԫ */
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
  
    PRINTF("SysTick��ϵͳ�δ�ʱ��\r\n");
  
    /* ��ʼ��LED���� */
    LED_GPIO_Config();  
    
    /* ����SysTick */
    SysTick_Init();
    
    while(1)
    {
      /* LED�� */
      CORE_BOARD_LED_ON;
      RGB_LED_COLOR_GREEN;
      /* ��ʱ */

      //SysTick_Delay_Ms(500);
      Delay_ms(500);
      
      /* LED�� */
      CORE_BOARD_LED_OFF;
      RGB_LED_COLOR_OFF;
      /* ��ʱ */

      //SysTick_Delay_Ms(500);
      Delay_ms(500);
      
    }     

}
/****************************END OF FILE**********************/




/*-------------------------------------------------------�����ķָ���------------------------------------------------*/


void BOARD_BootClockRUN(void)
{
  /********************��һ����**********************/
    /* ���� RTC OSC ʱ��Ƶ��. */
    CLOCK_SetRtcXtalFreq(32768U);
    /* ���� XTAL 24MHzʱ��Ƶ��. */
    CLOCK_SetXtalFreq(24000000U);
    
  /****************************�ڶ�����***************************/
    /* ����ʱ��ѡ��ģ��PeriphClk2Mux �� PeriphMux��ѡ��ʱ��*/
    CLOCK_SetMux(kCLOCK_PeriphClk2Mux, 1); /*PERIPH_CLK2 MUXѡ��OSC */
    CLOCK_SetMux(kCLOCK_PeriphMux, 1);     /* PERIPH_CLK MUXѡ��PERIPH_CLK2*/
    
  /******************************��������****************************/ 
    /* ����VDD_SOC Ϊ 1.5V*/
    DCDC->REG3 = (DCDC->REG3 & (~DCDC_REG3_TRG_MASK)) | DCDC_REG3_TRG(0x12);
    /*�ȴ�������Ч*/
    while (DCDC_REG0_STS_DC_OK_MASK !=(DCDC_REG0_STS_DC_OK_MASK & DCDC->REG0))
    {
    }
    
    /********************************���Ĳ���************************/   
    /* ��ʼ�� ARM PLL(PLL1). */
    CLOCK_InitArmPll(&armPllConfig_BOARD_BootClockRUN);
    /* ��ʼ��System PLL. */
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
    
    /*******************************���岿��************************/
    /* Set periph clock2 clock source. */ 
    CLOCK_SetMux(kCLOCK_PeriphClk2Mux, 0); //CBCMR[PERIPH_CLK2_SEL] 
    /* Set PERIPH_CLK2_PODF. */
    CLOCK_SetDiv(kCLOCK_PeriphClk2Div, 0); //CBCDR[PERIPH_CLK2_PODF]
    
    /*******************************��������************************/
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
    
    /*****************************���߲���****************************/
    /* Set PERCLK_PODF. */
    CLOCK_SetDiv(kCLOCK_PerclkDiv, 1);//CSCMR1[PERCLK_PODF]
    /* Set periph clock source. */
    CLOCK_SetMux(kCLOCK_PerclkMux, 0);//CSCMR1[PRECLK_CLK_SEL]
    
    /****************************�ڰ˲���****************************/
    /* Set Usdhc1 clock source. */
    CLOCK_SetMux(kCLOCK_Usdhc1Mux, 0);//CSCMR1[USDHCn_CLK_SEL]
    /* Set USDHC1_PODF. */
    CLOCK_SetDiv(kCLOCK_Usdhc1Div, 1);//CSCDR1[USDHCn_PODF]
    
    /******************************�ھŲ�****************************/
    /* Set Usdhc2 clock source. */
    CLOCK_SetMux(kCLOCK_Usdhc2Mux, 0);
    /* Set USDHC2_PODF. */
    CLOCK_SetDiv(kCLOCK_Usdhc2Div, 1);
   /***��������ʱ����������ߡ��ˡ��Ų������ƣ���ϸ��ο�ckock_config.c********/
}


static inline void CLOCK_SetRtcXtalFreq(uint32_t freq)
{
    g_rtcXtalFreq = freq;
}

static inline void CLOCK_SetXtalFreq(uint32_t freq)
{
    g_xtalFreq = freq;
}





/******************************��һ����****************************/
const clock_arm_pll_config_t armPllConfig_BOARD_BootClockRUN = {
    .loopDivider = 88, /* PLL loop divider, Fout = Fin * 44 */
};
/*****************************�ڶ�����****************************/
CLOCK_InitArmPll(&armPllConfig_BOARD_BootClockRUN);







    /* Set periph clock source. */
    CLOCK_SetMux(kCLOCK_PeriphMux, 0); //CBCDR[PERIPH_CLK_SEL]
    
    /*******************************��һ����**********************/
    /* Set AHB_PODF. */
    CLOCK_SetDiv(kCLOCK_AhbDiv, 0);    //CBCDR[AHB_PODF]
    
    /*******************************�ڶ�����*********************/
    /* Set IPG_PODF. */
    CLOCK_SetDiv(kCLOCK_IpgDiv, 3);    //CBCDR[IPG_PODF]
    /********************************��*/
    /* Set ARM_PODF. */
    CLOCK_SetDiv(kCLOCK_ArmDiv, 1);    //CACRR[ARM_PODF]
    /* Set preperiph clock source. */
    CLOCK_SetMux(kCLOCK_PrePeriphMux, 3);//CBCMR[PRE_PERIPH_CLK_SEL]




























