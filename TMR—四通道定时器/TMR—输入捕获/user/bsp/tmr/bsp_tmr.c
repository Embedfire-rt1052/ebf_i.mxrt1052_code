#include "fsl_gpio.h"
#include "pad_config.h"
#include "fsl_debug_console.h"
#include "./bsp/nvic/bsp_nvic.h"
#include "./bsp/tmr/bsp_tmr.h"



// 定时器输入捕获用户自定义变量结构体定义
volatile GPT_ICUserValueTypeDef GPT_ICUserValueStructure = {0,0,0,0,0};    


/**
* @brief  配置TMR相关引脚功能
* @param  无
* @retval 无
*/
void TMR_GPIO_Config(void)
{
    /*定义GPIO引脚配置结构体*/
  gpio_pin_config_t gpt_config; 
  
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_00_QTIMER3_TIMER0, 0U);
  IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B1_00_QTIMER3_TIMER0, TMR_INPUT_PAD_CONFIG_DATA);
  
  gpt_config.direction = kGPIO_DigitalInput; //输入模式
  //gpt_config.outputLogic =  0;                //默认高电平
  gpt_config.interruptMode = kGPIO_NoIntmode; //不使用中断
  
  /* 初始化  GPIO. */
  GPIO_PinInit(TMR_TIMER0_INPUT_GPIO, TMR_TIMER0_INPUT_GPIO_PIN, &gpt_config);
 
}


/**
* @brief  配置TMR工作模式
* @param  无
* @retval 无
*/  
void TMR_Init(void)
{
  qtmr_config_t qtmrConfig; /*定义TMR 定时器初始化结构体*/
  
  /*初始化输入引脚*/
  TMR_GPIO_Config();
  
  /*初始化TMR 定时器*/
  QTMR_GetDefaultConfig(&qtmrConfig);
  qtmrConfig.primarySource = kQTMR_ClockDivide_128;
  QTMR_Init(BOARD_QTMR_BASEADDR, BOARD_QTMR_INPUT_CAPTURE_CHANNEL, &qtmrConfig);
  
  
   /*设置自动重装载值*/
  QTMR_SetTimerPeriod(BOARD_QTMR_BASEADDR, BOARD_QTMR_INPUT_CAPTURE_CHANNEL,RELODE_COUNTER);
  
  /* 设置输入捕获模式*/
  QTMR_SetupInputCapture(BOARD_QTMR_BASEADDR, BOARD_QTMR_INPUT_CAPTURE_CHANNEL, QTMR_CounterInputPin, false, false, kQTMR_FallingEdge);
  
  /* 使能定时器捕获中断*/
  QTMR_EnableInterrupts(BOARD_QTMR_BASEADDR, BOARD_QTMR_INPUT_CAPTURE_CHANNEL, kQTMR_EdgeInterruptEnable);
  
  /* 使能定时器比较中断*/
  QTMR_EnableInterrupts(BOARD_QTMR_BASEADDR, BOARD_QTMR_INPUT_CAPTURE_CHANNEL, kQTMR_CompareInterruptEnable);
  
  /*设置中断优先级,*/
  set_IRQn_Priority(QTMR_IRQ_ID,Group4_PreemptPriority_6, Group4_SubPriority_0);
  /* 使能中断 */
  EnableIRQ(QTMR_IRQ_ID);
  /* 开启定时器，并设置位在时钟上升沿计数 */
  QTMR_StartTimer(BOARD_QTMR_BASEADDR, BOARD_QTMR_INPUT_CAPTURE_CHANNEL, kQTMR_PriSrcRiseEdge);
}



/*TMR定时器中断服务函数*/
void QTMR_IRQ_HANDLER(void)
{

 /*
  *当要被捕获的信号的周期大于定时器设定的比较值，触发比较中断，
  */ 
   if ((QTMR_GetStatus(BOARD_QTMR_BASEADDR,BOARD_QTMR_INPUT_CAPTURE_CHANNEL))&(kQTMR_CompareFlag))               
   {	
      if( GPT_ICUserValueStructure.Capture_StartFlag != 0 )
      {
        GPT_ICUserValueStructure.Capture_Period ++;	
      }
      /*清除比较中断*/
      QTMR_ClearStatusFlags(BOARD_QTMR_BASEADDR, BOARD_QTMR_INPUT_CAPTURE_CHANNEL, kQTMR_CompareFlag);       
   }
  
   /*捕获中断*/ 
  if ((QTMR_GetStatus(BOARD_QTMR_BASEADDR,BOARD_QTMR_INPUT_CAPTURE_CHANNEL))&(kQTMR_EdgeFlag))
  {
     if(GPT_ICUserValueStructure.Capture_FinishFlag != 1)
     {
        /*第一次捕获*/ 
        if ( GPT_ICUserValueStructure.Capture_StartFlag == 0 )
        {
          /*清除溢出次数*/
          GPT_ICUserValueStructure.Capture_Period = 0;
          
          /*读取当前计数值*/ 
          GPT_ICUserValueStructure.Capture_CcrValue_1 = QTMR_GetCurrentTimerCount(BOARD_QTMR_BASEADDR, BOARD_QTMR_INPUT_CAPTURE_CHANNEL );
          /*当第一次捕获到上升沿之后，就把捕获边沿配置为上升沿*/ 
          QTMR_SetupInputCapture(BOARD_QTMR_BASEADDR, BOARD_QTMR_INPUT_CAPTURE_CHANNEL, QTMR_CounterInputPin, false, false, kQTMR_RisingEdge);
          /*开始捕获标志置1*/ 			
          GPT_ICUserValueStructure.Capture_StartFlag = 1;			
        }
        /*上升沿捕获中断,第二次捕获*/ 
        else 
        {
          /*获取捕获比较寄存器的值，这个值就是捕获到的高电平的时间的值*/ 
          GPT_ICUserValueStructure.Capture_CcrValue_2 =  QTMR_GetCurrentTimerCount(BOARD_QTMR_BASEADDR, BOARD_QTMR_INPUT_CAPTURE_CHANNEL ); 

          /*当第二次捕获到上升沿之后，就把捕获边沿配置为下降沿，好开启新的一轮捕获*/ 
          QTMR_SetupInputCapture(BOARD_QTMR_BASEADDR, BOARD_QTMR_INPUT_CAPTURE_CHANNEL, QTMR_CounterInputPin, false, false, kQTMR_FallingEdge);
          /*开始捕获标志清0*/ 		
          GPT_ICUserValueStructure.Capture_StartFlag = 0;
          /*捕获完成标志置1*/ 		
          GPT_ICUserValueStructure.Capture_FinishFlag = 1;		
        }
      }
     QTMR_ClearStatusFlags(BOARD_QTMR_BASEADDR, BOARD_QTMR_INPUT_CAPTURE_CHANNEL, kQTMR_EdgeFlag); 
  } 
}








