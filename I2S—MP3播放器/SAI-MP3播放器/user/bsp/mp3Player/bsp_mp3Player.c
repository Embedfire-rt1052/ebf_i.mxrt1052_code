/*
******************************************************************************
* @file    bsp_my3player.c
* @author  fire
* @version V1.0
* @date    2018-xx-xx
* @brief   WM8960放音功能测试+mp3解码
******************************************************************************
*/

#include "bsp_mp3Player.h"
#include "fsl_debug_console.h"

#include "ff.h"
#include "mp3common.h"
#include "sai.h"
#include "fsl_wm8960.h"


/* 处理立体声音频数据时，输出缓冲区需要的最大大小为2304*16/8字节(16为PCM数据为16位)，
* 这里我们定义MP3BUFFER_SIZE为2304，实际输出缓冲区为MP3BUFFER_SIZE*2个字节
*/
#define MP3BUFFER_SIZE 3000
#define INPUTBUF_SIZE 3000

static HMP3Decoder Mp3Decoder;	/* mp3解码器指针	*/
static MP3FrameInfo Mp3FrameInfo; /* mP3帧信息  */
static MP3_TYPE mp3player;		  /* mp3播放设备 */

volatile int curretn_outbuffer = 0;	//记录正在执行DAM发送的数据缓冲区 ，0：正在发送数据缓冲区1,1：正在发送数据缓冲区2
static uint8_t bufflag = 0;			   /* 数据缓存区选择标志 */
volatile bool outbuffer1_full = false; //缓冲区满标志，
volatile bool outbuffer2_full = false;
uint8_t inputbuf[INPUTBUF_SIZE] = {0}; /* 解码输入缓冲区，1940字节为最大MP3帧大小  */
short outbuffer[2][MP3BUFFER_SIZE];	/* 解码输出缓冲区，也是I2S输入数据，实际占用字节数：RECBUFFER_SIZE*2 */

extern FIL g_fileObject;		   //文件描述结构体
extern sai_edma_handle_t txHandle; //dma 发送句柄, 执行数据发送时需要使用发送句柄。

UINT bw; /* 记录文件读取数据量 */

#define list_head_length 10
uint8_t list_head[list_head_length] = {0}; //定义标签头，主要用于计算mp3文件标签头的大小
int list_length = 0;					   //用于保存mp3 音乐文件标签长度


void delay(uint32_t count);


/**
  * @brief   MP3格式音频播放主程序
  * @param  无
  * @retval 无
  */
int times = 0;
void mp3PlayerDemo(const char *mp3file)
{
	uint8_t *read_ptr = inputbuf;
  sai_transfer_t xfer_buffer1 = {0}; //定义SAI 发送数据结构体

	int i = 0, outputSamps = 0;
	int read_offset = 0; /* 读偏移指针 */
	int bytes_left = 0;  /* 剩余字节数 */
	int result;

	/*打开文件*/
	result = f_open(&g_fileObject, mp3file, FA_READ);
	if (result != FR_OK)
	{
		PRINTF("Open mp3file :%s fail!!!->%d\r\n", mp3file, result);
		result = f_close(&g_fileObject);
		return; /* 停止播放 */
	}
	PRINTF("当前播放文件 -> %s\n", mp3file);

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
	if (Mp3Decoder == 0)
	{
		PRINTF("初始化helix解码库设备\n");
		return; /* 停止播放 */
	}
	PRINTF("初始化中...\n");

	delay(90000);


	mp3player.ucStatus = STA_PLAYING; /* 放音状态 */
	result = f_read(&g_fileObject, inputbuf, INPUTBUF_SIZE, &bw);
	if (result != FR_OK)
	{
		PRINTF("读取%s失败 -> %d\r\n", mp3file, result);
		MP3FreeDecoder(Mp3Decoder);
		return;
	}
	read_ptr = inputbuf;
	bytes_left = bw;
	/* 进入主程序循环体 */
	while (mp3player.ucStatus == STA_PLAYING)
	{
		read_offset = MP3FindSyncWord(read_ptr, bytes_left); //寻找帧同步，返回第一个同步字的位置
		if (read_offset < 0)								 //没有找到同步字
		{
			result = f_read(&g_fileObject, inputbuf, INPUTBUF_SIZE, &bw);
			if (result != FR_OK)
			{
				PRINTF("读取%s失败 -> %d\r\n", mp3file, result);
				break;
			}
			if(bw == 0)
			{
				return;
			}

			read_ptr = inputbuf;
			bytes_left = bw;
			continue; //跳出循环2，回到循环1
		}
		read_ptr += read_offset;   //偏移至同步字的位置
		bytes_left -= read_offset; //同步字之后的数据大小
		if (bytes_left < 1024)	 //补充数据
		{
			/* 注意这个地方因为采用的是DMA读取，所以一定要4字节对齐  */
			i = (uint32_t)(bytes_left)&3; //判断多余的字节
			if (i)
			{
				i = 4 - i; //需要补充的字节
			}

			memcpy(inputbuf + i, read_ptr, bytes_left);														//从对齐位置开始复制
			read_ptr = inputbuf + i;																		//指向数据对齐位置
			result = f_read(&g_fileObject, inputbuf + bytes_left + i, INPUTBUF_SIZE - bytes_left - i, &bw); //补充数据
			if (result != FR_OK)
			{
				PRINTF("读取%s失败 -> %d\r\n", mp3file, result);
				break;
			}
			bytes_left += bw; //有效数据流大小
		}

		/*选择写入缓冲区*/
		if (outbuffer1_full == false)
		{
			bufflag = 0;
		}
		else if (outbuffer2_full == false)
		{
			bufflag = 1;
		}

		result = MP3Decode(Mp3Decoder, &read_ptr, &bytes_left, outbuffer[bufflag], 0); //bufflag开始解码 参数：mp3解码结构体、输入流指针、输入流大小、输出流指针、数据格式
		if (result != ERR_MP3_NONE)													   //错误处理
		{
			switch (result)
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
				PRINTF("UNKNOWN ERROR:%d\r\n", result);
				// 跳过此帧
				if (bytes_left > 0)
				{
					bytes_left--;
					read_ptr++;
				}
				break;
			}

		}
		else
		{
			MP3GetLastFrameInfo(Mp3Decoder, &Mp3FrameInfo); //获取解码信息
			if (Mp3FrameInfo.outputSamps <= 0)
			{
				continue;
			}
			if (bufflag == 0)
			{
				outbuffer1_full = true;
			}
			else
			{
				outbuffer2_full = true;
			}
		}

		while (outbuffer1_full && outbuffer2_full)
			;

		outputSamps = Mp3FrameInfo.outputSamps; //PCM数据个数

		if (outbuffer1_full)
		{
			curretn_outbuffer = 0;
			/*执行发送*/
			xfer_buffer1.data = (uint8_t *)outbuffer[0];
			xfer_buffer1.dataSize = outputSamps * 2;

			SAI_TransferSendEDMA(DEMO_SAI, &txHandle, &xfer_buffer1);
		}
		else if (outbuffer2_full)
		{
			curretn_outbuffer = 1;
			/*执行发送*/
			xfer_buffer1.data = (uint8_t *)outbuffer[1];
			xfer_buffer1.dataSize = outputSamps * 2;
			SAI_TransferSendEDMA(DEMO_SAI, &txHandle, &xfer_buffer1);
		}
	}
}

// /***************************** (END OF FILE) *********************************/

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

