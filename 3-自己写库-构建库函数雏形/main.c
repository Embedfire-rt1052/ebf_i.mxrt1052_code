
/**
	�Լ�д�⣬�����⺯������
  */
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"


/*
  * ���½ڴ����Ǵ� ʹ�üĴ�������LED���½ڴ����޸Ĺ���
  * ����ѭ�򽥽���һ�£�ÿһ�����趼��Ӧһ���ִ��룬����
  * ���½ڴ����кö�����֣���ͬ���ֶ���ʹ����������������
  * ���ֻ���������Ĵ��벿�֡�
  *
  * ���������C���Ե� if else ���ƣ�ֻ�е�����Ϊ�������µ�
  * ����Żᱻ���룬���Բ�Ҫ�����ж��main��������Ϊ����ȷ��
  * ��ʵ���ձ����ֻ��һ��
 */

/* ԭ�Ĵ����汾������Ϊ�ٱ�ʾ���������*/
#if 0

/* �ɷ�ʽ��Ҫһ��������Ĵ�����ַ */	
#define GPIO1_DR						*(unsigned int*)(GPIO1_BASE+0x00)
#define GPIO1_GDIR					*(unsigned int*)(GPIO1_BASE+0x04)
#define GPIO1_PSR						*(unsigned int*)(GPIO1_BASE+0x08)
#define GPIO1_ICR1					*(unsigned int*)(GPIO1_BASE+0x0C)
#define GPIO1_ICR2					*(unsigned int*)(GPIO1_BASE+0x10)
#define GPIO1_IMR						*(unsigned int*)(GPIO1_BASE+0x14)
#define GPIO1_ISR						*(unsigned int*)(GPIO1_BASE+0x18)
#define GPIO1_EDGE_SEL			*(unsigned int*)(GPIO1_BASE+0x1C)

/**
  *   ������
  */
int main(void)
{	
	/* ����GPIO1�˿ڵ�ʱ�� */
	
	/* ��տ���GPIO1�˿�ʱ�ӵ�bit26��bit27 */
	
	CCM_CCGR1 &= ~(unsigned int)(3<<26);
	
	/* ��bit26��bit27����Ϊ0b01��������GPIO1ʱ�� */
	CCM_CCGR1 |= (unsigned int)(1<<26);
		
	/* ����MUX�Ĵ���Ϊ0x05����ʾ������������ͨGPIO */
	IOMUXC_MUX_GPIO_AD_B0_09 = (unsigned int)0x05;
	
	/* ����PAD�Ĵ����������ŵ����� */
	IOMUXC_PAD_GPIO_AD_B0_09 = (unsigned int)0x000B0;
	
	/*  ��������˵��:
		ʮ��������0x000B0 = ��������0000 0000 0000 1011 0000 b
	*/
	
	/* 	bit0:       	SRE:	0 b   		ѹ����: ��ѹ����
			bit1~bit2:  	-:		00 b    	�Ĵ���������
			bit3~bit5:  	DSE:	110 b 		����ǿ��: R0/6 ������Ϊ���ʱ��Ч ��
			bit6~bit7:		SPEED:10 b 			���� : medium(100MHz)
			bit8~bit10:		-:		000 b			�Ĵ���������
			bit11:				ODE:	0 b				��©����: �ر� 
																		����©����̬�������������ã���I2C ��
			bit12:				PKE:	0 b				��/����������: �ر�
			bit13:				PUE:	0 b				��/������ѡ��: �ر�����������������������ֵ��Ч
			bit14~bit15:	PUS:	00 b			����/����ѡ��: �ر�����������������������ֵ��Ч
			bit16:				HYS:	0 b				�ͻ�������: �ر� 
																	��������ʱ��Ч��ʩ���ش�������ʹ�ܺ���Թ�������������
			bit17~bit31: 	-:		0 b 			�Ĵ���������
	*/
		
	/* ����GPIO1_09���ŵķ���ʹ�����ģʽ */
	GPIO1_GDIR |= (unsigned int)(1<<9);

	/* ��������Ϊ�͵�ƽ������LED�� */
	GPIO1_DR &= ~(unsigned int)(1<<9);
	
	while(1);

}

