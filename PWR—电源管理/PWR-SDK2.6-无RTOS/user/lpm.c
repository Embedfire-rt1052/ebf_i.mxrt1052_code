/**
  ******************************************************************
  * @file    lpm.c
  * @author  fire
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   PWR-电源模式选择
  ******************************************************************
  * @attention
  *
  * 实验平台:野火  i.MXRT1052开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
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
 * @brief 使能常规LDO
 * @return 无
 *   @retval 无
 */
void EnableRegularLDO(void)
{
    /*  Enable Regular LDO 2P5 and 1P1 */
    PMU->REG_2P5_SET = PMU_REG_2P5_ENABLE_LINREG_MASK;
    PMU->REG_1P1_SET = PMU_REG_1P1_ENABLE_LINREG_MASK;
}
/**
 * @brief 失能常规LDO
 * @return 无
 *   @retval 无
 */
void DisableRegularLDO(void)
{
    /* Disable Regular LDO 2P5 and 1P1 */
    PMU->REG_2P5_CLR = PMU_REG_2P5_ENABLE_LINREG_MASK;
    PMU->REG_1P1_CLR = PMU_REG_1P1_ENABLE_LINREG_MASK;
}

/**
 * @brief 使能弱LDO
 * @return 无
 *   @retval 无
 */
void EnableWeakLDO(void)
{
    /*  Enable Weak LDO 2P5 and 1P1 */
    PMU->REG_2P5_SET = PMU_REG_2P5_ENABLE_WEAK_LINREG_MASK;
    PMU->REG_1P1_SET = PMU_REG_1P1_ENABLE_WEAK_LINREG_MASK;

    SDK_DelayAtLeastUs(40);
}

/**
 * @brief 失能弱LDO
 * @return 无
 *   @retval 无
 */
void DisableWeakLDO(void)
{
    /* Disable Weak LDO 2P5 and 1P1 */
    PMU->REG_2P5_CLR = PMU_REG_2P5_ENABLE_WEAK_LINREG_MASK;
    PMU->REG_1P1_CLR = PMU_REG_1P1_ENABLE_WEAK_LINREG_MASK;
}

/**
 * @brief 带隙使能
 * @return 无
 *   @retval 无
 */
void BandgapOn(void)
{
    /* 打开常规带隙并等待稳定 */
    PMU->MISC0_CLR = PMU_MISC0_REFTOP_PWD_MASK;
    while ((PMU->MISC0 & PMU_MISC0_REFTOP_VBGUP_MASK) == 0)
    {
    }
    /* 低功耗带隙禁用 */
    XTALOSC24M->LOWPWR_CTRL_CLR = XTALOSC24M_LOWPWR_CTRL_LPBG_SEL_MASK;
}

/**
 * @brief 带隙失能
 * @return 无
 *   @retval 无
 */
void BandgapOff(void)
{
    XTALOSC24M->LOWPWR_CTRL_SET = XTALOSC24M_LOWPWR_CTRL_LPBG_SEL_MASK;
    PMU->MISC0_SET              = PMU_MISC0_REFTOP_PWD_MASK;
}

/*!
 * @brief 将CCM MUX节点设置为特定值。
 *
 * @param mux   要设置哪个mux节点，请参阅\ ref clock_mux_t。
 * @param value 时钟复用值设置，不同的复用器具有不同的值范围。
 */
void CLOCK_SET_MUX(clock_mux_t mux, uint32_t value)
{
    uint32_t busyShift;

    busyShift               = CCM_TUPLE_BUSY_SHIFT(mux);
    CCM_TUPLE_REG(CCM, mux) = (CCM_TUPLE_REG(CCM, mux) & (~CCM_TUPLE_MASK(mux))) |
                              (((uint32_t)((value) << CCM_TUPLE_SHIFT(mux))) & CCM_TUPLE_MASK(mux));

    assert(busyShift <= CCM_NO_BUSY_WAIT);

    /* 时钟切换需要握手吗？ */
    if (CCM_NO_BUSY_WAIT != busyShift)
    {
        /* 等到CCM内部握手完成。 */
        while (CCM->CDHIPR & (1U << busyShift))
        {
        }
    }
}

/*!
 * @brief将CCM DIV节点设置为特定值。
 *
 * @param divider 要设置哪个div节点，请参阅\ ref clock_div_t。
 * @param value   时钟div值设置，不同的分频器具有不同的值范围。
 */
