/**
  ******************************************************************
  * @file    logictest.c
  * @author  fire
  * @version V1.1
  * @date    2018-xx-xx
  * @brief   led应用函数接口
  ******************************************************************
  * @attention
  *
  * 实验平台:野火  i.MXRT1052开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************
  */
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"  
  
#include "pad_config.h"  
#include "./logictest/logictest.h"   

//#define TEST_LOGIC_IGNORE i==11||i==20||i==21||i==24||i==25||i==28||i==29||i==30
#define TEST_LOGIC_IGNORE i==11||i==15||i==20||i==25||i==28||i==29||i==30
/*******************************************************************************
 * 宏
 ******************************************************************************/
/* 所有引脚均使用同样的PAD配置 */
#define LogicData_PAD_CONFIG_DATA            (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_2_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_1_PULL_KEEPER_ENABLED| \
                                        PUE_1_PULL_SELECTED| \
                                        PUS_3_22K_OHM_PULL_UP| \
                                        HYS_0_HYSTERESIS_DISABLED)   
    /* 配置说明 : */
    /* 转换速率: 转换速率慢
      驱动强度: R0/6 
      带宽配置 : medium(100MHz)
      开漏配置: 关闭 
      拉/保持器配置: 关闭
      拉/保持器选择: 保持器（上面已关闭，配置无效）
      上拉/下拉选择: 100K欧姆下拉（上面已关闭，配置无效）
      滞回器配置: 关闭 */     

#define LogicIn_PAD_CONFIG_DATA         (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_0_OUTPUT_DRIVER_DISABLED| \
                                        SPEED_2_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_1_PULL_KEEPER_ENABLED| \
                                        PUE_1_PULL_SELECTED| \
                                        PUS_3_22K_OHM_PULL_UP| \
                                        HYS_1_HYSTERESIS_ENABLED)   
    /* 配置说明 : */
    /* 转换速率: 转换速率慢
      驱动强度: 关闭
      速度配置 : medium(100MHz)
      开漏配置: 关闭 
      拉/保持器配置: 使能
      拉/保持器选择: 上下拉
      上拉/下拉选择: 22K欧姆上拉
      滞回器配置: 开启 （仅输入时有效，施密特触发器，使能后可以过滤输入噪声）*/
			
/*******************************************************************************
 * 声明
 ******************************************************************************/
static void LogicTest_IOMUXC_MUX_Config(void);
static void LogicTest_IOMUXC_PAD_Config(void);
static void LogicTest_GPIO_Mode_Config(void);

