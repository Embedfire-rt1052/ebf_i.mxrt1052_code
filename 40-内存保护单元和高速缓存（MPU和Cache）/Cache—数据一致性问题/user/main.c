/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   Cache-����һ����ʵ��
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
#include "./led/bsp_led.h"
#include "bsp_cache.h"

/*���ݻ���������*/
#define MEM_DMATRANSFER_LEN 10U * FSL_FEATURE_L1DCACHE_LINESIZE_BYTE
/* DMA ���䳬ʱʱ�� */
#define DMA_TRANSFER_TIMEOUT 0xFFFFU

volatile bool g_Transfer_Done;                                 //���崫����ɱ�־
AT_NONCACHEABLE_SECTION(uint8_t g_data[MEM_DMATRANSFER_LEN]);  //�������ݻ�������
uint32_t g_count = 0;                                          //����DAM����ȴ�����


/*******************************************************************
 * Prototypes
 *******************************************************************/
/**
 * @brief ��ʱһ��ʱ��
 */
void delay(uint32_t count);

/*******************************************************************
 * Code
 *******************************************************************/
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

void InvalidateDCache_test(void); //������Ч�����Ժ���
void CleanDCache_test(void);      //����������Ժ���

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
  /* ��ʼ���ڴ汣����Ԫ*/
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

  /*����Cache ����ʼ��DMA */
  Cache_DMA_Config();

//  /*�����������*/
//  CleanDCache_test();

  /*������Ч������*/
  InvalidateDCache_test();

  while (1)
    ;
}



/*�������*/
void CleanDCache_test(void)
{
  uint32_t count;              //����ѭ������
  volatile uint32_t readDummy; //�����ڶ�����ʵ������
  uint32_t startAddr;                                            //���汾ʵ��ʹ�õ��ڴ���ʼ��ַ

  startAddr = APP_MemoryInit();
  /*�ر�Cache*/
  APP_CacheConfig(false);

  /*ʹ�ó�ʼ��������g_data��startAddr�ڴ�ռ�*/
  for (count = 0; count < MEM_DMATRANSFER_LEN; count++)
  {
    g_data[count] = 0xaa;
    *(uint8_t *)(startAddr + count) = 0xbb;
  }

  /*����Cache ����ʼ��DMA */
  APP_CacheConfig(true);


  /*��һ�δ� ���ڴ���(startAddr)�����ݣ�ִ��֮��Cache�лᱣ�����ݱ���*/
  for (count = 0; count < MEM_DMATRANSFER_LEN; count++)
  {
    /* Access the memory first. */
    readDummy = *(uint8_t *)(startAddr + count);
    (void)readDummy;
  }

  /* ͨ��CPU��������,  */
  for (count = 0; count < MEM_DMATRANSFER_LEN; count++)
  {
    *(uint8_t *)(startAddr + count) = 0xff;
  }

  /* Get the real data in the memory . */
  APP_DMAMem2memTransfer((void *)startAddr, sizeof(g_data[0]), &g_data[0], sizeof(g_data[0]), sizeof(g_data));

  /* Wait for EDMA transfer finished. */
  while ((g_Transfer_Done != true) && (g_count < DMA_TRANSFER_TIMEOUT))
  {
    g_count++;
  }

  if (g_count != DMA_TRANSFER_TIMEOUT)
  {

    if (memcmp((void *)&g_data[0], (void *)startAddr, MEM_DMATRANSFER_LEN) != 0)
    {

      /*���ʵ���ڴ��е����ݣ�DMA��ȡ���Ĳ����ڴ��е�ʵ������*/
      PRINTF("\r\n ִ�л�������֮ǰ�ڴ��е��������£�\r\n");
      for (count = 0; count < MEM_DMATRANSFER_LEN; count++)
      {
        PRINTF("0X%02x, ", g_data[count]);
      }

      /*ִ���ڴ�����*/
      L1CACHE_CleanDCacheByRange(startAddr, MEM_DMATRANSFER_LEN);

      /* Transfer from the sdram to data[]. */
      APP_DMAMem2memTransfer((void *)startAddr, sizeof(g_data[0]), &g_data[0], sizeof(g_data[0]),
                             sizeof(g_data));

      /* Wait for EDMA transfer finished. */
      while ((g_Transfer_Done != true) && (g_count < DMA_TRANSFER_TIMEOUT))
      {
        g_count++;
      }
      /*�������*/
      if (g_count != DMA_TRANSFER_TIMEOUT)
      {
        if (memcmp((void *)&g_data[0], (void *)startAddr, MEM_DMATRANSFER_LEN) == 0)
        {
          /*���ʵ���ڴ��е����ݣ�DMA��ȡ���Ĳ����ڴ��е�ʵ������*/
          PRINTF("\r\n ִ�л�������֮���ڴ��е��������£�\r\n");
          for (count = 0; count < MEM_DMATRANSFER_LEN; count++)
          {
            PRINTF("0X%02x, ", g_data[count]);
          }
          PRINTF("\r\n ʵ�����\r\n");
        }
      }
    }
  }
}



