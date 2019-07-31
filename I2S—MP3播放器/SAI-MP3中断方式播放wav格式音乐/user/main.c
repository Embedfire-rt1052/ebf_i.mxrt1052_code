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


#include "fsl_lpi2c.h"
#include "fsl_wm8960.h"
#include "music.h"

#include "sai.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/







extern volatile bool isFinished;
extern sai_handle_t txHandle;


/*******************************************************************************
 * Code
 ******************************************************************************/



void BOARD_EnableSaiMclkOutput(bool enable)
{
  if (enable)
  {
    IOMUXC_GPR->GPR1 |= IOMUXC_GPR_GPR1_SAI1_MCLK_DIR_MASK;
  }
  else
  {
    IOMUXC_GPR->GPR1 &= (~IOMUXC_GPR_GPR1_SAI1_MCLK_DIR_MASK);
  }
}



void delay(uint32_t count)
{
  volatile uint32_t i = 0;
  for (i = 0; i < count; ++i)
  {
    __asm("NOP"); /* ����nop��ָ�� */
  }
}

AT_NONCACHEABLE_SECTION(FATFS g_fileSystem); /* File system object */
AT_NONCACHEABLE_SECTION(FIL g_fileObject);   /* File object */




/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
  
  int error = -1; //���󷵻�ֵ
  int bytesRead;  //�ļ���ȡ�õ���������

  /************��ֲ��������*******************/


  sai_transfer_t xfer;
  uint32_t temp = 0;
  uint32_t delayCycle = 500000;

  /* ��ʼ���ڴ汣����Ԫ */
  BOARD_ConfigMPU();
  /* ��ʼ������������ */
  BOARD_InitPins();
  /* ��ʼ��������ʱ�� */
  BOARD_BootClockRUN();
  /* ��ʼ�����Դ���*/
  BOARD_InitDebugConsole();

  BOARD_ConfigMPU();
  BOARD_InitPins();
  BOARD_BootClockRUN();
  BOARD_InitDebugConsole();
  /*******��ʼ����Ƶ���ʱ��****************/
  sai_init();


  /*�����ļ�ϵͳ*/
  f_mount_test(&g_fileSystem);

  /*���ļ�*/
  error = f_open(&g_fileObject, _T("/record/48.wav"), (FA_READ));
  if (error == 0)
  {
    PRINTF("���ļ��ɹ� \r\n");
  }
  else
  {
    PRINTF("���ļ�ʧ�� \r\n");
  }

  /*�ƶ��ļ��Ķ�дָ��*/
  if (f_lseek(&g_fileObject, 44U))
  {
    PRINTF("Set file pointer position failed. \r\n");
  }

  while (delayCycle)
  {
    __ASM("nop");
    delayCycle--;
  }

  error = f_read(&g_fileObject, music, MUSIC_LEN, &bytesRead);
  PRINTF("the data is %d\r\n", error);
  if ((error) || (MUSIC_LEN != bytesRead))
  {
    PRINTF("the data is %d\r\n", error);
    PRINTF("��ȡ�ļ�ʧ�� \r\n");
    while (1)
    {
      ;
    }
  }
  else
  {
    PRINTF("��ȡ�ļ��ɹ�. \r\n");
  }
  /*ѭ����ȡ�ļ�����д��*/
  while (1)
  {


    /*  xfer structure */
    temp = (uint32_t)music;
    xfer.data = (uint8_t *)temp;
    xfer.dataSize = MUSIC_LEN;

//    SAI_WriteBlocking(DEMO_SAI, 0U, kSAI_WordWidth16bits, (uint8_t *)temp, bytesRead);
    SAI_TransferSendNonBlocking(DEMO_SAI, &txHandle, &xfer);
    
    error = f_read(&g_fileObject, music, MUSIC_LEN, &bytesRead);
    
    PRINTF("the data is %d\r\n", error);
    if ((error) || (MUSIC_LEN != bytesRead))
    {
      PRINTF("the data is %d\r\n", error);
      PRINTF("��ȡ�ļ�ʧ�� \r\n");
      while (1)
      {
        ;
      }
    }
    else
    {
      PRINTF("��ȡ�ļ��ɹ�. \r\n");
    }

    /* Wait until finished */
    while (isFinished != true)
    {
    }
    isFinished = false;
    
  }

  //    /*  xfer structure */
  //    temp = (uint32_t)music;
  //    xfer.data = (uint8_t *)temp;
  //    xfer.dataSize = MUSIC_LEN;
  //    SAI_TransferSendNonBlocking(DEMO_SAI, &txHandle, &xfer);
  //
  //
  //
  //
  //    /* Wait until finished */
  //    while (isFinished != true)
  //    {
  //    }

  //   PRINTF("\n\r SAI example finished!\n\r ");
  while (1)
  {
    
  }
}

