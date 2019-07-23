/**
  ******************************************************************
  * @file    lpm.c
  * @author  fire
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   PWR-��Դģʽѡ��
  ******************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  i.MXRT1052������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************
  */

#include "lpm.h"
#include "fsl_gpc.h"
#include "fsl_dcdc.h"
#include "specific.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define LPM_GPC_IMR_NUM (sizeof(GPC->IMR) / sizeof(GPC->IMR[0]))

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint32_t g_savedPrimask;

/*******************************************************************************
 * Code
 ******************************************************************************/

/**
 * @brief ʹ�ܳ���LDO
 * @return ��
 *   @retval ��
 */
void EnableRegularLDO(void)
{
    /*  Enable Regular LDO 2P5 and 1P1 */
    PMU->REG_2P5_SET = PMU_REG_2P5_ENABLE_LINREG_MASK;
    PMU->REG_1P1_SET = PMU_REG_1P1_ENABLE_LINREG_MASK;
}
/**
 * @brief ʧ�ܳ���LDO
 * @return ��
 *   @retval ��
 */
void DisableRegularLDO(void)
{
    /* Disable Regular LDO 2P5 and 1P1 */
    PMU->REG_2P5_CLR = PMU_REG_2P5_ENABLE_LINREG_MASK;
    PMU->REG_1P1_CLR = PMU_REG_1P1_ENABLE_LINREG_MASK;
}

/**
 * @brief ʹ����LDO
 * @return ��
 *   @retval ��
 */
void EnableWeakLDO(void)
{
    /*  Enable Weak LDO 2P5 and 1P1 */
    PMU->REG_2P5_SET = PMU_REG_2P5_ENABLE_WEAK_LINREG_MASK;
    PMU->REG_1P1_SET = PMU_REG_1P1_ENABLE_WEAK_LINREG_MASK;

    SDK_DelayAtLeastUs(40);
}

/**
 * @brief ʧ����LDO
 * @return ��
 *   @retval ��
 */
void DisableWeakLDO(void)
{
    /* Disable Weak LDO 2P5 and 1P1 */
    PMU->REG_2P5_CLR = PMU_REG_2P5_ENABLE_WEAK_LINREG_MASK;
    PMU->REG_1P1_CLR = PMU_REG_1P1_ENABLE_WEAK_LINREG_MASK;
}

/**
 * @brief ��϶ʹ��
 * @return ��
 *   @retval ��
 */
void BandgapOn(void)
{
    /* �򿪳����϶���ȴ��ȶ� */
    PMU->MISC0_CLR = PMU_MISC0_REFTOP_PWD_MASK;
    while ((PMU->MISC0 & PMU_MISC0_REFTOP_VBGUP_MASK) == 0)
    {
    }
    /* �͹��Ĵ�϶���� */
    XTALOSC24M->LOWPWR_CTRL_CLR = XTALOSC24M_LOWPWR_CTRL_LPBG_SEL_MASK;
}

/**
 * @brief ��϶ʧ��
 * @return ��
 *   @retval ��
 */
void BandgapOff(void)
{
    XTALOSC24M->LOWPWR_CTRL_SET = XTALOSC24M_LOWPWR_CTRL_LPBG_SEL_MASK;
    PMU->MISC0_SET              = PMU_MISC0_REFTOP_PWD_MASK;
}

/*!
 * @brief ��CCM MUX�ڵ�����Ϊ�ض�ֵ��
 *
 * @param mux   Ҫ�����ĸ�mux�ڵ㣬�����\ ref clock_mux_t��
 * @param value ʱ�Ӹ���ֵ���ã���ͬ�ĸ��������в�ͬ��ֵ��Χ��
 */
void CLOCK_SET_MUX(clock_mux_t mux, uint32_t value)
{
    uint32_t busyShift;

    busyShift               = CCM_TUPLE_BUSY_SHIFT(mux);
    CCM_TUPLE_REG(CCM, mux) = (CCM_TUPLE_REG(CCM, mux) & (~CCM_TUPLE_MASK(mux))) |
                              (((uint32_t)((value) << CCM_TUPLE_SHIFT(mux))) & CCM_TUPLE_MASK(mux));

    assert(busyShift <= CCM_NO_BUSY_WAIT);

    /* ʱ���л���Ҫ������ */
    if (CCM_NO_BUSY_WAIT != busyShift)
    {
        /* �ȵ�CCM�ڲ�������ɡ� */
        while (CCM->CDHIPR & (1U << busyShift))
        {
        }
    }
}

