/**
  ******************************************************************************
  * @file    bsp_i2c_eeprom.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   EEPROM的专用iic驱动
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

#include "fsl_iomuxc.h"
#include "fsl_gpio.h"
#include "fsl_lpi2c.h"

#include "./delay/core_delay.h"   

#include "./eeprom/bsp_i2c_eeprom.h"
#include "./pad_config.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* I2C的SCL和SDA引脚使用同样的PAD配置 */
#define I2C_PAD_CONFIG_DATA         (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_1_MEDIUM_100MHz| \
                                        ODE_1_OPEN_DRAIN_ENABLED| \
                                        PKE_1_PULL_KEEPER_ENABLED| \
                                        PUE_0_KEEPER_SELECTED| \
                                        PUS_3_22K_OHM_PULL_UP| \
                                        HYS_0_HYSTERESIS_DISABLED)   
    /* 配置说明 : */
    /* 转换速率: 转换速率慢
        驱动强度: R0/6 
        带宽配置 : medium(100MHz)
        开漏配置: 使能 
        拉/保持器配置: 使能
        拉/保持器选择: 保持器
        上拉/下拉选择: 22K欧姆上拉(选择了保持器此配置无效)
        滞回器配置: 禁止 */ 
        
/* 用于超时检测的延时 */
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
* @brief  初始化EEPROM相关IOMUXC的MUX复用配置
* @param  无
* @retval 无
*/
static void I2C_EEPROM_IOMUXC_MUX_Config(void)
{
  /* SCL和SDA引脚，需要使能SION以接收数据 */
  IOMUXC_SetPinMux(EEPROM_SCL_IOMUXC, 1U);                                   
  IOMUXC_SetPinMux(EEPROM_SDA_IOMUXC, 1U);    
}

/**
* @brief  初始化EEPROM相关IOMUXC的PAD属性配置
* @param  无
* @retval 无
*/
static void I2C_EEPROM_IOMUXC_PAD_Config(void)
{
  /* SCL和SDA引脚 */
  IOMUXC_SetPinConfig(EEPROM_SCL_IOMUXC, I2C_PAD_CONFIG_DATA);                               
  IOMUXC_SetPinConfig(EEPROM_SDA_IOMUXC, I2C_PAD_CONFIG_DATA);   
}


/**
* @brief  初始化EEPROM使用的I2C外设
* @param  无
* @retval 无
*/  
static void EEPROM_I2C_ModeInit(void)
{
  lpi2c_master_config_t masterConfig; 
  
  /* 配置时钟 LPI2C */
  CLOCK_SetMux(kCLOCK_Lpi2cMux, LPI2C_CLOCK_SOURCE_SELECT);
  CLOCK_SetDiv(kCLOCK_Lpi2cDiv, LPI2C_CLOCK_SOURCE_DIVIDER);
  
  /*给masterConfig赋值为以下默认配置*/
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

  /* 把默认波特率改为I2C_BAUDRATE */
  masterConfig.baudRate_Hz = EEPROM_I2C_BAUDRATE;

  /*  使用以上配置初始化 LPI2C 外设 */
  LPI2C_MasterInit(EEPROM_I2C_MASTER, &masterConfig, LPI2C_MASTER_CLOCK_FREQUENCY);

}


/**
  * @brief 向EEPROM写入最多一页数据
  * @note  调用本函数后必须调用I2C_EEPROM_WaitStandbyState进行等待
  * @param ClientAddr:EEPROM的I2C设备地址(8位地址)
  * @param WriteAddr:写入的存储单元首地址
  * @param pBuffer:缓冲区指针
  * @param NumByteToWrite:要写的字节数，不可超过EEPROM_PAGE_SIZE定义的值
  * @retval  1不正常，0正常
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

  /* subAddress = WriteAddr, data = pBuffer 发送至从机
    起始信号start + 设备地址slaveaddress(w 写方向) + 
    发送缓冲数据tx data buffer + 停止信号stop */
  
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
  * @brief 向EEPROM写入不限量的数据
  * @param ClientAddr:EEPROM的I2C设备地址(8位地址)
  * @param WriteAddr:写入的存储单元首地址
  * @param pBuffer:缓冲区指针
  * @param NumByteToWrite:要写的字节数
  * @retval  无
  */