void CLOCK_SET_DIV(clock_div_t divider, uint32_t value)
{
    uint32_t busyShift;

    busyShift                   = CCM_TUPLE_BUSY_SHIFT(divider);
    CCM_TUPLE_REG(CCM, divider) = (CCM_TUPLE_REG(CCM, divider) & (~CCM_TUPLE_MASK(divider))) |
                                  (((uint32_t)((value) << CCM_TUPLE_SHIFT(divider))) & CCM_TUPLE_MASK(divider));

    assert(busyShift <= CCM_NO_BUSY_WAIT);

    /* 时钟切换需要握手吗？ */
    if (CCM_NO_BUSY_WAIT != busyShift)
    {
        /*等到CCM内部握手完成。 */
        while (CCM->CDHIPR & (1U << busyShift))
        {
        }
    }
}


void ClockSelectXtalOsc(void)
{
    /*启用XTAL 24MHz时钟源。 */
    CLOCK_InitExternalClk(0);
    /* 等待CCM操作完成 */
    CLOCK_CCM_HANDSHAKE_WAIT();
    /* 延时 */
    SDK_DelayAtLeastUs(40);
    /* 将时钟源切换到外部OSC。 */
    CLOCK_SwitchOsc(kCLOCK_XtalOsc);
    /* 关闭XTAL-OSC探测器 */
    CCM_ANALOG->MISC0_CLR = CCM_ANALOG_MISC0_OSC_XTALOK_EN_MASK;
    /* 关闭内部RC。 */
    CLOCK_DeinitRcOsc24M();
}

void ClockSelectRcOsc(void)
{
    /* 启用内部RC. */
    XTALOSC24M->LOWPWR_CTRL |= XTALOSC24M_LOWPWR_CTRL_RC_OSC_EN_MASK;
    /* 等待CCM操作完成 */
    CLOCK_CCM_HANDSHAKE_WAIT();
    /* 延时 */
    SDK_DelayAtLeastUs(4000);
    /* 将时钟源切换到内部RC. */
    XTALOSC24M->LOWPWR_CTRL_SET = XTALOSC24M_LOWPWR_CTRL_SET_OSC_SEL_MASK;
    /* 禁用XTAL 24MHz时钟源. */
    CCM_ANALOG->MISC0_SET = CCM_ANALOG_MISC0_XTAL_24M_PWD_MASK;
}


/**
 * @brief  LPM 初始化
 * @return 无
 *   @retval 无
 */
void LPM_Init(void)
{
    uint32_t i;
    uint32_t tmp_reg = 0;

    CLOCK_SetMode(kCLOCK_ModeRun);

    CCM->CGPR |= CCM_CGPR_INT_MEM_CLK_LPM_MASK;

    /* 启用RC OSC。 它需要至少4ms才能稳定，因此需要启用自调整. */
    XTALOSC24M->LOWPWR_CTRL |= XTALOSC24M_LOWPWR_CTRL_RC_OSC_EN_MASK;
    /* 配置RC OSC */
    XTALOSC24M->OSC_CONFIG0 = XTALOSC24M_OSC_CONFIG0_RC_OSC_PROG_CUR(0x4) | XTALOSC24M_OSC_CONFIG0_SET_HYST_MINUS(0x2) |
                              XTALOSC24M_OSC_CONFIG0_RC_OSC_PROG(0xA7) | XTALOSC24M_OSC_CONFIG0_START_MASK |
                              XTALOSC24M_OSC_CONFIG0_ENABLE_MASK;
    XTALOSC24M->OSC_CONFIG1 = XTALOSC24M_OSC_CONFIG1_COUNT_RC_CUR(0x40) | XTALOSC24M_OSC_CONFIG1_COUNT_RC_TRG(0x2DC);
    /* 延时 */
    SDK_DelayAtLeastUs(4000);
    /* 添加一些滞后 */
    tmp_reg = XTALOSC24M->OSC_CONFIG0;
    tmp_reg &= ~(XTALOSC24M_OSC_CONFIG0_HYST_PLUS_MASK | XTALOSC24M_OSC_CONFIG0_HYST_MINUS_MASK);
    tmp_reg |= XTALOSC24M_OSC_CONFIG0_HYST_PLUS(3) | XTALOSC24M_OSC_CONFIG0_HYST_MINUS(3);
    XTALOSC24M->OSC_CONFIG0 = tmp_reg;
    /* 设置COUNT_1M_TRG */
    tmp_reg = XTALOSC24M->OSC_CONFIG2;
    tmp_reg &= ~XTALOSC24M_OSC_CONFIG2_COUNT_1M_TRG_MASK;
    tmp_reg |= XTALOSC24M_OSC_CONFIG2_COUNT_1M_TRG(0x2d7);
    XTALOSC24M->OSC_CONFIG2 = tmp_reg;
    /* 硬件需要读取OSC_CONFIG0或OSC_CONFIG1以使OSC_CONFIG2写入工作 */
    tmp_reg                 = XTALOSC24M->OSC_CONFIG1;
    XTALOSC24M->OSC_CONFIG1 = tmp_reg;

    /* ERR007265 */
    IOMUXC_GPR->GPR1 |= IOMUXC_GPR_GPR1_GINT_MASK;

    /* 初始化GPC以屏蔽所有IRQ */
    for (i = 0; i < LPM_GPC_IMR_NUM; i++)
    {
        GPC->IMR[i] = 0xFFFFFFFFU;
    }
}

