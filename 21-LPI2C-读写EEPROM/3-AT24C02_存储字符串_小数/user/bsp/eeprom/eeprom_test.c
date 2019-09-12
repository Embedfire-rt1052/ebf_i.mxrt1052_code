/**
  ******************************************************************************
  * @file    eeprom_test.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   EEPROM测试
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  i.MXRT1052开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
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

/*存储小数和整数的数组，各7个*/
long double double_buffer[7] = {0};
int int_bufffer[7] = {0};




/**
  * @brief  I2C(AT24C02)存储长整数、小数测试
  * @param  无
  * @retval 无
  */
void EEPROM_Test(void)
{
  uint8_t i;
  /*读取数据标志位*/
  I2C_EEPROM_BufferRead(EEPROM_READ_ADDRESS_8_BIT, 0, &cal_flag, 1);
  
   if( cal_flag != 0xAB ) /*若标志等于0xab，表示之前已有写入数据*/
    {      
        PRINTF("\r\n没有检测到数据标志，EEPROM没有存储数据，即将进行小数写入实验\r\n");
        cal_flag =0xAB;
        
        /*写入标志到0地址*/
        I2C_EEPROM_Buffer_Write(EEPROM_WRITE_ADDRESS_8_BIT, 0, &cal_flag, 1); 
        
        /*生成要写入的数据*/
        for( i=0; i<7; i++ )
        {
           double_buffer[i] = i +0.1;
           int_bufffer[i]=i*500+1 ;
        }

        /*写入小数数据到地址10*/
        I2C_EEPROM_Buffer_Write(EEPROM_WRITE_ADDRESS_8_BIT,DOUBLE_ADDR,(void*)double_buffer, sizeof(double_buffer));
        /*写入整数数据到地址60*/
        I2C_EEPROM_Buffer_Write(EEPROM_WRITE_ADDRESS_8_BIT,LONGINT_ADDR,(void*)int_bufffer, sizeof(int_bufffer));
              
        PRINTF("向芯片写入数据：");
        /*打印到串口*/
        for( i=0; i<7; i++ )
        {
          PRINTF("小数tx = %f\r\n",double_buffer[i]);
          PRINTF("整数tx = %d\r\n",int_bufffer[i]);
        }
        
        PRINTF("\r\n请复位开发板，以读取数据进行检验\r\n");      
    
    }    
    else
    {      
         PRINTF("\r\n检测到数据标志\r\n");

        /*读取小数数据*/
        I2C_EEPROM_BufferRead(EEPROM_READ_ADDRESS_8_BIT, DOUBLE_ADDR,(void*)double_buffer, sizeof(double_buffer));
        
        /*读取整数数据*/
        I2C_EEPROM_BufferRead(EEPROM_READ_ADDRESS_8_BIT, LONGINT_ADDR, (void*)int_bufffer,sizeof(int_bufffer));
  
      
        PRINTF("\r\n从芯片读到数据：\r\n");     
        for( i=0; i<7; i++ )
        {
          PRINTF("小数 rx = %F \r\n",double_buffer[i]);
          PRINTF("整数 rx = %d \r\n",int_bufffer[i]);       
        }
      
    }   
}

/*********************************************END OF FILE**********************/
