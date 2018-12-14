/**
  ******************************************************************************
  * @file    eeprom_test.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   EEPROM����
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  i.MXRT1052������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
#include "fsl_debug_console.h"
#include "./led/bsp_led.h"
#include "./eeprom/bsp_i2c_eeprom.h"

/*******************************************************************
 * Definition
 *******************************************************************/
#define EEPROM_TEST_NUM           256
#define EEPORM_TEST_START_ADDR    0


/*******************************************************************
 * Variables
 *******************************************************************/
uint8_t EEPROM_Buffer_Write[256];
uint8_t EEPROM_Buffer_Read[256];


/**
  * @brief  I2C(AT24C02)��д����
  * @param  ��
  * @retval ��������1 ������������0
  */
uint8_t EEPROM_Test(void)
{
  uint16_t i;

  EEPROM_INFO("д�������");
    
  for ( i=0; i<EEPROM_TEST_NUM; i++ ) //��仺��
  {   
    EEPROM_Buffer_Write[i] = i;

    PRINTF("0x%02X ", EEPROM_Buffer_Write[i]);
    if((i+1)%10 == 0 || i == (EEPROM_TEST_NUM-1))    
        PRINTF("\r\n");    
   }
  
  //��I2c_Buf_Write��˳�����������д��EERPOM�� 
  I2C_EEPROM_Buffer_Write(EEPROM_WRITE_ADDRESS_8_BIT,
                             EEPORM_TEST_START_ADDR,
                             EEPROM_Buffer_Write,
                             EEPROM_TEST_NUM);
  
  EEPROM_INFO("д�ɹ�");
   
  EEPROM_INFO("����������");
  //��EEPROM��������˳�򱣳ֵ�I2c_Buf_Read��
  I2C_EEPROM_BufferRead(EEPROM_READ_ADDRESS_8_BIT,
                           EEPORM_TEST_START_ADDR,
                           EEPROM_Buffer_Read,
                           EEPROM_TEST_NUM); 
   
  //��I2c_Buf_Read�е�����ͨ�����ڴ�ӡ
  for (i=0; i<EEPROM_TEST_NUM; i++)
  { 
    if(EEPROM_Buffer_Read[i] != EEPROM_Buffer_Write[i])
    {
      PRINTF("0x%02X ", EEPROM_Buffer_Read[i]);
      RGB_LED_COLOR_RED;
      EEPROM_ERROR("����:I2C EEPROMд������������ݲ�һ��");
      return 0;
    }
    PRINTF("0x%02X ", EEPROM_Buffer_Read[i]);
    if((i+1)%10 == 0 || i == (EEPROM_TEST_NUM-1))    
        PRINTF("\r\n");
    
  }
  
  CORE_BOARD_LED_ON;
  RGB_LED_COLOR_GREEN;
  EEPROM_INFO("I2C(AT24C02)��д���Գɹ�");
  return 1;
}


/*********************************************END OF FILE**********************/
