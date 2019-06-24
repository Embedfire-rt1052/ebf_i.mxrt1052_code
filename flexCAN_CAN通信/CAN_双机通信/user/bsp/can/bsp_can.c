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
* @brief  CAN���ų�ʼ��
*/  
static void CAN_GPIO_Config(void)   
{
  IOMUXC_SetPinMux(FLEXCAN2_TX_IOMUXC, 0U);
  IOMUXC_SetPinMux(FLEXCAN2_RX_IOMUXC, 0U);
  
  IOMUXC_SetPinConfig(FLEXCAN2_TX_IOMUXC, FLEXCAN2_PAD_CONFIG_DATA);
  IOMUXC_SetPinConfig(FLEXCAN2_RX_IOMUXC, FLEXCAN2_PAD_CONFIG_DATA);
}

/**
* @brief  CAN����ģʽ��ʼ��
*������baudRate: CAN ͨ�Ų�����
       LoopBack: CAN ����ģʽ��bool������Ϊ0 ����������ģʽ��Ϊ1 �����ڻػ�ģʽ
*/
void CAN_Mode_Config(uint32_t baudRate, bool LoopBack)
{
  /*CAN ���ýṹ��*/
  flexcan_config_t flexcanConfig;  //FlexCANģ�����ýṹ
  /*Clock setting for FLEXCAN*/
  CLOCK_SetMux(kCLOCK_CanMux, FLEXCAN_CLOCK_SOURCE_SELECT);
  CLOCK_SetDiv(kCLOCK_CanDiv, FLEXCAN_CLOCK_SOURCE_DIVIDER);
  
  /* ��ʼ�� FlexCAN . */
  /*
  * flexcanConfig.clkSrc = kFLEXCAN_ClkSrcOsc;
  * flexcanConfig.baudRate = 1000000U;         //���ò�����Ϊ1 Mbps
  * flexcanConfig.baudRateFD = 2000000U;       //���԰�FD��������bps
  * flexcanConfig.maxMbNum = 16;               //�û�ʹ�õ���Ϣ���������������
  * flexcanConfig.enableLoopBack = false;      //���ûػ��Լ�ģʽ
  * flexcanConfig.enableSelfWakeup = false;    //���û�����Ի���ģʽ
  * flexcanConfig.enableIndividMask = false;   //���û����Rx��������
  * flexcanConfig.enableDoze = false;          //���û����dozeģʽ
  */
  FLEXCAN_GetDefaultConfig(&flexcanConfig);
  
  flexcanConfig.enableLoopBack = LoopBack;//����Ϊ�ػ�ģʽ
  flexcanConfig.baudRate = baudRate;      //����Ϊ�ػ�ģʽ
  FLEXCAN_Init(EXAMPLE_CAN, &flexcanConfig, EXAMPLE_CAN_CLK_FREQ);
}

/**
* @brief  ��ʼ��CAN���ջ�����
*/
void CAN_RX_Buffer_Config(uint32_t ID_STD,uint8_t RX_MB)
{
  flexcan_rx_mb_config_t mbConfig; //FlexCAN���ջ��������ýṹ
  
  
  mbConfig.format = kFLEXCAN_FrameFormatStandard; //CAN ֡��ʽ����׼֡����չ֡
  mbConfig.type = kFLEXCAN_FrameTypeData;         //CAN ֡���ͣ�����֡��Զ��֡
  mbConfig.id = FLEXCAN_ID_STD(ID_STD);           //��Ϣ������֡��ʶ����
  
  FLEXCAN_SetRxMbConfig(EXAMPLE_CAN, RX_MB, &mbConfig, true);//���ý�����Ϣ������
  
}
  



/**
* @brief  CAN��ʼ��
*/ 
void CAN_Config(void)
{
  /*CAN���ų�ʼ��*/
  CAN_GPIO_Config();
  /*CAN����ģʽ��ʼ��*/
  CAN_Mode_Config(1000000U,0);
}


/**************************END OF FILE************************************/









