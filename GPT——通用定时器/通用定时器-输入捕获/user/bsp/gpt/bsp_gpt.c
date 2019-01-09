#include "fsl_gpio.h"
#include "pad_config.h"

#include "./gpt/bsp_gpt.h"
#include "./led/bsp_led.h" 




// ��ʱ�����벶���û��Զ�������ṹ�嶨��
volatile GPT_ICUserValueTypeDef GPT_ICUserValueStructure = {0,0,0,0,0};
  
/**
* @brief  ����GPT������Ź���
* @param  ��
* @retval ��
*/
void GPT_GPIO_Config(void)
{
  /*����GPIO�������ýṹ��*/
  gpio_pin_config_t gpt_config;
  
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_40_GPT2_CAPTURE2, 0U);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_40_GPT2_CAPTURE2, GPT_COMPARE_PAD_CONFIG_DATA);
  
  gpt_config.direction = kGPIO_DigitalInput; //����ģʽ
  //gpt_config.outputLogic =  0;                //Ĭ�ϸߵ�ƽ
  gpt_config.interruptMode = kGPIO_NoIntmode; //��ʹ���ж�
  
  /* ��ʼ�� GPT COMPARE1 GPIO. */
  GPIO_PinInit(GPT2_CAPTURE2_GPIO, GPT2_CAPTURE2_GPIO_PIN, &gpt_config);
  
}

  
  
/**
* @brief  ����GPT����ģʽ
* @param  ��
* @retval ��
*/  
void GPT_Config(void)
{

  gpt_config_t gptConfig;
  
  /*��ʼ��GPT����*/
  GPT_GPIO_Config();
  
  /*GPT��ʱ������*/
  CLOCK_SetMux(kCLOCK_PerclkMux, EXAMPLE_GPT_CLOCK_SOURCE_SELECT);
  CLOCK_SetDiv(kCLOCK_PerclkDiv, EXAMPLE_GPT_CLOCK_DIVIDER_SELECT);
  
  /*��ʼ��GPT*/
  GPT_GetDefaultConfig(&gptConfig);
  GPT_Init(EXAMPLE_GPT, &gptConfig);
  
  
  /* ����ʱ�ӷ�Ƶ */
  GPT_SetClockDivider(EXAMPLE_GPT, GPT_DIVIDER);


  /*����λ����ģʽ*/
  GPT_SetInputOperationMode(EXAMPLE_GPT,kGPT_InputCapture_Channel2,kGPT_InputOperation_FallEdge);
  /*ʹ�����벶���ж�*/
  GPT_EnableInterrupts(EXAMPLE_GPT, kGPT_InputCapture2InterruptEnable);
  /*ʹ������ж�*/
  GPT_EnableInterrupts(EXAMPLE_GPT,kGPT_RollOverFlagInterruptEnable);
  

  /*ʹ���ж�*/
  EnableIRQ(GPT_IRQ_ID);

  /* ������ʱ�� */
  GPT_StartTimer(EXAMPLE_GPT);
}





/*�����жϷ�����*/
void EXAMPLE_GPT_IRQHandler(void)
{
  /*
  *��Ҫ��������źŵ����ڴ��ڶ�ʱ�������ʱʱ����ʱ���ͻ���������������ж�
  *���ʱ��������Ҫ�������Ķ�ʱ���ڼӵ������źŵ�ʱ������ȥ
  */ 
   if ( GPT_GetStatusFlags(EXAMPLE_GPT,kGPT_RollOverFlag) != false )               
   {	
      if ( GPT_ICUserValueStructure.Capture_StartFlag != 0 )
      {
        GPT_ICUserValueStructure.Capture_Period ++;	
      }
      GPT_ClearStatusFlags(EXAMPLE_GPT, kGPT_RollOverFlag); 		
   }
  
   /*�½��ز����ж�*/ 
  if (GPT_GetStatusFlags(EXAMPLE_GPT,kGPT_InputCapture2Flag) != false)
  {
    
      if(GPT_ICUserValueStructure.Capture_FinishFlag != 1)
      {
        /*��һ�β���*/ 
        if ( GPT_ICUserValueStructure.Capture_StartFlag == 0 )
        {
          /*����������*/
          GPT_ICUserValueStructure.Capture_Period = 0;
          
          /*��ȡ��ǰ����ֵ*/ 
          GPT_ICUserValueStructure.Capture_CcrValue_1 = GPT_GetInputCaptureValue(EXAMPLE_GPT,kGPT_InputCapture_Channel2);
          /*����һ�β���������֮�󣬾ͰѲ����������Ϊ������*/ 
          GPT_SetInputOperationMode(EXAMPLE_GPT,kGPT_InputCapture_Channel2,kGPT_InputOperation_RiseEdge);
          /*��ʼ�����־��1*/ 			
          GPT_ICUserValueStructure.Capture_StartFlag = 1;			
        }
        /*�����ز����ж�,�ڶ��β���*/ 
        else 
        {
          /*��ȡ����ȽϼĴ�����ֵ�����ֵ���ǲ��񵽵ĸߵ�ƽ��ʱ���ֵ*/ 
          GPT_ICUserValueStructure.Capture_CcrValue_2 = GPT_GetInputCaptureValue(EXAMPLE_GPT,kGPT_InputCapture_Channel2);   

          /*���ڶ��β���������֮�󣬾ͰѲ����������Ϊ�½��أ��ÿ����µ�һ�ֲ���*/ 
          GPT_SetInputOperationMode(EXAMPLE_GPT,kGPT_InputCapture_Channel2,kGPT_InputOperation_FallEdge);
          /*��ʼ�����־��0*/ 		
          GPT_ICUserValueStructure.Capture_StartFlag = 0;
          /*������ɱ�־��1	*/ 		
          GPT_ICUserValueStructure.Capture_FinishFlag = 1;		
         }
       }

     GPT_ClearStatusFlags(EXAMPLE_GPT, kGPT_InputCapture2Flag);
  }      
    
}