/**
 * @brief  LPM启用唤醒源
 * @return 无
 *   @retval 无
 */
void LPM_EnableWakeupSource(uint32_t irq)
{
    GPC_EnableIRQ(GPC, irq);
}

/**
 * @brief  LPM关闭唤醒源
 * @return 无
 *   @retval 无
 */
void LPM_DisableWakeupSource(uint32_t irq)
{
    GPC_DisableIRQ(GPC, irq);
}

/**
 * @brief  LPM进入等待模式
 * @return 无
 *   @retval 无
 */
void LPM_PreEnterWaitMode(void)
{
    g_savedPrimask = DisableGlobalIRQ();
    __DSB();
    __ISB();
}

/**
 * @brief  LPM退出等待模式
 * @return 无
 *   @retval 无
 */
void LPM_PostExitWaitMode(void)
{
    EnableGlobalIRQ(g_savedPrimask);
    __DSB();
    __ISB();
}

/**
 * @brief  LPM进入停止模式
 * @return 无
 *   @retval 无
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
 * @brief  配置LPM运行模式
 * @return 无
 *   @retval 无
 */
void LPM_SetRunModeConfig(void)
{
    CCM->CLPCR &= ~(CCM_CLPCR_LPM_MASK | CCM_CLPCR_ARM_CLK_DIS_ON_LPM_MASK);
}

/**
 * @brief  配置LPM等待模式
 * @return 无
 *   @retval 无
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
		译文：* ERR007265：CCM：使用不正确的低功率序列时，
      * SoC在ARM内核执行WFI之前进入低功耗模式。
     *
      *软件解决方法：
      * 1）软件应触发IRQ＃41（GPR_IRQ）始终挂起
      *通过设置IOMUXC_GPR_GPR1_GINT。
      * 2）软件应在设置CCM之前在GPC中取消屏蔽IRQ＃41
      *低功耗模式。
      * 3）软件应在CCM低功耗模式后立即屏蔽IRQ＃41
      *置位（设置CCM_CLPCR的0-1位）。
	*/
    GPC_EnableIRQ(GPC, GPR_IRQ_IRQn);
    clpcr      = CCM->CLPCR & (~(CCM_CLPCR_LPM_MASK | CCM_CLPCR_ARM_CLK_DIS_ON_LPM_MASK));
    CCM->CLPCR = clpcr | CCM_CLPCR_LPM(kCLOCK_ModeWait) | CCM_CLPCR_MASK_SCU_IDLE_MASK | CCM_CLPCR_MASK_L2CC_IDLE_MASK |
                 CCM_CLPCR_ARM_CLK_DIS_ON_LPM_MASK | CCM_CLPCR_STBY_COUNT_MASK | CCM_CLPCR_BYPASS_LPM_HS0_MASK |
                 CCM_CLPCR_BYPASS_LPM_HS1_MASK;
    GPC_DisableIRQ(GPC, GPR_IRQ_IRQn);
}

