/**
  ******************************************************************************
  * @file    bsp_i2c.c
  * @author  fire
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   MPU6050-加速度陀螺仪
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  i.MXRT1052开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
	
#include "./i2c/bsp_i2c.h"
#include "./uart/bsp_uart.h"

#include "fsl_iomuxc.h"
#include "fsl_gpio.h"  
  
#include "pad_config.h"     
#include "./delay/core_delay.h"  

#define I2C_DIR_WR	0		/* 写控制bit */
#define I2C_DIR_RD	1		/* 读控制bit */
	#define MPU_ADDR				0X68
/*******************************************************************************
 * 宏
 ******************************************************************************/
/* 所有引脚均使用同样的PAD配置 */
#define IIC_PAD_CONFIG_DATA            (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_2_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_0_PULL_KEEPER_DISABLED| \
                                        PUE_0_KEEPER_SELECTED| \
                                        PUS_0_100K_OHM_PULL_DOWN| \
                                        HYS_0_HYSTERESIS_DISABLED)  																		
    /* 配置说明 : */
    /* 转换速率: 转换速率慢
      驱动强度: R0/6 
      带宽配置 : medium(100MHz)
      开漏配置: 关闭 
      拉/保持器配置: 关闭
      拉/保持器选择: 保持器（上面已关闭，配置无效）
      上拉/下拉选择: 100K欧姆下拉（上面已关闭，配置无效）
      滞回器配置: 关闭 */     

/*******************************************************************************
 * 声明
 ******************************************************************************/
void IIC_IOMUXC_MUX_Config(void);
void IIC_IOMUXC_PAD_Config(void);
void IIC_GPIO_Mode_Config(void);

/**
* @brief  初始化IIC相关IOMUXC的MUX复用配置
* @param  无
* @retval 无
*/
static void IIC_IOMUXC_MUX_Config(void)
{  
  /* 设置引脚的复用模式为GPIO，不使用SION功能 */
  IOMUXC_SetPinMux(MPU6050_I2C_SCL_PIN_IOMUXC, 0U);
  IOMUXC_SetPinMux(MPU6050_I2C_SDA_PIN_IOMUXC, 0U); 
}

/**
* @brief  初始化LED相关IOMUXC的MUX复用配置
* @param  无
* @retval 无
*/
static void IIC_IOMUXC_PAD_Config(void)
{
  /* 核心板的LED灯 */    

  IOMUXC_SetPinConfig(MPU6050_I2C_SCL_PIN_IOMUXC,IIC_PAD_CONFIG_DATA);  
  IOMUXC_SetPinConfig(MPU6050_I2C_SDA_PIN_IOMUXC,IIC_PAD_CONFIG_DATA); 
}

/**
* @brief  配置SDA为输入模式
* @param  无
* @retval 无
*/
void I2C_SDA_IN_Config()
{
  /* 定义gpio初始化配置结构体 */
  gpio_pin_config_t iic_config;      
   /** GPIO配置 **/       
  iic_config.direction = kGPIO_DigitalInput; //输入模式	
  iic_config.outputLogic =  0;                //默认低电平
  iic_config.interruptMode = kGPIO_NoIntmode; //不使用中断
  /* 初始化 GPIO 模拟IIC. */
  GPIO_PinInit(MPU6050_I2C_SDA_PIN_GPIO, MPU6050_I2C_SDA_PIN, &iic_config);
}

/**
* @brief  配置SDA为输出模式
* @param  无
* @retval 无
*/
void I2C_SDA_OUT_Config()
{
  /* 定义gpio初始化配置结构体 */
  gpio_pin_config_t iic_config;      
   /** GPIO配置 **/       
  iic_config.direction = kGPIO_DigitalOutput; //输出模式	
  iic_config.outputLogic =  0;                //默认低电平
  iic_config.interruptMode = kGPIO_NoIntmode; //不使用中断
  /* 初始化 GPIO 模拟IIC. */
  GPIO_PinInit(MPU6050_I2C_SDA_PIN_GPIO, MPU6050_I2C_SDA_PIN, &iic_config);
}

 /**
  * @brief  初始化LED相关的GPIO模式
  * @param  无
  * @retval 无
  */
static void IIC_GPIO_Mode_Config(void)
{     	
  /* 定义gpio初始化配置结构体 */
  gpio_pin_config_t iic_config;      
    
   /** GPIO配置 **/       
  iic_config.direction = kGPIO_DigitalOutput; //输出模式	
  iic_config.outputLogic =  1;                //默认高电平
  iic_config.interruptMode = kGPIO_NoIntmode; //不使用中断  
  /* 初始化 GPIO 模拟IIC. */
  GPIO_PinInit(MPU6050_I2C_SCL_PIN_GPIO, MPU6050_I2C_SCL_PIN, &iic_config);
  GPIO_PinInit(MPU6050_I2C_SDA_PIN_GPIO, MPU6050_I2C_SDA_PIN, &iic_config);	
}

