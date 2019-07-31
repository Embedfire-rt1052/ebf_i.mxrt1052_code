/**
  ******************************************************************************
  * @file    bsp_i2c.c
  * @author  fire
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   MPU6050-���ٶ�������
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  i.MXRT1052������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
	
#include "./i2c/bsp_i2c.h"
#include "./uart/bsp_uart.h"

#include "fsl_iomuxc.h"
#include "fsl_gpio.h"  
  
#include "pad_config.h"     
#include "./delay/core_delay.h"  

#define I2C_DIR_WR	0		/* д����bit */
#define I2C_DIR_RD	1		/* ������bit */
	#define MPU_ADDR				0X68

/*******************************************************************************
 * ��
 ******************************************************************************/
/* �������ž�ʹ��ͬ����PAD���� */
#define IIC_PAD_CONFIG_DATA            (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_2_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_0_PULL_KEEPER_DISABLED| \
                                        PUE_0_KEEPER_SELECTED| \
                                        PUS_0_100K_OHM_PULL_DOWN| \
                                        HYS_0_HYSTERESIS_DISABLED)  																		
    /* ����˵�� : */
    /* ת������: ת��������
      ����ǿ��: R0/6 
      �������� : medium(100MHz)
      ��©����: �ر� 
      ��/����������: �ر�
      ��/������ѡ��: �������������ѹرգ�������Ч��
      ����/����ѡ��: 100Kŷķ�����������ѹرգ�������Ч��
      �ͻ�������: �ر� */     

/*******************************************************************************
 * ����
 ******************************************************************************/
void IIC_IOMUXC_MUX_Config(void);
void IIC_IOMUXC_PAD_Config(void);
void IIC_GPIO_Mode_Config(void);

/**
* @brief  ��ʼ��IIC���IOMUXC��MUX��������
* @param  ��
* @retval ��
*/
static void IIC_IOMUXC_MUX_Config(void)
{  
  /* �������ŵĸ���ģʽΪGPIO����ʹ��SION���� */
  IOMUXC_SetPinMux(MPU6050_I2C_SCL_PIN_IOMUXC, 0U);
  IOMUXC_SetPinMux(MPU6050_I2C_SDA_PIN_IOMUXC, 0U); 
}

/**
* @brief  ��ʼ��LED���IOMUXC��MUX��������
* @param  ��
* @retval ��
*/
static void IIC_IOMUXC_PAD_Config(void)
{
  /* ���İ��LED�� */    

  IOMUXC_SetPinConfig(MPU6050_I2C_SCL_PIN_IOMUXC,IIC_PAD_CONFIG_DATA);  
  IOMUXC_SetPinConfig(MPU6050_I2C_SDA_PIN_IOMUXC,IIC_PAD_CONFIG_DATA); 
}

/**
* @brief  ����SDAΪ����ģʽ
* @param  ��
* @retval ��
*/
void I2C_SDA_IN_Config()
{
  /* ����gpio��ʼ�����ýṹ�� */
  gpio_pin_config_t iic_config;      
   /** GPIO���� **/       
  iic_config.direction = kGPIO_DigitalInput; //����ģʽ	
  iic_config.outputLogic =  0;                //Ĭ�ϵ͵�ƽ
  iic_config.interruptMode = kGPIO_NoIntmode; //��ʹ���ж�
  /* ��ʼ�� GPIO ģ��IIC. */
  GPIO_PinInit(MPU6050_I2C_SDA_PIN_GPIO, MPU6050_I2C_SDA_PIN, &iic_config);
}

/**
* @brief  ����SDAΪ���ģʽ
* @param  ��
* @retval ��
*/
void I2C_SDA_OUT_Config()
{
  /* ����gpio��ʼ�����ýṹ�� */
  gpio_pin_config_t iic_config;      
   /** GPIO���� **/       
  iic_config.direction = kGPIO_DigitalOutput; //���ģʽ	
  iic_config.outputLogic =  0;                //Ĭ�ϵ͵�ƽ
  iic_config.interruptMode = kGPIO_NoIntmode; //��ʹ���ж�
  /* ��ʼ�� GPIO ģ��IIC. */
  GPIO_PinInit(MPU6050_I2C_SDA_PIN_GPIO, MPU6050_I2C_SDA_PIN, &iic_config);
}

 /**
  * @brief  ��ʼ��LED��ص�GPIOģʽ
  * @param  ��
  * @retval ��
  */
