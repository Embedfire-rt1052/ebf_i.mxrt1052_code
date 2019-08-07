/*
******************************************************************************
* @file    bsp_my3player.c
* @author  fire
* @version V1.0
* @date    2018-xx-xx
* @brief   WM8960�������ܲ���+mp3����
******************************************************************************
*/

#include "bsp_mp3Player.h"
#include "fsl_debug_console.h"

#include "ff.h"
#include "mp3common.h"
#include "sai.h"
#include "fsl_wm8960.h"


/* ������������Ƶ����ʱ�������������Ҫ������СΪ2304*16/8�ֽ�(16ΪPCM����Ϊ16λ)��
* �������Ƕ���MP3BUFFER_SIZEΪ2304��ʵ�����������ΪMP3BUFFER_SIZE*2���ֽ�
*/
#define MP3BUFFER_SIZE 3000
#define INPUTBUF_SIZE 3000

static HMP3Decoder Mp3Decoder;	/* mp3������ָ��	*/
static MP3FrameInfo Mp3FrameInfo; /* mP3֡��Ϣ  */
static MP3_TYPE mp3player;		  /* mp3�����豸 */

volatile int curretn_outbuffer = 0;	//��¼����ִ��DAM���͵����ݻ����� ��0�����ڷ������ݻ�����1,1�����ڷ������ݻ�����2
static uint8_t bufflag = 0;			   /* ���ݻ�����ѡ���־ */
volatile bool outbuffer1_full = false; //����������־��
volatile bool outbuffer2_full = false;
uint8_t inputbuf[INPUTBUF_SIZE] = {0}; /* �������뻺������1940�ֽ�Ϊ���MP3֡��С  */
short outbuffer[2][MP3BUFFER_SIZE];	/* ���������������Ҳ��I2S�������ݣ�ʵ��ռ���ֽ�����RECBUFFER_SIZE*2 */

extern FIL g_fileObject;		   //�ļ������ṹ��
extern sai_edma_handle_t txHandle; //dma ���;��, ִ�����ݷ���ʱ��Ҫʹ�÷��;����

UINT bw; /* ��¼�ļ���ȡ������ */

#define list_head_length 10
uint8_t list_head[list_head_length] = {0}; //�����ǩͷ����Ҫ���ڼ���mp3�ļ���ǩͷ�Ĵ�С
int list_length = 0;					   //���ڱ���mp3 �����ļ���ǩ����


void delay(uint32_t count);


/**
  * @brief   MP3��ʽ��Ƶ����������
  * @param  ��
  * @retval ��
  */
