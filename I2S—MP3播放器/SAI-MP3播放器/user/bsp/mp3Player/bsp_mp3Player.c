/*
******************************************************************************
* @file    recorder.c
* @author  fire
* @version V1.0
* @date    2015-xx-xx
* @brief   WM8978放音功能测试+mp3解码
******************************************************************************
* @attention
*
* 实验平台:秉火  STM32 F767 开发板  
* 论坛    :http://www.chuxue123.com
* 淘宝    :http://firestm32.taobao.com
*
******************************************************************************
*/
//#include <stdio.h>
//#include <string.h>
//#include "./usart/bsp_debug_usart.h"
//#include "./wm8978/bsp_wm8978.h"
//#include "ff.h"
#include "bsp_mp3Player.h"
#include "fsl_debug_console.h"

#include "ff.h"
#include "mp3common.h"
#include "sai.h"
#include "fsl_wm8960.h"
// #include "diskio.h"
// #include "fsl_sd_disk.h"
// #include "./bsp/sd_fatfs_test/bsp_sd_fatfs_test.h"
// #include "bsp_mp3dec.h"
////#include "./sai/bsp_sai.h"
///* 推荐使用以下格式mp3文件：
// * 采样率：44100Hz
// * 声  道：2
// * 比特率：320kbps
// */

/* 处理立体声音频数据时，输出缓冲区需要的最大大小为2304*16/8字节(16为PCM数据为16位)，
* 这里我们定义MP3BUFFER_SIZE为2304，实际输出缓冲区为MP3BUFFER_SIZE*2个字节
*/
#define MP3BUFFER_SIZE  2304
#define INPUTBUF_SIZE   3000

static HMP3Decoder Mp3Decoder; /* mp3解码器指针	*/
static MP3FrameInfo		Mp3FrameInfo;		/* mP3帧信息  */
static MP3_TYPE mp3player;         /* mp3播放设备 */
//volatile uint8_t Isread=0;           /* DMA传输完成标志 */
//static uint8_t bufflag=0;          /* 数据缓存区选择标志 */

//uint32_t led_delay=0;

uint8_t inputbuf[INPUTBUF_SIZE]={0};        /* 解码输入缓冲区，1940字节为最大MP3帧大小  */
short outbuffer[2][MP3BUFFER_SIZE];  /* 解码输出缓冲区，也是I2S输入数据，实际占用字节数：RECBUFFER_SIZE*2 */

//FIL file;											/* file objects */
//FRESULT result;

/*解码后的音乐缓冲区，四字节对齐*/
extern AT_NONCACHEABLE_SECTION_ALIGN(uint8_t audioBuff[BUFFER_SIZE], 4);
extern AT_NONCACHEABLE_SECTION_ALIGN(uint8_t audioBuff2[BUFFER_SIZE], 4);
extern volatile FIL g_fileObject; //文件描述结构体
/*定义缓冲区标志位*/
extern volatile bool buffer1_full;
extern volatile bool buffer2_full;

/*sai dma 接收、发送句柄*/
extern sai_edma_handle_t txHandle;
extern sai_edma_handle_t rxHandle;

/*发送状态*/
extern volatile bool istxFinished;

UINT bw; /* 记录文件读取数据量 */

#define list_head_length 10
uint8_t list_head[list_head_length] = {0}; //定义标签头，主要用于计算mp3文件标签头的大小
int list_length = 0;                       //用于保存mp3 音乐文件标签长度
static uint8_t bufflag=0;          /* 数据缓存区选择标志 */
volatile uint8_t Isread=0;           /* DMA传输完成标志 */


/**
  * @brief  获取MP3ID3V2文件头的大小
  * @param  输入MP3文件开头的数据，至少10个字节
  * @retval ID3V2的大小
  */
uint32_t mp3_GetID3V2_Size(unsigned char *buf)
{
  uint32_t ID3V2_size;

  if (buf[0] == 'I' && buf[1] == 'D' && buf[2] == '3') //存在ID3V2
  {
    ID3V2_size = (buf[6] << 21) | (buf[7] << 14) | (buf[8] << 7) | buf[9];
  }
  else //不存在ID3V2
    ID3V2_size = 0;

  return ID3V2_size;
}