/**
  * @brief  I2C I/O配置
  * @param  无
  * @retval 无
  */
static void I2C_GPIO_Config(void)
{
	  /* 初始化GPIO复用、属性、模式 */
  IIC_IOMUXC_MUX_Config();
  IIC_IOMUXC_PAD_Config();
  IIC_GPIO_Mode_Config();
}

/**
  * @brief  I2C 外设(GT91xx)初始化
  * @param  无
  * @retval 无
  */
void I2C_Init(void)
{
	I2C_GPIO_Config(); 
	I2C_SDA_1();
	I2C_SCL_1();
}
/*
*********************************************************************************************************
*	函 数 名: i2c_Delay
*	功能说明: I2C总线位延迟，最快400KHz
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void i2c_Delay(void)
{
	CPU_TS_Tmr_Delay_US(4);
//		CPU_TS_Tmr_Delay_US(10);
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Start
*	功能说明: CPU发起I2C总线启动信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/


void i2c_Start(void)
{
	/* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */
	I2C_SDA_OUT();   
	I2C_SDA_1();	  	  
	I2C_SCL_1();
	i2c_Delay();
	I2C_SDA_0();
	i2c_Delay();
	I2C_SCL_0();
}	 

/*
*********************************************************************************************************
*	函 数 名: i2c_Start
*	功能说明: CPU发起I2C总线停止信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/

void i2c_Stop(void)
{
	/* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
	I2C_SDA_OUT();
	I2C_SCL_0();
	I2C_SDA_0();
 	i2c_Delay();
	I2C_SCL_1(); 
	I2C_SDA_1();
	i2c_Delay();							   	
}

/*
*********************************************************************************************************
*	函 数 名: i2c_SendByte
*	功能说明: CPU向I2C总线设备发送8bit数据
*	形    参：_ucByte ： 等待发送的字节
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_SendByte(uint8_t _ucByte)
{
	uint8_t i;

	/* 先发送字节的高位bit7 */
	for (i = 0; i < 8; i++)
	{		
		if (_ucByte & 0x80)
		{
			I2C_SDA_1();
		}
		else
		{
			I2C_SDA_0();
		}
		i2c_Delay();
		I2C_SCL_1();
		i2c_Delay();	
		I2C_SCL_0();
		if (i == 7)
		{
			 I2C_SDA_1(); // 释放总线
		}
		_ucByte <<= 1;	/* 左移一个bit */
		i2c_Delay();
	}
}

/*
*********************************************************************************************************
*	函 数 名: i2c_WaitAck
*	功能说明: CPU产生一个时钟，并读取器件的ACK应答信号
*	形    参：无
*	返 回 值: 返回0表示正确应答，1表示无器件响应
*********************************************************************************************************
*/
uint8_t i2c_WaitAck(void)
{
	uint8_t re;
	I2C_SDA_IN();//配置输入模式
	I2C_SDA_1();	/* CPU释放SDA总线 */
	i2c_Delay();
	I2C_SCL_1();	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
	i2c_Delay();
	if (I2C_SDA_READ())	/* CPU读取SDA口线状态 */
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	I2C_SCL_0();
	i2c_Delay();
	I2C_SDA_OUT();
	return re;
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Ack
*	功能说明: CPU产生一个ACK信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_Ack(void)
{	
	I2C_SCL_0();
	I2C_SDA_OUT();
	I2C_SDA_0();/* CPU驱动SDA = 0 */
	i2c_Delay();
	I2C_SCL_1();/* CPU产生1个时钟 */
	i2c_Delay();
	I2C_SCL_0();
}

/*
*********************************************************************************************************
*	函 数 名: i2c_NAck
*	功能说明: CPU产生1个NACK信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_NAck(void)
{
	I2C_SDA_1();	/* CPU驱动SDA = 1 */
	i2c_Delay();
	I2C_SCL_1();	/* CPU产生1个时钟 */
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();	
}
/*
*********************************************************************************************************
*	函 数 名: i2c_ReadByte
*	功能说明: CPU从I2C总线设备读取8bit数据
*	形    参：无
*	返 回 值: 读到的数据
*********************************************************************************************************
*/
uint8_t i2c_ReadByte(unsigned char ack)
{
	uint8_t i;
	uint8_t value;

	/* 读到第1个bit为数据的bit7 */
	value = 0;
	I2C_SDA_IN();//配置输入模式
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		I2C_SCL_1();
		i2c_Delay();
		if (I2C_SDA_READ())
		{
			value++;
		}
		I2C_SCL_0();
		i2c_Delay();
	}
	if (!ack)i2c_NAck();
	else i2c_Ack();
	return value;
}