static void IIC_GPIO_Mode_Config(void)
{     	
  /* ����gpio��ʼ�����ýṹ�� */
  gpio_pin_config_t iic_config;      
    
   /** GPIO���� **/       
  iic_config.direction = kGPIO_DigitalOutput; //���ģʽ	
  iic_config.outputLogic =  1;                //Ĭ�ϸߵ�ƽ
  iic_config.interruptMode = kGPIO_NoIntmode; //��ʹ���ж�  
  /* ��ʼ�� GPIO ģ��IIC. */
  GPIO_PinInit(MPU6050_I2C_SCL_PIN_GPIO, MPU6050_I2C_SCL_PIN, &iic_config);
  GPIO_PinInit(MPU6050_I2C_SDA_PIN_GPIO, MPU6050_I2C_SDA_PIN, &iic_config);	
}

/**
  * @brief  I2C I/O����
  * @param  ��
  * @retval ��
  */
static void I2C_GPIO_Config(void)
{
	  /* ��ʼ��GPIO���á����ԡ�ģʽ */
  IIC_IOMUXC_MUX_Config();
  IIC_IOMUXC_PAD_Config();
  IIC_GPIO_Mode_Config();
}

/**
  * @brief  I2C ����(GT91xx)��ʼ��
  * @param  ��
  * @retval ��
  */
void I2C_Init(void)
{
  I2C_GPIO_Config(); 
	I2C_SDA_1();
	I2C_SCL_1();
}



/*
*********************************************************************************************************
*	�� �� ��: i2c_Delay
*	����˵��: I2C����λ�ӳ٣����400KHz
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
//*/
//static void i2c_Delay(void)
//{
//	/*��
//	 	�����ʱ����ͨ���߼������ǲ��Եõ��ġ�
//    ����������CPU��Ƶ216MHz ��MDK���뻷����1���Ż�
//      
//		ѭ������Ϊ200ʱ��SCLƵ�� = 100KHz  
//	 	ѭ������Ϊ30ʱ��SCLƵ�� = 400KHz 
//  */

//	CPU_TS_Tmr_Delay_US(4);
////		CPU_TS_Tmr_Delay_US(10);
//}

///*
//*********************************************************************************************************
//*	�� �� ��: i2c_Start
//*	����˵��: CPU����I2C���������ź�
//*	��    �Σ���
//*	�� �� ֵ: ��
//*********************************************************************************************************
//*/
//void i2c_Start(void)
//{
//	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C���������ź� */
//	I2C_SDA_1();
//	I2C_SCL_1();
//	i2c_Delay();
//	I2C_SDA_0();
//	i2c_Delay();
//	I2C_SCL_0();
//	i2c_Delay();
//}

///*
//*********************************************************************************************************
//*	�� �� ��: i2c_Start
//*	����˵��: CPU����I2C����ֹͣ�ź�
//*	��    �Σ���
//*	�� �� ֵ: ��
//*********************************************************************************************************
//*/
//void i2c_Stop(void)
//{
//	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź� */
//	I2C_SDA_0();
//	I2C_SCL_1();
//	i2c_Delay();
//	I2C_SDA_1();
//}

///*
//*********************************************************************************************************
//*	�� �� ��: i2c_SendByte
//*	����˵��: CPU��I2C�����豸����8bit����
//*	��    �Σ�_ucByte �� �ȴ����͵��ֽ�
//*	�� �� ֵ: ��
//*********************************************************************************************************
//*/
//void i2c_SendByte(uint8_t _ucByte)
//{
//	uint8_t i;

//	/* �ȷ����ֽڵĸ�λbit7 */
//	for (i = 0; i < 8; i++)
//	{		
//		if (_ucByte & 0x80)
//		{
//			I2C_SDA_1();
//		}
//		else
//		{
//			I2C_SDA_0();
//		}
//		i2c_Delay();
//		I2C_SCL_1();
//		i2c_Delay();	
//		I2C_SCL_0();
//		if (i == 7)
//		{
//			 I2C_SDA_1(); // �ͷ�����
//		}
//		_ucByte <<= 1;	/* ����һ��bit */
//		i2c_Delay();
//	}
//}

///*
//*********************************************************************************************************
//*	�� �� ��: i2c_ReadByte
//*	����˵��: CPU��I2C�����豸��ȡ8bit����
//*	��    �Σ���
//*	�� �� ֵ: ����������
//*********************************************************************************************************
//*/
//uint8_t i2c_ReadByte(void)
//{
//	uint8_t i;
//	uint8_t value;

