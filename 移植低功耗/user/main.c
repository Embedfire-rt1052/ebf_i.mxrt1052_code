#include "fsl_common.h"

#include "board.h"
#include "fsl_debug_console.h"
#include "bsp_lpm.h"
#include "fsl_gpt.h"
#include "fsl_lpuart.h"
#include "bsp_specific.h"

#include "pin_mux.h"
#include "clock_config.h" 


/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define CPU_NAME "iMXRT1052"

#define APP_WAKEUP_BUTTON_GPIO BOARD_USER_BUTTON_GPIO
#define APP_WAKEUP_BUTTON_GPIO_PIN BOARD_USER_BUTTON_GPIO_PIN
#define APP_WAKEUP_BUTTON_IRQ BOARD_USER_BUTTON_IRQ
#define APP_WAKEUP_BUTTON_IRQ_HANDLER BOARD_USER_BUTTON_IRQ_HANDLER
#define APP_WAKEUP_BUTTON_NAME BOARD_USER_BUTTON_NAME

#define APP_WAKEUP_GPT_BASE GPT2
#define APP_WAKEUP_GPT_IRQn GPT2_IRQn
#define APP_WAKEUP_GPT_IRQn_HANDLER GPT2_IRQHandler

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
//static uint8_t s_wakeupTimeout;            /* Wakeup timeout. (Unit: Second) */
//static app_wakeup_source_t s_wakeupSource; /* Wakeup source.                 */

static lpm_power_mode_t s_curRunMode = LPM_PowerModeOverRun;




void APP_SetRunMode(lpm_power_mode_t powerMode)
{
    s_curRunMode = powerMode;
}


void delay(uint32_t count)
{
  volatile uint32_t i = 0;
  for (i = 0; i < count; ++i)
  {
    __asm("NOP"); /* ����nop��ָ�� */
  }
}




/*!
 * @brief main demo function.
 */
int main(void)
{

    /* Init board hardware. */
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    
    /* Configure UART divider to default ���� ����ʱ�ӵ�oscʱ��*/
    CLOCK_SetMux(kCLOCK_UartMux, 1); /* Set UART source to OSC 24M */
    CLOCK_SetDiv(kCLOCK_UartDiv, 0); /* Set UART divider to 1 */
  
  
    
  
  
    BOARD_InitDebugConsole();
    PRINTF("\r\n *************22**********\r\n");

    /* Since SNVS_PMIC_STBY_REQ_GPIO5_IO02 will output a high-level signal under Stop Mode(Suspend Mode) and this pin is
     * connected to LCD power switch circuit. So it needs to be configured as a low-level output GPIO to reduce the
     * current. */
    BOARD_Init_PMIC_STBY_REQ();


    APP_PrintRunFrequency(1);
    delay(90000000);

      
    LPM_Init();
    delay(90000000);
    /* Set power mode to over run after power on */
//    APP_SetRunMode(LPM_PowerModeOverRun);

    
    LPM_OverDriveRun();
    delay(90000000);
    PRINTF("\r\n set over run\r\n");
    APP_PrintRunFrequency(1);

   
    
//    LPM_LowSpeedRun();//  132
//    APP_PrintRunFrequency(1);

    
    LPM_LowPowerRun();
    APP_PrintRunFrequency(1);

//    
//    ClockSetToLowPowerIdle();  //24
//    APP_PrintRunFrequency(1);
    while (1)
    {

    }
}

















































///**
//  ******************************************************************
//  * @file    main.c
//  * @author  fire
//  * @version V1.0
//  * @date    2018-xx-xx
//  * @brief   �����жϽ��ղ���
//  ******************************************************************
//  * @attention
//  *
//  * ʵ��ƽ̨:Ұ��  i.MXRT1052������ 
//  * ��̳    :http://www.firebbs.cn
//  * �Ա�    :http://firestm32.taobao.com
//  *CPU_MIMXRT1052DVL6B,PRINTF_FLOAT_ENABLE=1, SCANF_FLOAT_ENABLE=1, PRINTF_ADVANCED_ENABLE=1, SCANF_ADVANCED_ENABLE=1
//  ******************************************************************
//  */

//#include "fsl_debug_console.h"

