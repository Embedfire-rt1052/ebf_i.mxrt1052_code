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
#define DOUBLE_ADDR       10
#define LONGINT_ADDR      70


/*******************************************************************
 * Variable
 *******************************************************************/
uint8_t cal_flag = 0;

/*�洢С�������������飬��7��*/
long double double_buffer[7] = {0};
int int_bufffer[7] = {0};




/**
  * @brief  I2C(AT24C02)�洢��������С������
  * @param  ��
  * @retval ��
  */
void EEPROM_Test(void)
{
  uint8_t i;
  /*��ȡ���ݱ�־λ*/
  I2C_EEPROM_BufferRead(EEPROM_READ_ADDRESS_8_BIT, 0, &cal_flag, 1);
  
   if( cal_flag != 0xAB ) /*����־����0xab����ʾ֮ǰ����д������*/
    {      
        PRINTF("\r\nû�м�⵽���ݱ�־��EEPROMû�д洢���ݣ���������С��д��ʵ��\r\n");
        cal_flag =0xAB;
        
        /*д���־��0��ַ*/
        I2C_EEPROM_Buffer_Write(EEPROM_WRITE_ADDRESS_8_BIT, 0, &cal_flag, 1); 
        
        /*����Ҫд�������*/
        for( i=0; i<7; i++ )
        {
           double_buffer[i] = i +0.1;
           int_bufffer[i]=i*500+1 ;
        }

        /*д��С�����ݵ���ַ10*/
        I2C_EEPROM_Buffer_Write(EEPROM_WRITE_ADDRESS_8_BIT,DOUBLE_ADDR,(void*)double_buffer, sizeof(double_buffer));
        /*д���������ݵ���ַ60*/
        I2C_EEPROM_Buffer_Write(EEPROM_WRITE_ADDRESS_8_BIT,LONGINT_ADDR,(void*)int_bufffer, sizeof(int_bufffer));
              
        PRINTF("��оƬд�����ݣ�");
        /*��ӡ������*/
        for( i=0; i<7; i++ )
        {
          PRINTF("С��tx = %f\r\n",double_buffer[i]);
          PRINTF("����tx = %d\r\n",int_bufffer[i]);
        }
        
        PRINTF("\r\n�븴λ�����壬�Զ�ȡ���ݽ��м���\r\n");      
    
    }    
    else
    {      
         PRINTF("\r\n��⵽���ݱ�־\r\n");

        /*��ȡС������*/
        I2C_EEPROM_BufferRead(EEPROM_READ_ADDRESS_8_BIT, DOUBLE_ADDR,(void*)double_buffer, sizeof(double_buffer));
        
        /*��ȡ��������*/
        I2C_EEPROM_BufferRead(EEPROM_READ_ADDRESS_8_BIT, LONGINT_ADDR, (void*)int_bufffer,sizeof(int_bufffer));
  
      
        PRINTF("\r\n��оƬ�������ݣ�\r\n");     
        for( i=0; i<7; i++ )
        {
          PRINTF("С�� rx = %F \r\n",double_buffer[i]);
          PRINTF("���� rx = %d \r\n",int_bufffer[i]);       
        }
      
    }   
}

/*********************************************END OF FILE**********************/