/**
* @brief  初始化LED相关IOMUXC的MUX复用配置
* @param  无
* @retval 无
*/
static void LogicTest_IOMUXC_MUX_Config(void)
{ 
  /* 设置引脚的复用模式为GPIO，不使用SION功能 */
  IOMUXC_SetPinMux(Logic_595_SER_IOMUXC, 0U);
  IOMUXC_SetPinMux(Logic_595_LCLK_IOMUXC, 0U);
	IOMUXC_SetPinMux(Logic_595_SCLK_IOMUXC, 0U);
	
  /* 逻辑输入引脚，使用同样的IOMUXC MUX配置 */  
  IOMUXC_SetPinMux(LogicIn0_IOMUXC, 0U); 
  IOMUXC_SetPinMux(LogicIn1_IOMUXC, 0U);  
  IOMUXC_SetPinMux(LogicIn2_IOMUXC, 0U);
	IOMUXC_SetPinMux(LogicIn3_IOMUXC, 0U); 
  IOMUXC_SetPinMux(LogicIn4_IOMUXC, 0U);  
  IOMUXC_SetPinMux(LogicIn5_IOMUXC, 0U);
	IOMUXC_SetPinMux(LogicIn6_IOMUXC, 0U); 
  IOMUXC_SetPinMux(LogicIn7_IOMUXC, 0U);  
  IOMUXC_SetPinMux(LogicIn8_IOMUXC, 0U);
	IOMUXC_SetPinMux(LogicIn9_IOMUXC, 0U);
  IOMUXC_SetPinMux(LogicIn10_IOMUXC, 0U); 
//  IOMUXC_SetPinMux(LogicIn11_IOMUXC, 0U);  
  IOMUXC_SetPinMux(LogicIn12_IOMUXC, 0U);
	IOMUXC_SetPinMux(LogicIn13_IOMUXC, 0U); 
  IOMUXC_SetPinMux(LogicIn14_IOMUXC, 0U);  
  IOMUXC_SetPinMux(LogicIn15_IOMUXC, 0U);
	IOMUXC_SetPinMux(LogicIn16_IOMUXC, 0U); 
  IOMUXC_SetPinMux(LogicIn17_IOMUXC, 0U);  
  IOMUXC_SetPinMux(LogicIn18_IOMUXC, 0U);	
	IOMUXC_SetPinMux(LogicIn19_IOMUXC, 0U);
  IOMUXC_SetPinMux(LogicIn20_IOMUXC, 0U); 
  IOMUXC_SetPinMux(LogicIn21_IOMUXC, 0U);  
  IOMUXC_SetPinMux(LogicIn22_IOMUXC, 0U);
	IOMUXC_SetPinMux(LogicIn23_IOMUXC, 0U); 
  IOMUXC_SetPinMux(LogicIn24_IOMUXC, 0U);  
  IOMUXC_SetPinMux(LogicIn25_IOMUXC, 0U);
	IOMUXC_SetPinMux(LogicIn26_IOMUXC, 0U); 
  IOMUXC_SetPinMux(LogicIn27_IOMUXC, 0U);  
//  IOMUXC_SetPinMux(LogicIn28_IOMUXC, 0U);
//	IOMUXC_SetPinMux(LogicIn29_IOMUXC, 0U);
  IOMUXC_SetPinMux(LogicIn30_IOMUXC, 0U); 
//	IOMUXC_SetPinMux(LogicIn31_IOMUXC, 0U);	
}

