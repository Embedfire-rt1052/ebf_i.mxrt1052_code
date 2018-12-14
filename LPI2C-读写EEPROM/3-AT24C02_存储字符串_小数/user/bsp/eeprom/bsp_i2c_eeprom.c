/**
  ******************************************************************************
  * @file    bsp_i2c_eeprom.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
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

#include "./eeprom/bsp_i2c_eeprom.h"
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
static void I2C_EEPROM_IOMUXC_MUX_Config(void);
static void I2C_EEPROM_IOMUXC_PAD_Config(void);
static void EEPROM_I2C_ModeInit(void);

static  uint32_t I2C_Timeout_Callback(uint8_t errorCode);

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
static void I2C_EEPROM_IOMUXC_MUX_Config(void)
{
  /* SCL��SDA���ţ���Ҫʹ��SION�Խ������� */
  IOMUXC_SetPinMux(EEPROM_SCL_IOMUXC, 1U);                                   
  IOMUXC_SetPinMux(EEPROM_SDA_IOMUXC, 1U);    
}

/**
* @brief  ��ʼ��EEPROM���IOMUXC��PAD��������
* @param  ��
* @retval ��
*/
static void I2C_EEPROM_IOMUXC_PAD_Config(void)
{
  /* SCL��SDA���� */
  IOMUXC_SetPinConfig(EEPROM_SCL_IOMUXC, I2C_PAD_CONFIG_DATA);                               
  IOMUXC_SetPinConfig(EEPROM_SDA_IOMUXC, I2C_PAD_CONFIG_DATA);   
}


/**
* @brief  ��ʼ��EEPROMʹ�õ�I2C����
* @param  ��
* @retval ��
*/  
static void EEPROM_I2C_ModeInit(void)
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
  masterConfig.baudRate_Hz = EEPROM_I2C_BAUDRATE;

  /*  ʹ���������ó�ʼ�� LPI2C ���� */
  LPI2C_MasterInit(EEPROM_I2C_MASTER, &masterConfig, LPI2C_MASTER_CLOCK_FREQUENCY);

}


/**
  * @brief ��EEPROMд�����һҳ����
  * @note  ���ñ�������������I2C_EEPROM_WaitStandbyState���еȴ�
  * @param ClientAddr:EEPROM��I2C�豸��ַ(8λ��ַ)
  * @param WriteAddr:д��Ĵ洢��Ԫ�׵�ַ
  * @param pBuffer:������ָ��
  * @param NumByteToWrite:Ҫд���ֽ��������ɳ���EEPROM_PAGE_SIZE�����ֵ
  * @retval  1��������0����
  */
uint32_t I2C_EEPROM_Page_Write( uint8_t ClientAddr,
                                uint8_t WriteAddr, 
                                uint8_t* pBuffer,  
                                uint8_t NumByteToWrite)
{
  lpi2c_master_transfer_t masterXfer = {0};
  status_t reVal = kStatus_Fail;
  
  EEPROM_DEBUG_FUNC();
  
  if(NumByteToWrite>EEPROM_PAGE_SIZE)
  {
    EEPROM_ERROR("NumByteToWrite>EEPROM_PageSize\r\n");
    return 1;
  }

  /* subAddress = WriteAddr, data = pBuffer �������ӻ�
    ��ʼ�ź�start + �豸��ַslaveaddress(w д����) + 
    ���ͻ�������tx data buffer + ֹͣ�ź�stop */
  
  masterXfer.slaveAddress = (ClientAddr>>1);
  masterXfer.direction = kLPI2C_Write;
  masterXfer.subaddress = WriteAddr;
  masterXfer.subaddressSize = EEPROM_INER_ADDRESS_SIZE;
  masterXfer.data = pBuffer;
  masterXfer.dataSize = NumByteToWrite;
  masterXfer.flags = kLPI2C_TransferDefaultFlag;
  
  reVal = LPI2C_MasterTransferBlocking(EEPROM_I2C_MASTER, &masterXfer);
  
  if (reVal != kStatus_Success)
  {
      return 1;
  }
  
  return 0;

}