//	/* ������1��bitΪ���ݵ�bit7 */
//	value = 0;
//	I2C_SDA_IN();//��������ģʽ
//	for (i = 0; i < 8; i++)
//	{
//		value <<= 1;
//		I2C_SCL_1();
//		i2c_Delay();
//		if (I2C_SDA_READ())
//		{
//			value++;
//		}
//		I2C_SCL_0();
//		i2c_Delay();
//	}
//	I2C_SDA_OUT();
//	return value;
//}

///*
//*********************************************************************************************************
//*	�� �� ��: i2c_WaitAck
//*	����˵��: CPU����һ��ʱ�ӣ�����ȡ������ACKӦ���ź�
//*	��    �Σ���
//*	�� �� ֵ: ����0��ʾ��ȷӦ��1��ʾ��������Ӧ
//*********************************************************************************************************
//*/
//uint8_t i2c_WaitAck(void)
//{
//	uint8_t re;
//	I2C_SDA_IN();//��������ģʽ
//	I2C_SDA_1();	/* CPU�ͷ�SDA���� */
//	i2c_Delay();
//	I2C_SCL_1();	/* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
//	i2c_Delay();
//	if (I2C_SDA_READ())	/* CPU��ȡSDA����״̬ */
//	{
//		re = 1;
//	}
//	else
//	{
//		re = 0;
//	}
//	I2C_SCL_0();
//	i2c_Delay();
//	I2C_SDA_OUT();
//	return re;
//}

///*
//*********************************************************************************************************
//*	�� �� ��: i2c_Ack
//*	����˵��: CPU����һ��ACK�ź�
//*	��    �Σ���
//*	�� �� ֵ: ��
//*********************************************************************************************************
//*/
//void i2c_Ack(void)
//{
//	I2C_SDA_0();	/* CPU����SDA = 0 */
//	i2c_Delay();
//	I2C_SCL_1();	/* CPU����1��ʱ�� */
//	i2c_Delay();
//	I2C_SCL_0();
//	i2c_Delay();
//	I2C_SDA_1();	/* CPU�ͷ�SDA���� */
//}

///*
//*********************************************************************************************************
//*	�� �� ��: i2c_NAck
//*	����˵��: CPU����1��NACK�ź�
//*	��    �Σ���
//*	�� �� ֵ: ��
//*********************************************************************************************************
//*/
//void i2c_NAck(void)
//{
//	I2C_SDA_1();	/* CPU����SDA = 1 */
//	i2c_Delay();
//	I2C_SCL_1();	/* CPU����1��ʱ�� */
//	i2c_Delay();
//	I2C_SCL_0();
//	i2c_Delay();	
//}



//#define I2C_DIR_WR	0		/* д����bit */
//#define I2C_DIR_RD	1		/* ������bit */

///**
//  * @brief   ʹ��IIC��ȡ����
//  * @param   
//  * 	@arg ClientAddr:���豸��ַ
//  *		@arg pBuffer:����ɴӻ���ȡ�����ݵĻ�����ָ��
//  *		@arg NumByteToRead:��ȡ�����ݳ���
//  * @retval  ��
//  */
//uint32_t I2C_ReadBytes(uint8_t ClientAddr,uint8_t* pBuffer, uint16_t NumByteToRead)
//{
//	
//	/* ��1��������I2C���������ź� */
//	i2c_Start();
//	
//	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
//	i2c_SendByte(ClientAddr | I2C_DIR_RD);	/* �˴��Ƕ�ָ�� */
//	
//	/* ��3�����ȴ�ACK */
//	if (i2c_WaitAck() != 0)
//	{
//		goto cmd_fail;	/* ������Ӧ�� */
//	}

//	while(NumByteToRead) 
//  {
//   if(NumByteToRead == 1)
//    {
//			i2c_NAck();	/* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
//      
//      /* ����I2C����ֹͣ�ź� */
//      i2c_Stop();
//    }
//    
//   *pBuffer = i2c_ReadByte();
//    
//    /* ��ָ������ */
//    pBuffer++; 
//      
//    /*�������Լ� */
//    NumByteToRead--;
//    
//    i2c_Ack();	/* �м��ֽڶ����CPU����ACK�ź�(����SDA = 0) */  
//  }

//	/* ����I2C����ֹͣ�ź� */
//	i2c_Stop();
//	return 0;	/* ִ�гɹ� */

//cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
//	/* ����I2C����ֹͣ�ź� */
//	i2c_Stop();
//	return 1;
//}

///**
//  * @brief   ʹ��IICд������
//  * @param   
//  * 	@arg ClientAddr:���豸��ַ
//  *		@arg pBuffer:������ָ��
//  *     @arg NumByteToWrite:д���ֽ���
//  * @retval  ��
//  */
//uint32_t I2C_WriteBytes(uint8_t ClientAddr,uint8_t* pBuffer,  uint8_t NumByteToWrite)
//{
//	uint16_t m;	