/**
 * @brief  配置LPM停止模式
 * @return 无
 *   @retval 无
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
	
	译文：* ERR007265：CCM：使用不正确的低功率序列时，
      * SoC在ARM内核执行WFI之前进入低功耗模式。
     *
      *软件解决方法：
      * 1）软件应触发IRQ＃41（GPR_IRQ）始终挂起
      *通过设置IOMUXC_GPR_GPR1_GINT。
      * 2）软件应在设置CCM之前在GPC中取消屏蔽IRQ＃41
      *低功耗模式。
      * 3）软件应在CCM低功耗模式后立即屏蔽IRQ＃41
      *置位（设置CCM_CLPCR的0-1位）。*/
    GPC_EnableIRQ(GPC, GPR_IRQ_IRQn);
    clpcr      = CCM->CLPCR & (~(CCM_CLPCR_LPM_MASK | CCM_CLPCR_ARM_CLK_DIS_ON_LPM_MASK));
    CCM->CLPCR = clpcr | CCM_CLPCR_LPM(kCLOCK_ModeStop) | CCM_CLPCR_MASK_L2CC_IDLE_MASK | CCM_CLPCR_MASK_SCU_IDLE_MASK |
                 CCM_CLPCR_VSTBY_MASK | CCM_CLPCR_STBY_COUNT_MASK | CCM_CLPCR_SBYOS_MASK |
                 CCM_CLPCR_ARM_CLK_DIS_ON_LPM_MASK | CCM_CLPCR_BYPASS_LPM_HS0_MASK | CCM_CLPCR_BYPASS_LPM_HS1_MASK;
    GPC_DisableIRQ(GPC, GPR_IRQ_IRQn);
}

/**
 * @brief  LPM超载运行模式
 * @return 无
 *   @retval 无
 */
void LPM_OverDriveRun(void)
{
    /* CCM 模式 */
    DCDC_BootIntoCCM(DCDC);
    /* 连接内部负载电阻 */
    DCDC->REG1 |= DCDC_REG1_REG_RLOAD_SW_MASK;
    /* 将SOC电压调整为1.275V */
    DCDC_AdjustTargetVoltage(DCDC, 0x13, 0x1);

    /* 启用FET ODRIVE */
    PMU->REG_CORE_SET = PMU_REG_CORE_FET_ODRIVE_MASK;
    /* 连接vdd_high_in并连接vdd_snvs_in */
    PMU->MISC0_CLR = PMU_MISC0_DISCON_HIGH_SNVS_MASK;

    BandgapOn();
    /* 使能常规LDO */
    EnableRegularLDO();
    /* 失能 弱LDO */
    DisableWeakLDO();
    /* 设置超载运行的时钟 */
    ClockSetToOverDriveRun();
}

/**
 * @brief  LPM满载运行模式
 * @return 无
 *   @retval 无
 */
void LPM_FullSpeedRun(void)
{
    /* CCM 模式 */
    DCDC_BootIntoCCM(DCDC);
    /* 连接内部负载电阻 */
    DCDC->REG1 |= DCDC_REG1_REG_RLOAD_SW_MASK;
    /*将SOC电压调整为1.275V */
    DCDC_AdjustTargetVoltage(DCDC, 0x13, 0x1);

    /* 启用FET ODRIVE */
    PMU->REG_CORE_SET = PMU_REG_CORE_FET_ODRIVE_MASK;
    /* 启用FET ODRIVEConnect vdd_high_in并连接vdd_snvs_in */
    PMU->MISC0_CLR = PMU_MISC0_DISCON_HIGH_SNVS_MASK;

    BandgapOn();
     /* 使能常规LDO */
    EnableRegularLDO();
    /* 失能 弱LDO */
    DisableWeakLDO();
    /* 设置满载运行的时钟 */
    ClockSetToFullSpeedRun();
    /* 将SOC电压调整为1.15V */
    DCDC_AdjustTargetVoltage(DCDC, 0xe, 0x1);
}


/**
 * @brief  LPM低速运行模式
 * @return 无
 *   @retval 无
 */
void LPM_LowSpeedRun(void)
{
    /* CCM 模式 */
    DCDC_BootIntoCCM(DCDC);
    /* 连接内部负载电阻 */
    DCDC->REG1 |= DCDC_REG1_REG_RLOAD_SW_MASK;
    /* 将SOC电压调整为1.275V */
    DCDC_AdjustTargetVoltage(DCDC, 0x13, 0x1);

    /* 使能 FET ODRIVE */
    PMU->REG_CORE_SET = PMU_REG_CORE_FET_ODRIVE_MASK;
    /* 连接vdd_high_in并连接vdd_snvs_in */
    PMU->MISC0_CLR = PMU_MISC0_DISCON_HIGH_SNVS_MASK;

    BandgapOn();
    EnableRegularLDO();
    DisableWeakLDO();

    ClockSetToLowSpeedRun();

    /* 将SOC电压调整为1.15V */
    DCDC_AdjustTargetVoltage(DCDC, 0xe, 0x1);
}

/**
 * @brief  LPM低功耗运行模式
 * @return 无
 *   @retval 无
 */
