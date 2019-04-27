/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   ��дSD����������
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

#include "./bsp/sd/bsp_sd.h"  



#include "ff.h"
#include "diskio.h"
#include "fsl_sd_disk.h"
#include "./bsp/sd_fatfs_test/bsp_sd_fatfs_test.h"
    
    
#define BUFFER_SIZE (100U)

/*�ļ�ϵͳ�����ṹ��*/
FATFS g_fileSystem; /* File system object */
 

/* @brief decription about the read/write buffer
* The size of the read/write buffer should be a multiple of 512, since SDHC/SDXC card uses 512-byte fixed
* block length and this driver example is enabled with a SDHC/SDXC card.If you are using a SDSC card, you
* can define the block length by yourself if the card supports partial access.
* The address of the read/write buffer should align to the specific DMA data buffer address align value if
* DMA transfer is used, otherwise the buffer address is not important.
* At the same time buffer address/size should be aligned to the cache line size if cache is supported.
*/
SDK_ALIGN(uint8_t g_bufferWrite[SDK_SIZEALIGN(BUFFER_SIZE, SDMMC_DATA_BUFFER_ALIGN_CACHE)],
          MAX(SDMMC_DATA_BUFFER_ALIGN_CACHE, SDMMCHOST_DMA_BUFFER_ADDR_ALIGN));
SDK_ALIGN(uint8_t g_bufferRead[SDK_SIZEALIGN(BUFFER_SIZE, SDMMC_DATA_BUFFER_ALIGN_CACHE)],
          MAX(SDMMC_DATA_BUFFER_ALIGN_CACHE, SDMMCHOST_DMA_BUFFER_ADDR_ALIGN));


/**
 * @brief ��ʱһ��ʱ��
 */
void delay(uint32_t count);

/**
 * @note �������ڲ�ͬ���Ż�ģʽ����ʱʱ�䲻ͬ��
 *       ��flexspi_nor_debug��flexspi_nor_release�汾�ĳ����У�
 *       flexspi_nor_release�汾����ʱҪ�̵ö�  
 */ 
void delay(uint32_t count)
{   
    volatile uint32_t i = 0;
    for (i = 0; i < count; ++i)
    {
        __asm("NOP"); /* ����nop��ָ�� */
    }
}



/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
  
  volatile FIL file_object;   //�����ļ���������
  volatile DIR dir_object;    //Ŀ¼����ṹ��
  volatile FILINFO file_info; //�ļ���Ϣ�����ṹ��

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
  
  /*�����ļ�ϵͳ*/
  f_mount_test(&g_fileSystem);

  /*��SD����Ŀ¼����һ��Ŀ¼*/
  f_mkdir_test("/dir_1");
  
  /*������/dir_1/f_1.txt��*/
  f_touch_test("/dir_1/he.txt"); 
  
  /*���ļ�*/
  f_open_test("/dir_1/he.txt",&file_object);
  
   /*�ر��ļ�*/
  f_close_test(&file_object);
  
  /*����Ŀ¼*/
  f_mkdir_test("/dir_1/dir_2");
  
  /*��ȡ·���µ��ļ�Ҳ�ļ���*/
  f_readdir_test("/dir_1",&dir_object,&file_info);
 
 /*��ʼ�����ݻ�������Ϊ�ļ��Ķ�д��׼��*/
  memset(g_bufferWrite, 'a', sizeof(g_bufferWrite));
  g_bufferWrite[BUFFER_SIZE - 2U] = '\r';
  g_bufferWrite[BUFFER_SIZE - 1U] = '\n';
  
  PRINTF("\r\n��ʼ�ļ���д����......  \r\n");
  
  f_write_read_test("/dir15/he.txt", g_bufferWrite, g_bufferRead);  
  
  while (true)
  {
    
  }
}

/****************************END OF FILE**********************/

