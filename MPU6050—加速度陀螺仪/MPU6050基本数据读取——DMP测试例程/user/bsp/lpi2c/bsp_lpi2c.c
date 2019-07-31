/**
  ******************************************************************************
  * @file    bsp_lpi2c.c
  * @author  fire
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   EEPROM��ר��iic����
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

#include "fsl_iomuxc.h"
#include "fsl_gpio.h"
#include "fsl_lpi2c.h"

#include "./delay/core_delay.h"   

#include "./lpi2c/bsp_lpi2c.h"
#include "./mpu6050/mpu6050.h"
#include "./pad_config.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* I2C��SCL��SDA����ʹ��ͬ����PAD���� */
#define I2C_PAD_CONFIG_DATA         (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_1_MEDIUM_100MHz| \
                                        ODE_1_OPEN_DRAIN_ENABLED| \
                                        PKE_1_PULL_KEEPER_ENABLED| \
                                        PUE_0_KEEPER_SELECTED| \
                                        PUS_3_22K_OHM_PULL_UP| \
                                        HYS_0_HYSTERESIS_DISABLED)   
    /* ����˵�� : */
    /* ת������: ת��������
        ����ǿ��: R0/6 
        �������� : medium(100MHz)
        ��©����: ʹ�� 
        ��/����������: ʹ��
        ��/������ѡ��: ������
        ����/����ѡ��: 22Kŷķ����(ѡ���˱�������������Ч)
        �ͻ�������: ��ֹ */ 
        
/* ���ڳ�ʱ������ʱ */
#define EEPROM_DELAY_US(x)  CPU_TS_Tmr_Delay_US(x)  
        
        
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void I2C_IOMUXC_MUX_Config(void);
static void I2C_IOMUXC_PAD_Config(void);
static void I2C_ModeInit(void);

//static  uint32_t I2C_Timeout_Callback(uint8_t errorCode);

/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Code
 ******************************************************************************/


/**
* @brief  ��ʼ��EEPROM���IOMUXC��MUX��������
* @param  ��
* @retval ��
*/
static void I2C_IOMUXC_MUX_Config(void)
{
  /* SCL��SDA���ţ���Ҫʹ��SION�Խ������� */
  IOMUXC_SetPinMux(SCL_IOMUXC, 1U);                                   
  IOMUXC_SetPinMux(SDA_IOMUXC, 1U);    
}

/**
* @brief  ��ʼ��EEPROM���IOMUXC��PAD��������
* @param  ��
* @retval ��
*/
static void I2C_IOMUXC_PAD_Config(void)
{
  /* SCL��SDA���� */
  IOMUXC_SetPinConfig(SCL_IOMUXC, I2C_PAD_CONFIG_DATA);                               
  IOMUXC_SetPinConfig(SDA_IOMUXC, I2C_PAD_CONFIG_DATA);   
}


/**
* @brief  ��ʼ��EEPROMʹ�õ�I2C����
* @param  ��
* @retval ��
*/  
static void I2C_ModeInit(void)
{
  lpi2c_master_config_t masterConfig; 
  
  /* ����ʱ�� LPI2C */
  CLOCK_SetMux(kCLOCK_Lpi2cMux, LPI2C_CLOCK_SOURCE_SELECT);
  CLOCK_SetDiv(kCLOCK_Lpi2cDiv, LPI2C_CLOCK_SOURCE_DIVIDER);
  
  /*��masterConfig��ֵΪ����Ĭ������*/
  /*
   * masterConfig.debugEnable = false;
   * masterConfig.ignoreAck = false;
   * masterConfig.pinConfig = kLPI2C_2PinOpenDrain;
   * masterConfig.baudRate_Hz = 100000U;
   * masterConfig.busIdleTimeout_ns = 0;
   * masterConfig.pinLowTimeout_ns = 0;
   * masterConfig.sdaGlitchFilterWidth_ns = 0;
   * masterConfig.sclGlitchFilterWidth_ns = 0;
   */
  LPI2C_MasterGetDefaultConfig(&masterConfig);

  /* ��Ĭ�ϲ����ʸ�ΪI2C_BAUDRATE */
  masterConfig.baudRate_Hz = I2C_BAUDRATE;

  /*  ʹ���������ó�ʼ�� LPI2C ���� */
  LPI2C_MasterInit(I2C_MASTER, &masterConfig, LPI2C_MASTER_CLOCK_FREQUENCY);

}