/**
  * @brief ��EEPROMд�벻����������
  * @param ClientAddr:EEPROM��I2C�豸��ַ(8λ��ַ)
  * @param WriteAddr:д��Ĵ洢��Ԫ�׵�ַ
  * @param pBuffer:������ָ��
  * @param NumByteToWrite:Ҫд���ֽ���
  * @retval  ��
  */
void I2C_EEPROM_Buffer_Write( uint8_t ClientAddr,
                              uint8_t WriteAddr, 
                              uint8_t* pBuffer,  
                              uint16_t NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;
  /* ����Ҫ������ֽ�������ʼֵΪNumByteToWrite*/
  uint8_t NumByteToWriteRest = NumByteToWrite;
  /* ��������������д���
    1.д����׵�ַ�Ƿ���� 
    2.���һ��д���Ƿ�պ�д��һҳ */
  Addr = WriteAddr % EEPROM_PAGE_SIZE;
  count = EEPROM_PAGE_SIZE - Addr;
  
  /* ��NumByteToWrite > count��
     ��һҳд��count���ֽڣ��������ֽ��ٽ��к�������
     ������ (NumByteToWriteRest = NumByteToWrite - count) ���������NumOfPage��NumOfSingle���д���
     ��NumByteToWrite < count��
     ������һҳ���ݣ�ֱ����NumByteToWriteRest = NumByteToWrite���NumOfPage��NumOfSingle���� */
  NumByteToWriteRest = (NumByteToWrite > count) ? (NumByteToWrite - count) : NumByteToWrite;
  
  /* Ҫ����д���ҳ����������ǰcount�ֽڣ�*/
  NumOfPage =  NumByteToWriteRest / EEPROM_PAGE_SIZE;
  /* ���һҳҪд����ֽ�����������ǰcount�ֽڣ�*/
  NumOfSingle = NumByteToWriteRest % EEPROM_PAGE_SIZE;
 
  /* NumByteToWrite > countʱ����Ҫ������һҳд��count���ֽ�
     NumByteToWrite < countʱ������д˲��� */
  if(count != 0 && NumByteToWrite > count)
  {  
    I2C_EEPROM_Page_Write(ClientAddr, WriteAddr, pBuffer, count);
    I2C_EEPROM_WaitStandbyState(ClientAddr);
    WriteAddr += count;
    pBuffer += count;
  }   
  
  /* ����������� */
  if(NumOfPage== 0 ) 
  {
    I2C_EEPROM_Page_Write(ClientAddr, WriteAddr, pBuffer, NumOfSingle);
    I2C_EEPROM_WaitStandbyState(ClientAddr);
  }
  else
  {   
    /* �������ݴ���һҳ */
    while(NumOfPage--)
    {
      I2C_EEPROM_Page_Write(ClientAddr, WriteAddr, pBuffer, EEPROM_PAGE_SIZE);
      I2C_EEPROM_WaitStandbyState(ClientAddr);
      WriteAddr +=  EEPROM_PAGE_SIZE;
      pBuffer += EEPROM_PAGE_SIZE;  
    }
    /* ���һҳ */
    if(NumOfSingle != 0)
    {
      I2C_EEPROM_Page_Write(ClientAddr, WriteAddr, pBuffer, NumOfSingle); 
      I2C_EEPROM_WaitStandbyState(ClientAddr);
    }
  }  
}

/**
  * @brief ��EEPROM�����ȡһ������ 
  * @param ClientAddr:EEPROM��I2C�豸��ַ(8λ��ַ)
  * @param ReadAddr:�������ݵ�EEPROM�ĵ�ַ 
  * @param pBuffer[out]:��Ŵ�EEPROM��ȡ�����ݵĻ�����ָ��
  * @param NumByteToWrite:Ҫ��EEPROM��ȡ���ֽ��� 
  * @retval  ��
  */
