/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   ���ݰ���Ӧ�ú����ӿ�
  ******************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  i.MXRT1052������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
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

/*��ʱ��������ֵ*/
#define TPAD_ARR_MAX_VAL 	0XFFFF	

/*����û����ʱ��ʱ������ֵ*/
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
    /* ����жϱ�־ */
    QTMR_ClearStatusFlags(TPAD_QTMR_BASEADDR, TPAD_QTMR_INPUT_CAPTURE_CHANNEL, kQTMR_EdgeFlag);
    
    qtmrIsrFlag = true;

}

/***********************************
 *
 * ��ʱ�����벶������
 *
 ***********************************/
static void TIMx_CHx_Cap_Init(void)
{
  qtmr_config_t qtmrConfig;
  /* �������ŵĸ���ģʽ */
  IOMUXC_SetPinMux(
                   TPAD_QTMR_IOMUXC,    /* ����Ϊ��ʱ�����벶ץIO */
                   0U);                 /* ��ʹ��SION���� */																							 
  /* ����PAD�������� */  
  IOMUXC_SetPinConfig(TPAD_QTMR_IOMUXC,TMR_INPUT_PAD_CONFIG_DATA); 
  
  /* ��ȡϵͳĬ������ */
  QTMR_GetDefaultConfig(&qtmrConfig);
  /* ʹ��IP����ʱ�ӣ��˷�Ƶ */
  qtmrConfig.primarySource = kQTMR_ClockDivide_16;
  /* ָ����ʱ��ͨ����ʼ����ʱ�� */
  QTMR_Init(TPAD_QTMR_BASEADDR, TPAD_QTMR_INPUT_CAPTURE_CHANNEL, &qtmrConfig);
  /* �����Ӧͨ�������벶ץ���� */
  QTMR_SetupInputCapture(TPAD_QTMR_BASEADDR, TPAD_QTMR_INPUT_CAPTURE_CHANNEL, TPAD_QTMR_CounterInputPin, false, true, kQTMR_RisingEdge);
  /* ʹ���ж� */
  EnableIRQ(TPAD_QTMR_IRQ_ID);
  /* ʹ�ܶ�ʱ���Ƚ��ж� */
  QTMR_EnableInterrupts(TPAD_QTMR_BASEADDR, TPAD_QTMR_INPUT_CAPTURE_CHANNEL, kQTMR_EdgeInterruptEnable);
  /* ��ʱ����������0	 */
  TPAD_QTMR_BASEADDR->CHANNEL[TPAD_QTMR_INPUT_CAPTURE_CHANNEL].CNTR = 0;
  /* ��������ͨ���Լ�����Դʱ�ӵ�������Ե */
  QTMR_StartTimer(TPAD_QTMR_BASEADDR, TPAD_QTMR_INPUT_CAPTURE_CHANNEL, kQTMR_PriSrcRiseEdge);	
  
}

/****************************************
 *
 * Ϊ���ݰ����ŵ�
 * �����ʱ����־������
 *
 *****************************************/
static void TPAD_Reset(void)
{
  /* ����gpio��ʼ�����ýṹ�� */
  gpio_pin_config_t tap_config;
  
  /* �������ŵĸ���ģʽ������Ϊ��ͨIO����ʹ��SION����*/
  IOMUXC_SetPinMux(TPAD_IOMUXC, 0U);            
  
  /*��������PAD����*/
  IOMUXC_SetPinConfig(TPAD_IOMUXC, GPIO_PAD_CONFIG_DATA);
  
  /* TPAD��GPIO���� */       
  tap_config.direction = kGPIO_DigitalOutput; //���ģʽ
  tap_config.outputLogic =  0;                //Ĭ�ϵ͵�ƽ
  tap_config.interruptMode = kGPIO_NoIntmode; //��ʹ���ж�
  
  /* ��ʼ�� TPAD GPIO. */
  GPIO_PinInit(TPAD_GPIO, TPAD_GPIO_PIN, &tap_config);
  
  /*����һС��ʱ��͵�ƽ����֤�ŵ���ȫ*/
  Delay_ms(5);
  
  /*���³�ʼ����ʱ��Ϊ���벶ץ����*/
  TIMx_CHx_Cap_Init();
}


/****************************************************
 *
 * �õ���ʱ������ֵ
 * �����ʱ,��ֱ�ӷ��ض�ʱ���ļ���ֵ.
 *
 *****************************************************/