/*������Ч�����Ժ���,��*/
void InvalidateDCache_test(void)
{
  uint32_t count;              //����ѭ������
  volatile uint32_t readDummy; //�����ڶ�����ʵ������
  uint32_t startAddr;                                            //���汾ʵ��ʹ�õ��ڴ���ʼ��ַ

  startAddr = APP_MemoryInit();
  /*�ر�Cache*/
  APP_CacheConfig(false);

  /*ʹ�ó�ʼ��������g_data��startAddr�ڴ�ռ�*/
  for (count = 0; count < MEM_DMATRANSFER_LEN; count++)
  {
    g_data[count] = 0xaa;
    *(uint8_t *)(startAddr + count) = 0xbb;
  }
  
  /*����Cache ����ʼ��DMA */
  APP_CacheConfig(true);

  /*��һ�δ� ���ڴ���(startAddr)�����ݣ�ִ��֮��Cache�лᱣ�����ݱ���*/
  for (count = 0; count < MEM_DMATRANSFER_LEN; count++)
  {
    /* Access the memory first. */
    readDummy = *(uint8_t *)(startAddr + count);
    (void)readDummy;
  }

  /*ʹ��DMA�޸��ڴ������*/
  /* ʹ��DMA ����(startAddr) �е�����,��ʱCache��֪��������*/
  APP_DMAMem2memTransfer(&g_data[0], sizeof(g_data[0]), (void *)startAddr, sizeof(g_data[0]), sizeof(g_data));

  /*�ȴ� EDMA �������*/
  while ((g_Transfer_Done != true) && (g_count < DMA_TRANSFER_TIMEOUT))
  {
    g_count++;
  }

  /*�ȴ��������*/
  if (g_count != DMA_TRANSFER_TIMEOUT)
  {
    PRINTF("\r\n eDAM �洢�����洢���������\r\n");
    /*�Ƚϵ�һ��ʹ��dma д���������ԭ�����Ƿ�һ��*/
    if (memcmp((void *)&g_data[0], (void *)startAddr, MEM_DMATRANSFER_LEN) != 0)
    {
      PRINTF("\r\n ִ�л�����Ч��֮ǰʹ��CPU��ȡ�õ����������£�\r\n");
      for (count = 0; count < MEM_DMATRANSFER_LEN; count++)
      {
        PRINTF("0X%02x, ", *(uint8_t *)(startAddr + count));
      }

      /*��һ�£�ִ�л�����Ч����*/
      L1CACHE_InvalidateDCacheByRange(startAddr, MEM_DMATRANSFER_LEN);

      /* ִ�л�����Ч�����ٴ�ʹ��CPU��ȡ�ڴ��е����� */
      if (memcmp((void *)&g_data[0], (void *)startAddr, MEM_DMATRANSFER_LEN) == 0)
      {
        /*����һ��*/
        PRINTF("\r\n ִ�л�����Ч��֮��ʹ��CPU��ȡ�õ����������£�\r\n");
        for (count = 0; count < MEM_DMATRANSFER_LEN; count++)
        {
          PRINTF("0X%02x, ", *(uint8_t *)(startAddr + count));
        }
        // invalidateResult = true;
        PRINTF("\r\n ʵ�����\r\n");
      }
    }
    else
    {
      /*����ʧ�ܣ�û�г�������һ��������*/
      PRINTF("\r\n ����ʧ�ܣ�û�г�������һ�������⣡\r\n");
    }
  }
  else
  {
    /* DMA������󣬲���ʧ�� */
    PRINTF("\r\n DMA������󣬲���ʧ�ܣ�\r\n");
  }
}



/****************************END OF FILE**********************/