void delay(uint32_t count);

sai_transfer_t xfer_buffer1 = {0};

/**
  * @brief   MP3格式音频播放主程序
  * @param  无
  * @retval 无
  */
int times = 0;
void mp3PlayerDemo(const char *mp3file)
{
  uint8_t* read_ptr=inputbuf;
	uint32_t frames=0;
	int err=0, i=0, outputSamps=0;	
	int	read_offset = 0;				  /* 读偏移指针 */
	int	bytes_left = 0;					  /* 剩余字节数 */	
  int result ;
	
//	mp3player.ucFreq = SAI_AUDIOFREQ_DEFAULT;
//	mp3player.ucStatus = STA_IDLE;
//	mp3player.ucVolume = 40;
	
  /*打开文件*/
	err=f_open(&g_fileObject,mp3file,FA_READ);
	if(err!=FR_OK)
	{
		PRINTF("Open mp3file :%s fail!!!->%d\r\n",mp3file,err);
		result = f_close (&g_fileObject);
		return;	/* 停止播放 */
	}
	PRINTF("当前播放文件 -> %s\n",mp3file);
  
   /*读取mp3 标签头*/
   result = f_read(&g_fileObject, list_head, list_head_length, &bw);
   if ((result != FR_OK) || (list_head_length != bw))
   {
     PRINTF("读取%s失败 -> %d\r\n", mp3file, result);
     return;
   }
   list_length = mp3_GetID3V2_Size(list_head); //获取 mp3 文件标签长度。为设置文件偏移做准备
   
  
   result = f_lseek(&g_fileObject, list_length);
   if (result != FR_OK)
   {
     PRINTF("set f_lseek failed !\r\n");
     result = f_close(&g_fileObject);
     return;
   }
  
  
  
	
	//初始化MP3解码器
	Mp3Decoder = MP3InitDecoder();	
	if(Mp3Decoder==0)
	{
		PRINTF("初始化helix解码库设备\n");
		return;	/* 停止播放 */
	}
	PRINTF("初始化中...\n");
	
  delay(90000);
	// Delay_ms(10);	/* 延迟一段时间，等待I2S中断结束 */
// 	wm8978_Reset();		/* 复位WM8978到复位状态 */
// 	/* 配置WM8978芯片，输入为DAC，输出为耳机 */
// 	wm8978_CfgAudioPath(DAC_ON, EAR_LEFT_ON | EAR_RIGHT_ON);

// 	/* 调节音量，左右相同音量 */
// 	wm8978_SetOUT1Volume(mp3player.ucVolume);

// 	/* 配置WM8978音频接口为飞利浦标准I2S接口，16bit */
// 	wm8978_CfgAudioIF(SAI_I2S_STANDARD, 16);
	
// 	/*  初始化并配置I2S  */
//   SAI_Play_Stop();
// 	SAI_GPIO_Config();
// //	SAIxA_Tx_Config(SAI_I2S_STANDARD,SAI_PROTOCOL_DATASIZE_16BIT,mp3player.ucFreq);	
// //	SAIA_TX_DMA_Init();	
	
	bufflag=0;
	Isread=0;
	
	mp3player.ucStatus = STA_PLAYING;		/* 放音状态 */
  result=f_read(&g_fileObject,inputbuf,INPUTBUF_SIZE,&bw);
	if(result!=FR_OK)
	{
		PRINTF("读取%s失败 -> %d\r\n",mp3file,result);
		MP3FreeDecoder(Mp3Decoder);
		return;
	}
	read_ptr=inputbuf;
	bytes_left=bw;
	/* 进入主程序循环体 */
	while(mp3player.ucStatus == STA_PLAYING)
	{
		read_offset = MP3FindSyncWord(read_ptr, bytes_left);	//寻找帧同步，返回第一个同步字的位置
		if(read_offset < 0)										//没有找到同步字
		{
			result=f_read(&g_fileObject,inputbuf,INPUTBUF_SIZE,&bw);
			if(result!=FR_OK)
			{
				PRINTF("读取%s失败 -> %d\r\n",mp3file,result);
				break;
			}
			read_ptr=inputbuf;
			bytes_left=bw;
			continue;		//跳出循环2，回到循环1	
		}
		read_ptr += read_offset;				//偏移至同步字的位置
		bytes_left -= read_offset;				//同步字之后的数据大小	
		if(bytes_left < 1024)							//补充数据
		{
			/* 注意这个地方因为采用的是DMA读取，所以一定要4字节对齐  */
			i=(uint32_t)(bytes_left)&3;			//判断多余的字节
			if(i) i=4-i;						//需要补充的字节
			memcpy(inputbuf+i, read_ptr, bytes_left);	//从对齐位置开始复制
			read_ptr = inputbuf+i;										//指向数据对齐位置
			result = f_read(&g_fileObject, inputbuf+bytes_left+i, INPUTBUF_SIZE-bytes_left-i, &bw);//补充数据
			if(result!=FR_OK)
			{
				PRINTF("读取%s失败 -> %d\r\n",mp3file,result);
				break;
			}
			bytes_left += bw;		//有效数据流大小
		}
    
    
		err = MP3Decode(Mp3Decoder, &read_ptr, &bytes_left, outbuffer[bufflag], 0);	//bufflag开始解码 参数：mp3解码结构体、输入流指针、输入流大小、输出流指针、数据格式               兰若让老人了太，4嚄卡通版vo跑不跑53【】56		frames++;	
		if (err != ERR_MP3_NONE)	//错误处理
		{
			switch (err)
			{
				case ERR_MP3_INDATA_UNDERFLOW:
					PRINTF("ERR_MP3_INDATA_UNDERFLOW\r\n");
					result = f_read(&g_fileObject, inputbuf, INPUTBUF_SIZE, &bw);
					read_ptr = inputbuf;
					bytes_left = bw;
					break;		
				case ERR_MP3_MAINDATA_UNDERFLOW:
					/* do nothing - next call to decode will provide more mainData */
					PRINTF("ERR_MP3_MAINDATA_UNDERFLOW\r\n");
					break;		
				default:
					PRINTF("UNKNOWN ERROR:%d\r\n", err);		
					// 跳过此帧
					if (bytes_left > 0)
					{
						bytes_left --;
						read_ptr ++;
					}	
					break;
			}
			Isread=1;
		}
		else		//解码无错误，准备把数据输出到PCM
		{
			 MP3GetLastFrameInfo(Mp3Decoder, &Mp3FrameInfo);		//获取解码信息				
			 /* 输出到DAC */
			 outputSamps = Mp3FrameInfo.outputSamps;							//PCM数据个数
			 if (outputSamps > 0)
			 {
         xfer_buffer1.data = (uint8_t*)outbuffer[bufflag];
         xfer_buffer1.dataSize = outputSamps;
         istxFinished = false;
         SAI_TransferSendEDMA(DEMO_SAI, &txHandle, &xfer_buffer1);
         while(istxFinished == false);
         
         /*执行发送*/
         
//			 	if (Mp3FrameInfo.nChans == 1)	//单声道
//			 	{
//			 		//单声道数据需要复制一份到另一个声道
//			 		for (i = outputSamps - 1; i >= 0; i--)
//			 		{
//			 			outbuffer[bufflag][i * 2] = outbuffer[bufflag][i];
//			 			outbuffer[bufflag][i * 2 + 1] = outbuffer[bufflag][i];
//			 		}
//			 		outputSamps *= 2;
//			 	}//if (Mp3FrameInfo.nChans == 1)	//单声道
			 }//if (outputSamps > 0)
			
			// /* 根据解码信息设置采样率 */
			// if (Mp3FrameInfo.samprate != mp3player.ucFreq)	//采样率 
			// {
			// 	mp3player.ucFreq = Mp3FrameInfo.samprate;
				
			// 	printf(" \r\n Bitrate       %dKbps", Mp3FrameInfo.bitrate/1000);
			// 	printf(" \r\n Samprate      %dHz", mp3player.ucFreq);
			// 	printf(" \r\n BitsPerSample %db", Mp3FrameInfo.bitsPerSample);
			// 	printf(" \r\n nChans        %d", Mp3FrameInfo.nChans);
			// 	printf(" \r\n Layer         %d", Mp3FrameInfo.layer);
			// 	printf(" \r\n Version       %d", Mp3FrameInfo.version);
			// 	printf(" \r\n OutputSamps   %d", Mp3FrameInfo.outputSamps);
			// 	printf("\r\n");
			// 	if(mp3player.ucFreq >= SAI_AUDIOFREQ_DEFAULT)	//I2S_AudioFreq_Default = 2，正常的帧，每次都要改速率
			// 	{
          
			// 		SAIxA_Tx_Config(SAI_I2S_STANDARD,SAI_PROTOCOL_DATASIZE_16BIT,mp3player.ucFreq);						//根据采样率修改iis速率
      //     SAIA_TX_DMA_Init((uint32_t)(&outbuffer[0]),(uint32_t)&outbuffer[1],outputSamps);
			// 	}

			// 	SAI_Play_Start();
			
		}//else 解码正常
		
// 		if(file.fptr==file.fsize) 		//mp3文件读取完成，退出
// 		{
// 			printf("END\r\n");
// 			break;
// 		}	

// 		while(Isread==0)
// 		{
// 			led_delay++;
// 			if(led_delay==0xffffff)
// 			{
// 				led_delay=0;
// //				LED4_TOGGLE;
// 			}
// 			//Input_scan();		//等待DMA传输完成，此间可以运行按键扫描及处理事件
// 		}
// 		Isread=0;
	}
	// SAI_Play_Stop();
	// mp3player.ucStatus=STA_IDLE;
	// MP3FreeDecoder(Mp3Decoder);
	// f_close(&file);	
}



