/**
* @brief  初始化LED相关IOMUXC的MUX复用配置
* @param  无
* @retval 无
*/
static void LogicTest_IOMUXC_PAD_Config(void)
{
  /* 设置引脚的复用模式为GPIO，不使用SION功能 */
  IOMUXC_SetPinConfig(Logic_595_SER_IOMUXC, LogicData_PAD_CONFIG_DATA);
  IOMUXC_SetPinConfig(Logic_595_LCLK_IOMUXC, LogicData_PAD_CONFIG_DATA);
	IOMUXC_SetPinConfig(Logic_595_SCLK_IOMUXC, LogicData_PAD_CONFIG_DATA);
	
  /* 逻辑输入引脚，使用同样的IOMUXC PAD配置 */  
  IOMUXC_SetPinConfig(LogicIn0_IOMUXC, LogicIn_PAD_CONFIG_DATA);
  IOMUXC_SetPinConfig(LogicIn1_IOMUXC, LogicIn_PAD_CONFIG_DATA); 
  IOMUXC_SetPinConfig(LogicIn2_IOMUXC, LogicIn_PAD_CONFIG_DATA);
	IOMUXC_SetPinConfig(LogicIn3_IOMUXC, LogicIn_PAD_CONFIG_DATA);
  IOMUXC_SetPinConfig(LogicIn4_IOMUXC, LogicIn_PAD_CONFIG_DATA); 
  IOMUXC_SetPinConfig(LogicIn5_IOMUXC, LogicIn_PAD_CONFIG_DATA);
	IOMUXC_SetPinConfig(LogicIn6_IOMUXC, LogicIn_PAD_CONFIG_DATA); 
  IOMUXC_SetPinConfig(LogicIn7_IOMUXC, LogicIn_PAD_CONFIG_DATA);  
  IOMUXC_SetPinConfig(LogicIn8_IOMUXC, LogicIn_PAD_CONFIG_DATA);
	IOMUXC_SetPinConfig(LogicIn9_IOMUXC, LogicIn_PAD_CONFIG_DATA);
  IOMUXC_SetPinConfig(LogicIn10_IOMUXC, LogicIn_PAD_CONFIG_DATA); 
//  IOMUXC_SetPinConfig(LogicIn11_IOMUXC, LogicIn_PAD_CONFIG_DATA);  
  IOMUXC_SetPinConfig(LogicIn12_IOMUXC, LogicIn_PAD_CONFIG_DATA);
	IOMUXC_SetPinConfig(LogicIn13_IOMUXC, LogicIn_PAD_CONFIG_DATA); 
  IOMUXC_SetPinConfig(LogicIn14_IOMUXC, LogicIn_PAD_CONFIG_DATA);  
  IOMUXC_SetPinConfig(LogicIn15_IOMUXC, LogicIn_PAD_CONFIG_DATA);
	IOMUXC_SetPinConfig(LogicIn16_IOMUXC, LogicIn_PAD_CONFIG_DATA); 
  IOMUXC_SetPinConfig(LogicIn17_IOMUXC, LogicIn_PAD_CONFIG_DATA);  
  IOMUXC_SetPinConfig(LogicIn18_IOMUXC, LogicIn_PAD_CONFIG_DATA);	
	IOMUXC_SetPinConfig(LogicIn19_IOMUXC, LogicIn_PAD_CONFIG_DATA);
  IOMUXC_SetPinConfig(LogicIn20_IOMUXC, LogicIn_PAD_CONFIG_DATA); 
  IOMUXC_SetPinConfig(LogicIn21_IOMUXC, LogicIn_PAD_CONFIG_DATA);  
  IOMUXC_SetPinConfig(LogicIn22_IOMUXC, LogicIn_PAD_CONFIG_DATA);
	IOMUXC_SetPinConfig(LogicIn23_IOMUXC, LogicIn_PAD_CONFIG_DATA); 
  IOMUXC_SetPinConfig(LogicIn24_IOMUXC, LogicIn_PAD_CONFIG_DATA);  
  IOMUXC_SetPinConfig(LogicIn25_IOMUXC, LogicIn_PAD_CONFIG_DATA);
	IOMUXC_SetPinConfig(LogicIn26_IOMUXC, LogicIn_PAD_CONFIG_DATA); 
  IOMUXC_SetPinConfig(LogicIn27_IOMUXC, LogicIn_PAD_CONFIG_DATA);  
//  IOMUXC_SetPinConfig(LogicIn28_IOMUXC, LogicIn_PAD_CONFIG_DATA);
//	IOMUXC_SetPinConfig(LogicIn29_IOMUXC, LogicIn_PAD_CONFIG_DATA);
  IOMUXC_SetPinConfig(LogicIn30_IOMUXC, LogicIn_PAD_CONFIG_DATA); 
}


 /**
  * @brief  初始化LED相关的GPIO模式
  * @param  无
  * @retval 无
  */