/* �Ľ���1��ʹ�ýṹ����ʼĴ���������Ϊ�ٱ�ʾ���������*/
#elif 0
/**
  *   ������
  */
int main(void)
{	
	/* ����GPIO1�˿ڵ�ʱ�� */
	
	/* ��տ���GPIO1�˿�ʱ�ӵ�bit26��bit27 */
	
	CCM_CCGR1 &= ~(unsigned int)(3<<26);
	
	/* ��bit26��bit27����Ϊ0b01��������GPIO1ʱ�� */
	CCM_CCGR1 |= (unsigned int)(1<<26);
		
	/* ����MUX�Ĵ���Ϊ0x05����ʾ������������ͨGPIO */
	IOMUXC_MUX_GPIO_AD_B0_09 = (unsigned int)0x05;
	
	/* ����PAD�Ĵ����������ŵ����� */
	IOMUXC_PAD_GPIO_AD_B0_09 = (unsigned int)0x000B0;
	
	/*  ��������˵��:
		ʮ��������0x000B0 = ��������0000 0000 0000 1011 0000 b
	*/
	
	/* 	bit0:       	SRE:	0 b   		ѹ����: ��ѹ����
			bit1~bit2:  	-:		00 b    	�Ĵ���������
			bit3~bit5:  	DSE:	110 b 		����ǿ��: R0/6 ������Ϊ���ʱ��Ч ��
			bit6~bit7:		SPEED:10 b 			���� : medium(100MHz)
			bit8~bit10:		-:		000 b			�Ĵ���������
			bit11:				ODE:	0 b				��©����: �ر� 
																		����©����̬�������������ã���I2C ��
			bit12:				PKE:	0 b				��/����������: �ر�
			bit13:				PUE:	0 b				��/������ѡ��: �ر�����������������������ֵ��Ч
			bit14~bit15:	PUS:	00 b			����/����ѡ��: �ر�����������������������ֵ��Ч
			bit16:				HYS:	0 b				�ͻ�������: �ر� 
																	��������ʱ��Ч��ʩ���ش�������ʹ�ܺ���Թ�������������
			bit17~bit31: 	-:		0 b 			�Ĵ���������
	*/
		
	/* ����GPIO1_09���ŵķ���ʹ�����ģʽ */
	GPIO1->GDIR |= (unsigned int)(1<<9);

	/* ��������Ϊ�͵�ƽ������LED�� */
	GPIO1->DR &= ~(unsigned int)(1<<9);
	
	while(1);
}


/* �Ľ���2��ʹ��GPIO����������Ϊ�ٱ�ʾ��������� */
#elif 0
/**
  *   ������
  */
int main(void)
{	
	/* ʹ��GPIO��ʼ���ṹ�嶨��һ��������������GPIO */
	gpio_pin_config_t led_config;
	/* ����GPIO1�˿ڵ�ʱ�� */
	
	/* ��տ���GPIO1�˿�ʱ�ӵ�bit26��bit27 */
	
	CCM_CCGR1 &= ~(unsigned int)(3<<26);
	
	/* ��bit26��bit27����Ϊ0b01��������GPIO1ʱ�� */
	CCM_CCGR1 |= (unsigned int)(1<<26);
		
	/* ����MUX�Ĵ���Ϊ0x05����ʾ������������ͨGPIO */
	IOMUXC_MUX_GPIO_AD_B0_09 = (unsigned int)0x05;
	
	/* ����PAD�Ĵ����������ŵ����� */
	IOMUXC_PAD_GPIO_AD_B0_09 = (unsigned int)0x000B0;
	
	/*  ��������˵��:
		ʮ��������0x000B0 = ��������0000 0000 0000 1011 0000 b
	*/
	
	/* 	bit0:       	SRE:	0 b   		ѹ����: ��ѹ����
			bit1~bit2:  	-:		00 b    	�Ĵ���������
			bit3~bit5:  	DSE:	110 b 		����ǿ��: R0/6 ������Ϊ���ʱ��Ч ��
			bit6~bit7:		SPEED:10 b 			���� : medium(100MHz)
			bit8~bit10:		-:		000 b			�Ĵ���������
			bit11:				ODE:	0 b				��©����: �ر� 
																		����©����̬�������������ã���I2C ��
			bit12:				PKE:	0 b				��/����������: �ر�
			bit13:				PUE:	0 b				��/������ѡ��: �ر�����������������������ֵ��Ч
			bit14~bit15:	PUS:	00 b			����/����ѡ��: �ر�����������������������ֵ��Ч
			bit16:				HYS:	0 b				�ͻ�������: �ر� 
																	��������ʱ��Ч��ʩ���ش�������ʹ�ܺ���Թ�������������
			bit17~bit31: 	-:		0 b 			�Ĵ���������
	*/
		
//	/* ����GPIO1_09���ŵķ���ʹ�����ģʽ */
//	GPIO1_GDIR |= (unsigned int)(1<<9);

//	/* ��������Ϊ�͵�ƽ������LED�� */
//	GPIO1_DR &= ~(unsigned int)(1<<9);

	led_config.direction = kGPIO_DigitalOutput;	//���ģʽ
	led_config.outputLogic = 1;									//Ĭ�ϸߵ�ƽ
	led_config.interruptMode = kGPIO_NoIntmode;	//��ʹ���ж�
	
	/* ʹ��led_config ��ʼ��GPIO1_IO09*/
	GPIO_PinInit(GPIO1,9,&led_config);
	
	/* ����GPIO1_IO09Ϊ�͵�ƽ������LED�� */
	GPIO_PinWrite(GPIO1,9,0);
	
	while(1);

}