static uint16_t TPAD_Get_Val(void)
{
  /* �ȷŵ���ȫ������λ������ */	
  TPAD_Reset();
  
  /* �ȴ����������� */
  while (!(qtmrIsrFlag))
  {
    /*ѭ����ȡ����ƽϼĴ�����ֵ�������ʱ�򷵻ؼ���ֵ��*/
    if(TPAD_QTMR_BASEADDR->CHANNEL[TPAD_QTMR_INPUT_CAPTURE_CHANNEL].CAPT>(TPAD_ARR_MAX_VAL-500))
      return TPAD_QTMR_BASEADDR->CHANNEL[TPAD_QTMR_INPUT_CAPTURE_CHANNEL].CAPT;  	
  }
  qtmrIsrFlag = false;
  return TPAD_QTMR_BASEADDR->CHANNEL[TPAD_QTMR_INPUT_CAPTURE_CHANNEL].CAPT;  
} 	

/****************************************************
 *
 * ��ȡn��,ȡ���ֵ
 * n��������ȡ�Ĵ���
 * ����ֵ��n�ζ������������������ֵ
 * 
 *****************************************************/
static uint16_t TPAD_Get_MaxVal(uint8_t n)
{
  uint16_t temp=0;
  uint16_t res=0;
  while(n--)
  {
    temp=TPAD_Get_Val();/*�õ�һ��ֵ*/
    if(temp>res)res=temp;
  };
  return res;
}  

/********************************************************
*
* ��ʼ����������
* ��ÿ��ص�ʱ����������ȡֵ.
* ����ֵ:0,��ʼ���ɹ�;1,��ʼ��ʧ��
*
*********************************************************/
uint8_t TPAD_Init(void)
{
  uint16_t buf[10];
  uint32_t temp=0;
  uint8_t j,i;
  
  /*�趨��ʱ��Ԥ��Ƶ��Ŀ��ʱ��Ϊ��8.25MHz*/
  TIMx_CHx_Cap_Init();
  
  for(i=0;i<10;i++)/*������ȡ10��*/
  {				 
    buf[i]=TPAD_Get_Val();
    Delay_ms(10);	    
  }				    
  for(i=0;i<9;i++)/*����*/
  {
    for(j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])/*��������*/
      {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  temp=0;
  /*ȡ�м��6�����ݽ���ƽ��*/
  for(i=2;i<8;i++)
  {
    temp+=buf[i];
  }
  
  tpad_default_val=temp/6;	
  /* printf��ӡ��������ʹ�ã�����ȷ����ֵTPAD_GATE_VAL����Ӧ�ù�����Ӧע�͵� */
  /*PRINTF("tpad_default_val:%d\r\n",tpad_default_val);	*/
  
  /*��ʼ����������TPAD_ARR_MAX_VAL/2����ֵ,������!*/
  if(tpad_default_val>TPAD_ARR_MAX_VAL/2)
  {
    return 1;
  }
  
  return 0;		     	    					   
}
 

/*******************************************************************************
*
* ɨ�败������
* mode:0,��֧����������(����һ�α����ɿ����ܰ���һ��);1,֧����������(����һֱ����)
* ����ֵ:0,û�а���;1,�а���;			
*
*******************************************************************************/
/*��ֵ������ʱ��������(tpad_default_val + TPAD_GATE_VAL),����Ϊ����Ч����.*/
#define TPAD_GATE_VAL 	100	

uint8_t TPAD_Scan(uint8_t mode)
{
  /* 0,���Կ�ʼ���;>0,�����ܿ�ʼ���*/	
  static uint8_t keyen=0;
  /*ɨ����*/
  uint8_t res=0;
  //Ĭ�ϲ�������Ϊ3��*/
  uint8_t sample=3;	
  //����ֵ*/
  uint16_t rval;
  
  if(mode)
  {
    /*֧��������ʱ�����ò�������Ϊ6��*/
    sample=6;	
    /*֧������*/	
    keyen=0;	  
  }	
  /* ��ȡ��ǰ����ֵ(���� sample ��ɨ������ֵ) */
  rval=TPAD_Get_MaxVal(sample); 	
  /* printf��ӡ��������ʹ�ã�����ȷ����ֵTPAD_GATE_VAL����Ӧ�ù�����Ӧע�͵� */
  /*PRINTF("scan_rval=%d\n",rval);*/
  
  /*����tpad_default_val+TPAD_GATE_VAL,��С��10��tpad_default_val,����Ч*/
  if(rval>(tpad_default_val+TPAD_GATE_VAL)&&rval<(10*tpad_default_val))
  {			
    /* keyen==0,��Ч */		
    if(keyen==0)
    {
      res=1;		 
    }			
    keyen=3;/*����Ҫ�ٹ�3��֮����ܰ�����Ч  */ 
  }
  
  if(keyen)
  {
    keyen--;		
  }		
  return res;
}	 


/*********************************************END OF FILE**********************/