int times = 0;
void mp3PlayerDemo(const char *mp3file)
{
	uint8_t *read_ptr = inputbuf;
  sai_transfer_t xfer_buffer1 = {0}; //����SAI �������ݽṹ��

	int i = 0, outputSamps = 0;
	int read_offset = 0; /* ��ƫ��ָ�� */
	int bytes_left = 0;  /* ʣ���ֽ��� */
	int result;

	/*���ļ�*/
	result = f_open(&g_fileObject, mp3file, FA_READ);
	if (result != FR_OK)
	{
		PRINTF("Open mp3file :%s fail!!!->%d\r\n", mp3file, result);
		result = f_close(&g_fileObject);
		return; /* ֹͣ���� */
	}
	PRINTF("��ǰ�����ļ� -> %s\n", mp3file);

	/*��ȡmp3 ��ǩͷ*/
	result = f_read(&g_fileObject, list_head, list_head_length, &bw);
	if ((result != FR_OK) || (list_head_length != bw))
	{
		PRINTF("��ȡ%sʧ�� -> %d\r\n", mp3file, result);
		return;
	}

	list_length = mp3_GetID3V2_Size(list_head); //��ȡ mp3 �ļ���ǩ���ȡ�Ϊ�����ļ�ƫ����׼��
	result = f_lseek(&g_fileObject, list_length);
	if (result != FR_OK)
	{
		PRINTF("set f_lseek failed !\r\n");
		result = f_close(&g_fileObject);
		return;
	}

	//��ʼ��MP3������
	Mp3Decoder = MP3InitDecoder();
	if (Mp3Decoder == 0)
	{
		PRINTF("��ʼ��helix������豸\n");
		return; /* ֹͣ���� */
	}
	PRINTF("��ʼ����...\n");

	delay(90000);


	mp3player.ucStatus = STA_PLAYING; /* ����״̬ */
	result = f_read(&g_fileObject, inputbuf, INPUTBUF_SIZE, &bw);
	if (result != FR_OK)
	{
		PRINTF("��ȡ%sʧ�� -> %d\r\n", mp3file, result);
		MP3FreeDecoder(Mp3Decoder);
		return;
	}
	read_ptr = inputbuf;
	bytes_left = bw;
	/* ����������ѭ���� */
	while (mp3player.ucStatus == STA_PLAYING)
	{
		read_offset = MP3FindSyncWord(read_ptr, bytes_left); //Ѱ��֡ͬ�������ص�һ��ͬ���ֵ�λ��
		if (read_offset < 0)								 //û���ҵ�ͬ����
		{
			result = f_read(&g_fileObject, inputbuf, INPUTBUF_SIZE, &bw);
			if (result != FR_OK)
			{
				PRINTF("��ȡ%sʧ�� -> %d\r\n", mp3file, result);
				break;
			}
			if(bw == 0)
			{
				return;
			}

			read_ptr = inputbuf;
			bytes_left = bw;
			continue; //����ѭ��2���ص�ѭ��1
		}
		read_ptr += read_offset;   //ƫ����ͬ���ֵ�λ��
		bytes_left -= read_offset; //ͬ����֮������ݴ�С
		if (bytes_left < 1024)	 //��������
		{
			/* ע������ط���Ϊ���õ���DMA��ȡ������һ��Ҫ4�ֽڶ���  */
			i = (uint32_t)(bytes_left)&3; //�ж϶�����ֽ�
			if (i)
			{
				i = 4 - i; //��Ҫ������ֽ�
			}

			memcpy(inputbuf + i, read_ptr, bytes_left);														//�Ӷ���λ�ÿ�ʼ����
			read_ptr = inputbuf + i;																		//ָ�����ݶ���λ��
			result = f_read(&g_fileObject, inputbuf + bytes_left + i, INPUTBUF_SIZE - bytes_left - i, &bw); //��������
			if (result != FR_OK)
			{
				PRINTF("��ȡ%sʧ�� -> %d\r\n", mp3file, result);
				break;
			}
			bytes_left += bw; //��Ч��������С
		}

		/*ѡ��д�뻺����*/
		if (outbuffer1_full == false)
		{
			bufflag = 0;
		}
		else if (outbuffer2_full == false)
		{
			bufflag = 1;
		}

		result = MP3Decode(Mp3Decoder, &read_ptr, &bytes_left, outbuffer[bufflag], 0); //bufflag��ʼ���� ������mp3����ṹ�塢������ָ�롢��������С�������ָ�롢���ݸ�ʽ
		if (result != ERR_MP3_NONE)													   //������
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
				// ������֡
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
			MP3GetLastFrameInfo(Mp3Decoder, &Mp3FrameInfo); //��ȡ������Ϣ
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

		outputSamps = Mp3FrameInfo.outputSamps; //PCM���ݸ���

		if (outbuffer1_full)
		{
			curretn_outbuffer = 0;
			/*ִ�з���*/
			xfer_buffer1.data = (uint8_t *)outbuffer[0];
			xfer_buffer1.dataSize = outputSamps * 2;

			SAI_TransferSendEDMA(DEMO_SAI, &txHandle, &xfer_buffer1);
		}
		else if (outbuffer2_full)
		{
			curretn_outbuffer = 1;
			/*ִ�з���*/
			xfer_buffer1.data = (uint8_t *)outbuffer[1];
			xfer_buffer1.dataSize = outputSamps * 2;
			SAI_TransferSendEDMA(DEMO_SAI, &txHandle, &xfer_buffer1);
		}
	}
}

// /***************************** (END OF FILE) *********************************/

/**
  * @brief  ��ȡMP3ID3V2�ļ�ͷ�Ĵ�С
  * @param  ����MP3�ļ���ͷ�����ݣ�����10���ֽ�
  * @retval ID3V2�Ĵ�С
  */
uint32_t mp3_GetID3V2_Size(unsigned char *buf)
{
	uint32_t ID3V2_size;

	if (buf[0] == 'I' && buf[1] == 'D' && buf[2] == '3') //����ID3V2
	{
		ID3V2_size = (buf[6] << 21) | (buf[7] << 14) | (buf[8] << 7) | buf[9];
	}
	else //������ID3V2
		ID3V2_size = 0;

	return ID3V2_size;
}

