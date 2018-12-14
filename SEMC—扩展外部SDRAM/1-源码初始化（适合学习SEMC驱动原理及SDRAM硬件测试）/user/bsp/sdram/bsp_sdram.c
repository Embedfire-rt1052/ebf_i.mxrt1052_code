/**
  ******************************************************************
  * @file    bsp_sdram.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   SDRAM驱动
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
#include "./sdram/bsp_sdram.h"  

/*******************************************************************************
 * 宏
 ******************************************************************************/

/* 所有引脚均使用同样的PAD配置 */
#define SDRAM_PAD_CONFIG_DATA            (SRE_1_FAST_SLEW_RATE| \
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
static void SDRAM_IOMUXC_MUX_Config(void);
static void SDRAM_IOMUXC_PAD_Config(void);
static status_t SDRAM_SEMC_Config(void);


/**
* @brief  初始化SDRAM相关IOMUXC的MUX复用配置
* @param  无
* @retval 无
*/
static void SDRAM_IOMUXC_MUX_Config(void)
{
  /* 除DQS信号外，其余引脚均可不开启SION功能 */
  /* DATA信号线DATA00~DATA15 */  
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_00_SEMC_DATA00, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_01_SEMC_DATA01, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_02_SEMC_DATA02, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_03_SEMC_DATA03, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_04_SEMC_DATA04, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_05_SEMC_DATA05, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_06_SEMC_DATA06, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_07_SEMC_DATA07, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_30_SEMC_DATA08, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_31_SEMC_DATA09, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_32_SEMC_DATA10, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_33_SEMC_DATA11, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_34_SEMC_DATA12, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_35_SEMC_DATA13, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_36_SEMC_DATA14, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_37_SEMC_DATA15, 0U);  
  
  /* DQS信号线，必须使能SION功能 */  
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_39_SEMC_DQS, 1U);  

  /* ADDR信号线ADDR00~ADDR12 */  
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_09_SEMC_ADDR00, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_10_SEMC_ADDR01, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_11_SEMC_ADDR02, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_12_SEMC_ADDR03, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_13_SEMC_ADDR04, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_14_SEMC_ADDR05, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_15_SEMC_ADDR06, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_16_SEMC_ADDR07, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_17_SEMC_ADDR08, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_18_SEMC_ADDR09, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_23_SEMC_ADDR10, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_19_SEMC_ADDR11, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_20_SEMC_ADDR12, 0U); 

  /* BA0、BA1信号 */  
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_21_SEMC_BA0, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_22_SEMC_BA1, 0U);  

  /* RAS、CAS信号 */  
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_25_SEMC_RAS, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_24_SEMC_CAS, 0U);   

  /* WE、CS0信号 */  
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_28_SEMC_WE, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_29_SEMC_CS0, 0U);    

  /* DM0、DM1信号 */  
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_08_SEMC_DM00, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_38_SEMC_DM01, 0U);    
 
  /* CLK、CKE信号 */  
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_26_SEMC_CLK, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_27_SEMC_CKE, 0U);  
}

/**
* @brief  初始化SDRAM相关IOMUXC的PAD属性配置
* @param  无
* @retval 无
*/
static void SDRAM_IOMUXC_PAD_Config(void)
{
  /* 所有引脚均使用同样的PAD配置 */
   /* DATA信号线DATA00~DATA15 */  
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_00_SEMC_DATA00, SDRAM_PAD_CONFIG_DATA);  
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_01_SEMC_DATA01, SDRAM_PAD_CONFIG_DATA);                            
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_02_SEMC_DATA02, SDRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_03_SEMC_DATA03, SDRAM_PAD_CONFIG_DATA);                            
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_04_SEMC_DATA04, SDRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_05_SEMC_DATA05, SDRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_06_SEMC_DATA06, SDRAM_PAD_CONFIG_DATA);                            
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_07_SEMC_DATA07, SDRAM_PAD_CONFIG_DATA);  
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_30_SEMC_DATA08, SDRAM_PAD_CONFIG_DATA);         
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_31_SEMC_DATA09, SDRAM_PAD_CONFIG_DATA);                            
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_32_SEMC_DATA10, SDRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_33_SEMC_DATA11, SDRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_34_SEMC_DATA12, SDRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_35_SEMC_DATA13, SDRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_36_SEMC_DATA14, SDRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_37_SEMC_DATA15, SDRAM_PAD_CONFIG_DATA);                             
  
  /* DQS信号线 */
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_39_SEMC_DQS, SDRAM_PAD_CONFIG_DATA);    
  
  /* ADDR信号线ADDR00~ADDR12 */               
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_09_SEMC_ADDR00, SDRAM_PAD_CONFIG_DATA);                            
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_10_SEMC_ADDR01, SDRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_11_SEMC_ADDR02, SDRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_12_SEMC_ADDR03, SDRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_13_SEMC_ADDR04, SDRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_14_SEMC_ADDR05, SDRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_15_SEMC_ADDR06, SDRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_16_SEMC_ADDR07, SDRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_17_SEMC_ADDR08, SDRAM_PAD_CONFIG_DATA);                            
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_18_SEMC_ADDR09, SDRAM_PAD_CONFIG_DATA);                            
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_23_SEMC_ADDR10, SDRAM_PAD_CONFIG_DATA);  
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_19_SEMC_ADDR11, SDRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_20_SEMC_ADDR12, SDRAM_PAD_CONFIG_DATA);  
  
  /* BA0、BA1信号 */  
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_21_SEMC_BA0, SDRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_22_SEMC_BA1, SDRAM_PAD_CONFIG_DATA);                            
  
  /* RAS、CAS信号 */  
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_25_SEMC_RAS, SDRAM_PAD_CONFIG_DATA);                          
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_24_SEMC_CAS, SDRAM_PAD_CONFIG_DATA);  
  
  /* WE、CS0信号 */  
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_28_SEMC_WE, SDRAM_PAD_CONFIG_DATA);                            
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_29_SEMC_CS0, SDRAM_PAD_CONFIG_DATA);  

  /* DM0、DM1信号 */  
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_08_SEMC_DM00, SDRAM_PAD_CONFIG_DATA);                                     
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_38_SEMC_DM01, SDRAM_PAD_CONFIG_DATA); 

  /* CLK、CKE信号 */  
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_26_SEMC_CLK, SDRAM_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_27_SEMC_CKE, SDRAM_PAD_CONFIG_DATA);    
}

