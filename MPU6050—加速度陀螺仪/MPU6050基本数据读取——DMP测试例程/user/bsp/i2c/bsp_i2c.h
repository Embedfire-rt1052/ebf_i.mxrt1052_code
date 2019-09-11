#ifndef __BSP_I2C_H
#define	__BSP_I2C_H

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"


/*设定的MPU6050 IIC设备地址*/
#define MPU6050_ADDR              0xD0

#define I2CT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT         ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))

/*I2C引脚*/
#define MPU6050_I2C_SCL_PIN_GPIO             GPIO1
#define MPU6050_I2C_SCL_PIN         				 (16U)
#define MPU6050_I2C_SCL_PIN_IOMUXC           IOMUXC_GPIO_AD_B1_00_GPIO1_IO16

#define MPU6050_I2C_SDA_PIN_GPIO             GPIO1
#define MPU6050_I2C_SDA_PIN         				 (17U)
#define MPU6050_I2C_SDA_PIN_IOMUXC           IOMUXC_GPIO_AD_B1_01_GPIO1_IO17

//软件IIC使用的宏

#define I2C_SCL_1()  GPIO_PinWrite(MPU6050_I2C_SCL_PIN_GPIO, MPU6050_I2C_SCL_PIN,1)		/* SCL = 1 */
#define I2C_SCL_0()  GPIO_PinWrite(MPU6050_I2C_SCL_PIN_GPIO, MPU6050_I2C_SCL_PIN,0)		/* SCL = 0 */

#define I2C_SDA_1()  GPIO_PinWrite(MPU6050_I2C_SDA_PIN_GPIO, MPU6050_I2C_SDA_PIN,1)		/* SDA = 1 */
#define I2C_SDA_0()  GPIO_PinWrite(MPU6050_I2C_SDA_PIN_GPIO, MPU6050_I2C_SDA_PIN,0)		/* SDA = 0 */

#define I2C_SDA_READ()  GPIO_ReadPadStatus(MPU6050_I2C_SDA_PIN_GPIO, MPU6050_I2C_SDA_PIN)	/* 读SDA口线状态 */ //GPIO_PinReadPadStatus GPIO_PinRead

#define I2C_SDA_IN()	I2C_SDA_IN_Config()		/*	配置SDA为输入模式	*/
#define I2C_SDA_OUT()	I2C_SDA_OUT_Config()	/*  配置SDA为输出模式 */

/*信息输出*/
#define I2C_DEBUG_ON         1
#define I2C_DEBUG_FUNC_ON    0

#define I2C_INFO(fmt,arg...)           PRINTF("<<-I2C-INFO->> "fmt"\n",##arg)
#define I2C_ERROR(fmt,arg...)          PRINTF("<<-I2C-ERROR->> "fmt"\n",##arg)
#define I2C_DEBUG(fmt,arg...)          do{\
                                          if(I2C_DEBUG_ON)\
                                          PRINTF("<<-I2C-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)

#define I2C_DEBUG_FUNC()               do{\
                                         if(I2C_DEBUG_FUNC_ON)\
                                         PRINTF("<<-I2C-FUNC->> Func:%s@Line:%d\n",__func__,__LINE__);\
                                       }while(0)

//函数接口
void I2C_Init(void);

//uint32_t Sensor_write(uint8_t reg_add,uint8_t reg_dat);
//uint32_t Sensor_Read(uint8_t reg_add,unsigned char* Read,uint8_t num);
extern void I2C_SDA_OUT_Config(void);
extern void I2C_SDA_IN_Config(void);
																			 
extern uint8_t Sensor_write_DMP(uint8_t slave_addr,uint8_t reg_add,uint8_t num,unsigned char* write)  ;
extern uint8_t Sensor_Read_DMP(uint8_t slave_addr,uint8_t reg_add,uint8_t num,unsigned char* Read)		;			
																			
																	 
#endif /* __BSP_I2C */