static void LogicTest_GPIO_Mode_Config(void)
{     
  /* 定义gpio初始化配置结构体 */
  gpio_pin_config_t LogicData_config;      
	gpio_pin_config_t LogicIn_config;  
  
   /** 核心板的LogicData，GPIO配置 **/       
  LogicData_config.direction = kGPIO_DigitalOutput; //输出模式
  LogicData_config.outputLogic =  0;                //默认高电平
  LogicData_config.interruptMode = kGPIO_NoIntmode; //不使用中断
  
  /* 初始化 LED GPIO. */
  GPIO_PinInit(Logic_595_SER_GPIO, Logic_595_SER_GPIO_PIN, &LogicData_config);
	GPIO_PinInit(Logic_595_SCLK_GPIO, Logic_595_SCLK_GPIO_PIN, &LogicData_config);
	GPIO_PinInit(Logic_595_LCLK_GPIO, Logic_595_LCLK_GPIO_PIN, &LogicData_config);  

  
	   /** 核心板的LogicData，GPIO配置 **/       
  LogicIn_config.direction = kGPIO_DigitalInput; //输入模式
  LogicIn_config.interruptMode = kGPIO_NoIntmode; //不使用中断
	/* 配置逻辑输入引脚 */
  GPIO_PinInit(LogicIn0_GPIO, LogicIn0_GPIO_PIN, &LogicIn_config);
  GPIO_PinInit(LogicIn1_GPIO, LogicIn1_GPIO_PIN, &LogicIn_config);
  GPIO_PinInit(LogicIn2_GPIO, LogicIn2_GPIO_PIN, &LogicIn_config);
  GPIO_PinInit(LogicIn3_GPIO, LogicIn3_GPIO_PIN, &LogicIn_config);
  GPIO_PinInit(LogicIn4_GPIO, LogicIn4_GPIO_PIN, &LogicIn_config);
  GPIO_PinInit(LogicIn5_GPIO, LogicIn5_GPIO_PIN, &LogicIn_config);
  GPIO_PinInit(LogicIn6_GPIO, LogicIn6_GPIO_PIN, &LogicIn_config);
  GPIO_PinInit(LogicIn7_GPIO, LogicIn7_GPIO_PIN, &LogicIn_config);
  GPIO_PinInit(LogicIn8_GPIO, LogicIn8_GPIO_PIN, &LogicIn_config);
  GPIO_PinInit(LogicIn9_GPIO, LogicIn9_GPIO_PIN, &LogicIn_config);
  GPIO_PinInit(LogicIn10_GPIO, LogicIn10_GPIO_PIN, &LogicIn_config);
//  GPIO_PinInit(LogicIn11_GPIO, LogicIn11_GPIO_PIN, &LogicIn_config);
  GPIO_PinInit(LogicIn12_GPIO, LogicIn12_GPIO_PIN, &LogicIn_config);
  GPIO_PinInit(LogicIn13_GPIO, LogicIn13_GPIO_PIN, &LogicIn_config);
  GPIO_PinInit(LogicIn14_GPIO, LogicIn14_GPIO_PIN, &LogicIn_config);
  GPIO_PinInit(LogicIn15_GPIO, LogicIn15_GPIO_PIN, &LogicIn_config);
  GPIO_PinInit(LogicIn16_GPIO, LogicIn16_GPIO_PIN, &LogicIn_config);
  GPIO_PinInit(LogicIn17_GPIO, LogicIn17_GPIO_PIN, &LogicIn_config);
  GPIO_PinInit(LogicIn18_GPIO, LogicIn18_GPIO_PIN, &LogicIn_config);
  GPIO_PinInit(LogicIn19_GPIO, LogicIn19_GPIO_PIN, &LogicIn_config);
  GPIO_PinInit(LogicIn20_GPIO, LogicIn20_GPIO_PIN, &LogicIn_config);
  GPIO_PinInit(LogicIn21_GPIO, LogicIn21_GPIO_PIN, &LogicIn_config);
  GPIO_PinInit(LogicIn22_GPIO, LogicIn22_GPIO_PIN, &LogicIn_config);
  GPIO_PinInit(LogicIn23_GPIO, LogicIn23_GPIO_PIN, &LogicIn_config);
  GPIO_PinInit(LogicIn24_GPIO, LogicIn24_GPIO_PIN, &LogicIn_config);
  GPIO_PinInit(LogicIn25_GPIO, LogicIn25_GPIO_PIN, &LogicIn_config);
  GPIO_PinInit(LogicIn26_GPIO, LogicIn26_GPIO_PIN, &LogicIn_config);
  GPIO_PinInit(LogicIn27_GPIO, LogicIn27_GPIO_PIN, &LogicIn_config);
//  GPIO_PinInit(LogicIn28_GPIO, LogicIn28_GPIO_PIN, &LogicIn_config);
//  GPIO_PinInit(LogicIn29_GPIO, LogicIn29_GPIO_PIN, &LogicIn_config);
  GPIO_PinInit(LogicIn30_GPIO, LogicIn30_GPIO_PIN, &LogicIn_config);

}

/**
  * @brief  初始化控制LED的IO
  * @param  无
  * @retval 无
  */