/*!
 * @brief��CCM DIV�ڵ�����Ϊ�ض�ֵ��
 *
 * @param divider Ҫ�����ĸ�div�ڵ㣬�����\ ref clock_div_t��
 * @param value   ʱ��divֵ���ã���ͬ�ķ�Ƶ�����в�ͬ��ֵ��Χ��
 */
void CLOCK_SET_DIV(clock_div_t divider, uint32_t value)
{
    uint32_t busyShift;

    busyShift                   = CCM_TUPLE_BUSY_SHIFT(divider);
    CCM_TUPLE_REG(CCM, divider) = (CCM_TUPLE_REG(CCM, divider) & (~CCM_TUPLE_MASK(divider))) |
                                  (((uint32_t)((value) << CCM_TUPLE_SHIFT(divider))) & CCM_TUPLE_MASK(divider));

    assert(busyShift <= CCM_NO_BUSY_WAIT);

    /* ʱ���л���Ҫ������ */
    if (CCM_NO_BUSY_WAIT != busyShift)
    {
        /*�ȵ�CCM�ڲ�������ɡ� */
        while (CCM->CDHIPR & (1U << busyShift))
        {
        }
    }
}


void ClockSelectXtalOsc(void)
{
    /*����XTAL 24MHzʱ��Դ�� */
    CLOCK_InitExternalClk(0);
    /* �ȴ�CCM������� */
    CLOCK_CCM_HANDSHAKE_WAIT();
    /* ��ʱ */
    SDK_DelayAtLeastUs(40);
    /* ��ʱ��Դ�л����ⲿOSC�� */
    CLOCK_SwitchOsc(kCLOCK_XtalOsc);
    /* �ر�XTAL-OSC̽���� */
    CCM_ANALOG->MISC0_CLR = CCM_ANALOG_MISC0_OSC_XTALOK_EN_MASK;
    /* �ر��ڲ�RC�� */
    CLOCK_DeinitRcOsc24M();
}

void ClockSelectRcOsc(void)
{
    /* �����ڲ�RC. */
    XTALOSC24M->LOWPWR_CTRL |= XTALOSC24M_LOWPWR_CTRL_RC_OSC_EN_MASK;
    /* �ȴ�CCM������� */
    CLOCK_CCM_HANDSHAKE_WAIT();
    /* ��ʱ */
    SDK_DelayAtLeastUs(4000);
    /* ��ʱ��Դ�л����ڲ�RC. */
    XTALOSC24M->LOWPWR_CTRL_SET = XTALOSC24M_LOWPWR_CTRL_SET_OSC_SEL_MASK;
    /* ����XTAL 24MHzʱ��Դ. */
    CCM_ANALOG->MISC0_SET = CCM_ANALOG_MISC0_XTAL_24M_PWD_MASK;
}


/**
 * @brief  LPM ��ʼ��
 * @return ��
 *   @retval ��
 */