uint32_t I2C_EEPROM_BufferRead( uint8_t ClientAddr,
                                uint8_t ReadAddr, 
                                uint8_t* pBuffer, 
                                uint16_t NumByteToRead)
{
  lpi2c_master_transfer_t masterXfer = {0};
  status_t reVal = kStatus_Fail;

  EEPROM_DEBUG_FUNC();

  /* subAddress = ReadAddr, data = pBuffer �Դӻ�������
    ��ʼ�ź�start + �豸��ַslaveaddress(w д����) + �ӵ�ַsubAddress + 
    �ظ���ʼ�ź�repeated start + �豸��ַslaveaddress(r ������) + 
    ���ջ�������rx data buffer + ֹͣ�ź�stop */
  masterXfer.slaveAddress = (ClientAddr>>1);
  masterXfer.direction = kLPI2C_Read;
  masterXfer.subaddress = (uint32_t)ReadAddr;
  masterXfer.subaddressSize = EEPROM_INER_ADDRESS_SIZE;
  masterXfer.data = pBuffer;
  masterXfer.dataSize = NumByteToRead;
  masterXfer.flags = kLPI2C_TransferDefaultFlag;

  reVal = LPI2C_MasterTransferBlocking(EEPROM_I2C_MASTER, &masterXfer);

  if (reVal != kStatus_Success)
  {
      return 1;
  }
  
  return 0;
}

/**
  * @brief  IIC�ȴ���ʱ���ñ��������������Ϣ
  * @param  None.
  * @retval ����0xff����ʾIIC��ȡ����ʧ��
  */
static  uint32_t I2C_Timeout_Callback(uint8_t errorCode)
{
  /* �ڴ˴����д����� */
  EEPROM_ERROR("I2C �ȴ���ʱ!errorCode = %d",errorCode);
  
  return 0xFF;
}


/**
  * @brief  ���ڵȴ�EEPROM���ڲ�д��ʱ��
  *         ��I2C_EEPROM_Page_Write��������뱻����
  * @param  ClientAddr:�豸��ַ��8λ��ַ��
  * @retval �ȴ�����Ϊ0���ȴ���ʱΪ1
  */
uint8_t I2C_EEPROM_WaitStandbyState(uint8_t ClientAddr)      
{
  status_t lpi2c_status;
    /* �ȴ����� 40*I2CT_LONG_TIMEOUT us����Ϊ��ʱ */
  uint32_t delay_count = I2CT_LONG_TIMEOUT;   

  do
  {
    /* �����Ӧ���־���Ա���һ�μ�� */
    LPI2C_MasterClearStatusFlags(EEPROM_I2C_MASTER, kLPI2C_MasterNackDetectFlag);
    
    /* ��EEPROM����д�����Ѱַ�źţ��Լ���Ƿ���Ӧ */
    lpi2c_status = LPI2C_MasterStart(EEPROM_I2C_MASTER, (ClientAddr>>1), kLPI2C_Write);
    
    /* ����ȴ�����30us���Ż������Ӧ���־*/
    EEPROM_DELAY_US(40);
    
    /* ���LPI2C MSR�Ĵ�����NDF��־������ȷ��delay_countû����0������0��Ϊ��ʱ���˳��� */
  }while(EEPROM_I2C_MASTER->MSR & kLPI2C_MasterNackDetectFlag && delay_count-- );
  
  /* �����Ӧ���־����ֹ��һ��ͨѶ���� */
  LPI2C_MasterClearStatusFlags(EEPROM_I2C_MASTER, kLPI2C_MasterNackDetectFlag);

  /* ����ֹͣ�źţ���ֹ�´�ͨѶ���� */  
  lpi2c_status = LPI2C_MasterStop(EEPROM_I2C_MASTER);
  /* ����ȴ�����10us��ȷ��ֹͣ�źŷ������*/
  EEPROM_DELAY_US(10);

  /* ����ʧ�ܻ�ǰ��ĵȴ���ʱ */
  if(delay_count == 0 || lpi2c_status != kStatus_Success)
  {
    I2C_Timeout_Callback(3);
    return 1;
  }
  
  return 0;
}


/**
* @brief  I2C��ʼ��
* @param  ��
* @retval ��
*/
void I2C_EEPROM_Init(void)
{
  EEPROM_DEBUG_FUNC();

  /* ��ʼ��������IOMUXC��� */
  I2C_EEPROM_IOMUXC_MUX_Config();
  I2C_EEPROM_IOMUXC_PAD_Config();

  /* ��ʼ��I2C���蹤��ģʽ */
  EEPROM_I2C_ModeInit(); 
  
}



/*********************************************END OF FILE**********************/
