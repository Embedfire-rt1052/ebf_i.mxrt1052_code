#include "fsl_gpio.h"
#include "pad_config.h"
#include "fsl_debug_console.h"
#include "./bsp/nvic/bsp_nvic.h"
#include "./bsp/tmr/bsp_tmr.h"



// ��ʱ�����벶���û��Զ�������ṹ�嶨��
volatile GPT_ICUserValueTypeDef GPT_ICUserValueStructure = {0,0,0,0,0};    


/**
* @brief  ����TMR������Ź���
* @param  ��
* @retval ��
*/
void TMR_GPIO_Config(void)
{
    /*����GPIO�������ýṹ��*/
  gpio_pin_config_t gpt_config; 
  
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_00_QTIMER3_TIMER0, 0U);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_00_QTIMER3_TIMER0, TMR_INPUT_PAD_CONFIG_DATA);
  
  gpt_config.direction = kGPIO_DigitalInput; //����ģʽ
  //gpt_config.outputLogic =  0;                //Ĭ�ϸߵ�ƽ
  gpt_config.interruptMode = kGPIO_NoIntmode; //��ʹ���ж�
  
  /* ��ʼ��  GPIO. */
  GPIO_PinInit(TMR_TIMER0_INPUT_GPIO, TMR_TIMER0_INPUT_GPIO_PIN, &gpt_config);
 
}


/**
* @brief  ����TMR����ģʽ
* @param  ��
* @retval ��
*/  
void TMR_Init(void)
{
  qtmr_config_t qtmrConfig; /*����TMR ��ʱ����ʼ���ṹ��*/
  
  /*��ʼ����������*/
  TMR_GPIO_Config();
  
  /*��ʼ��TMR ��ʱ��*/
  QTMR_GetDefaultConfig(&qtmrConfig);
  qtmrConfig.primarySource = kQTMR_ClockDivide_128;
  QTMR_Init(BOARD_QTMR_BASEADDR, BOARD_QTMR_INPUT_CAPTURE_CHANNEL, &qtmrConfig);
  
  
   /*�����Զ���װ��ֵ*/
  QTMR_SetTimerPeriod(BOARD_QTMR_BASEADDR, BOARD_QTMR_INPUT_CAPTURE_CHANNEL,RELODE_COUNTER);
  
  /* �������벶��ģʽ*/
  QTMR_SetupInputCapture(BOARD_QTMR_BASEADDR, BOARD_QTMR_INPUT_CAPTURE_CHANNEL, QTMR_CounterInputPin, false, false, kQTMR_FallingEdge);
  
  /* ʹ�ܶ�ʱ�������ж�*/
  QTMR_EnableInterrupts(BOARD_QTMR_BASEADDR, BOARD_QTMR_INPUT_CAPTURE_CHANNEL, kQTMR_EdgeInterruptEnable);
  
  /* ʹ�ܶ�ʱ���Ƚ��ж�*/
  QTMR_EnableInterrupts(BOARD_QTMR_BASEADDR, BOARD_QTMR_INPUT_CAPTURE_CHANNEL, kQTMR_CompareInterruptEnable);
  
  /*�����ж����ȼ�,*/
  set_IRQn_Priority(QTMR_IRQ_ID,Group4_PreemptPriority_6, Group4_SubPriority_0);
  /* ʹ���ж� */
  EnableIRQ(QTMR_IRQ_ID);
  /* ������ʱ����������λ��ʱ�������ؼ��� */
  QTMR_StartTimer(BOARD_QTMR_BASEADDR, BOARD_QTMR_INPUT_CAPTURE_CHANNEL, kQTMR_PriSrcRiseEdge);
}



/*TMR��ʱ���жϷ�����*/
void QTMR_IRQ_HANDLER(void)
{

 /*
  *��Ҫ��������źŵ����ڴ��ڶ�ʱ���趨�ıȽ�ֵ�������Ƚ��жϣ�
  */ 
   if ((QTMR_GetStatus(BOARD_QTMR_BASEADDR,BOARD_QTMR_INPUT_CAPTURE_CHANNEL))&(kQTMR_CompareFlag))               
   {	
      if( GPT_ICUserValueStructure.Capture_StartFlag != 0 )
      {
        GPT_ICUserValueStructure.Capture_Period ++;	
      }
      /*����Ƚ��ж�*/
      QTMR_ClearStatusFlags(BOARD_QTMR_BASEADDR, BOARD_QTMR_INPUT_CAPTURE_CHANNEL, kQTMR_CompareFlag);       
   }
  
   /*�����ж�*/ 
  if ((QTMR_GetStatus(BOARD_QTMR_BASEADDR,BOARD_QTMR_INPUT_CAPTURE_CHANNEL))&(kQTMR_EdgeFlag))
  {
     if(GPT_ICUserValueStructure.Capture_FinishFlag != 1)
     {
        /*��һ�β���*/ 
        if ( GPT_ICUserValueStructure.Capture_StartFlag == 0 )
        {
          /*����������*/
          GPT_ICUserValueStructure.Capture_Period = 0;
          
          /*��ȡ��ǰ����ֵ*/ 
          GPT_ICUserValueStructure.Capture_CcrValue_1 = QTMR_GetCurrentTimerCount(BOARD_QTMR_BASEADDR, BOARD_QTMR_INPUT_CAPTURE_CHANNEL );
          /*����һ�β���������֮�󣬾ͰѲ����������Ϊ������*/ 
          QTMR_SetupInputCapture(BOARD_QTMR_BASEADDR, BOARD_QTMR_INPUT_CAPTURE_CHANNEL, QTMR_CounterInputPin, false, false, kQTMR_RisingEdge);
          /*��ʼ�����־��1*/ 			
          GPT_ICUserValueStructure.Capture_StartFlag = 1;			
        }
        /*�����ز����ж�,�ڶ��β���*/ 
        else 
        {
          /*��ȡ����ȽϼĴ�����ֵ�����ֵ���ǲ��񵽵ĸߵ�ƽ��ʱ���ֵ*/ 
          GPT_ICUserValueStructure.Capture_CcrValue_2 =  QTMR_GetCurrentTimerCount(BOARD_QTMR_BASEADDR, BOARD_QTMR_INPUT_CAPTURE_CHANNEL ); 

          /*���ڶ��β���������֮�󣬾ͰѲ����������Ϊ�½��أ��ÿ����µ�һ�ֲ���*/ 
          QTMR_SetupInputCapture(BOARD_QTMR_BASEADDR, BOARD_QTMR_INPUT_CAPTURE_CHANNEL, QTMR_CounterInputPin, false, false, kQTMR_FallingEdge);
          /*��ʼ�����־��0*/ 		
          GPT_ICUserValueStructure.Capture_StartFlag = 0;
          /*������ɱ�־��1*/ 		
          GPT_ICUserValueStructure.Capture_FinishFlag = 1;		
        }
      }
     QTMR_ClearStatusFlags(BOARD_QTMR_BASEADDR, BOARD_QTMR_INPUT_CAPTURE_CHANNEL, kQTMR_EdgeFlag); 
  } 
}








