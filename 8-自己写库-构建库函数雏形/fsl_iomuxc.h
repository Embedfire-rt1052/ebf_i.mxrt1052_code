/**
	* IOMUXC����������ͷ�ļ�
  */
  
/* GPIO_AD_B0_09���ŵĸ��ù��� */
/*
 * ���Ӧ�������ֵ������IOMUXC_SetPinMux������ǰ�������
 *  muxRegister:������MUX�Ĵ����ĵ�ַ
 *  muxMode:Ҫ���õĸ���ģʽ
 *  inputRegister:��ѡ��Ҫ���õļĴ�����ַ
 *  inputDaisy:Ҫ��������ѡ�ļĴ��������ֵ
 *  configRegister:������PAD�������üĴ����ĵ�ַ
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
 * ����IOMUXC���裬�������ŵ�MUX����ģʽ
 *  ǰ�������������ֱ�������Ź��ܵĺ�
 *
 * �������Ӱ� PTA6 ����Ϊ�������Ź��� lpuart0_tx:
 *  IOMUXC_SetPinMux(IOMUXC_PTA6_LPUART0_TX, 0);
 *  
 * ������������GPIO_AD_B0_09 ��Ϊ��ͨIO���� GPIO1_IO09:
 * 
 * IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_09_GPIO1_IO09, 0);
 *  
 *  ����˵��
 *  muxRegister:������MUX�Ĵ����ĵ�ַ
 *  muxMode:Ҫ���õĸ���ģʽ
 *  inputRegister:��ѡ��Ҫ���õļĴ�����ַ
 *  inputDaisy:Ҫ��������ѡ�ļĴ��������ֵ
 *  configRegister:������PAD�������üĴ����ĵ�ַ
 *  inputOnfield:SION�����·�������ֵ��1��0
 */
static inline void IOMUXC_SetPinMux(uint32_t muxRegister,
                                    uint32_t muxMode,
                                    uint32_t inputRegister,
                                    uint32_t inputDaisy,
                                    uint32_t configRegister,
                                    uint32_t inputOnfield)
{
		/* ����MUX_MODE��SION */
    *((volatile uint32_t *)muxRegister) =
        IOMUXC_SW_MUX_CTL_PAD_MUX_MODE(muxMode) | IOMUXC_SW_MUX_CTL_PAD_SION(inputOnfield);
	
		/* ����ѡ�Ĵ�����0����������ֵ */
    if (inputRegister)
    {
        *((volatile uint32_t *)inputRegister) = inputDaisy;
    }
}


/*
 *  ����IOMUXC���裬�������ŵ�PAD����
 *  ǰ�������������ֱ�������Ź��ܵĺ�
 *
 * ������������I2C���ŵ�SCL���� IOMUXC_PTA3_LPI2C0_SCLS:
 * 
 * IOMUXC_SetPinConfig(IOMUXC_PTA3_LPI2C0_SCLS,
 *										 IOMUXC_SW_PAD_CTL_PAD_PUS_MASK|
 *										 IOMUXC_SW_PAD_CTL_PAD_PUS(2U))
 *  
 *  ����˵��
 *  muxRegister:������MUX�Ĵ����ĵ�ַ��������ʵ��ûʹ�������
 *  muxMode:Ҫ���õĸ���ģʽ��������ʵ��ûʹ�������
 *  inputRegister:��ѡ��Ҫ���õļĴ�����ַ��������ʵ��ûʹ�������
 *  inputDaisy:Ҫ��������ѡ�ļĴ��������ֵ��������ʵ��ûʹ�������
 *  configRegister:������PAD�������üĴ����ĵ�ַ
 *  configValue:Ҫ��PAD�������üĴ�������ֵ
 */
static inline void IOMUXC_SetPinConfig(uint32_t muxRegister,
                                       uint32_t muxMode,
                                       uint32_t inputRegister,
                                       uint32_t inputDaisy,
                                       uint32_t configRegister,
                                       uint32_t configValue)
{
		/* ���������üĴ�����ֵ */
    if (configRegister)
    {
        *((volatile uint32_t *)configRegister) = configValue;
    }
}

