/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   FlexSPI����д�ⲿSPI NorFlash
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

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "./norflash/bsp_norflash.h"
#include "./led/bsp_led.h"  
#include "./bsp/sd/bsp_sd.h"  
#include "./key/bsp_key.h"
#include "res_mgr.h"
/***************************SD��ͷ�ļ�******************************/
#include "ff.h"
#include "diskio.h"
#include "fsl_sd_disk.h"
#include "./bsp/sd_fatfs_test/bsp_sd_fatfs_test.h"
/*******************************************************************
 * Prototypes
 *******************************************************************/

extern FATFS sd_fs;													/* Work area (file system object) for logical drives */

//Ҫ���Ƶ��ļ�·������aux_data.c�޸�
extern char src_dir[];

extern int NorFlash_AHBCommand_Test(void);
extern int NorFlash_IPCommand_Test(void);
extern int SPI_FLASH_BulkErase(void);
/*******************************************************************
 * Code
 *******************************************************************/

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
  FRESULT error = FR_NOT_READY;
  
  /* ��ʼ���ڴ汣����Ԫ */
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

  LED_GPIO_Config();
  /* ��ʼ��KEY���� */
  Key_GPIO_Config();
  /* ��ʼ��FlexSPI���� */  
  FlexSPI_NorFlash_Init();
  
  error = f_mount_test(&sd_fs);
  
  //����ļ�ϵͳ����ʧ�ܾ��˳�
  if(error != FR_OK)
  {
    PRINTF("f_mount ERROR!������������SD��Ȼ�����¸�λ������!\n\r");
    RGB_RED_LED_ON;
    while(1);
  }    
  
  PRINTF("\r\n ��һ��WAUP����ʼ��д�ֿⲢ�����ļ���FLASH�� \r\n");
  PRINTF("\r\n ע��ò������FLASH��ԭ���ݻᱻɾ������ \r\n");     
  while(Key_Scan(CORE_BOARD_WAUP_KEY_GPIO, CORE_BOARD_WAUP_KEY_GPIO_PIN));   
  
  PRINTF("\r\n ���ڽ�����Ƭ������ʱ��ܳ��������ĵȺ�...\r\n");
  
  SPI_FLASH_BulkErase();
  
  Make_Catalog(src_dir,0);
  /* ��¼ Ŀ¼��Ϣ��FLASH*/
  Burn_Catalog();   
  /* ���� Ŀ¼ ��¼������FLASH*/
  Burn_Content();
  /* У����¼������ */
  Check_Resource();    
  while(1)
  {
  }     

}
/****************************END OF FILE**********************/