void LPM_Init(void)
{
    uint32_t i;
    uint32_t tmp_reg = 0;

    CLOCK_SetMode(kCLOCK_ModeRun);

    CCM->CGPR |= CCM_CGPR_INT_MEM_CLK_LPM_MASK;

    /* ����RC OSC�� ����Ҫ����4ms�����ȶ��������Ҫ�����Ե���. */
    XTALOSC24M->LOWPWR_CTRL |= XTALOSC24M_LOWPWR_CTRL_RC_OSC_EN_MASK;
    /* ����RC OSC */
    XTALOSC24M->OSC_CONFIG0 = XTALOSC24M_OSC_CONFIG0_RC_OSC_PROG_CUR(0x4) | XTALOSC24M_OSC_CONFIG0_SET_HYST_MINUS(0x2) |
                              XTALOSC24M_OSC_CONFIG0_RC_OSC_PROG(0xA7) | XTALOSC24M_OSC_CONFIG0_START_MASK |
                              XTALOSC24M_OSC_CONFIG0_ENABLE_MASK;
    XTALOSC24M->OSC_CONFIG1 = XTALOSC24M_OSC_CONFIG1_COUNT_RC_CUR(0x40) | XTALOSC24M_OSC_CONFIG1_COUNT_RC_TRG(0x2DC);
    /* ��ʱ */
    SDK_DelayAtLeastUs(4000);
    /* ���һЩ�ͺ� */
    tmp_reg = XTALOSC24M->OSC_CONFIG0;
    tmp_reg &= ~(XTALOSC24M_OSC_CONFIG0_HYST_PLUS_MASK | XTALOSC24M_OSC_CONFIG0_HYST_MINUS_MASK);
    tmp_reg |= XTALOSC24M_OSC_CONFIG0_HYST_PLUS(3) | XTALOSC24M_OSC_CONFIG0_HYST_MINUS(3);
    XTALOSC24M->OSC_CONFIG0 = tmp_reg;
    /* ����COUNT_1M_TRG */
    tmp_reg = XTALOSC24M->OSC_CONFIG2;
    tmp_reg &= ~XTALOSC24M_OSC_CONFIG2_COUNT_1M_TRG_MASK;
    tmp_reg |= XTALOSC24M_OSC_CONFIG2_COUNT_1M_TRG(0x2d7);
    XTALOSC24M->OSC_CONFIG2 = tmp_reg;
    /* Ӳ����Ҫ��ȡOSC_CONFIG0��OSC_CONFIG1��ʹOSC_CONFIG2д�빤�� */
    tmp_reg                 = XTALOSC24M->OSC_CONFIG1;
    XTALOSC24M->OSC_CONFIG1 = tmp_reg;

    /* ERR007265 */
    IOMUXC_GPR->GPR1 |= IOMUXC_GPR_GPR1_GINT_MASK;

    /* ��ʼ��GPC����������IRQ */
    for (i = 0; i < LPM_GPC_IMR_NUM; i++)
    {
        GPC->IMR[i] = 0xFFFFFFFFU;
    }
}

/**
 * @brief  LPM���û���Դ
 * @return ��
 *   @retval ��
 */
void LPM_EnableWakeupSource(uint32_t irq)
{
    GPC_EnableIRQ(GPC, irq);
}

/**
 * @brief  LPM�رջ���Դ
 * @return ��
 *   @retval ��
 */
void LPM_DisableWakeupSource(uint32_t irq)
{
    GPC_DisableIRQ(GPC, irq);
}

/**
 * @brief  LPM����ȴ�ģʽ
 * @return ��
 *   @retval ��
 */
void LPM_PreEnterWaitMode(void)
{
    g_savedPrimask = DisableGlobalIRQ();
    __DSB();
    __ISB();
}

/**
 * @brief  LPM�˳��ȴ�ģʽ
 * @return ��
 *   @retval ��
 */
void LPM_PostExitWaitMode(void)
{
    EnableGlobalIRQ(g_savedPrimask);
    __DSB();
    __ISB();
}

/**
 * @brief  LPM����ֹͣģʽ
 * @return ��
 *   @retval ��
 */
void LPM_PreEnterStopMode(void)
{
    g_savedPrimask = DisableGlobalIRQ();
    __DSB();
    __ISB();
}

void LPM_PostExitStopMode(void)
{
    EnableGlobalIRQ(g_savedPrimask);
    __DSB();
    __ISB();
}
/**
 * @brief  ����LPM����ģʽ
 * @return ��
 *   @retval ��
 */
void LPM_SetRunModeConfig(void)
{
    CCM->CLPCR &= ~(CCM_CLPCR_LPM_MASK | CCM_CLPCR_ARM_CLK_DIS_ON_LPM_MASK);
}

/**
 * @brief  ����LPM�ȴ�ģʽ
 * @return ��
 *   @retval ��
 */