// /**
//   * @brief   MP3格式音频播放主程序
//   * @param  无
//   * @retval 无
//   */
// int times = 0;
// void mp3PlayerDemo(const char *mp3file)
// {
//   int result = -1; //错误返回值

//   int read_offset = 0; //读偏移指针
//   int bytes_left = 0;  //剩余字节数
//   uint32_t test = 0;

//   sai_transfer_t xfer_buffer1 = {0};
//   sai_transfer_t xfer_buffer2 = {0};

//   /*打开文件，并检查是否成功*/
//   result = f_open(&g_fileObject, mp3file, FA_READ);
//   if (result != FR_OK)
//   {
//     PRINTF("Open mp3file :%s fail!!!->%d\r\n", mp3file, result);
//     result = f_close(&g_fileObject);
//     return; /* 停止播放 */
//   }
//   PRINTF("当前播放文件 -> %s\n", mp3file);

//   /*读取mp3 标签头*/
//   result = f_read(&g_fileObject, list_head, list_head_length, &bw);
//   if ((result != FR_OK) || (list_head_length != bw))
//   {
//     PRINTF("读取%s失败 -> %d\r\n", mp3file, result);
//     return;
//   }
//   list_length = mp3_GetID3V2_Size(list_head); //获取 mp3 文件标签长度。为设置文件偏移做准备

