/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   DCD初始化
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
#include "pin_mux.h"
#include "clock_config.h"

#include "./led/bsp_led.h"


extern bool SEMC_SDRAMReadWriteTest(void);
extern void SEMC_SDRAMReadWriteSpeedTest(void);
extern bool SDRAM_FullChipTest(void);

/*******************************************************************
 * Prototypes
 *******************************************************************/


/*******************************************************************
 * Code
 *******************************************************************/

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
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
    
    LED_GPIO_Config();
 
    /*在DCD配置中已完成初始化，不需要再初始化SDRAM */
//    /* 初始化SDRAM */
//    if (SDRAM_Init() != kStatus_Success)
//    {
//        RGB_LED_COLOR_RED;
//        PRINTF("\r\n SEMC SDRAM Init Failed\r\n");
//    }
  
    /* 直接测试 */
    PRINTF("*****本工程在fire_imxrt1052_sdram_ini_dcd文件中已初始化SDRAM*****\r\n");
	  PRINTF("*****在系统运行启动文件前SDRAM就已正常初始化*****\r\n");
	  PRINTF("*****这种初始化方式特别适合于系统启动后就需要SDRAM的应用*****\r\n");

    PRINTF("下面对SDRAM进行读写测试\r\n");

    if(SEMC_SDRAMReadWriteTest() && SDRAM_FullChipTest())
    {
      RGB_LED_COLOR_GREEN;
      CORE_BOARD_LED_ON;
      PRINTF("\r\n SDRAM测试成功！\r\n");
    }
    else
    {
      RGB_LED_COLOR_RED;
      CORE_BOARD_LED_OFF;
      PRINTF("\r\n SDRAM测试失败！\r\n");
    }
    
    PRINTF("\r\n SDRAM读写测试结束\r\n");   
    
    while(1)
    {
      
    }    
}
/****************************END OF FILE**********************/