void LPM_SetWaitModeConfig(void)
{
    uint32_t clpcr;

    /*
     * ERR007265: CCM: When improper low-power sequence is used,
     * the SoC enters low power mode before the ARM core executes WFI.
     *
     * Software workaround:
     * 1) Software should trigger IRQ #41 (GPR_IRQ) to be always pending
     *      by setting IOMUXC_GPR_GPR1_GINT.
     * 2) Software should then unmask IRQ #41 in GPC before setting CCM
     *      Low-Power mode.
     * 3) Software should mask IRQ #41 right after CCM Low-Power mode
     *      is set (set bits 0-1 of CCM_CLPCR).
     */
	/*
		���ģ�* ERR007265��CCM��ʹ�ò���ȷ�ĵ͹�������ʱ��
      * SoC��ARM�ں�ִ��WFI֮ǰ����͹���ģʽ��
     *
      *������������
      * 1�����Ӧ����IRQ��41��GPR_IRQ��ʼ�չ���
      *ͨ������IOMUXC_GPR_GPR1_GINT��
      * 2�����Ӧ������CCM֮ǰ��GPC��ȡ������IRQ��41
      *�͹���ģʽ��
      * 3�����Ӧ��CCM�͹���ģʽ����������IRQ��41
      *��λ������CCM_CLPCR��0-1λ����
	*/
    GPC_EnableIRQ(GPC, GPR_IRQ_IRQn);
    clpcr      = CCM->CLPCR & (~(CCM_CLPCR_LPM_MASK | CCM_CLPCR_ARM_CLK_DIS_ON_LPM_MASK));
    CCM->CLPCR = clpcr | CCM_CLPCR_LPM(kCLOCK_ModeWait) | CCM_CLPCR_MASK_SCU_IDLE_MASK | CCM_CLPCR_MASK_L2CC_IDLE_MASK |
                 CCM_CLPCR_ARM_CLK_DIS_ON_LPM_MASK | CCM_CLPCR_STBY_COUNT_MASK | CCM_CLPCR_BYPASS_LPM_HS0_MASK |
                 CCM_CLPCR_BYPASS_LPM_HS1_MASK;
    GPC_DisableIRQ(GPC, GPR_IRQ_IRQn);
}

/**
 * @brief  ����LPMֹͣģʽ
 * @return ��
 *   @retval ��
 */
void LPM_SetStopModeConfig(void)
{
    uint32_t clpcr;

    /*
     * ERR007265: CCM: When improper low-power sequence is used,
     * the SoC enters low power mode before the ARM core executes WFI.
     *
     * Software workaround:
     * 1) Software should trigger IRQ #41 (GPR_IRQ) to be always pending
     *      by setting IOMUXC_GPR_GPR1_GINT.
     * 2) Software should then unmask IRQ #41 in GPC before setting CCM
     *      Low-Power mode.
     * 3) Software should mask IRQ #41 right after CCM Low-Power mode
     *      is set (set bits 0-1 of CCM_CLPCR).
     */
	/*
	
	���ģ�* ERR007265��CCM��ʹ�ò���ȷ�ĵ͹�������ʱ��
      * SoC��ARM�ں�ִ��WFI֮ǰ����͹���ģʽ��
     *
      *������������
      * 1�����Ӧ����IRQ��41��GPR_IRQ��ʼ�չ���
      *ͨ������IOMUXC_GPR_GPR1_GINT��
      * 2�����Ӧ������CCM֮ǰ��GPC��ȡ������IRQ��41
      *�͹���ģʽ��
      * 3�����Ӧ��CCM�͹���ģʽ����������IRQ��41
      *��λ������CCM_CLPCR��0-1λ����*/
    GPC_EnableIRQ(GPC, GPR_IRQ_IRQn);
    clpcr      = CCM->CLPCR & (~(CCM_CLPCR_LPM_MASK | CCM_CLPCR_ARM_CLK_DIS_ON_LPM_MASK));
    CCM->CLPCR = clpcr | CCM_CLPCR_LPM(kCLOCK_ModeStop) | CCM_CLPCR_MASK_L2CC_IDLE_MASK | CCM_CLPCR_MASK_SCU_IDLE_MASK |
                 CCM_CLPCR_VSTBY_MASK | CCM_CLPCR_STBY_COUNT_MASK | CCM_CLPCR_SBYOS_MASK |
                 CCM_CLPCR_ARM_CLK_DIS_ON_LPM_MASK | CCM_CLPCR_BYPASS_LPM_HS0_MASK | CCM_CLPCR_BYPASS_LPM_HS1_MASK;
    GPC_DisableIRQ(GPC, GPR_IRQ_IRQn);
}

/**
 * @brief  LPM��������ģʽ
 * @return ��
 *   @retval ��
 */