/**
* @brief  初始化SDRAM相关的SEMC配置
* @param  无
* @retval 无
*/
static status_t SDRAM_SEMC_Config(void)
{  
    semc_config_t config;
    semc_sdram_config_t sdramconfig;
    uint32_t clockFrq = EXAMPLE_SEMC_CLK_FREQ;

    /* 结构体的内容全部清零 */
    memset(&config, 0, sizeof(semc_config_t));
    memset(&sdramconfig, 0, sizeof(semc_sdram_config_t));

    /* 使用默认配置初始化 SEMC. */
    SEMC_GetDefaultConfig(&config);
    
    /* ！注意！此处必须使用这个模式，
       否则SDRAM驱动时钟不能跑到高频率 */
    config.dqsMode = kSEMC_Loopbackdqspad;
    SEMC_Init(SEMC, &config);

    /* 配置SDRAM */
    /* 选择片选引脚，本应用使用CS0，此配置无效 */
    sdramconfig.csxPinMux = kSEMC_MUXCSX0;    
    /* 本SDRAM映射的基地址 */
    sdramconfig.address = 0x80000000;       
    /* SDRAM的容量大小，单位为KBytes, 32MB = 32*1024*1KBytes*/
    sdramconfig.memsize_kbytes = 32 * 1024; 
    /* SDRAM的数据线宽度 */
    sdramconfig.portSize = kSEMC_PortSize16Bit;
    /* 突发读写的最大长度 */
    sdramconfig.burstLen = kSEMC_Sdram_BurstLen8;
    /* 列地址宽度 */
    sdramconfig.columnAddrBitNum = kSEMC_SdramColunm_9bit;
    /* CAS Latency */
    sdramconfig.casLatency = kSEMC_LatencyTwo;
    /* 预充电至行有效的时间 Trp */
    sdramconfig.tPrecharge2Act_Ns = 15;   
    /* 行有效至读写的时间 Trcd */
    sdramconfig.tAct2ReadWrite_Ns = 15;   
    /* 刷新恢复时间，使用Trfc、Txsr中较大的一个值 */
    sdramconfig.tRefreshRecovery_Ns = 67; 
    /* 写恢复时间，2 个Tclk时钟 */
    sdramconfig.tWriteRecovery_Ns = 2 * (1000000000 / clockFrq);    
    /* CKE off 的最小时间，1 个Tclk时钟 */
    sdramconfig.tCkeOff_Ns = 1 * (1000000000 / clockFrq);
    /* 行有效至预充电时间，Tras */
    sdramconfig.tAct2Prechage_Ns = 42; 
    /* 自刷新的恢复时间，取tRefreshRecovery_Ns同样的值 */
    sdramconfig.tSelfRefRecovery_Ns = 67;
    /* 两个刷新命令之间的时间 */
    sdramconfig.tRefresh2Refresh_Ns = 60;
    /* 两个行有效命令之间的时间 */
    sdramconfig.tAct2Act_Ns = 60;
    /* 分频器定时周期，160个Tclk时钟 */
    sdramconfig.tPrescalePeriod_Ns = 160 * (1000000000 / clockFrq); 
    /* 刷新命令的周期，即多久刷新一行，64ms/8192  */
    sdramconfig.refreshPeriod_nsPerRow = 64 * 1000000 / 8192; 
    /* 紧急刷新阈值 */
    sdramconfig.refreshUrgThreshold = sdramconfig.refreshPeriod_nsPerRow;
    /* 刷新命令的突发长度 */
    sdramconfig.refreshBurstLen = 1;
    
    /* 使用以上配置初始化，并使用CS0，即用在Region 0区域 */
    return SEMC_ConfigureSDRAM(SEMC, kSEMC_SDRAM_CS0, &sdramconfig, clockFrq);
}

/**
* @brief  初始化SDRAM
* @param  无
* @retval 无
*/
status_t SDRAM_Init(void)
{  
  /* 配置SDRAM时钟 */
  /* 0：Periph_clk
     1：SEMC alternative clock
     使用 SEMC alternative clock */
  CLOCK_SetMux(kCLOCK_SemcMux, 1);
  /* 0：PLL2 PFD2
     1：PLL3 PFD1
     alternative clock 使用PLL2 PFD2 */
  CLOCK_SetMux(kCLOCK_SemcAltMux, 0);  
  /* 分频后得到SEMC_CLK_ROOT，
     SEMC_CLK_ROOT = PLL2 PFD2/(1+1) */
  CLOCK_SetDiv(kCLOCK_SemcDiv, 1);

  SDRAM_IOMUXC_MUX_Config();
  SDRAM_IOMUXC_PAD_Config();
  return SDRAM_SEMC_Config();
}

/*********************************************END OF FILE**********************/
