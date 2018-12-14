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
  ******************************************************************
  */
  

#include "fsl_debug_console.h"

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"

#include "./led/bsp_led.h" 
#include "./uart/bsp_usart.h"


static void Show_Message(void);
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
  char ch;
  /* ��ʼ���ڴ汣����Ԫ */
  BOARD_ConfigMPU();
  /* ��ʼ������������ */
  BOARD_InitPins();
  /* ��ʼ��������ʱ�� */
  BOARD_BootClockRUN();
  /* ��ʼ�����Դ��� */
  BOARD_InitDebugConsole();
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

  /*��ʼ��uart1*/
  USART_Config();
    

  while(1)
  {
    /* ��ȡ�ַ�ָ�� */
    ch = GETCHAR();
    PRINTF("���յ��ַ���%c\n",ch);

    /* �����ַ�ָ�����RGB�ʵ���ɫ */
    switch(ch)
    {
      case '1':
        RGB_LED_COLOR_RED;
      break;
      case '2':
        RGB_LED_COLOR_GREEN;
      break;
      case '3':
        RGB_LED_COLOR_BLUE;
      break;
      case '4':
        RGB_LED_COLOR_YELLOW;
      break;
      case '5':
        RGB_LED_COLOR_PURPLE;
      break;
      case '6':
        RGB_LED_COLOR_CYAN;
      break;
      case '7':
        RGB_LED_COLOR_WHITE;
      break;
      case '8':
        RGB_LED_COLOR_OFF;
      break;
      default:
        /* �������ָ��ָ���ַ�����ӡ��ʾ��Ϣ */
        Show_Message();
      break;
      } 
    }

}


static void Show_Message(void)
{
  PRINTF("\r\n   ����һ��ͨ������ͨ��ָ�����RGB�ʵ�ʵ�� \n");
  PRINTF("ʹ��  USART  ����Ϊ��%d 8-N-1 \n",DEBUG_USART_BAUDRATE);
  PRINTF("������ӵ�ָ������RGB�ʵ���ɫ��ָ���Ӧ���£�\n");
  PRINTF("   ָ��   ------ �ʵ���ɫ \n");
  PRINTF("     1    ------    �� \n");
  PRINTF("     2    ------    �� \n");
  PRINTF("     3    ------    �� \n");
  PRINTF("     4    ------    �� \n");
  PRINTF("     5    ------    �� \n");
  PRINTF("     6    ------    �� \n");
  PRINTF("     7    ------    �� \n");
  PRINTF("     8    ------    �� \n");  
}

/****************************END OF FILE**********************/