void LPM_OverDriveRun(void)
{
    /* CCM ģʽ */
    DCDC_BootIntoCCM(DCDC);
    /* �����ڲ����ص��� */
    DCDC->REG1 |= DCDC_REG1_REG_RLOAD_SW_MASK;
    /* ��SOC��ѹ����Ϊ1.275V */
    DCDC_AdjustTargetVoltage(DCDC, 0x13, 0x1);

    /* ����FET ODRIVE */
    PMU->REG_CORE_SET = PMU_REG_CORE_FET_ODRIVE_MASK;
    /* ����vdd_high_in������vdd_snvs_in */
    PMU->MISC0_CLR = PMU_MISC0_DISCON_HIGH_SNVS_MASK;

    BandgapOn();
    /* ʹ�ܳ���LDO */
    EnableRegularLDO();
    /* ʧ�� ��LDO */
    DisableWeakLDO();
    /* ���ó������е�ʱ�� */
    ClockSetToOverDriveRun();
}

/**
 * @brief  LPM��������ģʽ
 * @return ��
 *   @retval ��
 */
void LPM_FullSpeedRun(void)
{
    /* CCM ģʽ */
    DCDC_BootIntoCCM(DCDC);
    /* �����ڲ����ص��� */
    DCDC->REG1 |= DCDC_REG1_REG_RLOAD_SW_MASK;
    /*��SOC��ѹ����Ϊ1.275V */
    DCDC_AdjustTargetVoltage(DCDC, 0x13, 0x1);

    /* ����FET ODRIVE */
    PMU->REG_CORE_SET = PMU_REG_CORE_FET_ODRIVE_MASK;
    /* ����FET ODRIVEConnect vdd_high_in������vdd_snvs_in */
    PMU->MISC0_CLR = PMU_MISC0_DISCON_HIGH_SNVS_MASK;

    BandgapOn();
     /* ʹ�ܳ���LDO */
    EnableRegularLDO();
    /* ʧ�� ��LDO */
    DisableWeakLDO();
    /* �����������е�ʱ�� */
    ClockSetToFullSpeedRun();
    /* ��SOC��ѹ����Ϊ1.15V */
    DCDC_AdjustTargetVoltage(DCDC, 0xe, 0x1);
}


/**
 * @brief  LPM��������ģʽ
 * @return ��
 *   @retval ��
 */
void LPM_LowSpeedRun(void)
{
    /* CCM ģʽ */
    DCDC_BootIntoCCM(DCDC);
    /* �����ڲ����ص��� */
    DCDC->REG1 |= DCDC_REG1_REG_RLOAD_SW_MASK;
    /* ��SOC��ѹ����Ϊ1.275V */
    DCDC_AdjustTargetVoltage(DCDC, 0x13, 0x1);

    /* ʹ�� FET ODRIVE */
    PMU->REG_CORE_SET = PMU_REG_CORE_FET_ODRIVE_MASK;
    /* ����vdd_high_in������vdd_snvs_in */
    PMU->MISC0_CLR = PMU_MISC0_DISCON_HIGH_SNVS_MASK;

    BandgapOn();
    EnableRegularLDO();
    DisableWeakLDO();

    ClockSetToLowSpeedRun();

    /* ��SOC��ѹ����Ϊ1.15V */
    DCDC_AdjustTargetVoltage(DCDC, 0xe, 0x1);
}

/**
 * @brief  LPM�͹�������ģʽ
 * @return ��
 *   @retval ��
 */
void LPM_LowPowerRun(void)
{
    ClockSetToLowPowerRun();

    /* �ϵ� USBPHY */
    PowerDownUSBPHY();

    /* ��SOC��ѹ����Ϊ0.95V */
    DCDC_AdjustTargetVoltage(DCDC, 0x6, 0x1);
    /* DCM ģʽ */
    DCDC_BootIntoDCM(DCDC);
    /* �Ͽ����ص�����ڲ� */
    DCDC->REG1 &= ~DCDC_REG1_REG_RLOAD_SW_MASK;
    /* ���������Χ�Ƚ��� */
    DCDC->REG0 |= DCDC_REG0_PWD_CMP_OFFSET_MASK;

    /* ʹ�� FET ODRIVE */
    PMU->REG_CORE_SET = PMU_REG_CORE_FET_ODRIVE_MASK;
    /* ����vdd_high_in������vdd_snvs_in */
    PMU->MISC0_CLR = PMU_MISC0_DISCON_HIGH_SNVS_MASK;

    EnableWeakLDO();
    DisableRegularLDO();
    BandgapOff();
}

