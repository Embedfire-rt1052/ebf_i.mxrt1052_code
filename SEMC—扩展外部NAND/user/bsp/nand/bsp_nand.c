/**
  ******************************************************************
  * @file    bsp_nand.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   NAND����
  ******************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  i.MXRT1052������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
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
 * ��
 ******************************************************************************/

/* �������ž�ʹ��ͬ����PAD���� */
#define NAND_PAD_CONFIG_DATA            (SRE_1_FAST_SLEW_RATE| \
                                            DSE_7_R0_7| \
                                            SPEED_3_MAX_200MHz| \
                                            ODE_0_OPEN_DRAIN_DISABLED| \
                                            PKE_1_PULL_KEEPER_ENABLED| \
                                            PUE_0_KEEPER_SELECTED| \
                                            PUS_0_100K_OHM_PULL_DOWN| \
                                            HYS_1_HYSTERESIS_ENABLED)   
    /* ����˵�� : */
    /* ת������: ת�����ʿ�
        ����ǿ��: R0/7 
        �������� : max(200MHz)
        ��©����: �ر� 
        ��/����������: ʹ��
        ��/������ѡ��: ������
        ����/����ѡ��: 100Kŷķ����(ѡ���˱�������������Ч)
        �ͻ�������: ʹ�� */

/*******************************************************************************
 * ����
 ******************************************************************************/
static void NAND_IOMUXC_MUX_Config(void);
static void NAND_IOMUXC_PAD_Config(void);

/**
* @brief  ��ʼ��NAND���IOMUXC��MUX��������
* @param  ��
* @retval ��
*/
static void NAND_IOMUXC_MUX_Config(void)
{
  /* �������ž�������SION���� */
  /* DATA�ź���DATA00~DATA15 */  
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_00_SEMC_DATA00, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_01_SEMC_DATA01, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_02_SEMC_DATA02, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_03_SEMC_DATA03, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_04_SEMC_DATA04, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_05_SEMC_DATA05, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_06_SEMC_DATA06, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_07_SEMC_DATA07, 0U);                                    

  /* ADDR�ź���ADDR09~ADDR12 */                                     
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_18_SEMC_ADDR09, 0U);                                                                       
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_19_SEMC_ADDR11, 0U);                                    
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_20_SEMC_ADDR12, 0U); 

		/* BA1�ź� */  
	IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_22_SEMC_BA1, 0U); 
  /* CSX0�ź� */                                     
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_41_SEMC_CSX00, 0U);    

  /* RDY�ź� */  
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_40_SEMC_RDY, 0U);                                      
 
}

/**
* @brief  ��ʼ��NAND���IOMUXC��PAD��������
* @param  ��
* @retval ��
*/
static void NAND_IOMUXC_PAD_Config(void)
{
  /* �������ž�ʹ��ͬ����PAD���� */
   /* DATA�ź���DATA00~DATA15 */  
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_00_SEMC_DATA00, NAND_PAD_CONFIG_DATA);  
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_01_SEMC_DATA01, NAND_PAD_CONFIG_DATA);                            
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_02_SEMC_DATA02, NAND_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_03_SEMC_DATA03, NAND_PAD_CONFIG_DATA);                            
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_04_SEMC_DATA04, NAND_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_05_SEMC_DATA05, NAND_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_06_SEMC_DATA06, NAND_PAD_CONFIG_DATA);                            
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_07_SEMC_DATA07, NAND_PAD_CONFIG_DATA);                             
  
  /* ADDR�ź���ADDR00~ADDR12 */                                         
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_18_SEMC_ADDR09, NAND_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_19_SEMC_ADDR11, NAND_PAD_CONFIG_DATA);                             
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_20_SEMC_ADDR12, NAND_PAD_CONFIG_DATA);  
  
	/* BA1�ź� */  
	IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_22_SEMC_BA1, NAND_PAD_CONFIG_DATA); 
  /* CSX0�ź� */                        
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_41_SEMC_CSX00, NAND_PAD_CONFIG_DATA);  

  /* RDY�ź� */  
  IOMUXC_SetPinConfig(IOMUXC_GPIO_EMC_40_SEMC_RDY, NAND_PAD_CONFIG_DATA);                               
}

void delayUs(uint32_t delay_us)
{
    uint32_t s_tickPerMicrosecond = CLOCK_GetFreq(kCLOCK_CpuClk) / 1000000;

    /* ȷ��������ֵ����0*/
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
    .cePinMux = kSEMC_MUXCSX0,     /*!< ����Ƭѡ */
    .axiAddress = EXAMPLE_SEMC_NAND_AXI_START_ADDRESS,  /*!< ����ַ */
    .axiMemsize_kbytes = 2 * 1024 * 1024,     /*!< AXI�ӿ�flash��С 8*1024*2*1024*1024 = 16Gb. */
    .ipgAddress = EXAMPLE_SEMC_NAND_IPG_START_ADDRESS,  /*!< The base address. */
    .ipgMemsize_kbytes = 2 * 1024 * 1024,     /*!< IPG�ӿ�flash��С 8*1024*2*1024*1024 = 16Gb. */    
    .rdyactivePolarity = kSEMC_RdyActiveLow,    /*!< RDY���� */
    .arrayAddrOption = kSEMC_NandAddrOption_5byte_CA2RA3,
    .edoModeEnabled = false,                   /*!< ��ַģʽ */
    .columnAddrBitNum = kSEMC_NandColum_12bit,
    .burstLen = kSEMC_Nand_BurstLen1,        /*!< ͻ������ */
    .portSize = kSEMC_PortSize8Bit,         /*!< �ӿ�λ�� */
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
//	/* ����ʱ��Ϊ132Mhz */
//	CLOCK_SetMux(kCLOCK_SemcMux, 1);
//	CLOCK_SetDiv(kCLOCK_SemcDiv, 2);	 

	/* �������� */
	memset(&config, 0, sizeof(semc_config_t));
	/* ��ʼ��SEMC. */
	SEMC_GetDefaultConfig(&config);
	SEMC_Init(SEMC, &config);
	/* ΪNAND���������ʹ������SEMCʱ��Դ */
	semcMemConfig.clkSrc_Hz = EXAMPLE_SEMC_CLK_FREQ;
}
/*********************************************END OF FILE**********************/