//  /*����0������ֹͣ�źţ������ڲ�д������*/
//  i2c_Stop();
//  
//  /* ͨ���������Ӧ��ķ�ʽ���ж��ڲ�д�����Ƿ����, һ��С�� 10ms 			
//    CLKƵ��Ϊ200KHzʱ����ѯ����Ϊ30������
//  */
//  for (m = 0; m < 1000; m++)
//  {				
//    /* ��1��������I2C���������ź� */
//    i2c_Start();
//    
//    /* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
//    i2c_SendByte(ClientAddr | I2C_DIR_WR);	/* �˴���дָ�� */
//    
//    /* ��3��������һ��ʱ�ӣ��ж������Ƿ���ȷӦ�� */
//    if (i2c_WaitAck() == 0)
//    {
//      break;
//    }
//  }
//  if (m  == 1000)
//  {
//    goto cmd_fail;	/* EEPROM����д��ʱ */
//  }	
//	
//  while(NumByteToWrite--)
//  {
//  /* ��4������ʼд������ */
//  i2c_SendByte(*pBuffer);

//  /* ��5�������ACK */
//  if (i2c_WaitAck() != 0)
//  {
//    goto cmd_fail;	/* ������Ӧ�� */
//  }
//  
//      pBuffer++;	/* ��ַ��1 */		
//  }
//	
//	/* ����ִ�гɹ�������I2C����ֹͣ�ź� */
//	i2c_Stop();
//	return 0;

//cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
//	/* ����I2C����ֹͣ�ź� */
//	i2c_Stop();
//	return 1;
//}

///**
//  * @brief   д���ݵ�MPU6050�Ĵ���
//  * @param   reg_add:�Ĵ�����ַ
//	* @param reg_data:Ҫд�������
//  * @retval  
//  */
//uint32_t Sensor_write(uint8_t reg_add,uint8_t reg_dat)
//{ 
//	/*����0������ֹͣ�źţ������ڲ�д������*/
//	i2c_Stop();
//	/* ��1��������I2C���������ź� */
//	i2c_Start();
//	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
//	i2c_SendByte(MPU6050_ADDR | I2C_DIR_WR);	/* �˴���дָ�� */
//	/* ��3��������һ��ʱ�ӣ��ж������Ƿ���ȷӦ�� */
//	if (i2c_WaitAck() != 0)
//	{
//	  goto cmd_fail;	/* EEPROM����д��ʱ */
//	}
//	/* ��4������ʼд��Ĵ�����ַ */
//	i2c_SendByte(reg_add);

//	/* ��5�������ACK */
//	if (i2c_WaitAck() != 0)
//	{
//		goto cmd_fail;	/* ������Ӧ�� */
//	}
//	/* ��5������ʼд������ */
//	i2c_SendByte(reg_dat);

//	/* ��5�������ACK */
//	if (i2c_WaitAck() != 0)
//	{
//		goto cmd_fail;	/* ������Ӧ�� */
//	}	
//	
//	/* ����ִ�гɹ�������I2C����ֹͣ�ź� */
//	i2c_Stop();
//	return 0;

//cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
//	/* ����I2C����ֹͣ�ź� */
//	i2c_Stop();
//	return 1;
//}

///**
//  * @brief   д���ݵ�MPU6050�Ĵ���
//  * @param   reg_add:�Ĵ�����ַ
//	* @param reg_data:Ҫд�������
//  * @retval  /
//  */
//uint32_t Sensor_Read(uint8_t reg_add,unsigned char* Read,uint8_t num)
//{ 
//	/* ��0������ֹͣ�źţ������ڲ�д������*/
//	i2c_Stop();
//	/* ��1��������I2C���������ź� */
//	i2c_Start();
//	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
//	i2c_SendByte(MPU6050_ADDR | I2C_DIR_WR);	/* �˴���дָ�� */
//	/* ��3��������һ��ʱ�ӣ��ж������Ƿ���ȷӦ�� */
//	if (i2c_WaitAck() != 0)
//	{
//	  goto cmd_fail;	/* EEPROM����д��ʱ */
//	}
//	/* ��4������ʼд��Ĵ�����ַ */
//	i2c_SendByte(reg_add);
//	/* ��5�������ACK */
//	if (i2c_WaitAck() != 0)
//	{
//		goto cmd_fail;	/* ������Ӧ�� */
//	}
//    /* ����I2C����ֹͣ�ź� */
//    i2c_Stop();
//	
//    /* ��6��������I2C���������ź� */
//	i2c_Start();
//	
//	/* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
//	i2c_SendByte(MPU6050_ADDR | I2C_DIR_RD);	/* �˴��Ƕ�ָ�� */