//�ر������ж�(���ǲ�����fault��NMI�ж�)
__asm void INTX_DISABLE(void)
{
	CPSID   I
	BX      LR	  
}
//���������ж�
__asm void INTX_ENABLE(void)
{
	CPSIE   I
	BX      LR  
}

// 

/**
  * @brief   ��ȡ����
  * @param   reg_add:�Ĵ�����ַ
	* @param reg_data:Ҫд�������
  * @retval  
  */
uint32_t Sensor_Read_hardware_dmp(uint8_t slave_addr,uint8_t reg_add,uint8_t length,uint8_t *Read)
{ 
	lpi2c_master_transfer_t masterXfer = {0};
  status_t reVal = kStatus_Fail;

 // EEPROM_DEBUG_FUNC();
	INTX_DISABLE();

  /* subAddress = ReadAddr, data = pBuffer �Դӻ�������
    ��ʼ�ź�start + �豸��ַslaveaddress(w д����) + �ӵ�ַsubAddress + 
    �ظ���ʼ�ź�repeated start + �豸��ַslaveaddress(r ������) + 
    ���ջ�������rx data buffer + ֹͣ�ź�stop */
  masterXfer.slaveAddress = slave_addr;//MPU6050_DEFAULT_ADDRESS;
  masterXfer.direction = kLPI2C_Read;
  masterXfer.subaddress = (uint32_t)reg_add;//Ҫ�����ݵļĴ�����ַ
  masterXfer.subaddressSize = 1;
  masterXfer.data = Read;						//���ݻ�����
  masterXfer.dataSize = length;			//��ȡ���ݵĸ���
  masterXfer.flags = kLPI2C_TransferDefaultFlag;
  reVal = LPI2C_MasterTransferBlocking(I2C_MASTER, &masterXfer);//I2C_MASTER LPI2C1_BASE
	//INTX_ENABLE();
	if(reVal==kStatus_Fail)
	{
		return 1;
	}
	else 
	{
		return 0;	
	}

}

/**
  * @brief   д���ݵ�MPU6050�Ĵ���
  * @param   reg_add:�Ĵ�����ַ
	* @param reg_data:Ҫд�������
  * @retval  
  */

//uint32_t I2C_WriteBytes(uint8_t ClientAddr,uint8_t* pBuffer,  uint8_t NumByteToWrite)

uint32_t Sensor_write_hardware_dmp(uint8_t slave_addr,uint8_t reg_add,uint8_t length,uint8_t *reg_dat)
{ 
  lpi2c_master_transfer_t masterXfer = {0};
  status_t reVal = kStatus_Fail;
  
  //EEPROM_DEBUG_FUNC();
	INTX_DISABLE();
 
  masterXfer.slaveAddress = slave_addr;//IIC�ӻ���ַ MPU6050_ADDRESS MPU6050_DEFAULT_ADDRESS
  masterXfer.direction = kLPI2C_Write;			//д������
  masterXfer.subaddress = reg_add;				//��ȡ�Ĵ�����ַ
  masterXfer.subaddressSize = 1;						//����Ĭ��һ���ֽ�
	masterXfer.data = &reg_dat;								//Ҫд�������
  masterXfer.dataSize = length;									//����Ĭ��һ���ֽ�
  masterXfer.flags = kLPI2C_TransferDefaultFlag;
  
  reVal = LPI2C_MasterTransferBlocking(I2C_MASTER, &masterXfer);//LPI2C1
	//INTX_ENABLE();
	if(reVal==kStatus_Fail)
	{
		return 1;
	}
	else 
	{
		return 0;	
	}

}

////�ر������ж�(���ǲ�����fault��NMI�ж�)
//__asm void INTX_DISABLE(void)
//{
//	CPSID   I
//	BX      LR	  
//}

////���������ж�
//__asm void INTX_ENABLE(void)
//{
//	CPSIE   I
//	BX      LR  
//}

//uint32_t MPU_Write_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
//{
//    uint32_t status=0;
//    lpi2c_master_transfer_t masterXfer = {0};
//    INTX_DISABLE();
//    //����I2C xfer�ṹ��
//    masterXfer.slaveAddress=addr;             //�豸��ַ
//    masterXfer.direction=kLPI2C_Write;        //д������
//    masterXfer.subaddress=(uint32_t)reg;           //Ҫ��ȡ�ļĴ�����ַ
//    masterXfer.subaddressSize=1;              //��ַ����һ���ֽ�
//    masterXfer.data=buf;                      //Ҫд�������
//    masterXfer.dataSize=len;                  //д�����ݳ���1���ֽ�
//    masterXfer.flags=kLPI2C_TransferDefaultFlag;