void LogicTest_GPIO_Config(void)
{
  /* 初始化GPIO复用、属性、模式 */
  LogicTest_IOMUXC_MUX_Config();
  LogicTest_IOMUXC_PAD_Config();
  LogicTest_GPIO_Mode_Config();
}


extern void delay(uint32_t count);

void LogicOutByte(uint8_t data)
{
  for(uint8_t i = 0; i < 8; i++)
  {
    if((data << i) & 0x80)
    {
      Logic_SER(1);
    }
    else
    {
      Logic_SER(0);
    }
    Logic_SCLK(1);
		CPU_TS_Tmr_Delay_US(Logic_DELAY_COUNT);
    Logic_SCLK(0);
  }
  Logic_LCLK(1);
	CPU_TS_Tmr_Delay_US(Logic_DELAY_COUNT);
  Logic_LCLK(0);
}

void LogicOutWord(uint32_t data)
{
  for(uint8_t i = 0; i < 32; i++)
  {
    if((data << i) & 0x80000000)
    {
      Logic_SER(1);
    }
    else
    {
      Logic_SER(0);
    }
    Logic_SCLK(0);
		CPU_TS_Tmr_Delay_US(Logic_DELAY_COUNT);
    Logic_SCLK(1);
		
  }
  Logic_LCLK(0);
	CPU_TS_Tmr_Delay_US(Logic_DELAY_COUNT);
  Logic_LCLK(1);
}

uint8_t LogicInStatus(uint8_t InNum)
{
	uint8_t state;
	CPU_TS_Tmr_Delay_US(10000);
	switch(InNum)
	{
		case 0:
				state = GPIO_PinRead(LogicIn0_GPIO,LogicIn0_GPIO_PIN);
				break;
		case 1:
				state = GPIO_PinRead(LogicIn1_GPIO,LogicIn1_GPIO_PIN);
				break;
		case 2:
				state = GPIO_PinRead(LogicIn2_GPIO,LogicIn2_GPIO_PIN);
				break;		
		case 3:
				state = GPIO_PinRead(LogicIn3_GPIO,LogicIn3_GPIO_PIN);
				break;
		case 4:
				state = GPIO_PinRead(LogicIn4_GPIO,LogicIn4_GPIO_PIN);
				break;
		case 5:
				state = GPIO_PinRead(LogicIn5_GPIO,LogicIn5_GPIO_PIN);
				break;
		case 6:
				state = GPIO_PinRead(LogicIn6_GPIO,LogicIn6_GPIO_PIN);
				break;		
		case 7:
				state = GPIO_PinRead(LogicIn7_GPIO,LogicIn7_GPIO_PIN);
				break;
		case 8:
				state = GPIO_PinRead(LogicIn8_GPIO,LogicIn8_GPIO_PIN);
				break;
		case 9:
				state = GPIO_PinRead(LogicIn9_GPIO,LogicIn9_GPIO_PIN);
				break;
		case 10:
				state = GPIO_PinRead(LogicIn10_GPIO,LogicIn10_GPIO_PIN);
				break;
		case 11:
				state = GPIO_PinRead(LogicIn11_GPIO,LogicIn11_GPIO_PIN);
				break;
		case 12:
				state = GPIO_PinRead(LogicIn12_GPIO,LogicIn12_GPIO_PIN);
				break;		
		case 13:
				state = GPIO_PinRead(LogicIn13_GPIO,LogicIn13_GPIO_PIN);
				break;
		case 14:
				state = GPIO_PinRead(LogicIn14_GPIO,LogicIn14_GPIO_PIN);
				break;
		case 15:
				state = GPIO_PinRead(LogicIn15_GPIO,LogicIn15_GPIO_PIN);
				break;
		case 16:
				state = GPIO_PinRead(LogicIn16_GPIO,LogicIn16_GPIO_PIN);
				break;		
		case 17:
				state = GPIO_PinRead(LogicIn17_GPIO,LogicIn17_GPIO_PIN);
				break;
		case 18:
				state = GPIO_PinRead(LogicIn18_GPIO,LogicIn18_GPIO_PIN);
				break;
		case 19:
				state = GPIO_PinRead(LogicIn19_GPIO,LogicIn19_GPIO_PIN);
				break;
		case 20:
				state = GPIO_PinRead(LogicIn20_GPIO,LogicIn20_GPIO_PIN);
				break;
		case 21:
				state = GPIO_PinRead(LogicIn21_GPIO,LogicIn21_GPIO_PIN);
				break;
		case 22:
				state = GPIO_PinRead(LogicIn22_GPIO,LogicIn22_GPIO_PIN);
				break;		
		case 23:
				state = GPIO_PinRead(LogicIn23_GPIO,LogicIn23_GPIO_PIN);
				break;
		case 24:
				state = GPIO_PinRead(LogicIn24_GPIO,LogicIn24_GPIO_PIN);
				break;
		case 25:
				state = GPIO_PinRead(LogicIn25_GPIO,LogicIn25_GPIO_PIN);
				break;
		case 26:
			state = GPIO_PinRead(LogicIn26_GPIO,LogicIn26_GPIO_PIN);
				break;		
		case 27:
				state = GPIO_PinRead(LogicIn27_GPIO,LogicIn27_GPIO_PIN);
				break;
		case 28:
				state = GPIO_PinRead(LogicIn28_GPIO,LogicIn28_GPIO_PIN);
				break;
		case 29:
				state = GPIO_PinRead(LogicIn29_GPIO,LogicIn29_GPIO_PIN);
				break;
		case 30:
				state = GPIO_PinRead(LogicIn30_GPIO,LogicIn30_GPIO_PIN);
				break;
//	case 31:
//			state = GPIO_PinRead(LogicIn28_GPIO,LogicIn31_GPIO_PIN);
//			break;
		default:state = 0xff;	
				break;		
	}
	return state;
}

