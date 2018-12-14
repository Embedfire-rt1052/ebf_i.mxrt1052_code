/**
  ******************************************************************
  * @file    bsp_nand.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   NAND驱动
  ******************************************************************
  * @attention
  *
  * 实验平台:野火  i.MXRT1052开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************
  */
#include "fsl_debug_console.h"  
#include "fsl_iomuxc.h"
#include "fsl_semc.h"  
	
#include "pad_config.h"  
#include "./nand/bsp_nand.h"  
#include "board.h"
#include "clock_config.h"
#include "pin_mux.h"
#include "fsl_nand_flash.h"
#include "fsl_semc_nand_flash.h"

/*******************************************************************************
 * 宏
 ******************************************************************************/

/* 所有引脚均使用同样的PAD配置 */
#define NAND_PAD_CONFIG_DATA            (SRE_1_FAST_SLEW_RATE| \
                                            DSE_7_R0_7| \
                                            SPEED_3_MAX_200MHz| \
                                            ODE_0_OPEN_DRAIN_DISABLED| \
                                            PKE_1_PULL_KEEPER_ENABLED| \
                                            PUE_0_KEEPER_SELECTED| \
                                            PUS_0_100K_OHM_PULL_DOWN| \
                                            HYS_1_HYSTERESIS_ENABLED)   
    /* 配置说明 : */
    /* 转换速率: 转换速率快
        驱动强度: R0/7 
        带宽配置 : max(200MHz)
        开漏配置: 关闭 
        拉/保持器配置: 使能
        拉/保持器选择: 保持器
        上拉/下拉选择: 100K欧姆下拉(选择了保持器此配置无效)
        滞回器配置: 使能 */

/*******************************************************************************
 * 声明
 ******************************************************************************/
static void NAND_IOMUXC_MUX_Config(void);
static void NAND_IOMUXC_PAD_Config(void);

/**
* @brief  初始化NAND相关IOMUXC的MUX复用配置
* @param  无
* @retval 无
*/
static void NAND_IOMUXC_MUX_Config(void)
{
  /* 所有引脚均不开启SION功能 */
  /* DATA信号线DATA00~DATA15 */  
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_00_SEMC_DATA00, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_01_SEMC_DATA01, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_02_SEMC_DATA02, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_03_SEMC_DATA03, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_04_SEMC_DATA04, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_05_SEMC_DATA05, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_06_SEMC_DATA06, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_07_SEMC_DATA07, 0U);                                    

  /* ADDR信号线ADDR09~ADDR12 */                                     
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_18_SEMC_ADDR09, 0U);                                                                       
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_19_SEMC_ADDR11, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_20_SEMC_ADDR12, 0U); 

		/* BA1信号 */  
	IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_22_SEMC_BA1, 0U); 
  /* CSX0信号 */                                     
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_41_SEMC_CSX00, 0U);    

  /* RDY信号 */  
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_40_SEMC_RDY, 0U);                                      
 
}

/**
* @brief  初始化NAND相关IOMUXC的PAD属性配置
* @param  无
* @retval 无
*/
static void NAND_IOMUXC_PAD_Config(void)
{
  /* 所有引脚均使用同样的PAD配置 */
   /* DATA信号线DATA00~DATA15 */  
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_00_SEMC_DATA00, NAND_PAD_CONFIG_DATA);  
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_01_SEMC_DATA01, NAND_PAD_CONFIG_DATA);                            
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_02_SEMC_DATA02, NAND_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_03_SEMC_DATA03, NAND_PAD_CONFIG_DATA);                            
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_04_SEMC_DATA04, NAND_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_05_SEMC_DATA05, NAND_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_06_SEMC_DATA06, NAND_PAD_CONFIG_DATA);                            
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_07_SEMC_DATA07, NAND_PAD_CONFIG_DATA);                             
  
  /* ADDR信号线ADDR00~ADDR12 */                                         
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_18_SEMC_ADDR09, NAND_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_19_SEMC_ADDR11, NAND_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_20_SEMC_ADDR12, NAND_PAD_CONFIG_DATA);  
  
	/* BA1信号 */  
	IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_22_SEMC_BA1, NAND_PAD_CONFIG_DATA); 
  /* CSX0信号 */                        
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_41_SEMC_CSX00, NAND_PAD_CONFIG_DATA);  

  /* RDY信号 */  
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_40_SEMC_RDY, NAND_PAD_CONFIG_DATA);                               
}

void delayUs(uint32_t delay_us)
{
    uint32_t s_tickPerMicrosecond = CLOCK_GetFreq(kCLOCK_CpuClk) / 1000000;

    /* 确保这个这个值大于0*/
    if (!s_tickPerMicrosecond)
    {
        s_tickPerMicrosecond = 1;
    }
    delay_us = delay_us * s_tickPerMicrosecond;
    while (delay_us)
    {
        __NOP();
        delay_us --;
    }
}

semc_nand_config_t semcNandConfig = {
    .cePinMux = kSEMC_MUXCSX0,     /*!< 设置片选 */
    .axiAddress = EXAMPLE_SEMC_NAND_AXI_START_ADDRESS,  /*!< 基地址 */
    .axiMemsize_kbytes = 2 * 1024 * 1024,     /*!< AXI接口flash大小 8*1024*2*1024*1024 = 16Gb. */
    .ipgAddress = EXAMPLE_SEMC_NAND_IPG_START_ADDRESS,  /*!< The base address. */
    .ipgMemsize_kbytes = 2 * 1024 * 1024,     /*!< IPG接口flash大小 8*1024*2*1024*1024 = 16Gb. */    
    .rdyactivePolarity = kSEMC_RdyActiveLow,    /*!< RDY极性 */
    .arrayAddrOption = kSEMC_NandAddrOption_5byte_CA2RA3,
    .edoModeEnabled = false,                   /*!< 地址模式 */
    .columnAddrBitNum = kSEMC_NandColum_12bit,
    .burstLen = kSEMC_Nand_BurstLen1,        /*!< 突发长度 */
    .portSize = kSEMC_PortSize8Bit,         /*!< 接口位宽 */
    .timingConfig = NULL,
};

semc_mem_nand_config_t semcMemConfig = {
    .semcNandConfig = &semcNandConfig,
    .delayUS = delayUs,
    .onfiVersion = kNandOnfiVersion_1p0,
    .readyCheckOption = kNandReadyCheckOption_SR,
    .eccCheckType = kNandEccCheckType_DeviceECC,
};

nand_config_t nandConfig = {
    .memControlConfig = (void *)&semcMemConfig,
    .driverBaseAddr = (void *)EXAMPLE_SEMC,
};

void BOARD_InitNand(void)
{	
	semc_config_t config;

	NAND_IOMUXC_MUX_Config();
	NAND_IOMUXC_PAD_Config();
//	/* 设置时钟为132Mhz */
//	CLOCK_SetMux(kCLOCK_SemcMux, 1);
//	CLOCK_SetDiv(kCLOCK_SemcDiv, 2);	 

	/* 参数置零 */
	memset(&config, 0, sizeof(semc_config_t));
	/* 初始化SEMC. */
	SEMC_GetDefaultConfig(&config);
	SEMC_Init(SEMC, &config);
	/* 为NAND闪存控制器使用设置SEMC时钟源 */
	semcMemConfig.clkSrc_Hz = EXAMPLE_SEMC_CLK_FREQ;
}
/*********************************************END OF FILE**********************/
