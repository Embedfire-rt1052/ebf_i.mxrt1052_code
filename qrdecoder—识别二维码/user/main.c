/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   ��ά���������
  ******************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  i.MXRT1052������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************
  */
#include "fsl_debug_console.h"

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include ".\lcd\lcd.h"
#include ".\camera\bsp_ov5640.h"
#include "qr_decoder_user.h"
#include "fsl_log.h"
/*******************************************************************
 * Prototypes
 *******************************************************************/

/*******************************************************************
 * Code
 *******************************************************************/

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
		char  qr_type_len=0;
    short qr_data_len=0;
    char qr_type_buf[10];
    char qr_data_buf[512];
    int addr=0;
    int i=0,j=0;
    char qr_num=0;
	
	  /* ��ʼ���ڴ汣����Ԫ */
    BOARD_ConfigMPU();
		/* ��ʼ������������ */
    BOARD_InitPins();
		/* ��ʼ��������ʱ�� */
    BOARD_BootClockRUN();
		/* ��ʼ�����Դ��� */
		BOARD_InitDebugConsole();
		/* ��ʼ��Һ���ӿ�*/
    BOARD_InitLcd();
		/* ��ӡϵͳʱ�� */
		PRINTF("\r\n");
		PRINTF("*****��ӭʹ�� Ұ��i.MX RT1052 ������*****\r\n");
		PRINTF("CPU:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_CpuClk));
		PRINTF("AHB:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_AhbClk));
		PRINTF("SEMC:            %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SemcClk));
		PRINTF("SYSPLL:          %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllClk));
		PRINTF("SYSPLLPFD0:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd0Clk));
		PRINTF("SYSPLLPFD1:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd1Clk));
		PRINTF("SYSPLLPFD2:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd2Clk));
		PRINTF("SYSPLLPFD3:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd3Clk));	
		
		PRINTF("CSI RGB565 example start...\r\n");	
		Camera_Init();
    while(1)
		{
        ELCDIF_ClearInterruptStatus(APP_ELCDIF, kELCDIF_CurFrameDone);
        /* �ȴ��ǻ���������ڻ״̬ */
        while (!(kELCDIF_CurFrameDone & ELCDIF_GetInterruptStatus(APP_ELCDIF)))
        {
        }

        CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, activeFrameAddr);
        activeFrameAddr = inactiveFrameAddr;

        /* �ȴ���ȡ����֡����������ʾ */
        while (kStatus_Success != CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &inactiveFrameAddr))
        {
        }
        ELCDIF_SetNextBufferAddr(APP_ELCDIF, inactiveFrameAddr);			
				/*��ȡһ֡ͼƬ��FSMC_LCD_ADDRESSΪ���ͼƬ���׵�ַ*/
				/*LCD_PIXEL_WIDTHΪͼƬ��ȣ�LCD_PIXEL_HEIGHTΪͼƬ�߶�*/
				get_image(activeFrameAddr,800,480);
				
				//��ά��ʶ�𣬷���ʶ������ĸ���
				qr_num = QR_decoder();
					 
				if(qr_num)
				{
					 //ʶ��ɹ������������־
					 //beep_on_flag =1;
					 
					 //����������ǰ���ʶ������ĸ�����װ�õĶ�ά���飬��Щ������Ҫ
					 //����ʶ������ĸ�������������ͨ�����ڷ��͵���λ�������ն�
					 for(i=0;i < qr_num;i++)
					 {
							 qr_type_len = decoded_buf[i][addr++];//��ȡ�������ͳ���
							 
							 for(j=0;j < qr_type_len;j++)
							 {
									qr_type_buf[j]=decoded_buf[i][addr++];//��ȡ������������
							 }
							 qr_data_len  = decoded_buf[i][addr++]<<8; //��ȡ�������ݳ��ȸ�8λ
							 qr_data_len |= decoded_buf[i][addr++];    //��ȡ�������ݳ��ȵ�8λ
							 
							 for(j=0;j < qr_data_len;j++)
							 {
									qr_data_buf[j]=decoded_buf[i][addr++];//��ȡ��������
							 }

							 LOG_Push((unsigned char *)qr_type_buf, qr_type_len);//���ڷ��ͽ�������
							 LOG_Push((unsigned char *)":", 1);    //���ڷ��ͷָ���
							 LOG_Push((unsigned char *)qr_data_buf, qr_data_len);//���ڷ��ͽ�������
							 LOG_Push((unsigned char *)"\r\n", 2); //���ڷ��ͷָ���
							 addr =0;//����
					 }
									 
				} 
		
		}			

}