//#include "board.h"
//#include "pin_mux.h"

//#include "clock_config.h"
//#include "./bsp/nvic/bsp_nvic.h"
//#include "./bsp/led/bsp_led.h"
//#include "./bsp/adc/bsp_adc.h"
//#include "bsp_lpm.h"
//#include "bsp_specific.h"

//volatile bool ADC_ConversionDoneFlag; //��־λ
//volatile uint32_t ADC_ConvertedValue;


//static lpm_power_mode_t s_curRunMode = LPM_PowerModeOverRun;
//static const char *s_modeNames[]     = {"Over RUN",    "Full Run",       "Low Speed Run", "Low Power Run",
//                                    "System Idle", "Low Power Idle", "Suspend",
//#if (HAS_WAKEUP_PIN)
//                                    "SNVS"
//#endif
//};



///*******************************************************************
// * Prototypes
// *******************************************************************/
///**
// * @brief ��ʱһ��ʱ��
// */
//void delay(uint32_t count);

///*******************************************************************
// * Code
// *******************************************************************/
///**
// * @note �������ڲ�ͬ���Ż�ģʽ����ʱʱ�䲻ͬ��
// *       ��flexspi_nor_debug��flexspi_nor_release�汾�ĳ����У�
// *       flexspi_nor_release�汾����ʱҪ�̵ö�  
// */
//void delay(uint32_t count)
//{
//  volatile uint32_t i = 0;
//  for (i = 0; i < count; ++i)
//  {
//    __asm("NOP"); /* ����nop��ָ�� */
//  }
//}


///*******************2.6��������**********************/
//void APP_SetRunMode(lpm_power_mode_t powerMode)
//{
//    s_curRunMode = powerMode;
//}



//static void APP_ShowPowerMode(lpm_power_mode_t powerMode)
//{
//    if (powerMode <= LPM_PowerModeRunEnd)
//    {
//        PRINTF("    Power mode: %s\r\n", s_modeNames[powerMode]);
//        APP_PrintRunFrequency(1);
//    }
//    else
//    {
//        assert(0);
//    }
//}









///**
//  * @brief  ������
//  * @param  ��
//  * @retval ��
//  */
//int main(void)
//{

//  /* ��ʼ���ڴ汣����Ԫ */
//  BOARD_ConfigMPU();
//  /* ��ʼ������������ */
//  BOARD_InitPins();
//  /* ��ʼ��������ʱ�� */
//  BOARD_BootClockRUN();
//  /* ��ʼ�����Դ��� */
//  BOARD_InitDebugConsole();

//  /*�����ж����ȼ�����*/
//  Set_NVIC_PriorityGroup(Group_4);



//  /* ��ʼ��LED���� */
//  LED_GPIO_Config();
//  /* Define the init structure for the input switch pin */
//  gpio_pin_config_t swConfig = {
//      kGPIO_DigitalInput,
//      0,
//      kGPIO_IntRisingEdge,
//  };

//// /*****************���³�ʼ��UART1*******************/

//  
//  
////  /* Configure UART divider to default ���� ����ʱ�ӵ�oscʱ��*/
////  CLOCK_SetMux(kCLOCK_UartMux, 1); /* Set UART source to OSC 24M */
////  CLOCK_SetDiv(kCLOCK_UartDiv, 0); /* Set UART divider to 1 */

//  BOARD_InitDebugConsole();

//  /* Since SNVS_PMIC_STBY_REQ_GPIO5_IO02 will output a high-level signal under Stop Mode(Suspend Mode) and this pin is
//     * connected to LCD power switch circuit. So it needs to be configured as a low-level output GPIO to reduce the
//     * current. */
//    BOARD_Init_PMIC_STBY_REQ();

//  APP_PrintRunFrequency(0);

//  
//  LPM_Init();
//  /* Take some delay */
//  SDK_DelayAtLeastUs(40000);

//  /* Set power mode to over run after power on */
//  APP_SetRunMode(LPM_PowerModeOverRun);
//  LPM_OverDriveRun();
//  
//  
//  
//  APP_PrintRunFrequency(0);

//  while (1)
//  {
//    
//  }
//}

///****************************END OF FILE**********************/