/**
 * @brief  LPM����ϵͳ����
 * @return ��
 *   @retval ��
 */
void LPM_EnterSystemIdle(void)
{
    /* ���õȴ�ģʽ���� */
    LPM_SetWaitModeConfig();
    /* ���õ͹���ʱ���� */
    SetLowPowerClockGate();
    /* ��ʱ�����óɵ͹��Ŀ��� */
    ClockSetToSystemIdle();
    /* �ϵ� USBPHY */
    PowerDownUSBPHY();
    /* DCDC �� 1.15V */
    DCDC_AdjustTargetVoltage(DCDC, 0xe, 0x1);
    /* DCM ģʽ */
    DCDC_BootIntoDCM(DCDC);
    /* �Ͽ����ص�����ڲ� */
    DCDC->REG1 &= ~DCDC_REG1_REG_RLOAD_SW_MASK;
    /* ���������Χ�Ƚ��� */
    DCDC->REG0 |= DCDC_REG0_PWD_CMP_OFFSET_MASK;
    /* ʹ�� FET ODRIVE */
    PMU->REG_CORE_SET = PMU_REG_CORE_FET_ODRIVE_MASK;
    /* ����vdd_high_in������vdd_snvs_in */
    PMU->MISC0_CLR = PMU_MISC0_DISCON_HIGH_SNVS_MASK;
    /* ʹ����LDO */
    EnableRegularLDO();
    /* ʧ�ܳ���LDO*/
    DisableWeakLDO();
    /* ��϶ʧ�� */
    BandgapOn();
    /* ��Χ�豸�������ģʽ */
    PeripheralEnterDozeMode();
    __DSB();
    __WFI();
    __ISB();
}

/**
 * @brief  LPM�˳�ϵͳ����
 * @return ��
 *   @retval ��
 */
void LPM_ExitSystemIdle(void)
{
    PeripheralExitDozeMode();
    LPM_SetRunModeConfig();
}

/**
 * @brief  LPM����͹��Ŀ���
 * @return ��
 *   @retval ��
 */
void LPM_EnterLowPowerIdle(void)
{
     /*************************��һ����*********************/
     /* ���õȴ�ģʽ���� */
    LPM_SetWaitModeConfig();
     /* ���õ͹���ʱ���� */
    SetLowPowerClockGate();
     /* ��ʱ�����óɵ͹��Ŀ��� */
    ClockSetToLowPowerIdle();
    /*�ϵ� USBPHY */
    PowerDownUSBPHY();
     /*************************�ڶ�����*********************/
    /* ��SOC��ѹ����Ϊ0.95V */
    DCDC_AdjustTargetVoltage(DCDC, 0x6, 0x1);
    /* DCM ģʽ */
    DCDC_BootIntoDCM(DCDC);
    /* �Ͽ����ص�����ڲ� */
    DCDC->REG1 &= ~DCDC_REG1_REG_RLOAD_SW_MASK;
    /* ���������Χ�Ƚ���*/
    DCDC->REG0 |= DCDC_REG0_PWD_CMP_OFFSET_MASK;
    /* ʹ�� FET ODRIVE */
    PMU->REG_CORE_SET = PMU_REG_CORE_FET_ODRIVE_MASK;
    /* ����vdd_high_in������vdd_snvs_in */
    PMU->MISC0_CLR = PMU_MISC0_DISCON_HIGH_SNVS_MASK;
     /*************************��������*********************/
    /* ʹ����LDO */
    EnableWeakLDO();
    /* ʧ�ܳ���LDO */
    DisableRegularLDO();
    /* ��϶ʧ�� */
    BandgapOff();
    /* ��Χ�豸�������ģʽ */
    PeripheralEnterDozeMode();
    __DSB();
    __WFI();
    __ISB();
}

/**
 * @brief  LPM�˳��͹��Ŀ���
 * @return ��
 *   @retval ��
 */
void LPM_ExitLowPowerIdle(void)
{
    /* �����˳�����ģʽ */
    PeripheralExitDozeMode();
    /* ��������ģʽ */
    LPM_SetRunModeConfig();
}

