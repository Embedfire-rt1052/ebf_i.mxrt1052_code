/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   FlexSPI—读写外部SPI NorFlash
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
#include "./norflash/bsp_norflash.h"
#include "./led/bsp_led.h"  
#include "./bsp/sd/bsp_sd.h"  
#include "./key/bsp_key.h"
#include "res_mgr.h"
/***************************SD卡头文件******************************/
#include "ff.h"
#include "diskio.h"
#include "fsl_sd_disk.h"
#include "./bsp/sd_fatfs_test/bsp_sd_fatfs_test.h"
/*******************************************************************
 * Prototypes
 *******************************************************************/

extern FATFS sd_fs;													/* Work area (file system object) for logical drives */

//要复制的文件路径，到aux_data.c修改
extern char src_dir[];

extern int NorFlash_AHBCommand_Test(void);
extern int NorFlash_IPCommand_Test(void);
extern int SPI_FLASH_BulkErase(void);
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
  FRESULT error = FR_NOT_READY;
  
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
  /* 初始化KEY引脚 */
  Key_GPIO_Config();
  /* 初始化FlexSPI外设 */  
  FlexSPI_NorFlash_Init();
  
  error = f_mount_test(&sd_fs);
  
  //如果文件系统挂载失败就退出
  if(error != FR_OK)
  {
    PRINTF("f_mount ERROR!请给开发板插入SD卡然后重新复位开发板!\n\r");
    RGB_RED_LED_ON;
    while(1);
  }    
  
  PRINTF("\r\n 按一次WAUP键开始烧写字库并复制文件到FLASH。 \r\n");
  PRINTF("\r\n 注意该操作会把FLASH的原内容会被删除！！ \r\n");     
  while(Key_Scan(CORE_BOARD_WAUP_KEY_GPIO, CORE_BOARD_WAUP_KEY_GPIO_PIN));   
  
  PRINTF("\r\n 正在进行整片擦除，时间很长，请耐心等候...\r\n");
  
  SPI_FLASH_BulkErase();
  
  Make_Catalog(src_dir,0);
  /* 烧录 目录信息至FLASH*/
  Burn_Catalog();   
  /* 根据 目录 烧录内容至FLASH*/
  Burn_Content();
  /* 校验烧录的内容 */
  Check_Resource();    
  while(1)
  {
  }     

}
/****************************END OF FILE**********************/
