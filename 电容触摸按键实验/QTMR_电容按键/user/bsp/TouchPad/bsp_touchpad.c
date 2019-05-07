/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   电容按键应用函数接口
  ******************************************************************
  * @attention
  *
  * 实验平台:野火  i.MXRT1052开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************
  */	
#include "./Bsp/TouchPad/bsp_touchpad.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_qtmr.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_iomuxc.h"
#include "pad_config.h" 
#include "./SysTick/bsp_SysTick.h"

/*定时器最大计数值*/
#define TPAD_ARR_MAX_VAL 	0XFFFF	

/*保存没按下时定时器计数值*/
volatile uint16_t tpad_default_val=0;

/*******************************************************************************
 * Variables
 ******************************************************************************/

volatile bool qtmrIsrFlag = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
void TPAD_QTMR_IRQ_HANDLER(void)
{   
    /* 清除中断标志 */
    QTMR_ClearStatusFlags(TPAD_QTMR_BASEADDR, TPAD_QTMR_INPUT_CAPTURE_CHANNEL, kQTMR_EdgeFlag);
    
    qtmrIsrFlag = true;

}

/***********************************
 *
 * 定时器输入捕获配置
 *
 ***********************************/
static void TIMx_CHx_Cap_Init(void)
{
  qtmr_config_t qtmrConfig;
  /* 设置引脚的复用模式 */
  IOMUXC_SetPinMux(
                   TPAD_QTMR_IOMUXC,    /* 配置为定时器输入捕抓IO */
                   0U);                 /* 不使用SION功能 */																							 
  /* 引脚PAD属性配置 */  
  IOMUXC_SetPinConfig(TPAD_QTMR_IOMUXC,TMR_INPUT_PAD_CONFIG_DATA); 
  
  /* 获取系统默认配置 */
  QTMR_GetDefaultConfig(&qtmrConfig);
  /* 使用IP总线时钟，八分频 */
  qtmrConfig.primarySource = kQTMR_ClockDivide_16;
  /* 指定定时器通道初始化定时器 */
  QTMR_Init(TPAD_QTMR_BASEADDR, TPAD_QTMR_INPUT_CAPTURE_CHANNEL, &qtmrConfig);
  /* 键入对应通道的输入捕抓功能 */
  QTMR_SetupInputCapture(TPAD_QTMR_BASEADDR, TPAD_QTMR_INPUT_CAPTURE_CHANNEL, TPAD_QTMR_CounterInputPin, false, true, kQTMR_RisingEdge);
  /* 使能中断 */
  EnableIRQ(TPAD_QTMR_IRQ_ID);
  /* 使能定时器比较中断 */
  QTMR_EnableInterrupts(TPAD_QTMR_BASEADDR, TPAD_QTMR_INPUT_CAPTURE_CHANNEL, kQTMR_EdgeInterruptEnable);
  /* 定时器计数器归0	 */
  TPAD_QTMR_BASEADDR->CHANNEL[TPAD_QTMR_INPUT_CAPTURE_CHANNEL].CNTR = 0;
  /* 启动输入通道以计算主源时钟的上升边缘 */
  QTMR_StartTimer(TPAD_QTMR_BASEADDR, TPAD_QTMR_INPUT_CAPTURE_CHANNEL, kQTMR_PriSrcRiseEdge);	
  
}

/****************************************
 *
 * 为电容按键放电
 * 清除定时器标志及计数
 *
 *****************************************/
static void TPAD_Reset(void)
{
  /* 定义gpio初始化配置结构体 */
  gpio_pin_config_t tap_config;
  
  /* 设置引脚的复用模式，配置为普通IO，不使用SION功能*/
  IOMUXC_SetPinMux(TPAD_IOMUXC, 0U);            
  
  /*设置引脚PAD属性*/
  IOMUXC_SetPinConfig(TPAD_IOMUXC, GPIO_PAD_CONFIG_DATA);
  
  /* TPAD，GPIO配置 */       
  tap_config.direction = kGPIO_DigitalOutput; //输出模式
  tap_config.outputLogic =  0;                //默认低电平
  tap_config.interruptMode = kGPIO_NoIntmode; //不使用中断
  
  /* 初始化 TPAD GPIO. */
  GPIO_PinInit(TPAD_GPIO, TPAD_GPIO_PIN, &tap_config);
  
  /*保持一小段时间低电平，保证放电完全*/
  Delay_ms(5);
  
  /*重新初始化定时器为输入捕抓功能*/
  TIMx_CHx_Cap_Init();
}


/****************************************************
 *
 * 得到定时器捕获值
 * 如果超时,则直接返回定时器的计数值.
 *
 *****************************************************/