/**
 * @brief  LPM������ͣģʽ
 * @return ��
 *   @retval ��
 */
void LPM_EnterSuspend()
{
	  /*************************��һ����*********************/
    uint32_t i;
    uint32_t gpcIMR[LPM_GPC_IMR_NUM];
    /*����LPMֹͣģʽ*/
    LPM_SetStopModeConfig();
    /* ���õ͹���ʱ���� */
    SetLowPowerClockGate();
		/*************************�ڶ�����*********************/
    /* �Ͽ����ص�����ڲ� */
    DCDC->REG1 &= ~DCDC_REG1_REG_RLOAD_SW_MASK;
    /* �ص� FlexRAM0 */
    GPC->CNTR |= GPC_CNTR_PDRAM0_PGE_MASK;
    /* �ص� FlexRAM1 */
    PGC->MEGA_CTRL |= PGC_MEGA_CTRL_PCR_MASK;
    /*�����������ݸ��ٻ�����ȷ���������ı��浽RAM�� */
    SCB_CleanDCache();
    SCB_DisableDCache();
		/*************************��������*********************/
    /* ��LP��ѹ����Ϊ0.925V */
    DCDC_AdjustTargetVoltage(DCDC, 0x13, 0x1);
    /* �л�DCDC��ʹ��DCDC�ڲ�OSC */
    DCDC_SetClockSource(DCDC, kDCDC_ClockInternalOsc);
    /* �ϵ� USBPHY */
    PowerDownUSBPHY();
    /* ����ʱ�ر�CPU */
    PGC->CPU_CTRL = PGC_CPU_CTRL_PCR_MASK;
    /* ʹ�� FET ODRIVE */
    PMU->REG_CORE_SET = PMU_REG_CORE_FET_ODRIVE_MASK;
    /* ����vdd_high_in������vdd_snvs_in*/
    PMU->MISC0_CLR = PMU_MISC0_DISCON_HIGH_SNVS_MASK;
    /* STOP_MODE���ã���ֹͣģʽ�¹ر�RTC���������ģ�� */
    PMU->MISC0_CLR = PMU_MISC0_STOP_MODE_CONFIG_MASK;
	/*************************���Ĳ���*********************/
		 /*������RBC������֮ǰ��������GPC�ж�
			*����Ѿ��жϣ���������������̫��
			*�ȴ���*/
    /* ѭ�����  gpcIMR�Ĵ���*/
    for (i = 0; i < LPM_GPC_IMR_NUM; i++)
    {
        gpcIMR[i]   = GPC->IMR[i];
        GPC->IMR[i] = 0xFFFFFFFFU;
    }
		/*CCM���޷�ͨ����Դ�ſشӵȴ�/ֹͣģʽ�ָ�
      *��REG_BYPASS_COUNTER����Ϊ2
      *�ڴ�����RBC��·����������ֹ�жϡ� RBC��̨
      *��Ҫ������2*/
    CCM->CCR = (CCM->CCR & ~CCM_CCR_REG_BYPASS_COUNT_MASK) | CCM_CCR_REG_BYPASS_COUNT(2);
    CCM->CCR |= (CCM_CCR_OSCNT(0xAF) | CCM_CCR_COSC_EN_MASK | CCM_CCR_RBC_EN_MASK);
		 /*�����ӳ�һ�����3usec��
			*���Զ�ѭ�����㹻�ˡ� ��Ҫ�����ӳ���ȷ����һ��
			*����ж��Ѿ�����RBC���������Կ�ʼ����
			*������ARM��������DSM_requestʱ�жϵ��*/
    SDK_DelayAtLeastUs(3);
		/*************************���岿��*********************/
    /* �ָ�����GPC�жϡ� */
    for (i = 0; i < LPM_GPC_IMR_NUM; i++)
    {
        GPC->IMR[i] = gpcIMR[i];
    }
    /*�����˳�ֹͣģʽ*/
    PeripheralEnterStopMode();
    __DSB();
    __WFI();
    __ISB();
}

/**
 * @brief  LPM����SNVS
 * @return ��
 *   @retval ��
 */
void LPM_EnterSNVS(void)
{
    SNVS->LPCR |= SNVS_LPCR_TOP_MASK;
    while (1) /* �ص� */
    {
    }
}