void LPM_LowPowerRun(void)
{
    ClockSetToLowPowerRun();

    /* 断电 USBPHY */
    PowerDownUSBPHY();

    /* 将SOC电压调整为0.95V */
    DCDC_AdjustTargetVoltage(DCDC, 0x6, 0x1);
    /* DCM 模式 */
    DCDC_BootIntoDCM(DCDC);
    /* 断开负载电阻的内部 */
    DCDC->REG1 &= ~DCDC_REG1_REG_RLOAD_SW_MASK;
    /* 掉电输出范围比较器 */
    DCDC->REG0 |= DCDC_REG0_PWD_CMP_OFFSET_MASK;

    /* 使能 FET ODRIVE */
    PMU->REG_CORE_SET = PMU_REG_CORE_FET_ODRIVE_MASK;
    /* 连接vdd_high_in并连接vdd_snvs_in */
    PMU->MISC0_CLR = PMU_MISC0_DISCON_HIGH_SNVS_MASK;

    EnableWeakLDO();
    DisableRegularLDO();
    BandgapOff();
}

/**
 * @brief  LPM进入系统空闲
 * @return 无
 *   @retval 无
 */
void LPM_EnterSystemIdle(void)
{
    /* 设置等待模式配置 */
    LPM_SetWaitModeConfig();
    /* 设置低功耗时钟门 */
    SetLowPowerClockGate();
    /* 将时钟设置成低功耗空闲 */
    ClockSetToSystemIdle();
    /* 断电 USBPHY */
    PowerDownUSBPHY();
    /* DCDC 到 1.15V */
    DCDC_AdjustTargetVoltage(DCDC, 0xe, 0x1);
    /* DCM 模式 */
    DCDC_BootIntoDCM(DCDC);
    /* 断开负载电阻的内部 */
    DCDC->REG1 &= ~DCDC_REG1_REG_RLOAD_SW_MASK;
    /* 掉电输出范围比较器 */
    DCDC->REG0 |= DCDC_REG0_PWD_CMP_OFFSET_MASK;
    /* 使能 FET ODRIVE */
    PMU->REG_CORE_SET = PMU_REG_CORE_FET_ODRIVE_MASK;
    /* 连接vdd_high_in并连接vdd_snvs_in */
    PMU->MISC0_CLR = PMU_MISC0_DISCON_HIGH_SNVS_MASK;
    /* 使能弱LDO */
    EnableRegularLDO();
    /* 失能常规LDO*/
    DisableWeakLDO();
    /* 带隙失能 */
    BandgapOn();
    /* 外围设备进入打盹模式 */
    PeripheralEnterDozeMode();
    __DSB();
    __WFI();
    __ISB();
}

/**
 * @brief  LPM退出系统空闲
 * @return 无
 *   @retval 无
 */
void LPM_ExitSystemIdle(void)
{
    PeripheralExitDozeMode();
    LPM_SetRunModeConfig();
}

/**
 * @brief  LPM进入低功耗空闲
 * @return 无
 *   @retval 无
 */
void LPM_EnterLowPowerIdle(void)
{
     /*************************第一部分*********************/
     /* 设置等待模式配置 */
    LPM_SetWaitModeConfig();
     /* 设置低功耗时钟门 */
    SetLowPowerClockGate();
     /* 将时钟设置成低功耗空闲 */
    ClockSetToLowPowerIdle();
    /*断电 USBPHY */
    PowerDownUSBPHY();
     /*************************第二部分*********************/
    /* 将SOC电压调整为0.95V */
    DCDC_AdjustTargetVoltage(DCDC, 0x6, 0x1);
    /* DCM 模式 */
    DCDC_BootIntoDCM(DCDC);
    /* 断开负载电阻的内部 */
    DCDC->REG1 &= ~DCDC_REG1_REG_RLOAD_SW_MASK;
    /* 掉电输出范围比较器*/
    DCDC->REG0 |= DCDC_REG0_PWD_CMP_OFFSET_MASK;
    /* 使能 FET ODRIVE */
    PMU->REG_CORE_SET = PMU_REG_CORE_FET_ODRIVE_MASK;
    /* 连接vdd_high_in并连接vdd_snvs_in */
    PMU->MISC0_CLR = PMU_MISC0_DISCON_HIGH_SNVS_MASK;
     /*************************第三部分*********************/
    /* 使能弱LDO */
    EnableWeakLDO();
    /* 失能常规LDO */
    DisableRegularLDO();
    /* 带隙失能 */
    BandgapOff();
    /* 外围设备进入打盹模式 */
    PeripheralEnterDozeMode();
    __DSB();
    __WFI();
    __ISB();
}