static uint16_t TPAD_Get_Val(void)
{
  /* 先放电完全，并复位计数器 */	
  TPAD_Reset();
  
  /* 等待捕获上升沿 */
  while (!(qtmrIsrFlag))
  {
    /*循环读取捕获计较寄存器的值，如果超时则返回计数值。*/
    if(TPAD_QTMR_BASEADDR->CHANNEL[TPAD_QTMR_INPUT_CAPTURE_CHANNEL].CAPT>(TPAD_ARR_MAX_VAL-500))
      return TPAD_QTMR_BASEADDR->CHANNEL[TPAD_QTMR_INPUT_CAPTURE_CHANNEL].CAPT;  	
  }
  qtmrIsrFlag = false;
  return TPAD_QTMR_BASEADDR->CHANNEL[TPAD_QTMR_INPUT_CAPTURE_CHANNEL].CAPT;  
} 	

/****************************************************
 *
 * 读取n次,取最大值
 * n：连续获取的次数
 * 返回值：n次读数里面读到的最大读数值
 * 
 *****************************************************/
static uint16_t TPAD_Get_MaxVal(uint8_t n)
{
  uint16_t temp=0;
  uint16_t res=0;
  while(n--)
  {
    temp=TPAD_Get_Val();/*得到一次值*/
    if(temp>res)res=temp;
  };
  return res;
}  

/********************************************************
*
* 初始化触摸按键
* 获得空载的时候触摸按键的取值.
* 返回值:0,初始化成功;1,初始化失败
*
*********************************************************/
uint8_t TPAD_Init(void)
{
  uint16_t buf[10];
  uint32_t temp=0;
  uint8_t j,i;
  
  /*设定定时器预分频器目标时钟为：8.25MHz*/
  TIMx_CHx_Cap_Init();
  
  for(i=0;i<10;i++)/*连续读取10次*/
  {				 
    buf[i]=TPAD_Get_Val();
    Delay_ms(10);	    
  }				    
  for(i=0;i<9;i++)/*排序*/
  {
    for(j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])/*升序排列*/
      {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  temp=0;
  /*取中间的6个数据进行平均*/
  for(i=2;i<8;i++)
  {
    temp+=buf[i];
  }
  
  tpad_default_val=temp/6;	
  /* printf打印函数调试使用，用来确定阈值TPAD_GATE_VAL，在应用工程中应注释掉 */
  /*PRINTF("tpad_default_val:%d\r\n",tpad_default_val);	*/
  
  /*初始化遇到超过TPAD_ARR_MAX_VAL/2的数值,不正常!*/
  if(tpad_default_val>TPAD_ARR_MAX_VAL/2)
  {
    return 1;
  }
  
  return 0;		     	    					   
}
 

/*******************************************************************************
*
* 扫描触摸按键
* mode:0,不支持连续触发(按下一次必须松开才能按下一次);1,支持连续触发(可以一直按下)
* 返回值:0,没有按下;1,有按下;			
*
*******************************************************************************/
/*阈值：捕获时间必须大于(tpad_default_val + TPAD_GATE_VAL),才认为是有效触摸.*/
#define TPAD_GATE_VAL 	100	

uint8_t TPAD_Scan(uint8_t mode)
{
  /* 0,可以开始检测;>0,还不能开始检测*/	
  static uint8_t keyen=0;
  /*扫描结果*/
  uint8_t res=0;
  //默认采样次数为3次*/
  uint8_t sample=3;	
  //捕获值*/
  uint16_t rval;
  
  if(mode)
  {
    /*支持连按的时候，设置采样次数为6次*/
    sample=6;	
    /*支持连按*/	
    keyen=0;	  
  }	
  /* 获取当前捕获值(返回 sample 次扫描的最大值) */
  rval=TPAD_Get_MaxVal(sample); 	
  /* printf打印函数调试使用，用来确定阈值TPAD_GATE_VAL，在应用工程中应注释掉 */
  /*PRINTF("scan_rval=%d\n",rval);*/
  
  /*大于tpad_default_val+TPAD_GATE_VAL,且小于10倍tpad_default_val,则有效*/
  if(rval>(tpad_default_val+TPAD_GATE_VAL)&&rval<(10*tpad_default_val))
  {			
    /* keyen==0,有效 */		
    if(keyen==0)
    {
      res=1;		 
    }			
    keyen=3;/*至少要再过3次之后才能按键有效  */ 
  }
  
  if(keyen)
  {
    keyen--;		
  }		
  return res;
}	 


/*********************************************END OF FILE**********************/