/* �Ľ���3��ʹ��IOMUXC����������Ϊ�ٱ�ʾ��������� */
#elif 1

/**
  *   ������
  */
int main(void)
{	
	/* ʹ��GPIO��ʼ���ṹ�嶨��һ��������������GPIO */
	gpio_pin_config_t led_config;
  
	/* ����GPIO1�˿ڵ�ʱ�� */
	
	/* ��տ���GPIO1�˿�ʱ�ӵ�bit26��bit27 */
	
	CCM_CCGR1 &= ~(unsigned int)(3<<26);
	
	/* ��bit26��bit27����Ϊ0b01��������GPIO1ʱ�� */
	CCM_CCGR1 |= (unsigned int)(1<<26);
		 
  /* ��������ΪMUX_MODE��SION
      IOMUXC_GPIO_AD_B0_09_GPIO1_IO09���ʾ�����������
      0x401F80E0U,  GPIO_AD_B0_09��MUX�Ĵ����ĵ�ַ
      0x5U,         ѡ��ATL5,��GPIO����
      0,            ��ѡ��Ҫͬʱ���õļĴ�����Ϊ0��ʾ������
      0,            ��ѡ�Ĵ���Ҫ�����ֵ
      0x401F82D0U   GPIO_AD_B0_09��PAD�������üĴ����ĵ�ַ
  */
  /*  ���ø���ΪGPIO1_IO09����
      ��ʹ��SION����
  */  
	IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_09_GPIO1_IO09, 
										0 );		


	
	/* ������������ */
	IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_09_GPIO1_IO09,
											SRE_0_SLOW_SLEW_RATE|
											DSE_6_R0_6|
											SPEED_2_MEDIUM_100MHz|
											ODE_0_OPEN_DRAIN_DISABLED|
											PKE_0_PULL_KEEPER_DISABLED|
											PUE_0_KEEPER_SELECTED|
											PUS_0_100K_OHM_PULL_DOWN|
											HYS_0_HYSTERESIS_DISABLED
											);

	led_config.direction = kGPIO_DigitalOutput;	//���ģʽ
	led_config.outputLogic = 1;									//Ĭ�ϸߵ�ƽ
	led_config.interruptMode = kGPIO_NoIntmode;	//��ʹ���ж�
	
	/* ʹ��led_config ��ʼ��GPIO1_IO09*/
	GPIO_PinInit(GPIO1,9,&led_config);
	
	/* ����GPIO1_IO09Ϊ�͵�ƽ������LED�� */
	GPIO_PinWrite(GPIO1,9,0);
	
	while(1);

}

#endif



// ����Ϊ�գ�Ŀ����Ϊ��ƭ��������������
void SystemInit(void)
{	
}