/**
 * @brief  LPM退出低功耗空闲
 * @return 无
 *   @retval 无
 */
void LPM_ExitLowPowerIdle(void)
{
    /* 外设退出打盹模式 */
    PeripheralExitDozeMode();
    /* 设置运行模式 */
    LPM_SetRunModeConfig();
}

/**
 * @brief  LPM进入暂停模式
 * @return 无
 *   @retval 无
 */
void LPM_EnterSuspend()
{
	  /*************************第一部分*********************/
    uint32_t i;
    uint32_t gpcIMR[LPM_GPC_IMR_NUM];
    /*设置LPM停止模式*/
    LPM_SetStopModeConfig();
    /* 设置低功耗时钟门 */
    SetLowPowerClockGate();
		/*************************第二部分*********************/
    /* 断开负载电阻的内部 */
    DCDC->REG1 &= ~DCDC_REG1_REG_RLOAD_SW_MASK;
    /* 关掉 FlexRAM0 */
    GPC->CNTR |= GPC_CNTR_PDRAM0_PGE_MASK;
    /* 关掉 FlexRAM1 */
    PGC->MEGA_CTRL |= PGC_MEGA_CTRL_PCR_MASK;
    /*清理并禁用数据高速缓存以确保将上下文保存到RAM中 */
    SCB_CleanDCache();
    SCB_DisableDCache();
		/*************************第三部分*********************/
    /* 将LP电压调整为0.925V */
    DCDC_AdjustTargetVoltage(DCDC, 0x13, 0x1);
    /* 切换DCDC以使用DCDC内部OSC */
    DCDC_SetClockSource(DCDC, kDCDC_ClockInternalOsc);
    /* 断电 USBPHY */
    PowerDownUSBPHY();
    /* 请求时关闭CPU */
    PGC->CPU_CTRL = PGC_CPU_CTRL_PCR_MASK;
    /* 使能 FET ODRIVE */
    PMU->REG_CORE_SET = PMU_REG_CORE_FET_ODRIVE_MASK;
    /* 连接vdd_high_in并连接vdd_snvs_in*/
    PMU->MISC0_CLR = PMU_MISC0_DISCON_HIGH_SNVS_MASK;
    /* STOP_MODE配置，在停止模式下关闭RTC以外的所有模拟 */
    PMU->MISC0_CLR = PMU_MISC0_STOP_MODE_CONFIG_MASK;
	/*************************第四部分*********************/
		 /*在启用RBC计数器之前屏蔽所有GPC中断
			*如果已经中断，请避免计数器启动太早
			*等待。*/
    /* 循环清楚  gpcIMR寄存器*/
    for (i = 0; i < LPM_GPC_IMR_NUM; i++)
    {
        gpcIMR[i]   = GPC->IMR[i];
        GPC->IMR[i] = 0xFFFFFFFFU;
    }
		/*CCM：无法通过电源门控从等待/停止模式恢复
      *将REG_BYPASS_COUNTER配置为2
      *在此启用RBC旁路计数器以阻止中断。 RBC柜台
      *需要不少于2*/
    CCM->CCR = (CCM->CCR & ~CCM_CCR_REG_BYPASS_COUNT_MASK) | CCM_CCR_REG_BYPASS_COUNT(2);
    CCM->CCR |= (CCM_CCR_OSCNT(0xAF) | CCM_CCR_COSC_EN_MASK | CCM_CCR_RBC_EN_MASK);
		 /*现在延迟一会儿（3usec）
			*所以短循环就足够了。 需要这种延迟来确保这一点
			*如果中断已经挂起，RBC计数器可以开始计数
			*或者在ARM即将断言DSM_request时中断到达。*/
    SDK_DelayAtLeastUs(3);
		/*************************第五部分*********************/
    /* 恢复所有GPC中断。 */
    for (i = 0; i < LPM_GPC_IMR_NUM; i++)
    {
        GPC->IMR[i] = gpcIMR[i];
    }
    /*外设退出停止模式*/
    PeripheralEnterStopMode();
    __DSB();
    __WFI();
    __ISB();
}

/**
 * @brief  LPM进入SNVS
 * @return 无
 *   @retval 无
 */
void LPM_EnterSNVS(void)
{
    SNVS->LPCR |= SNVS_LPCR_TOP_MASK;
    while (1) /* 关掉 */
    {
    }
}