void I2C_EEPROM_Buffer_Write( uint8_t ClientAddr,
                              uint8_t WriteAddr, 
                              uint8_t* pBuffer,  
                              uint16_t NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;
  /* 后续要处理的字节数，初始值为NumByteToWrite*/
  uint8_t NumByteToWriteRest = NumByteToWrite;
  /* 根据以下情况进行处理：
    1.写入的首地址是否对齐 
    2.最后一次写入是否刚好写满一页 */
  Addr = WriteAddr % EEPROM_PAGE_SIZE;
  count = EEPROM_PAGE_SIZE - Addr;
  
  /* 若NumByteToWrite > count：
     第一页写入count个字节，对其余字节再进行后续处理，
     所以用 (NumByteToWriteRest = NumByteToWrite - count) 求出后续的NumOfPage和NumOfSingle进行处理。
     若NumByteToWrite < count：
     即不足一页数据，直接用NumByteToWriteRest = NumByteToWrite求出NumOfPage和NumOfSingle即可 */
  NumByteToWriteRest = (NumByteToWrite > count) ? (NumByteToWrite - count) : NumByteToWrite;
  
  /* 要完整写入的页数（不包括前count字节）*/
  NumOfPage =  NumByteToWriteRest / EEPROM_PAGE_SIZE;
  /* 最后一页要写入的字节数（不包括前count字节）*/
  NumOfSingle = NumByteToWriteRest % EEPROM_PAGE_SIZE;
 
  /* NumByteToWrite > count时，需要先往第一页写入count个字节
     NumByteToWrite < count时无需进行此操作 */
  if(count != 0 && NumByteToWrite > count)
  {  
    I2C_EEPROM_Page_Write(ClientAddr, WriteAddr, pBuffer, count);
    I2C_EEPROM_WaitStandbyState(ClientAddr);
    WriteAddr += count;
    pBuffer += count;
  }   
  
  /* 处理后续数据 */
  if(NumOfPage== 0 ) 
  {
    I2C_EEPROM_Page_Write(ClientAddr, WriteAddr, pBuffer, NumOfSingle);
    I2C_EEPROM_WaitStandbyState(ClientAddr);
  }
  else
  {   
    /* 后续数据大于一页 */
    while(NumOfPage--)
    {
      I2C_EEPROM_Page_Write(ClientAddr, WriteAddr, pBuffer, EEPROM_PAGE_SIZE);
      I2C_EEPROM_WaitStandbyState(ClientAddr);
      WriteAddr +=  EEPROM_PAGE_SIZE;
      pBuffer += EEPROM_PAGE_SIZE;  
    }
    /* 最后一页 */
    if(NumOfSingle != 0)
    {
      I2C_EEPROM_Page_Write(ClientAddr, WriteAddr, pBuffer, NumOfSingle); 
      I2C_EEPROM_WaitStandbyState(ClientAddr);
    }
  }  
}

/**
  * @brief 从EEPROM里面读取一块数据 
  * @param ClientAddr:EEPROM的I2C设备地址(8位地址)
  * @param ReadAddr:接收数据的EEPROM的地址 
  * @param pBuffer[out]:存放从EEPROM读取的数据的缓冲区指针
  * @param NumByteToWrite:要从EEPROM读取的字节数 
  * @retval  无
  */
uint32_t I2C_EEPROM_BufferRead( uint8_t ClientAddr,
                                uint8_t ReadAddr, 
                                uint8_t* pBuffer, 
                                uint16_t NumByteToRead)
{
  lpi2c_master_transfer_t masterXfer = {0};
  status_t reVal = kStatus_Fail;

  EEPROM_DEBUG_FUNC();

  /* subAddress = ReadAddr, data = pBuffer 自从机处接收
    起始信号start + 设备地址slaveaddress(w 写方向) + 子地址subAddress + 
    重复起始信号repeated start + 设备地址slaveaddress(r 读方向) + 
    接收缓冲数据rx data buffer + 停止信号stop */
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
  * @brief  IIC等待超时调用本函数输出调试信息
  * @param  None.
  * @retval 返回0xff，表示IIC读取数据失败
  */
static  uint32_t I2C_Timeout_Callback(uint8_t errorCode)
{
  /* 在此处进行错误处理 */
  EEPROM_ERROR("I2C 等待超时!errorCode = %d",errorCode);
  
  return 0xFF;
}


/**
  * @brief  用于等待EEPROM的内部写入时序，
  *         在I2C_EEPROM_Page_Write函数后必须被调用
  * @param  ClientAddr:设备地址（8位地址）
  * @retval 等待正常为0，等待超时为1
  */
uint8_t I2C_EEPROM_WaitStandbyState(uint8_t ClientAddr)      
{
  status_t lpi2c_status;
    /* 等待超过 40*I2CT_LONG_TIMEOUT us后认为超时 */
  uint32_t delay_count = I2CT_LONG_TIMEOUT;   

  do
  {
    /* 清除非应答标志，以便下一次检测 */
    LPI2C_MasterClearStatusFlags(EEPROM_I2C_MASTER, kLPI2C_MasterNackDetectFlag);
    
    /* 对EEPROM发出写方向的寻址信号，以检测是否响应 */
    lpi2c_status = LPI2C_MasterStart(EEPROM_I2C_MASTER, (ClientAddr>>1), kLPI2C_Write);
    
    /* 必须等待至少30us，才会产生非应答标志*/
    EEPROM_DELAY_US(40);
    
    /* 检测LPI2C MSR寄存器的NDF标志，并且确认delay_count没减到0（减到0认为超时，退出） */
  }while(EEPROM_I2C_MASTER->MSR & kLPI2C_MasterNackDetectFlag && delay_count-- );
  
  /* 清除非应答标志，防止下一次通讯错误 */
  LPI2C_MasterClearStatusFlags(EEPROM_I2C_MASTER, kLPI2C_MasterNackDetectFlag);

  /* 产生停止信号，防止下次通讯出错 */  
  lpi2c_status = LPI2C_MasterStop(EEPROM_I2C_MASTER);
  /* 必须等待至少10us，确保停止信号发送完成*/
  EEPROM_DELAY_US(10);

  /* 产生失败或前面的等待超时 */
  if(delay_count == 0 || lpi2c_status != kStatus_Success)
  {
    I2C_Timeout_Callback(3);
    return 1;
  }
  
  return 0;
}


/**
* @brief  I2C初始化
* @param  无
* @retval 无
*/
void I2C_EEPROM_Init(void)
{
  EEPROM_DEBUG_FUNC();

  /* 初始化各引脚IOMUXC相关 */
  I2C_EEPROM_IOMUXC_MUX_Config();
  I2C_EEPROM_IOMUXC_PAD_Config();

  /* 初始化I2C外设工作模式 */
  EEPROM_I2C_ModeInit(); 
  
}



/*********************************************END OF FILE**********************/