//    if(LPI2C_MasterTransferBlocking(LPI2C1,&masterXfer)==kStatus_Fail)status=1;
//    INTX_ENABLE();
//    return status;
//} 

//uint32_t MPU_Read_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
//{ 
//    uint32_t status=0;
//    lpi2c_master_transfer_t masterXfer = {0};
//    INTX_DISABLE();
//    //����I2C xfer�ṹ��
//    masterXfer.slaveAddress=addr;             //�豸��ַ
//    masterXfer.direction=kLPI2C_Read;         //������
//    masterXfer.subaddress=(uint32_t)reg;           //Ҫ��ȡ�ļĴ�����ַ
//    masterXfer.subaddressSize=1;              //�ӵ�ַ����
//    masterXfer.data=buf;                     //���ݻ�����
//    masterXfer.dataSize=len;                  //Ҫ��ȡ�����ݳ���
//    masterXfer.flags=kLPI2C_TransferDefaultFlag;
//    if(LPI2C_MasterTransferBlocking(LPI2C1,&masterXfer)==kStatus_Fail)status=1;

//    INTX_ENABLE();
//    return status;      
//}





/**
  * @brief   ��ȡ����
  * @param   reg_add:�Ĵ�����ַ
	* @param reg_data:Ҫд�������
  * @retval  
  */
uint32_t Sensor_Read_hardware(uint8_t reg_add,unsigned char* Read,uint8_t num)
{ 
	lpi2c_master_transfer_t masterXfer = {0};
  status_t reVal = kStatus_Fail;

  EEPROM_DEBUG_FUNC();

  /* subAddress = ReadAddr, data = pBuffer �Դӻ�������
    ��ʼ�ź�start + �豸��ַslaveaddress(w д����) + �ӵ�ַsubAddress + 
    �ظ���ʼ�ź�repeated start + �豸��ַslaveaddress(r ������) + 
    ���ջ�������rx data buffer + ֹͣ�ź�stop */
  masterXfer.slaveAddress = MPU6050_DEFAULT_ADDRESS;
  masterXfer.direction = kLPI2C_Read;
  masterXfer.subaddress = (uint32_t)reg_add;//Ҫ�����ݵļĴ�����ַ
  masterXfer.subaddressSize = 1;
  masterXfer.data = Read;						//���ݻ�����
  masterXfer.dataSize = num;			//��ȡ���ݵĸ���
  masterXfer.flags = kLPI2C_TransferDefaultFlag;
  reVal = LPI2C_MasterTransferBlocking(I2C_MASTER, &masterXfer);//I2C_MASTER
  if (reVal != kStatus_Success)
  {
      return 1;
  }
  
  return 0;
}

/**
  * @brief   д���ݵ�MPU6050�Ĵ���
  * @param   reg_add:�Ĵ�����ַ
	* @param reg_data:Ҫд�������
  * @retval  
  */
uint32_t Sensor_write_hardware(uint8_t reg_add,uint8_t reg_dat)
{ 
  lpi2c_master_transfer_t masterXfer = {0};
  status_t reVal = kStatus_Fail;
  
  EEPROM_DEBUG_FUNC();
 
  masterXfer.slaveAddress = MPU6050_DEFAULT_ADDRESS;//IIC�ӻ���ַ MPU6050_ADDRESS
  masterXfer.direction = kLPI2C_Write;			//д������
  masterXfer.subaddress = reg_add;				//��ȡ�Ĵ�����ַ
  masterXfer.subaddressSize = 1;						//����Ĭ��һ���ֽ�
  masterXfer.data = &reg_dat;								//Ҫд�������
  masterXfer.dataSize = 1;									//����Ĭ��һ���ֽ�
  masterXfer.flags = kLPI2C_TransferDefaultFlag;
  
  reVal = LPI2C_MasterTransferBlocking(I2C_MASTER, &masterXfer);
  
  if (reVal != kStatus_Success)
  {
      return 1;
  }
  
  return 0;

}

/**
* @brief  Ӳ��I2C��ʼ��
* @param  ��
* @retval ��
*/
void I2C_Init_Hard(void)
{
  //EEPROM_DEBUG_FUNC();

  /* ��ʼ��������IOMUXC��� */
  I2C_IOMUXC_MUX_Config();
  I2C_IOMUXC_PAD_Config();

  /* ��ʼ��I2C���蹤��ģʽ */
  I2C_ModeInit(); 
}

/*********************************************END OF FILE**********************/
