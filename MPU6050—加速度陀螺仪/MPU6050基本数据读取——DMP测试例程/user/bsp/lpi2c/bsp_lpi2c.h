#ifndef __BSP_LPI2C_H
#define __BSP_LPI2C_H

#include <stdio.h>
#include "fsl_common.h"
#include "fsl_lpi2c.h"

/*
选择LPI2C的时钟源
0 derive clock from pll3_60m
1 derive clock from osc_clk
*/
/* 选择 USB1 PLL/8 (480/8 = 60MHz) 作为lpi2c主机时钟源, */
#define LPI2C_CLOCK_SOURCE_SELECT     (0U)
/* lpi2c主机 时钟源的时钟分频因子 */
#define LPI2C_CLOCK_SOURCE_DIVIDER    (5U)
/* 获取 lpi2c 时钟频率LPI2C_CLK_ROOT = 60/(5+1) = 10MHz */
#define LPI2C_CLOCK_FREQUENCY ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (LPI2C_CLOCK_SOURCE_DIVIDER + 1U))

#define LPI2C_MASTER_CLOCK_FREQUENCY  LPI2C_CLOCK_FREQUENCY

/* lpi2c外设编号 */
#define I2C_MASTER_BASE   (LPI2C1_BASE)
#define I2C_MASTER        ((LPI2C_Type *)I2C_MASTER_BASE)
/* lpi2c波特率 */
//#define I2C_BAUDRATE      400000U
#define I2C_BAUDRATE      100000U

/*! @brief I2C引脚定义 */
#define SCL_IOMUXC       IOMUXC_GPIO_AD_B1_00_LPI2C1_SCL
#define SDA_IOMUXC       IOMUXC_GPIO_AD_B1_01_LPI2C1_SDA

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


//函数接口
int32_t EEPROM_I2C_ReadBytes(uint8_t client_addr, uint8_t *buf, int32_t len);

void I2C_EEPROM_Init(void);
uint32_t I2C_WriteBytes(uint8_t ClientAddr,uint8_t* pBuffer,  uint8_t NumByteToWrite);
uint32_t I2C_ReadBytes(uint8_t ClientAddr,uint8_t* pBuffer, uint16_t NumByteToRead);
                                       

extern uint32_t Sensor_write_hardware(uint8_t reg_add,uint8_t reg_dat);
extern uint32_t Sensor_Read_hardware(uint8_t reg_add,unsigned char* Read,uint8_t num);
//uint32_t Sensor_write_hardware_dmp(unsigned char slave_addr,unsigned char reg_add,unsigned char length,unsigned char const *reg_dat)																					
//extern uint32_t Sensor_write_hardware_dmp(unsigned char slave_addr,unsigned char reg_add,unsigned char length,unsigned char const *reg_dat)   ;
//extern uint32_t Sensor_Read_hardware_dmp(unsigned char slave_addr,uint8_t reg_add,unsigned char length,unsigned char* Read) ;
//											

extern uint32_t Sensor_write_hardware_dmp(uint8_t slave_addr,uint8_t reg_add,uint8_t length,uint8_t *reg_dat);
extern uint32_t Sensor_Read_hardware_dmp(uint8_t slave_addr,uint8_t reg_add,uint8_t length,uint8_t *Read);
																					;
extern void I2C_Init_Hard(void);


#endif /* __BSP_LPI2C_H */
