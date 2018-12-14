/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   二维码解码例程
  ******************************************************************
  * @attention
  *
  * 实验平台:野火  i.MXRT1052开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
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
  * @brief  主函数
  * @param  无
  * @retval 无
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
	
	  /* 初始化内存保护单元 */
    BOARD_ConfigMPU();
		/* 初始化开发板引脚 */
    BOARD_InitPins();
		/* 初始化开发板时钟 */
    BOARD_BootClockRUN();
		/* 初始化调试串口 */
		BOARD_InitDebugConsole();
		/* 初始化液晶接口*/
    BOARD_InitLcd();
		/* 打印系统时钟 */
		PRINTF("\r\n");
		PRINTF("*****欢迎使用 野火i.MX RT1052 开发板*****\r\n");
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
        /* 等待非活动缓冲区处于活动状态 */
        while (!(kELCDIF_CurFrameDone & ELCDIF_GetInterruptStatus(APP_ELCDIF)))
        {
        }

        CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, activeFrameAddr);
        activeFrameAddr = inactiveFrameAddr;

        /* 等待获取完整帧缓冲区以显示 */
        while (kStatus_Success != CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &inactiveFrameAddr))
        {
        }
        ELCDIF_SetNextBufferAddr(APP_ELCDIF, inactiveFrameAddr);			
				/*获取一帧图片，FSMC_LCD_ADDRESS为存放图片的首地址*/
				/*LCD_PIXEL_WIDTH为图片宽度，LCD_PIXEL_HEIGHT为图片高度*/
				get_image(activeFrameAddr,800,480);
				
				//二维码识别，返回识别条码的个数
				qr_num = QR_decoder();
					 
				if(qr_num)
				{
					 //识别成功，蜂鸣器响标志
					 //beep_on_flag =1;
					 
					 //解码的数据是按照识别条码的个数封装好的二维数组，这些数据需要
					 //根据识别条码的个数，按组解包并通过串口发送到上位机串口终端
					 for(i=0;i < qr_num;i++)
					 {
							 qr_type_len = decoded_buf[i][addr++];//获取解码类型长度
							 
							 for(j=0;j < qr_type_len;j++)
							 {
									qr_type_buf[j]=decoded_buf[i][addr++];//获取解码类型名称
							 }
							 qr_data_len  = decoded_buf[i][addr++]<<8; //获取解码数据长度高8位
							 qr_data_len |= decoded_buf[i][addr++];    //获取解码数据长度低8位
							 
							 for(j=0;j < qr_data_len;j++)
							 {
									qr_data_buf[j]=decoded_buf[i][addr++];//获取解码数据
							 }

							 LOG_Push((unsigned char *)qr_type_buf, qr_type_len);//串口发送解码类型
							 LOG_Push((unsigned char *)":", 1);    //串口发送分隔符
							 LOG_Push((unsigned char *)qr_data_buf, qr_data_len);//串口发送解码数据
							 LOG_Push((unsigned char *)"\r\n", 2); //串口发送分隔符
							 addr =0;//清零
					 }
									 
				} 
		
		}			

}