//	/* ��8�������ACK */
//	if (i2c_WaitAck() != 0)
//	{
//		goto cmd_fail;	/* ������Ӧ�� */
//	}
//	while(num) 
//   {

//		*Read = i2c_ReadByte();
//    
//		/* ��ָ������ */
//		Read++; 
//      
//		if(num == 1)
//		{
//			i2c_NAck();	/* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
//		}
//		else
//		{
//			i2c_Ack();	/* �м��ֽڶ����CPU����ACK�ź�(����SDA = 0) */  
//		}				
//		/*�������Լ� */
//		num--;
//  }

//	/* ����I2C����ֹͣ�ź� */
//	i2c_Stop();
//	return 0;	/* ִ�гɹ� */

//cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
//	/* ����I2C����ֹͣ�ź� */
//	i2c_Stop();
//	return 1;
//}


///*********************************************END OF FILE**********************/



// //MPU IIC ��ʱ����
//void MPU_IIC_Delay(void)
//{
//	//delay_us(2);
//	CPU_TS_Tmr_Delay_US(1);
//}


////����IIC��ʼ�ź�
//void MPU_IIC_Start(void)
//{
////	MPU_SDA_OUT();     //sda�����
////	MPU_IIC_SDA=1;	  	  
////	I2C_SCL_1=1;
////	MPU_IIC_Delay();
//// 	MPU_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
////	MPU_IIC_Delay();
////	MPU_IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
//	I2C_SDA_OUT();     //sda�����
//	I2C_SDA_1();	  	  
//	I2C_SCL_1();
//	MPU_IIC_Delay();
//	I2C_SDA_0();//START:when CLK is high,DATA change form high to low 
//	MPU_IIC_Delay();
//	I2C_SCL_0();//ǯסI2C���ߣ�׼�����ͻ�������� 
//}	  
////����IICֹͣ�ź�
//void MPU_IIC_Stop(void)
//{
//	I2C_SDA_OUT();//sda�����
//	I2C_SCL_0();
//	I2C_SDA_0();//STOP:when CLK is high DATA change form low to high
// 	MPU_IIC_Delay();
//	I2C_SCL_1(); 
//	I2C_SDA_1();//����I2C���߽����ź�
//	MPU_IIC_Delay();							   	
//}
////�ȴ�Ӧ���źŵ���
////����ֵ��1������Ӧ��ʧ��
////        0������Ӧ��ɹ�
//uint8_t MPU_IIC_Wait_Ack(void)
//{
//	uint8_t ucErrTime=0;
//	I2C_SDA_IN();      //SDA����Ϊ����  
//	I2C_SDA_1();MPU_IIC_Delay();	   
//	I2C_SCL_1();MPU_IIC_Delay();	 
//	while(I2C_SDA_READ())
//	{
//		ucErrTime++;
//		if(ucErrTime>250)
//		{
//			MPU_IIC_Stop();
//			return 1;
//		}
//	}
//	I2C_SCL_0();//ʱ�����0 	   
//	return 0;  
//} 
////����ACKӦ��
//void MPU_IIC_Ack(void)
//{
////	MPU_IIC_SCL=0;
////	MPU_SDA_OUT();
////	MPU_IIC_SDA=0;
////	MPU_IIC_Delay();
////	MPU_IIC_SCL=1;
////	MPU_IIC_Delay();
////	MPU_IIC_SCL=0;
//	
//	I2C_SCL_0();
//	I2C_SDA_OUT();
//	I2C_SDA_0();
//	MPU_IIC_Delay();
//	I2C_SCL_1();
//	MPU_IIC_Delay();
//	I2C_SCL_0();
//	
//}
////������ACKӦ��		    
//void MPU_IIC_NAck(void)
//{
//	I2C_SCL_0();
//	I2C_SDA_OUT();
//	I2C_SDA_1();
//	MPU_IIC_Delay();
//	I2C_SCL_1();
//	MPU_IIC_Delay();
//	I2C_SCL_0();
//}					 				     
////IIC����һ���ֽ�
////���شӻ�����Ӧ��
////1����Ӧ��
////0����Ӧ��			  
//void MPU_IIC_Send_Byte(uint8_t txd)
//{                        
//	uint8_t i;
//	I2C_SDA_OUT(); 	    
//	I2C_SCL_0();//����ʱ�ӿ�ʼ���ݴ���
//	/* �ȷ����ֽڵĸ�λbit7 */
//	for (i = 0; i < 8; i++)
//	{		
//		if (txd & 0x80)
//		{
//			I2C_SDA_1();
//		}
//		else
//		{
//			I2C_SDA_0();
//		}
//		i2c_Delay();
//		I2C_SCL_1();
//		i2c_Delay();	
//		I2C_SCL_0();
//		if (i == 7)
//		{
//			 I2C_SDA_1(); // �ͷ�����
//		}
//		txd <<= 1;	/* ����һ��bit */
//		i2c_Delay();
//	}
//} 	  


////��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
//uint8_t MPU_IIC_Read_Byte(unsigned char ack)
//{
//		unsigned char i,receive=0;
//	I2C_SDA_IN();//SDA����Ϊ����
//    for(i=0;i<8;i++ )
//	{
//        I2C_SCL_0(); 
//        MPU_IIC_Delay();
//		I2C_SCL_1();
//        receive<<=1;
//        if(I2C_SDA_READ())receive++;   
//		MPU_IIC_Delay(); 
//    }					 
//    if (!ack)
//        MPU_IIC_NAck();//����nACK
//    else
//        MPU_IIC_Ack(); //����ACK   
//    return receive;

//}

////IICдһ���ֽ� 
////reg:�Ĵ�����ַ
////data:����
////����ֵ:0,����
////    ����,�������
//uint8_t MPU_Write_Byte(uint8_t reg,uint8_t data) 				 
//{ 
//	#define MPU_ADDR				0X68
//    MPU_IIC_Start(); 
//	MPU_IIC_Send_Byte((MPU_ADDR<<1)|0);//����������ַ+д����	
//	if(MPU_IIC_Wait_Ack())	//�ȴ�Ӧ��
//	{
//		MPU_IIC_Stop();		 
//		return 1;		
//	}
//    MPU_IIC_Send_Byte(reg);	//д�Ĵ�����ַ
//    MPU_IIC_Wait_Ack();		//�ȴ�Ӧ�� 
//	MPU_IIC_Send_Byte(data);//��������
//	if(MPU_IIC_Wait_Ack())	//�ȴ�ACK
//	{
//		MPU_IIC_Stop();	 
//		return 1;		 
//	}		 
//    MPU_IIC_Stop();	 
//	return 0;
//}
////IIC��һ���ֽ� 
////reg:�Ĵ�����ַ 
////����ֵ:����������
//uint8_t MPU_Read_Byte(uint8_t reg)
//{
//	#define MPU_ADDR				0X68
//	uint8_t res;
//    MPU_IIC_Start(); 
//	MPU_IIC_Send_Byte((MPU_ADDR<<1)|0);//����������ַ+д����	
//	MPU_IIC_Wait_Ack();		//�ȴ�Ӧ�� 
//    MPU_IIC_Send_Byte(reg);	//д�Ĵ�����ַ
//    MPU_IIC_Wait_Ack();		//�ȴ�Ӧ��
//    MPU_IIC_Start();
//	MPU_IIC_Send_Byte((MPU_ADDR<<1)|1);//����������ַ+������	
//    MPU_IIC_Wait_Ack();		//�ȴ�Ӧ�� 
//	res=MPU_IIC_Read_Byte(0);//��ȡ����,����nACK 
//    MPU_IIC_Stop();			//����һ��ֹͣ���� 
//	return res;		
//}
////IIC����д
////addr:������ַ 
////reg:�Ĵ�����ַ
////len:д�볤��
////buf:������
////����ֵ:0,����
////    ����,�������
//uint8_t MPU_Write_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
//{
//	uint8_t i; 
//    MPU_IIC_Start(); 
//	MPU_IIC_Send_Byte((addr<<1)|0);//����������ַ+д����	
//	if(MPU_IIC_Wait_Ack())	//�ȴ�Ӧ��
//	{
//		MPU_IIC_Stop();		 
//		return 1;		
//	}
//    MPU_IIC_Send_Byte(reg);	//д�Ĵ�����ַ
//    MPU_IIC_Wait_Ack();		//�ȴ�Ӧ��
//	for(i=0;i<len;i++)
//	{
//		MPU_IIC_Send_Byte(buf[i]);	//��������
//		if(MPU_IIC_Wait_Ack())		//�ȴ�ACK
//		{
//			MPU_IIC_Stop();	 
//			return 1;		 
//		}		
//	}    
//    MPU_IIC_Stop();	 
//	return 0;	
//} 
////IIC������
////addr:������ַ
////reg:Ҫ��ȡ�ļĴ�����ַ
////len:Ҫ��ȡ�ĳ���
////buf:��ȡ�������ݴ洢��
////����ֵ:0,����			
////    ����,������� 
//uint8_t MPU_Read_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
//{ 
// 	MPU_IIC_Start(); 
//	MPU_IIC_Send_Byte((addr<<1)|0);//����������ַ+д����	
//	if(MPU_IIC_Wait_Ack())	//�ȴ�Ӧ��
//	{
//		MPU_IIC_Stop();		 
//		return 1;		
//	}
//    MPU_IIC_Send_Byte(reg);	//д�Ĵ�����ַ
//    MPU_IIC_Wait_Ack();		//�ȴ�Ӧ��
//    MPU_IIC_Start();
//	MPU_IIC_Send_Byte((addr<<1)|1);//����������ַ+������	
//    MPU_IIC_Wait_Ack();		//�ȴ�Ӧ�� 
//	while(len)
//	{
//		if(len==1)*buf=MPU_IIC_Read_Byte(0);//������,����nACK 
//		else *buf=MPU_IIC_Read_Byte(1);		//������,����ACK  
//		len--;
//		buf++; 
//	}    
//    MPU_IIC_Stop();	//����һ��ֹͣ���� 
//	return 0;	
//}

