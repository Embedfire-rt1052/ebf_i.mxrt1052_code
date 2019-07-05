/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   ELCDIF—液晶显示（显示英文）
  ******************************************************************
  * @attention
  *
  * 实验平台:野火  i.MXRT1052开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************
  */
#include <stdio.h>

#include "fsl_debug_console.h"
#include "fsl_elcdif.h"


#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"

#include "./led/bsp_led.h" 
#include "./lcd/bsp_lcd.h" 
#include "./systick/bsp_systick.h"


#include "./bsp/sd/bsp_sd.h"  
#include "ff.h"
#include "diskio.h"
#include "fsl_sd_disk.h"
#include "bsp_sd_fatfs_test.h"



#define BUFFER_SIZE (100U)

/*文件系统描述结构体*/
FATFS g_fileSystem; /* File system object */


/* 用于存储当前字体颜色和字体背景颜色的变量*/
extern volatile  pixel_t CurrentTextColor;//默认为白色
extern volatile  pixel_t CurrentBackColor;//默认为黑色


/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
  volatile FIL file_object;   //定义文件描述符，
  volatile DIR dir_object;    //目录对象结构体
  volatile FILINFO file_info; //文件信息描述结构体
  
  
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
  
  PRINTF("*****液晶显示英文*****\r\n");
  
//  /*挂载文件系统*/
//  f_mount_test(&g_fileSystem);
//  /*获取路径下的文件和文件夹*/
//  PRINTF("/Font 目录下的字库文件");
//  f_readdir_test("/Font",&dir_object,&file_info);
  
  
  /* 初始化LCD */
  LCD_Init(LCD_INTERRUPT_ENABLE);
  
  RGB_LED_COLOR_BLUE;
  
  CurrentTextColor = CL_RED;  
  /*显示中英文测试*/
  LCD_DisplayStringLine_EN_CH(1,(uint8_t* )"AABBCCDD大大小小");

  while(1)
  {
    
  }
}

/****************************END OF FILE**********************/
