/**
******************************************************************
* @file    main.c
* @author  fire
* @version V1.0
* @date    2018-xx-xx
* @brief   串口中断接收测试
******************************************************************
* @attention
*
* 实验平台:野火  i.MXRT1052开发板 
* 论坛    :http://www.firebbs.cn
* 淘宝    :http://firestm32.taobao.com
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

/*全局变量，保存CAN接收状态，false：没有接收到消息。true:接收到消息并且还未被处理*/
volatile bool rxComplete = false;

/*全局变量，定义接收和发送消息结构体*/
flexcan_frame_t txFrame, rxFrame;



//typedef struct _flexcan_frame
//{
//    struct
//    {
//        uint32_t timestamp : 16; /*!<         . */
//        uint32_t length : 4;     /*!< CAN frame payload length in bytes(Range: 0~8). */
//        uint32_t type : 1;       /*!< CAN数据包类型，数据帧或远程帧，     CAN Frame Type(DATA or REMOTE). */
//        uint32_t format : 1;     /*!< CAN数据包格式，标准格式或扩展格式， CAN Frame Identifier(STD or EXT format). */
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
* @brief 延时一段时间
*/
void delay(uint32_t count);

/*******************************************************************
* Code
*******************************************************************/
/**
* @note 本函数在不同的优化模式下延时时间不同，
*       如flexspi_nor_debug和flexspi_nor_release版本的程序中，
*       flexspi_nor_release版本的延时要短得多  
*/ 
void delay(uint32_t count)
{
  volatile uint32_t i = 0;
  for (i = 0; i < count; ++i)
  {
    __asm("NOP"); /* 调用nop空指令 */
  }
}

/**
* @brief  主函数
* @param  无
* @retval 无
*/
int main(void)
{
  /*CAN 配置结构体*/


  
  
  /* 初始化内存保护单元 */
  BOARD_ConfigMPU();
  /* 初始化开发板引脚 */
  BOARD_InitPins();
  /* 初始化开发板时钟 */
  BOARD_BootClockRUN();
  /* 初始化调试串口 */
  BOARD_InitDebugConsole();
  
  /*设置中断优先级分组*/
  Set_NVIC_PriorityGroup(Group_4); 
  
  /* 打印系统时钟 */
  PRINTF("\r\n");
  PRINTF("*****欢迎使用 野火i.MX RT1052 开发板*****\r\n");
  PRINTF("CPU:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_CpuClk));
  PRINTF("AHB:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_AhbClk));
  PRINTF("SEMC:            %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SemcClk));
  PRINTF("SYSPLL:          %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllClk));
  PRINTF("SYSPLLPFD0:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd0Clk));
  PRINTF("SYSPLLPFD1:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd1Clk));
  PRINTF("SYSPLLPFD2:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd2Clk));
  PRINTF("SYSPLLPFD3:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd3Clk));  
  
  /* 初始化LED引脚 */
  LED_GPIO_Config(); 
  
  PRINTF("\r\n==FlexCAN loopback functional example -- Start.==\r\n\r\n");
  CAN_Config();
  
  while(1)
  {



    
    


    CAN_RX_Buffer_Config();//初始化接收缓冲区
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
    
    /*防止转换速度过快导致串口软件接收异常*/
    delay(LED_DELAY_COUNT/3);
    
  }
  
}



/****************************END OF FILE**********************/

