/**
  ******************************************************************
  * @file    sdram_test.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   ��SDRAM���ж�д���Ե�����
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
#include "./delay/core_delay.h"   

#define SDRAM_START_ADDRESS       (0x80000000U)
#define SDRAM_MEM_LENGTH          (32*1024*1024)

#define SEMC_EXAMPLE_DATALEN      (0x1000U)
#define SEMC_EXAMPLE_WRITETIMES   (1000U)


uint32_t sdram_writeBuffer[SEMC_EXAMPLE_DATALEN];
uint32_t sdram_readBuffer[SEMC_EXAMPLE_DATALEN];

/**
  * @brief  ʹ��32λָ����з���SDRAM����
  * @param  ��
  * @retval ���Խ����trueΪ������falseΪʧ��
  */
bool SEMC_SDRAMReadWrite32Bit(void)
{
    uint32_t index;
    uint32_t datalen = SEMC_EXAMPLE_DATALEN;
    /* ָ�� SDRAM ���Ե���ʼ��ַ
       �Ը�ָ��ָ��ĵ�ַ��ֵ�ᴥ����SDRAM��д�� 
       �Ը�ָ��ָ��ĵ�ַ��ȡ�ᴥ����SDRAM�Ķ�ȡ */
    uint32_t *sdram = (uint32_t *)SDRAM_START_ADDRESS; 
    bool result = true;

    PRINTF("\r\n ʹ��32λ�ķ�ʽ��SDRAMд��, ��ʼ��ַ��0x%x, д�볤�ȣ�%d \r\n", 
           sdram, datalen);
    
    /* ��SDRAMд������ */
    for (index = 0; index < datalen; index++)
    {
        sdram_writeBuffer[index] = index;
        sdram[index] = sdram_writeBuffer[index];
    }

    PRINTF("\r\n ʹ��32λ�ķ�ʽ���ȡSDRAM, ��ʼ��ַ��0x%x, д�볤�ȣ�%d \r\n", 
           sdram, datalen);
    
    /* ��SDRAM�ж�ȡ���� */
    for (index = 0; index < datalen; index++)
    {
        sdram_readBuffer[index] = sdram[index];
    }

    PRINTF("\r\n ��ʼ�Ƚ϶�д������...\r\n");
    /* �Ƚ����������������� */
    while (datalen--)
    {
        if (sdram_writeBuffer[datalen] != sdram_readBuffer[datalen])
        {
            result = false;
            break;
        }
    }

    if (!result)
    {
        PRINTF("\r\n ����ʹ��32λ��дSDRAM�ıȽϽ����һ��\r\n");
    }
    else
    {
        PRINTF("\r\n �ɹ���ʹ��32λ��дSDRAM�ıȽϽ��һ��\r\n");
    }
    
    return result;
}

/**
  * @brief  ʹ��16λָ����з���SDRAM����
  * @param  ��
  * @retval ���Խ����trueΪ������falseΪʧ��
  */
static bool SEMC_SDRAMReadWrite16Bit(void)
{
    uint32_t index;
    uint32_t datalen = SEMC_EXAMPLE_DATALEN;
    /* ָ�� SDRAM ���Ե���ʼ��ַ
     �Ը�ָ��ָ��ĵ�ַ��ֵ�ᴥ����SDRAM��д�� 
     �Ը�ָ��ָ��ĵ�ַ��ȡ�ᴥ����SDRAM�Ķ�ȡ */
    uint16_t *sdram = (uint16_t *)SDRAM_START_ADDRESS; 
    bool result = true;

    PRINTF("\r\n ʹ��16λ�ķ�ʽ��SDRAMд��, ��ʼ��ַ��0x%x, д�볤�ȣ�%d \r\n", 
           sdram, datalen);
  
    memset(sdram_writeBuffer, 0, sizeof(sdram_writeBuffer));
    memset(sdram_readBuffer, 0, sizeof(sdram_readBuffer));

    /* ��SDRAMд������ */
    for (index = 0; index < datalen; index++)
    {
        sdram_writeBuffer[index] = index % 0xFFFF;
        sdram[index] = sdram_writeBuffer[index];
    }

    PRINTF("\r\n ʹ��16λ�ķ�ʽ���ȡSDRAM, ��ʼ��ַ��0x%x, д�볤�ȣ�%d \r\n", 
           sdram, datalen);

    
    /* ��SDRAM�ж�ȡ���� */
    for (index = 0; index < datalen; index++)
    {
        sdram_readBuffer[index] = sdram[index];
    }

    PRINTF("\r\n ��ʼ�Ƚ϶�д������...\r\n");
    /* �Ƚ����������������� */
    while (datalen--)
    {
        if (sdram_writeBuffer[datalen] != sdram_readBuffer[datalen])
        {
            result = false;
            break;
        }
    }

    if (!result)
    {
        PRINTF("\r\n ����ʹ��16λ��дSDRAM�ıȽϽ����һ��!\r\n");
    }
    else
    {
        PRINTF("\r\n �ɹ���ʹ��16λ��дSDRAM�ıȽϽ��һ��\r\n");
    }
    
    return result;
}

/**
  * @brief  ʹ��8λָ����з���SDRAM����
  * @param  ��
  * @retval ���Խ����trueΪ������falseΪʧ��
  */
