#ifndef __I2C_EEPROM_H
#define __I2C_EEPROM_H

#include <stdio.h>
#include "fsl_common.h"
#include "fsl_lpi2c.h"

/*
ѡ��LPI2C��ʱ��Դ
0 derive clock from pll3_60m
1 derive clock from osc_clk
*/
/* ѡ�� USB1 PLL/8 (480/8 = 60MHz) ��Ϊlpi2c����ʱ��Դ, */
#define LPI2C_CLOCK_SOURCE_SELECT     (0U)
/* lpi2c���� ʱ��Դ��ʱ�ӷ�Ƶ���� */
#define LPI2C_CLOCK_SOURCE_DIVIDER    (5U)
/* ��ȡ lpi2c ʱ��Ƶ��LPI2C_CLK_ROOT = 60/(5+1) = 10MHz */
#define LPI2C_CLOCK_FREQUENCY ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (LPI2C_CLOCK_SOURCE_DIVIDER + 1U))

#define LPI2C_MASTER_CLOCK_FREQUENCY  LPI2C_CLOCK_FREQUENCY

/* lpi2c������ */
#define EEPROM_I2C_MASTER_BASE   (LPI2C1_BASE)
#define EEPROM_I2C_MASTER        ((LPI2C_Type *)EEPROM_I2C_MASTER_BASE)
/* lpi2c������ */
#define EEPROM_I2C_BAUDRATE      400000U

/*! @brief I2C���Ŷ��� */
#define EEPROM_SCL_IOMUXC       IOMUXC_GPIO_AD_B1_00_LPI2C1_SCL
#define EEPROM_SDA_IOMUXC       IOMUXC_GPIO_AD_B1_01_LPI2C1_SDA

/* �ȴ���ʱʱ�� */
#define I2CT_FLAG_TIMEOUT         ((uint32_t)0x100)
#define I2CT_LONG_TIMEOUT         ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))

/* EEPROM ���ܿռ��С*/
#define EEPROM_SIZE                         256
/* AT24C01/02ÿҳ��8���ֽ� */
#define EEPROM_PAGE_SIZE                    8
/* EEPROM�豸��ַ */
#define EEPROM_ADDRESS_7_BIT                    (0xA0>>1)   
#define EEPROM_WRITE_ADDRESS_8_BIT              (0xA0)   
#define EEPROM_READ_ADDRESS_8_BIT               (0xA1)   

/* EEPROM�ڲ��洢��Ԫ��ַ�Ĵ�С����λ���ֽ�*/
#define EEPROM_INER_ADDRESS_SIZE            0x01   

/***************************PART1:ON/OFF define*******************************/

#define EEPROM_DEBUG_ON             1
#define EEPROM_DEBUG_ARRAY_ON       1
#define EEPROM_DEBUG_FUNC_ON      0
// Log define
#define EEPROM_INFO(fmt,arg...)           PRINTF("<<-EEPROM-INFO->> "fmt"\n",##arg)
#define EEPROM_ERROR(fmt,arg...)          PRINTF("<<-EEPROM-ERROR->> "fmt"\n",##arg)
#define EEPROM_DEBUG(fmt,arg...)          do{\
                                         if(EEPROM_DEBUG_ON)\
                                         PRINTF("<<-EEPROM-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)

#define EEPROM_DEBUG_ARRAY(array, num)    do{\
                                             int32_t i;\
                                             uint8_t* a = array;\
                                             if(EEPROM_DEBUG_ARRAY_ON)\
                                             {\
                                                PRINTF("<<-EEPROM-DEBUG-ARRAY->>\n");\
                                                for (i = 0; i < (num); i++)\
                                                {\
                                                    PRINTF("%02x   ", (a)[i]);\
                                                    if ((i + 1 ) %10 == 0)\
                                                    {\
                                                        PRINTF("\n");\
                                                    }\
                                                }\
                                                PRINTF("\n");\
                                            }\
                                          }while(0)

#define EEPROM_DEBUG_FUNC()             do{\
                                           if(EEPROM_DEBUG_FUNC_ON)\
                                           PRINTF("<<-EEPROM-FUNC->> Func:%s@Line:%d\n",__func__,__LINE__);\
                                          }while(0)


//�����ӿ�
int32_t EEPROM_I2C_ReadBytes(uint8_t client_addr, uint8_t *buf, int32_t len);

void I2C_EEPROM_Init(void);
uint32_t I2C_WriteBytes(uint8_t ClientAddr,uint8_t* pBuffer,  uint8_t NumByteToWrite);
uint32_t I2C_ReadBytes(uint8_t ClientAddr,uint8_t* pBuffer, uint16_t NumByteToRead);
                                       
uint32_t I2C_EEPROM_BufferRead( uint8_t ClientAddr,
                                uint8_t ReadAddr, 
                                uint8_t* pBuffer, 
                                uint16_t NumByteToRead);                                       

uint32_t I2C_EEPROM_Page_Write( uint8_t ClientAddr,
                                uint8_t WriteAddr, 
                                uint8_t* pBuffer,  
                                uint8_t NumByteToWrite);
                                       
void I2C_EEPROM_Buffer_Write( uint8_t ClientAddr,
                              uint8_t WriteAddr, 
                              uint8_t* pBuffer,  
                              uint16_t NumByteToWrite);
                                       

uint8_t I2C_EEPROM_WaitStandbyState(uint8_t ClientAddr);


#endif /* __I2C_EEPROM_H */