//   /*设置读、写偏移值，跳过 mp3 标签（长度设置存在问题，是不是要关闭文件重新设置长度？，list_length 长度是否包含
//   *标签头的10字节，测试显示没有包含，暂时直接设置文件读写指针。）*/
//   result = f_lseek(&g_fileObject, list_length);
//   if (result != FR_OK)
//   {
//     PRINTF("set f_lseek failed !\r\n");
//     result = f_close(&g_fileObject);
//     return;
//   }

//   /*初始化MP3解码器*/
//   Mp3Decoder = MP3InitDecoder();
//   if (Mp3Decoder == 0)
//   {
//     PRINTF("初始化helix解码库设备\n");
//     return; /* 停止播放 */
//   }
//   PRINTF("初始化中...\n");

//   delay(90000);
//   /*读取文件内容到inputbuf*/
//   result = f_read(&g_fileObject, inputbuf, INPUTBUF_SIZE, &bw);
//   if (result != FR_OK)
//   {
//     PRINTF("读取%s失败 -> %d\r\n", mp3file, result);
//     MP3FreeDecoder(Mp3Decoder);
//     return;
//   }
//   else
//   {
//     /* code */
//     PRINTF("读取%s成功 -> %d\r\n", mp3file, result);
//   }

//   /*获取数据帧偏移*/
//   read_offset = MP3FindSyncWord(inputbuf, bw); //寻找帧同步，返回第一个同步字的位置
//   if (read_offset < 0)                         //没有找到同步字
//   {
//     result = f_read(&g_fileObject, inputbuf, INPUTBUF_SIZE, &bw);
//     if (result != FR_OK)
//     {
//       PRINTF("读取%s失败 -> %d\r\n", mp3file, result);
//     }
//   }

