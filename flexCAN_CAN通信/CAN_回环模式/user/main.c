/**
******************************************************************
* @file    main.c
* @author  fire
* @version V1.0
* @date    2018-xx-xx
* @brief   �����жϽ��ղ���
******************************************************************
* @attention
*
* ʵ��ƽ̨:Ұ��  i.MXRT1052������ 
* ��̳    :http://www.firebbs.cn
* �Ա�    :http://firestm32.taobao.com
*CPU_MIMXRT1052DVL6B,PRINTF_FLOAT_ENABLE=1, SCANF_FLOAT_ENABLE=1, PRINTF_ADVANCED_ENABLE=1, SCANF_ADVANCED_ENABLE=1
******************************************************************
*/


#include "fsl_debug_console.h"

#include "board.h"
#include "pin_mux.h"
#include "fsl_flexcan.h"


#include "clock_config.h"
#include "./bsp/nvic/bsp_nvic.h"
#include "./bsp/led/bsp_led.h" 
#include "./can/bsp_can.h"








/*******************************************************************************
 * Variables
 ******************************************************************************/

/*ȫ�ֱ���������CAN����״̬��false��û�н��յ���Ϣ��true:���յ���Ϣ���һ�δ������*/
volatile bool rxComplete = false;

/*ȫ�ֱ�����������պͷ�����Ϣ�ṹ��*/
flexcan_frame_t txFrame, rxFrame;



//typedef struct _flexcan_frame
//{
//    struct
//    {
//        uint32_t timestamp : 16; /*!<         . */
//        uint32_t length : 4;     /*!< CAN frame payload length in bytes(Range: 0~8). */
//        uint32_t type : 1;       /*!< CAN���ݰ����ͣ�����֡��Զ��֡��     CAN Frame Type(DATA or REMOTE). */
//        uint32_t format : 1;     /*!< CAN���ݰ���ʽ����׼��ʽ����չ��ʽ�� CAN Frame Identifier(STD or EXT format). */
//        uint32_t : 1;            /*!< Reserved. */
//        uint32_t idhit : 9;      /*!< CAN Rx FIFO filter hit id(This value is only used in Rx FIFO receive mode). */
//    };
//    struct
//    {
//        uint32_t id : 29; /*!< CAN Frame Identifier, should be set using FLEXCAN_ID_EXT() or FLEXCAN_ID_STD() macro. */
//        uint32_t : 3;     /*!< Reserved. */
//    };
//    union
//    {
//        struct
//        {
//            uint32_t dataWord0; /*!< CAN Frame payload word0. */
//            uint32_t dataWord1; /*!< CAN Frame payload word1. */
//        };
//        struct
//        {
//            uint8_t dataByte3; /*!< CAN Frame payload byte3. */
//            uint8_t dataByte2; /*!< CAN Frame payload byte2. */
//            uint8_t dataByte1; /*!< CAN Frame payload byte1. */
//            uint8_t dataByte0; /*!< CAN Frame payload byte0. */
//            uint8_t dataByte7; /*!< CAN Frame payload byte7. */
//            uint8_t dataByte6; /*!< CAN Frame payload byte6. */
//            uint8_t dataByte5; /*!< CAN Frame payload byte5. */
//            uint8_t dataByte4; /*!< CAN Frame payload byte4. */
//        };
//    };
//} flexcan_frame_t;


/*******************************************************************************
 * Code
 ******************************************************************************/





/*******************************************************************
* Prototypes
*******************************************************************/
/**
* @brief ��ʱһ��ʱ��
*/
void delay(uint32_t count);

/*******************************************************************
* Code
*******************************************************************/
/**
* @note �������ڲ�ͬ���Ż�ģʽ����ʱʱ�䲻ͬ��
*       ��flexspi_nor_debug��flexspi_nor_release�汾�ĳ����У�
*       flexspi_nor_release�汾����ʱҪ�̵ö�  
*/ 
void delay(uint32_t count)
{
  volatile uint32_t i = 0;
  for (i = 0; i < count; ++i)
  {
    __asm("NOP"); /* ����nop��ָ�� */
  }
}

/**
* @brief  ������
* @param  ��
* @retval ��
*/
int main(void)
{
  /*CAN ���ýṹ��*/


  
  
  /* ��ʼ���ڴ汣����Ԫ */
  BOARD_ConfigMPU();
  /* ��ʼ������������ */
  BOARD_InitPins();
  /* ��ʼ��������ʱ�� */
  BOARD_BootClockRUN();
  /* ��ʼ�����Դ��� */
  BOARD_InitDebugConsole();
  
  /*�����ж����ȼ�����*/
  Set_NVIC_PriorityGroup(Group_4); 
  
  /* ��ӡϵͳʱ�� */
  PRINTF("\r\n");
  PRINTF("*****��ӭʹ�� Ұ��i.MX RT1052 ������*****\r\n");
  PRINTF("CPU:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_CpuClk));
  PRINTF("AHB:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_AhbClk));
  PRINTF("SEMC:            %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SemcClk));
  PRINTF("SYSPLL:          %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllClk));
  PRINTF("SYSPLLPFD0:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd0Clk));
  PRINTF("SYSPLLPFD1:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd1Clk));
  PRINTF("SYSPLLPFD2:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd2Clk));
  PRINTF("SYSPLLPFD3:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd3Clk));  
  
  /* ��ʼ��LED���� */
  LED_GPIO_Config(); 
  
  PRINTF("\r\n==FlexCAN loopback functional example -- Start.==\r\n\r\n");
  CAN_Config();
  
  while(1)
  {



    
    


    CAN_RX_Buffer_Config();//��ʼ�����ջ�����
    CAN_TX_Buffer_Config();
    


    PRINTF("Send message from MB%d to MB%d\r\n", TX_MESSAGE_BUFFER_NUM, RX_MESSAGE_BUFFER_NUM);

    PRINTF("tx word0 = 0x%x\r\n", txFrame.dataWord0);
    PRINTF("tx word1 = 0x%x\r\n", txFrame.dataWord1);


/* Send data through Tx Message Buffer using polling function. */

    
    FLEXCAN_TransferSendBlocking(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, &txFrame);


    /* Waiting for Message receive finish. */
    while (!rxComplete)
    {
    }

    PRINTF("\r\nReceived message from MB%d\r\n", RX_MESSAGE_BUFFER_NUM);

    PRINTF("rx word0 = 0x%x\r\n", rxFrame.dataWord0);
    PRINTF("rx word1 = 0x%x\r\n", rxFrame.dataWord1);


    /* Stop FlexCAN Send & Receive. */
    FLEXCAN_DisableMbInterrupts(EXAMPLE_CAN, 1 << RX_MESSAGE_BUFFER_NUM);

    PRINTF("\r\n==FlexCAN loopback functional example -- Finish.==\r\n");
    while (1)
    {
      
    }
    
    /*��ֹת���ٶȹ��쵼�´�����������쳣*/
    delay(LED_DELAY_COUNT/3);
    
  }
  
}



/****************************END OF FILE**********************/

