#include "fsl_debug_console.h"
#include "./can/bsp_can.h"
#include "./can/bsp_can.h"

//#define FLEXCAN2_TX_GPIO                GPIO1
//#define FLEXCAN2_TX_GPIO_PIN            (14U)
//#define FLEXCAN2_TX_IOMUXC              IOMUXC_GPIO_AD_B0_14_FLEXCAN2_TX
//
//#define FLEXCAN2_RX_GPIO                GPIO1
//#define FLEXCAN2_RX_GPIO_PIN            (15U)
//#define FLEXCAN2_RX_IOMUXC              IOMUXC_GPIO_AD_B0_15_FLEXCAN2_RX

/**
* @brief  CAN引脚初始化
*/  
static void CAN_GPIO_Config(void)   
{
  IOMUXC_SetPinMux(FLEXCAN2_TX_IOMUXC, 0U);
  IOMUXC_SetPinMux(FLEXCAN2_RX_IOMUXC, 0U);
  
  IOMUXC_SetPinConfig(FLEXCAN2_TX_IOMUXC, FLEXCAN2_PAD_CONFIG_DATA);
  IOMUXC_SetPinConfig(FLEXCAN2_RX_IOMUXC, FLEXCAN2_PAD_CONFIG_DATA);
}

/**
* @brief  CAN工作模式初始化
*参数：baudRate: CAN 通信波特率
       LoopBack: CAN 工作模式，bool变量，为0 工作在正常模式，为1 工作在回环模式
*/
void CAN_Mode_Config(uint32_t baudRate, bool LoopBack)
{
  /*CAN 配置结构体*/
  flexcan_config_t flexcanConfig;  //FlexCAN模块配置结构
  /*Clock setting for FLEXCAN*/
  CLOCK_SetMux(kCLOCK_CanMux, FLEXCAN_CLOCK_SOURCE_SELECT);
  CLOCK_SetDiv(kCLOCK_CanDiv, FLEXCAN_CLOCK_SOURCE_DIVIDER);
  
  /* 初始化 FlexCAN . */
  /*
  * flexcanConfig.clkSrc = kFLEXCAN_ClkSrcOsc;
  * flexcanConfig.baudRate = 1000000U;         //设置波特率为1 Mbps
  * flexcanConfig.baudRateFD = 2000000U;       //柔性版FD波特率在bps
  * flexcanConfig.maxMbNum = 16;               //用户使用的消息缓冲区的最大数量
  * flexcanConfig.enableLoopBack = false;      //禁用回环自检模式
  * flexcanConfig.enableSelfWakeup = false;    //启用或禁用自唤醒模式
  * flexcanConfig.enableIndividMask = false;   //启用或禁用Rx单个掩码
  * flexcanConfig.enableDoze = false;          //启用或禁用doze模式
  */
  FLEXCAN_GetDefaultConfig(&flexcanConfig);
  
  flexcanConfig.enableLoopBack = LoopBack;//设置为回环模式
  flexcanConfig.baudRate = baudRate;      //设置为回环模式
  FLEXCAN_Init(EXAMPLE_CAN, &flexcanConfig, EXAMPLE_CAN_CLK_FREQ);
}

/**
* @brief  初始化CAN接收缓冲区
*/
void CAN_RX_Buffer_Config(uint32_t ID_STD,uint8_t RX_MB)
{
  flexcan_rx_mb_config_t mbConfig; //FlexCAN接收缓冲区配置结构
  
  
  mbConfig.format = kFLEXCAN_FrameFormatStandard; //CAN 帧格式，标准帧或扩展帧
  mbConfig.type = kFLEXCAN_FrameTypeData;         //CAN 帧类型，数据帧或远程帧
  mbConfig.id = FLEXCAN_ID_STD(ID_STD);           //消息缓冲区帧标识符，
  
  FLEXCAN_SetRxMbConfig(EXAMPLE_CAN, RX_MB, &mbConfig, true);//配置接受消息缓冲区
  
}
  



/**
* @brief  CAN初始化
*/ 
void CAN_Config(void)
{
  /*CAN引脚初始化*/
  CAN_GPIO_Config();
  /*CAN工作模式初始化*/
  CAN_Mode_Config(1000000U,0);
}


/**************************END OF FILE************************************/