uint8_t logicTestInOut(void)
{
		uint8_t i;
		PRINTF("Test Out 0x00000000\r\n");
		LogicOutWord(0);
		for(i=0;i<31;i++)
		{
			if(TEST_LOGIC_IGNORE)
			{
				continue;
			}
			else if(LogicInStatus(i)!=0)
			{
				PRINTF("LogicIn%d level=%d\r\n",i,LogicInStatus(i));
				return 1;
			}
		}
		PRINTF("Test Out 0XFFFFFFFF\r\n");
		LogicOutWord(0XFFFFFFFF);
		for(i=0;i<31;i++)
		{
			if(TEST_LOGIC_IGNORE)
			{
				continue;
			}
			else if(LogicInStatus(i)!=1)
			{
				PRINTF("LogicIn%d level=%d\r\n",i,LogicInStatus(i));
				return 1;
			}
		}
		PRINTF("Test Out 0XAAAAAAAA\r\n");
		LogicOutWord(0Xaaaaaaaa);
		for(i=0;i<31;i++)
		{
			if(TEST_LOGIC_IGNORE)
			{
				continue;
			}
			if(i%2)
			{
				if(LogicInStatus(i)!=1)
				{
					PRINTF("LogicIn%d level=%d\r\n",i,LogicInStatus(i));
					return 1;
				}
			}
			else
			{
				if(LogicInStatus(i)!=0)
				{
					PRINTF("LogicIn%d level=%d\r\n",i,LogicInStatus(i));
					return 1;
				}
			}			
		}
		PRINTF("Test Out 0X55555555\r\n");
		LogicOutWord(0X55555555);
		for(i=0;i<31;i++)
		{
			if(TEST_LOGIC_IGNORE)
			{
				continue;
			}
			if(i%2)
			{
				if(LogicInStatus(i)!=0)
				{
					PRINTF("LogicIn%d level=%d\r\n",i,LogicInStatus(i));
					return 1;
				}
			}
			else
			{
				if(LogicInStatus(i)!=1)
				{
					PRINTF("LogicIn%d level=%d\r\n",i,LogicInStatus(i));
					return 1;
				}
			}			
		}		
		return 0;
}

/*********************************************END OF FILE**********************/
