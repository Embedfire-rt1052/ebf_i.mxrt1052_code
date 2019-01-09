#include "fsl_gpio.h"
#include "pad_config.h"

#include "./gpt/bsp_gpt.h"
#include "./led/bsp_led.h" 




// 定时器输入捕获用户自定义变量结构体定义
volatile GPT_ICUserValueTypeDef GPT_ICUserValueStructure = {0,0,0,0,0};
  
/**
* @brief  配置GPT相关引脚功能
* @param  无
* @retval 无
*/
void GPT_GPIO_Config(void)
{
  /*定义GPIO引脚配置结构体*/
  gpio_pin_config_t gpt_config;
  
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_40_GPT2_CAPTURE2, 0U);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_40_GPT2_CAPTURE2, GPT_COMPARE_PAD_CONFIG_DATA);
  
  gpt_config.direction = kGPIO_DigitalInput; //输入模式
  //gpt_config.outputLogic =  0;                //默认高电平
  gpt_config.interruptMode = kGPIO_NoIntmode; //不使用中断
  
  /* 初始化 GPT COMPARE1 GPIO. */
  GPIO_PinInit(GPT2_CAPTURE2_GPIO, GPT2_CAPTURE2_GPIO_PIN, &gpt_config);
  
}

  
  
/**
* @brief  配置GPT工作模式
* @param  无
* @retval 无
*/  
void GPT_Config(void)
{

  gpt_config_t gptConfig;
  
  /*初始化GPT引脚*/
  GPT_GPIO_Config();
  
  /*GPT的时钟设置*/
  CLOCK_SetMux(kCLOCK_PerclkMux, EXAMPLE_GPT_CLOCK_SOURCE_SELECT);
  CLOCK_SetDiv(kCLOCK_PerclkDiv, EXAMPLE_GPT_CLOCK_DIVIDER_SELECT);
  
  /*初始化GPT*/
  GPT_GetDefaultConfig(&gptConfig);
  GPT_Init(EXAMPLE_GPT, &gptConfig);
  
  
  /* 设置时钟分频 */
  GPT_SetClockDivider(EXAMPLE_GPT, GPT_DIVIDER);


  /*设置位输入模式*/
  GPT_SetInputOperationMode(EXAMPLE_GPT,kGPT_InputCapture_Channel2,kGPT_InputOperation_FallEdge);
  /*使能输入捕获中断*/
  GPT_EnableInterrupts(EXAMPLE_GPT, kGPT_InputCapture2InterruptEnable);
  /*使能溢出中断*/
  GPT_EnableInterrupts(EXAMPLE_GPT,kGPT_RollOverFlagInterruptEnable);
  

  /*使能中断*/
  EnableIRQ(GPT_IRQ_ID);

  /* 开启定时器 */
  GPT_StartTimer(EXAMPLE_GPT);
}





/*定义中断服务函数*/
void EXAMPLE_GPT_IRQHandler(void)
{
  /*
  *当要被捕获的信号的周期大于定时器的最长定时时，定时器就会溢出，产生更新中断
  *这个时候我们需要把这个最长的定时周期加到捕获信号的时间里面去
  */ 
   if ( GPT_GetStatusFlags(EXAMPLE_GPT,kGPT_RollOverFlag) != false )               
   {	
      if ( GPT_ICUserValueStructure.Capture_StartFlag != 0 )
      {
        GPT_ICUserValueStructure.Capture_Period ++;	
      }
      GPT_ClearStatusFlags(EXAMPLE_GPT, kGPT_RollOverFlag); 		
   }
  
   /*下降沿捕获中断*/ 
  if (GPT_GetStatusFlags(EXAMPLE_GPT,kGPT_InputCapture2Flag) != false)
  {
    
      if(GPT_ICUserValueStructure.Capture_FinishFlag != 1)
      {
        /*第一次捕获*/ 
        if ( GPT_ICUserValueStructure.Capture_StartFlag == 0 )
        {
          /*清除溢出次数*/
          GPT_ICUserValueStructure.Capture_Period = 0;
          
          /*读取当前计数值*/ 
          GPT_ICUserValueStructure.Capture_CcrValue_1 = GPT_GetInputCaptureValue(EXAMPLE_GPT,kGPT_InputCapture_Channel2);
          /*当第一次捕获到上升沿之后，就把捕获边沿配置为上升沿*/ 
          GPT_SetInputOperationMode(EXAMPLE_GPT,kGPT_InputCapture_Channel2,kGPT_InputOperation_RiseEdge);
          /*开始捕获标志置1*/ 			
          GPT_ICUserValueStructure.Capture_StartFlag = 1;			
        }
        /*上升沿捕获中断,第二次捕获*/ 
        else 
        {
          /*获取捕获比较寄存器的值，这个值就是捕获到的高电平的时间的值*/ 
          GPT_ICUserValueStructure.Capture_CcrValue_2 = GPT_GetInputCaptureValue(EXAMPLE_GPT,kGPT_InputCapture_Channel2);   

          /*当第二次捕获到上升沿之后，就把捕获边沿配置为下降沿，好开启新的一轮捕获*/ 
          GPT_SetInputOperationMode(EXAMPLE_GPT,kGPT_InputCapture_Channel2,kGPT_InputOperation_FallEdge);
          /*开始捕获标志清0*/ 		
          GPT_ICUserValueStructure.Capture_StartFlag = 0;
          /*捕获完成标志置1	*/ 		
          GPT_ICUserValueStructure.Capture_FinishFlag = 1;		
         }
       }

     GPT_ClearStatusFlags(EXAMPLE_GPT, kGPT_InputCapture2Flag);
  }      
    
}









