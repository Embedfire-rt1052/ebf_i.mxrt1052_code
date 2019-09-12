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

#define BUFFER_SIZE (100U)

/*Card�ṹ������*/
extern sd_card_t g_sd;

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



 FATFS g_fileSystem; /* File system object */
 FIL g_fileObject;   /* File object */


/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
  FRESULT error;
  DIR directory; /* Directory object */
  FILINFO fileInformation;
  UINT bytesWritten;
  UINT bytesRead;
  const TCHAR driverNumberBuffer[3U] = {SDDISK + '0', ':', '/'};
  volatile bool failedFlag = false;
  char ch = '0';
  BYTE work[FF_MAX_SS];
  
  
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
  //FR_NO_FILESYSTEM
  error = f_mount(&g_fileSystem, driverNumberBuffer, 0U);
  if (error)
  {
    /*���������ж�*/
    if(error == FR_NO_FILESYSTEM)//��û���ļ�ϵͳ
    {
      PRINTF("SD����û���ļ�ϵͳ�������ļ�ϵͳ�����ʽ������SD����\r\n ȷ���Ƿ������\r\n");
      PRINTF("���롮y��ȷ����ʽ�������롮n��ȡ��\r\n");
      while(true)
      {
        ch = GETCHAR();
        PUTCHAR(ch);
        if(ch =='y'|| ch == 'Y')
        {
          /*ΪSD�������ļ�ϵͳ*/
#if FF_USE_MKFS
          PRINTF("\r\n�����ļ�ϵͳ...... SD������Խ�󣬸ù��̳���ʱ��Խ����\r\n");
          if (f_mkfs(driverNumberBuffer, FM_ANY, 0U, work, sizeof work))
          {
            PRINTF("֪���ļ�ϵͳʧ��.\r\n");
            return -1;
          }
          else
          {
            PRINTF("�����ļ�ϵͳ�ɹ�.\r\n");
          }
#endif /* FF_USE_MKFS */
        }
        else if(ch =='n'|| ch == 'N')
        {
          /*�˳�����*/
          return -1;
        }
        
        PRINTF("���롮y��ȷ����ʽ�������롮n��ȡ��\r\n");
      }  
    }
    else//����������ʱ������ֱ���˳�����
    {
      PRINTF("�����ļ�ϵͳʧ��\r\n");
      return -1;
    }

  }
  else 
  {
    PRINTF("Mount volume success.\r\n");
  }
  
  
  /*�ж��Ƿ�����ʹ�����·��*/
#if (FF_FS_RPATH >= 2U)
    error = f_chdrive((char const *)&driverNumberBuffer[0U]);
    if (error)
    {
        PRINTF("Change drive failed.\r\n");
        return -1;
    }
#endif
    


  /*---------------------------���� "/dir_1"Ŀ¼-------------------------------*/
  PRINTF("\r\n ����Ŀ¼ ��/dir_1��......\r\n");
  error = f_mkdir(_T("/dir_1"));
  if (error)
  {
    if (error == FR_EXIST)
    {
      PRINTF("Ŀ¼�Ѿ�����\r\n");
    }
    else
    {
      PRINTF("����Ŀ¼ʧ��.\r\n");
      return -1;
    }
  }
  else
  {
    PRINTF("����Ŀ¼�ɹ�\r\n");
  }
  
  /*-------------------------��"/dir_1"Ŀ¼�ﴴ��һ���ļ�------------*/
  PRINTF("\r\n�ڡ�/dir_1��Ŀ¼�´���һ����f_1.txt���ļ�......\r\n");
  error = f_open(&g_fileObject, _T("/dir_1/f_1.txt"), (FA_WRITE | FA_READ | FA_CREATE_ALWAYS));
  if (error)
  {
    if (error == FR_EXIST)
    {
      PRINTF("�ļ��Ѿ�����.\r\n");
    }
    else
    {
      PRINTF("�����ļ�ʧ��\r\n");
      return -1;
    }
  }
  else
  {
    PRINTF("�����ļ��ɹ�...... \r\n");
  }
  
  
