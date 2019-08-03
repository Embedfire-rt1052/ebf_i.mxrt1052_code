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

#include "sai.h"
#include "fsl_sai.h"
#include "fsl_edma.h"
#include "fsl_clock.h"
#include "fsl_wm8960.h"
#include "fsl_codec_common.h"
#include "fsl_lpi2c.h"
#include "fsl_wm8960.h"

/*sai dma ���ա����;��*/
extern sai_edma_handle_t txHandle;
extern sai_edma_handle_t rxHandle;

/*���ֻ����������ֽڶ���*/
__attribute__((section("NonCacheable.init")))
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t audioBuff[BUFFER_SIZE], 4);
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t audioBuff2[BUFFER_SIZE], 4);
/*���建������־λ*/
volatile bool buffer1_full = false;
volatile bool buffer2_full = false;

/*���͡�����״̬*/
volatile bool istxFinished = false;
volatile bool isrxFinished = false;

/*ִ��˫�����ȡ����*/
void double_buffer(void);

/*�ļ�ϵͳ�����ṹ��*/
FATFS g_fileSystem; /* File system object */
FIL g_fileObject;
volatile bool file_read_finished = false;

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
  int error = -1;
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

  /*��ʼ��SAI*/
  SAI1_IIC_init();
  SAI1_Init();
  SAI1_DMAConfig();

  /*�����ļ�ϵͳ*/
  f_mount_test(&g_fileSystem);

  while (1)
  {
    /*���ļ�*/
    error = f_open(&g_fileObject, _T("/record/ctl.wav"), (FA_READ));
    if (error == 0)
    {
      PRINTF("���ļ��ɹ� \r\n");
      file_read_finished = false;
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

    /*�������ֲ���*/
    double_buffer();
    PRINTF("music play finished \r\n");
    while(1);
  }
}

/*ִ��˫�����ȡ*/
void double_buffer(void)
{
  int error = -1;
  int bytesRead = 0;

  sai_transfer_t xfer_buffer1 = {0};
  sai_transfer_t xfer_buffer2 = {0};

  error = f_read(&g_fileObject, audioBuff, BUFFER_SIZE, (UINT *)&bytesRead);
  if (error)
  {
    PRINTF("read error %d\r\n", error);
    while (1)
      ;
  }
  else
  {
    xfer_buffer1.data = audioBuff;
    xfer_buffer1.dataSize = bytesRead;
    buffer1_full = true;
  }

  while (1)
  {

    if (buffer1_full)
    {
      istxFinished = false;

      /*ִ�з���buffer1*/
      SAI_TransferSendEDMA(DEMO_SAI, &txHandle, &xfer_buffer1);
      while (istxFinished == false)
      {
        /*buffer2Ϊ�գ������ݵ�buffer2*/
        if (!buffer2_full)
        {
          error = f_read(&g_fileObject, audioBuff2, BUFFER_SIZE, (UINT *)&bytesRead);
          if (error)
          {
            PRINTF("read error %d\r\n", error);
            while (1)
              ;
          }
          else
          {
            xfer_buffer2.data = audioBuff2;
            xfer_buffer2.dataSize = bytesRead;
            if (bytesRead != BUFFER_SIZE)
            {
              file_read_finished = true;
            }

            /*����buffer2����־*/
            buffer2_full = true;
          }
        }
      }
      buffer1_full = false;
    }


    /*buffer2��������buffer2�е�����*/
    if (buffer2_full)
    {
      istxFinished = false;

      /*ִ��buffer2����*/
      SAI_TransferSendEDMA(DEMO_SAI, &txHandle, &xfer_buffer2);
      
      /*�ȴ��������*/
      while (istxFinished == false)
      {
        /*buffer1Ϊ�գ������ݵ�buffer1*/
        if (!buffer1_full)
        {
          error = f_read(&g_fileObject, audioBuff, BUFFER_SIZE, (UINT *)&bytesRead);
          if (error)
          {
            PRINTF("read error %d\r\n", error);
            while (1)
              ;
          }
          else
          {
            xfer_buffer1.data = audioBuff;
            xfer_buffer1.dataSize = bytesRead;
            if (bytesRead != BUFFER_SIZE)
            {
              file_read_finished = true;
            }
            buffer1_full = true;
          }
        }
      }
      buffer2_full = false;
    }

    /*�ļ���д�������������*/
    if(file_read_finished && (!buffer1_full) &&(!buffer2_full))
    {
      if (buffer1_full)
      {
        istxFinished = false;
        SAI_TransferSendEDMA(DEMO_SAI, &txHandle, &xfer_buffer1);
        while (istxFinished == false)
          ;
      }
      if (buffer2_full)
      {
        istxFinished = false;
        SAI_TransferSendEDMA(DEMO_SAI, &txHandle, &xfer_buffer2);
        while (istxFinished == false)
          ;
      }

      error = f_close(&g_fileObject);
      if (error)
      {
        PRINTF("close file filed  %d\r\n");
        while (1)
          ;
      }
      /*�ļ���ȡ�������ҳɹ��ر��ļ�*/
      PRINTF("file read finished  %d\r\n");
      break;
    }
  }
}

///****************************END OF FILE**********************/