static bool SEMC_SDRAMReadWrite8Bit(void)
{
    uint32_t index;
    uint32_t datalen = SEMC_EXAMPLE_DATALEN;
    /* ָ�� SDRAM ���Ե���ʼ��ַ
     �Ը�ָ��ָ��ĵ�ַ��ֵ�ᴥ����SDRAM��д�� 
     �Ը�ָ��ָ��ĵ�ַ��ȡ�ᴥ����SDRAM�Ķ�ȡ */
    uint8_t *sdram = (uint8_t *)SDRAM_START_ADDRESS; 
    bool result = true;

    PRINTF("\r\n ʹ��8λ�ķ�ʽ��SDRAMд��, ��ʼ��ַ��0x%x, д�볤�ȣ�%d \r\n", 
           sdram, datalen);

    memset(sdram_writeBuffer, 0, sizeof(sdram_writeBuffer));
    memset(sdram_readBuffer, 0, sizeof(sdram_readBuffer));

    /* ��SDRAMд������ */
    for (index = 0; index < datalen; index++)
    {
        sdram_writeBuffer[index] = index % 0x100;
        sdram[index] = sdram_writeBuffer[index];
    }

    PRINTF("\r\n ʹ��8λ�ķ�ʽ���ȡSDRAM, ��ʼ��ַ��0x%x, д�볤�ȣ�%d \r\n", 
           sdram, datalen);
    
    /* ��SDRAM�ж�ȡ���� */
    for (index = 0; index < datalen; index++)
    {
        sdram_readBuffer[index] = sdram[index];
    }

    PRINTF("\r\n ��ʼ�Ƚ϶�д������...\r\n");
    /* �Ƚ����������������� */
    while (datalen--)
    {
        if (sdram_writeBuffer[datalen] != sdram_readBuffer[datalen])
        {
            result = false;
            break;
        }
    }

    if (!result)
    {
        PRINTF("\r\n ����ʹ��8λ��дSDRAM�ıȽϽ����һ��!\r\n");
    }
    else
    {
        PRINTF("\r\n �ɹ���ʹ��8λ��дSDRAM�ıȽϽ��һ��\r\n");
    }
   
    return result;
}

/**
  * @brief  ��32��16��8λ�ķ�ʽ��SDRAM���ж�д����
  * @param  ��
  * @retval true���������� false:����ʧ��
  */
bool SEMC_SDRAMReadWriteTest(void)
{
  bool result;
  
	/* 32Bit���ݶ�д����*/
	result = SEMC_SDRAMReadWrite32Bit();
  if(!result) return result;
  
	/* 16Bit���ݶ�д���� */
	result = SEMC_SDRAMReadWrite16Bit();
  if(!result) return result;

	/* 8Bit���ݶ�д���� */
	result = SEMC_SDRAMReadWrite8Bit();

  return result;
}


/**
  * @brief  ��ƬSDRAM��д����
  * @param  None
  * @retval ��������true���쳣����false
  */
bool SDRAM_FullChipTest(void)
{
  /*д�����ݼ�����*/
  uint32_t counter=0;
  
  /* 8λ������ */
  uint8_t ubWritedata_8b = 0, ubReaddata_8b = 0;  
  
  /* 16λ������ */
  uint16_t uhWritedata_16b = 0, uhReaddata_16b = 0; 
  
  PRINTF("\r\n ���ڼ����ƬSDRAM����8λ��16λ�ķ�ʽ��д��ƬSDRAM...\r\n");


  /*��8λ��ʽ��д���ݣ���У��*/
  
  /* ��SDRAM����ȫ������Ϊ0 ��SDRAM_MEM_LENGTH����8λΪ��λ�� */
  for (counter = 0x00; counter < SDRAM_MEM_LENGTH; counter++)
  {
    *(__IO uint8_t*) (SDRAM_START_ADDRESS + counter) = (uint8_t)0x0;
  }
  
  /* ������SDRAMд������  8λ */
  for (counter = 0; counter < SDRAM_MEM_LENGTH; counter++)
  {
    *(__IO uint8_t*) (SDRAM_START_ADDRESS + counter) = (uint8_t)(ubWritedata_8b + counter);
  }
  
  /* ��ȡ SDRAM ���ݲ����*/
  for(counter = 0; counter<SDRAM_MEM_LENGTH;counter++ )
  {
		/* �Ӹõ�ַ�������� */
    ubReaddata_8b = *(__IO uint8_t*)(SDRAM_START_ADDRESS + counter);  
    
		/* ������ݣ�������ȣ���������,���ؼ��ʧ�ܽ�� */
    if(ubReaddata_8b != (uint8_t)(ubWritedata_8b + counter))      
    {
      PRINTF("\r\n 8λ���ݶ�д����\r\n");
      return false;
    }
  }	
  
  /*��16λ��ʽ��д���ݣ������*/  
  /* ��SDRAM����ȫ������Ϊ0 */
  for (counter = 0x00; counter < SDRAM_MEM_LENGTH/2; counter++)
  {
    *(__IO uint16_t*) (SDRAM_START_ADDRESS + 2*counter) = (uint16_t)0x00;
  }
  
  /* ������SDRAMд������  16λ */
  for (counter = 0; counter < SDRAM_MEM_LENGTH/2; counter++)
  {
    *(__IO uint16_t*) (SDRAM_START_ADDRESS + 2*counter) = (uint16_t)(uhWritedata_16b + counter);
  }
  
    /* ��ȡ SDRAM ���ݲ����*/
  for(counter = 0; counter<SDRAM_MEM_LENGTH/2;counter++ )
  {
		/* �Ӹõ�ַ�������� */
    uhReaddata_16b = *(__IO uint16_t*)(SDRAM_START_ADDRESS + 2*counter);  
    
		/* ������ݣ�������ȣ���������,���ؼ��ʧ�ܽ�� */
    if(uhReaddata_16b != (uint16_t)(uhWritedata_16b + counter))      
    {
      PRINTF("\r\n 16λ���ݶ�д����\r\n");
      return false;
    }
  }  
  PRINTF("\r\n SDRAM��Ƭ��д����������\r\n"); 

  return true;  
}

/****************************END OF FILE**********************/