//      if (f_close(&g_fileObject))
//    {
//        PRINTF("\r\nClose file failed.\r\n");
//        return -1;
//    }
//    else
//    {
//      PRINTF("\r\nClose file success.\r\n");
//    }
  
  /*-------------------��"/dir_1"Ŀ¼�ﴴ��һ��Ŀ¼------------------------*/
  PRINTF("\r\n�ڡ�/dir_1��Ŀ¼�´���һ����dir_2���ļ���......\r\n");
  error = f_mkdir(_T("/dir_1/dir_2"));
  if (error)
  {
    if (error == FR_EXIST)
    {
      PRINTF("�ļ����Ѵ���.\r\n");
    }
    else
    {
      PRINTF("������dir_2���ļ���ʧ��\r\n");
      return -1;
    }
  }
  else
  {
    PRINTF("������dir_2���ļ��гɹ�......\r\n");
  }
  
  /*---------------------���"/dir_1"Ŀ¼�µ��ļ����ļ�-------------------*/
  PRINTF("\r\n�г���/dir_1��Ŀ¼�µ�����......\r\n");
  if (f_opendir(&directory, "/dir_1"))
  {
    PRINTF("��·��ʧ��\r\n");
    return -1;
  }
  
  for (;;)
  {
    error = f_readdir(&directory, &fileInformation);
    
    /* To the end. */
    if ((error != FR_OK) || (fileInformation.fname[0U] == 0U))
    {
      break;
    }
    if (fileInformation.fname[0] == '.')
    {
      continue;
    }
    if (fileInformation.fattrib & AM_DIR)
    {
      PRINTF("�ļ� : %s.\r\n", fileInformation.fname);
    }
    else
    {
      PRINTF("�ļ��� : %s.\r\n", fileInformation.fname);
    }
  }
  
  /*��ʼ�����ݻ�������Ϊ�ļ��Ķ�д��׼��*/
  memset(g_bufferWrite, 'a', sizeof(g_bufferWrite));
  g_bufferWrite[BUFFER_SIZE - 2U] = '\r';
  g_bufferWrite[BUFFER_SIZE - 1U] = '\n';
  
  PRINTF("\r\n  ��ʼ�ļ���д����......  \r\n");
  
  
  
   while (true)
    {
        if (failedFlag || (ch == 'q'))
        {
            break;
        }
        /* �� "/dir_1/f_1.dat" �ļ���д��g_bufferWrite������������ */
        PRINTF("\r\nд�����ݵ���/dir_1/f_1.txt���ļ�\r\n");
        error = f_write(&g_fileObject, g_bufferWrite, sizeof(g_bufferWrite), &bytesWritten);
        if ((error) || (bytesWritten != sizeof(g_bufferWrite)))
        {
            PRINTF("д���ļ�ʧ��. \r\n");
            failedFlag = true;
            continue;
        }
        else
        {
          PRINTF("д���ļ��ɹ� \r\n");
        }

        /* �ƶ��ļ���дָ�뵽�ļ���ʼ�� */
        if (f_lseek(&g_fileObject, 0U))
        {
            PRINTF("�����ļ���дָ��ʧ�� \r\n");
            failedFlag = true;
            continue;
        }
        else
        {
          PRINTF("�����ļ���дָ��ɹ� \r\n");
        }
        
        /*��ȡ"/dir_1/f_1.dat"�ļ������ݵ� g_bufferRead ������*/
        PRINTF("��ȡ��/dir_1/f_1.txt���ļ�������\r\n");
        memset(g_bufferRead, 0U, sizeof(g_bufferRead));
        error = f_read(&g_fileObject, g_bufferRead, sizeof(g_bufferRead), &bytesRead);
        if ((error) || (bytesRead != sizeof(g_bufferRead)))
        {
            PRINTF("��ȡ�ļ�ʧ�� \r\n");
            failedFlag = true;
            continue;
        }
        else
        {
            PRINTF("��ȡ�ļ��ɹ�. \r\n");
        }
        
        
        /*�Ƚ϶�д�����Ƿ�һ��*/
        PRINTF("�Ƚ϶�д����......\r\n");
        if (memcmp(g_bufferWrite, g_bufferRead, sizeof(g_bufferWrite)))
        {
            PRINTF("�ļ���д���ݲ�һ��\r\n");
            failedFlag = true;
            continue;
        }
        else
        {
          PRINTF("�ļ���д����һ��\r\n");
        }
        
        
        PRINTF("\r\n���� 'q' �˳���д����.\r\n���������ַ����ٴν��ж�д����\r\n");
        ch = GETCHAR();
        PUTCHAR(ch);
    }
    PRINTF("\r\n�ر��ļ�������������.\r\n");

    if (f_close(&g_fileObject))
    {
        PRINTF("\r\nClose file failed.\r\n");
        return -1;
    }
    else
    {
      PRINTF("\r\nClose file success.\r\n");
    }
  
    while(1);		
}

//void 



/****************************END OF FILE**********************/


/************************��һ����*************************/