//   // bytes_left = bw - read_offset;

//   //bufflag开始解码 参数：mp3解码结构体、输入流指针、输入流大小、输出流指针、数据格式
//   result = MP3Decode(Mp3Decoder, (char **)&inputbuf, &bytes_left, (short *)audioBuff, 0);
//   if (result != ERR_MP3_NONE) //错误处理
//   {
//     switch (result)
//     {
//     case ERR_MP3_INDATA_UNDERFLOW:
//       PRINTF("ERR_MP3_INDATA_UNDERFLOW\r\n");
//       break;
//     case ERR_MP3_MAINDATA_UNDERFLOW:
//       /* do nothing - next call to decode will provide more mainData */
//       PRINTF("ERR_MP3_MAINDATA_UNDERFLOW\r\n");
//       break;
//     default:
//       PRINTF("UNKNOWN ERROR:%d\r\n", result);
//     }
//   }
//   xfer_buffer1.data = audioBuff;
//   xfer_buffer1.dataSize = BUFFER_SIZE;
//   buffer1_full = true;

//   /*主循环*/
//   while (1)
//   {
//     /*buffer1 有数据*/
//     if (buffer1_full)
//     {
//       /*执行发送buffer1*/
//       SAI_TransferSendEDMA(DEMO_SAI, &txHandle, &xfer_buffer1);
//       while (istxFinished == false)
//       {
//         ;
//       }
//     }

//     if (buffer1_full == false)
//     {
//       /*读数据*/
//       /*读取文件内容到inputbuf*/
//       result = f_read(&g_fileObject, inputbuf, INPUTBUF_SIZE, &bw);
//       if (result != FR_OK)
//       {
//         PRINTF("读取%s失败 -> %d\r\n", mp3file, result);
//         MP3FreeDecoder(Mp3Decoder);
//         return;
//       }

//       //bufflag开始解码 参数：mp3解码结构体、输入流指针、输入流大小、输出流指针、数据格式
//       result = MP3Decode(Mp3Decoder, (char **)&inputbuf, &bytes_left, (short *)audioBuff, 0);
//       if (result != ERR_MP3_NONE) //错误处理
//       {
//         PRINTF("error \r\n");
//       }
//       xfer_buffer1.data = audioBuff;
//       xfer_buffer1.dataSize = BUFFER_SIZE;
//       buffer1_full = true;
//       /*进行转码*/
//     }
//     /* code */
//     ;
//   }
// }

// ///* DMA发送完成中断回调函数 */
// ///* 缓冲区内容已经播放完成，需要切换缓冲区，进行新缓冲区内容播放
// //   同时读取WAV文件数据填充到已播缓冲区  */
// //void MusicPlayer_SAI_DMA_TX_Callback(void)
// //{

// //  if(DMA_Instance->CR&(1<<19)) //当前使用Memory1数据
// //  {
// //    bufflag=0;                       //可以将数据读取到缓冲区0
// //  }
// //  else                               //当前使用Memory0数据
// //  {
// //    bufflag=1;                       //可以将数据读取到缓冲区1
// //  }
// //  Isread=1;                          // DMA传输完成标志
// //
// //}

// /***************************** (END OF FILE) *********************************/