/**
  * @brief   使用IIC写入数据
  * @param   
  * 	@arg ClientAddr:从设备地址
  *		@arg pBuffer:缓冲区指针
  *     @arg NumByteToWrite:写的字节数
  * @retval  无
  */
uint8_t I2C_WriteBytes(uint8_t reg_add,uint8_t data) 				 
{ 
	/* 第1步：发起I2C总线启动信号 */
	i2c_Start(); 
	 /* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	i2c_SendByte((MPU_ADDR<<1)|I2C_DIR_WR);
	 /* 第3步：发送一个时钟，判断器件是否正确应答 */
	if(i2c_WaitAck())	
	{
		i2c_Stop();		 
		return 1;		
	}
	 /* 第4步：开始写入数据 */
	i2c_SendByte(reg_add);
	 /* 第5步：检查ACK */
	i2c_WaitAck();
	i2c_SendByte(data);
	if(i2c_WaitAck())
	{
		i2c_Stop();	 
		return 1;		 
	}		 
    i2c_Stop();	 
	return 0;
}

/**
  * @brief   使用IIC读取数据
  * @param   
  * 	@arg ClientAddr:从设备地址
  *		@arg pBuffer:存放由从机读取的数据的缓冲区指针
  *		@arg NumByteToRead:读取的数据长度
  * @retval  无
  */
uint8_t I2C_ReadBytes(uint8_t reg_add)
{
	uint8_t res;
	/* 第1步：发起I2C总线启动信号 */
	i2c_Start(); 
	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	i2c_SendByte((MPU_ADDR<<1)|I2C_DIR_WR);
	/* 第3步：等待ACK */
	i2c_WaitAck();		
	/* 发送写寄存器地址 */
	i2c_SendByte(reg_add);	
	/* 等待ACK */
	i2c_WaitAck();		
	i2c_Start();
	i2c_SendByte((MPU_ADDR<<1)|I2C_DIR_RD);
	i2c_WaitAck();		
	res=i2c_ReadByte(0);
	/* 发送I2C总线停止信号 */
	i2c_Stop();			
	return res;		
}

/**
  * @brief  写入字节
  * @param   
  */
uint8_t Sensor_write_DMP(uint8_t slave_addr,uint8_t reg_add,uint8_t num,unsigned char* write)
{
	uint8_t i; 
	 /* 第1步：发起I2C总线启动信号 */
	i2c_Start(); 
	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	i2c_SendByte((slave_addr<<1)|I2C_DIR_WR);//发送器件地址+写命令	
	/* 第3步：发送一个时钟，判断器件是否正确应答 */
	if(i2c_WaitAck())	
	{
		/* 发送I2C总线停止信号 */
		i2c_Stop();		 
		return 1;		
	}

	i2c_SendByte(reg_add);	
	/*等待ACK */
	i2c_WaitAck();		
	for(i=0;i<num;i++)
	{
		/* 第4步：开始写入数据 */
		i2c_SendByte(write[i]);	
		/* 第5步：检查ACK */
		if(i2c_WaitAck())	
		{
			/* 发送I2C总线停止信号 */
			i2c_Stop();	 
			return 1;		 
		}		
	}    
	/* 发送I2C总线停止信号 */
	i2c_Stop();	 
	return 0;	
} 

/**
  * @brief   读取6050寄存器
  */
uint8_t Sensor_Read_DMP(uint8_t slave_addr,uint8_t reg_add,uint8_t num,unsigned char* Read)
{ 
	/* 第1步：发起I2C总线启动信号 */
	i2c_Start(); 
	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	i2c_SendByte((slave_addr<<1)|I2C_DIR_WR);//发送器件地址+写命令	
	/* 第3步：发送一个时钟，判断器件是否正确应答 */
	if(i2c_WaitAck())	//等待应答
	{
		i2c_Stop();		 
		return 1;		
	}
	/* 第4步：开始写入寄存器地址 */
	i2c_SendByte(reg_add);	
	/* 第5步：等待ACK */
	i2c_WaitAck();		//等待应答
  /* 第6步：发起I2C总线启动信号 */
	i2c_Start();
	/*  发起I2C总线启动信号  */
	i2c_SendByte((slave_addr<<1)|I2C_DIR_RD);
	i2c_WaitAck();	
	while(num)
	{
		/* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
		if(num==1)*Read=i2c_ReadByte(0);
		/* 中间字节读完后，CPU产生ACK信号(驱动SDA = 0) */  
		else *Read=i2c_ReadByte(1);		
		/*计数器自减 */
		num --;
		/* 读指针自增 */
		Read++; 
	}    
	/* 发送I2C总线停止信号 */
	i2c_Stop();	
	return 0;	
}




/*********************************************END OF FILE**********************/