/*******************/

static void i2c_Delay(void)
{
	/*��
	 	�����ʱ����ͨ���߼������ǲ��Եõ��ġ�
    ����������CPU��Ƶ216MHz ��MDK���뻷����1���Ż�
      
		ѭ������Ϊ200ʱ��SCLƵ�� = 100KHz  
	 	ѭ������Ϊ30ʱ��SCLƵ�� = 400KHz 
  */

	CPU_TS_Tmr_Delay_US(4);
//		CPU_TS_Tmr_Delay_US(10);
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_Start
*	����˵��: CPU����I2C���������ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/


void i2c_Start(void)
{
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C���������ź� */
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
*	�� �� ��: i2c_Start
*	����˵��: CPU����I2C����ֹͣ�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/

void i2c_Stop(void)
{
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź� */
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
*	�� �� ��: i2c_SendByte
*	����˵��: CPU��I2C�����豸����8bit����
*	��    �Σ�_ucByte �� �ȴ����͵��ֽ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_SendByte(uint8_t _ucByte)
{
	uint8_t i;

	/* �ȷ����ֽڵĸ�λbit7 */
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
			 I2C_SDA_1(); // �ͷ�����
		}
		_ucByte <<= 1;	/* ����һ��bit */
		i2c_Delay();
	}
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_WaitAck
*	����˵��: CPU����һ��ʱ�ӣ�����ȡ������ACKӦ���ź�
*	��    �Σ���
*	�� �� ֵ: ����0��ʾ��ȷӦ��1��ʾ��������Ӧ
*********************************************************************************************************
*/
uint8_t i2c_WaitAck(void)
{
	uint8_t re;
	I2C_SDA_IN();//��������ģʽ
	I2C_SDA_1();	/* CPU�ͷ�SDA���� */
	i2c_Delay();
	I2C_SCL_1();	/* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
	i2c_Delay();
	if (I2C_SDA_READ())	/* CPU��ȡSDA����״̬ */
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
*	�� �� ��: i2c_Ack
*	����˵��: CPU����һ��ACK�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_Ack(void)
{	
	I2C_SCL_0();
	I2C_SDA_OUT();
	I2C_SDA_0();/* CPU����SDA = 0 */
	i2c_Delay();
	I2C_SCL_1();/* CPU����1��ʱ�� */
	i2c_Delay();
	I2C_SCL_0();
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_NAck
*	����˵��: CPU����1��NACK�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_NAck(void)
{
	I2C_SDA_1();	/* CPU����SDA = 1 */
	i2c_Delay();
	I2C_SCL_1();	/* CPU����1��ʱ�� */
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();	
}
/*
*********************************************************************************************************
*	�� �� ��: i2c_ReadByte
*	����˵��: CPU��I2C�����豸��ȡ8bit����
*	��    �Σ���
*	�� �� ֵ: ����������
*********************************************************************************************************
*/
uint8_t i2c_ReadByte(unsigned char ack)
{
	uint8_t i;
	uint8_t value;

	/* ������1��bitΪ���ݵ�bit7 */
	value = 0;
	I2C_SDA_IN();//��������ģʽ
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

/*********************************************END OF FILE**********************/

/**
  * @brief   ʹ��IICд������
  * @param   
  * 	@arg ClientAddr:���豸��ַ
  *		@arg pBuffer:������ָ��
  *     @arg NumByteToWrite:д���ֽ���
  * @retval  ��
  */
uint8_t I2C_WriteBytes(uint8_t reg_add,uint8_t data) 				 
{ 
	/* ��1��������I2C���������ź� */
	i2c_Start(); 
	 /* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	i2c_SendByte((MPU_ADDR<<1)|I2C_DIR_WR);
	 /* ��3��������һ��ʱ�ӣ��ж������Ƿ���ȷӦ�� */
	if(i2c_WaitAck())	
	{
		i2c_Stop();		 
		return 1;		
	}
	 /* ��4������ʼд������ */
	i2c_SendByte(reg_add);
	 /* ��5�������ACK */
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
  * @brief   ʹ��IIC��ȡ����
  * @param   
  * 	@arg ClientAddr:���豸��ַ
  *		@arg pBuffer:����ɴӻ���ȡ�����ݵĻ�����ָ��
  *		@arg NumByteToRead:��ȡ�����ݳ���
  * @retval  ��
  */
uint8_t I2C_ReadBytes(uint8_t reg_add)
{
	uint8_t res;
	/* ��1��������I2C���������ź� */
	i2c_Start(); 
	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	i2c_SendByte((MPU_ADDR<<1)|I2C_DIR_WR);
	/* ��3�����ȴ�ACK */
	i2c_WaitAck();		
	/* ����д�Ĵ�����ַ */
	i2c_SendByte(reg_add);	
	/* �ȴ�ACK */
	i2c_WaitAck();		
	i2c_Start();
	i2c_SendByte((MPU_ADDR<<1)|I2C_DIR_RD);
	i2c_WaitAck();		
	res=i2c_ReadByte(0);
	/* ����I2C����ֹͣ�ź� */
	i2c_Stop();			
	return res;		
}

/**
  * @brief  д���ֽ�
  * @param   
  */
uint8_t Sensor_write_DMP(uint8_t slave_addr,uint8_t reg_add,uint8_t num,unsigned char* write)
{
	uint8_t i; 
	 /* ��1��������I2C���������ź� */
	i2c_Start(); 
	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	i2c_SendByte((slave_addr<<1)|I2C_DIR_WR);//����������ַ+д����	
	/* ��3��������һ��ʱ�ӣ��ж������Ƿ���ȷӦ�� */
	if(i2c_WaitAck())	
	{
		/* ����I2C����ֹͣ�ź� */
		i2c_Stop();		 
		return 1;		
	}

	i2c_SendByte(reg_add);	
	/*�ȴ�ACK */
	i2c_WaitAck();		
	for(i=0;i<num;i++)
	{
		/* ��4������ʼд������ */
		i2c_SendByte(write[i]);	
		/* ��5�������ACK */
		if(i2c_WaitAck())	
		{
			/* ����I2C����ֹͣ�ź� */
			i2c_Stop();	 
			return 1;		 
		}		
	}    
	/* ����I2C����ֹͣ�ź� */
	i2c_Stop();	 
	return 0;	
} 

/**
  * @brief   ��ȡ6050�Ĵ���
  */
uint8_t Sensor_Read_DMP(uint8_t slave_addr,uint8_t reg_add,uint8_t num,unsigned char* Read)
{ 
	/* ��1��������I2C���������ź� */
	i2c_Start(); 
	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	i2c_SendByte((slave_addr<<1)|I2C_DIR_WR);//����������ַ+д����	
	/* ��3��������һ��ʱ�ӣ��ж������Ƿ���ȷӦ�� */
	if(i2c_WaitAck())	//�ȴ�Ӧ��
	{
		i2c_Stop();		 
		return 1;		
	}
	/* ��4������ʼд��Ĵ�����ַ */
	i2c_SendByte(reg_add);	
	/* ��5�����ȴ�ACK */
	i2c_WaitAck();		//�ȴ�Ӧ��
  /* ��6��������I2C���������ź� */
	i2c_Start();
	/*  ����I2C���������ź�  */
	i2c_SendByte((slave_addr<<1)|I2C_DIR_RD);
	i2c_WaitAck();	
	while(num)
	{
		/* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
		if(num==1)*Read=i2c_ReadByte(0);
		/* �м��ֽڶ����CPU����ACK�ź�(����SDA = 0) */  
		else *Read=i2c_ReadByte(1);		
		/*�������Լ� */
		num --;
		/* ��ָ������ */
		Read++; 
	}    
	/* ����I2C����ֹͣ�ź� */
	i2c_Stop();	
	return 0;	
}

















