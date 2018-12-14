/**
	* IOMUXC外设控制相关头文件
  */
  
/* GPIO_AD_B0_09引脚的复用功能 */
/*
 * 宏对应的五个数值是用在IOMUXC_SetPinMux函数的前五个参数
 *  muxRegister:本引脚MUX寄存器的地址
 *  muxMode:要配置的复用模式
 *  inputRegister:可选的要设置的寄存器地址
 *  inputDaisy:要给上述可选的寄存器赋予的值
 *  configRegister:本引脚PAD属性配置寄存器的地址
*/
#define IOMUXC_GPIO_AD_B0_09_JTAG_TDI 				0x401F80E0U, 0x0U, 0, 					0, 		0x401F82D0U
#define IOMUXC_GPIO_AD_B0_09_FLEXPWM2_PWMA03 	0x401F80E0U, 0x1U, 0x401F8474U, 0x3U, 0x401F82D0U
#define IOMUXC_GPIO_AD_B0_09_ENET_RX_DATA02 	0x401F80E0U, 0x2U, 0, 					0, 		0x401F82D0U
#define IOMUXC_GPIO_AD_B0_09_SAI2_TX_DATA 		0x401F80E0U, 0x3U, 0, 					0, 		0x401F82D0U
#define IOMUXC_GPIO_AD_B0_09_CSI_DATA04 			0x401F80E0U, 0x4U, 0x401F8408U, 0x1U, 0x401F82D0U
#define IOMUXC_GPIO_AD_B0_09_GPIO1_IO09 			0x401F80E0U, 0x5U, 0, 					0, 		0x401F82D0U
#define IOMUXC_GPIO_AD_B0_09_XBAR1_IN21 			0x401F80E0U, 0x6U, 0x401F8658U, 0x1U, 0x401F82D0U
#define IOMUXC_GPIO_AD_B0_09_GPT2_CLK 				0x401F80E0U, 0x7U, 0, 					0, 		0x401F82D0U


/*
 * 设置IOMUXC外设，配置引脚的MUX复用模式
 *  前五个函数参数可直接用引脚功能的宏
 *
 * 以下例子把 PTA6 配置为串口引脚功能 lpuart0_tx:
 *  IOMUXC_SetPinMux(IOMUXC_PTA6_LPUART0_TX, 0);
 *  
 * 以下例子设置GPIO_AD_B0_09 作为普通IO功能 GPIO1_IO09:
 * 
 * IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_09_GPIO1_IO09, 0);
 *  
 *  参数说明
 *  muxRegister:本引脚MUX寄存器的地址
 *  muxMode:要配置的复用模式
 *  inputRegister:可选的要设置的寄存器地址
 *  inputDaisy:要给上述可选的寄存器赋予的值
 *  configRegister:本引脚PAD属性配置寄存器的地址
 *  inputOnfield:SION输入回路配置域的值，1或0
 */
static inline void IOMUXC_SetPinMux(uint32_t muxRegister,
                                    uint32_t muxMode,
                                    uint32_t inputRegister,
                                    uint32_t inputDaisy,
                                    uint32_t configRegister,
                                    uint32_t inputOnfield)
{
		/* 设置MUX_MODE及SION */
    *((volatile uint32_t *)muxRegister) =
        IOMUXC_SW_MUX_CTL_PAD_MUX_MODE(muxMode) | IOMUXC_SW_MUX_CTL_PAD_SION(inputOnfield);
	
		/* 若可选寄存器非0，则向它赋值 */
    if (inputRegister)
    {
        *((volatile uint32_t *)inputRegister) = inputDaisy;
    }
}


/*
 *  设置IOMUXC外设，配置引脚的PAD属性
 *  前五个函数参数可直接用引脚功能的宏
 *
 * 以下例子配置I2C引脚的SCL属性 IOMUXC_PTA3_LPI2C0_SCLS:
 * 
 * IOMUXC_SetPinConfig(IOMUXC_PTA3_LPI2C0_SCLS,
 *										 IOMUXC_SW_PAD_CTL_PAD_PUS_MASK|
 *										 IOMUXC_SW_PAD_CTL_PAD_PUS(2U))
 *  
 *  参数说明
 *  muxRegister:本引脚MUX寄存器的地址，本函数实际没使用这参数
 *  muxMode:要配置的复用模式，本函数实际没使用这参数
 *  inputRegister:可选的要设置的寄存器地址，本函数实际没使用这参数
 *  inputDaisy:要给上述可选的寄存器赋予的值，本函数实际没使用这参数
 *  configRegister:本引脚PAD属性配置寄存器的地址
 *  configValue:要给PAD属性配置寄存器赋的值
 */
static inline void IOMUXC_SetPinConfig(uint32_t muxRegister,
                                       uint32_t muxMode,
                                       uint32_t inputRegister,
                                       uint32_t inputDaisy,
                                       uint32_t configRegister,
                                       uint32_t configValue)
{
		/* 向属性配置寄存器赋值 */
    if (configRegister)
    {
        *((volatile uint32_t *)configRegister) = configValue;
    }
}

