/**
******************************************************************
* @file    main.c
* @author  fire
* @version V1.0
* @date    2018-xx-xx
* @brief   SD������
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
#include "fsl_sd.h"
#include "./bsp/sd_fatfs_test/bsp_sd_fatfs_test.h"
#include "fsl_sdmmc_host.h"


#include "ff.h"
#include "diskio.h"
#include "fsl_sd_disk.h"


extern  FATFS g_fileSystem; /* File system object */
extern  FIL g_fileObject;   /* File object */


void f_mount_test(void)
{
  FRESULT error;
  const TCHAR driverNumberBuffer[3U] = {SDDISK + '0', ':', '/'};
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
    
}



/****************************END OF FILE**********************/
