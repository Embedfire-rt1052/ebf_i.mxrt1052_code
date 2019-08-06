/*
******************************************************************************
* @file    recorder.c
* @author  fire
* @version V1.0
* @date    2015-xx-xx
* @brief   WM8978�������ܲ���+mp3����
******************************************************************************
* @attention
*
* ʵ��ƽ̨:����  STM32 F767 ������  
* ��̳    :http://www.chuxue123.com
* �Ա�    :http://firestm32.taobao.com
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
///* �Ƽ�ʹ�����¸�ʽmp3�ļ���
// * �����ʣ�44100Hz
// * ��  ����2
// * �����ʣ�320kbps
// */

/* ������������Ƶ����ʱ�������������Ҫ������СΪ2304*16/8�ֽ�(16ΪPCM����Ϊ16λ)��
* �������Ƕ���MP3BUFFER_SIZEΪ2304��ʵ�����������ΪMP3BUFFER_SIZE*2���ֽ�
*/
#define MP3BUFFER_SIZE  2304
#define INPUTBUF_SIZE   3000

static HMP3Decoder Mp3Decoder; /* mp3������ָ��	*/
static MP3FrameInfo		Mp3FrameInfo;		/* mP3֡��Ϣ  */
static MP3_TYPE mp3player;         /* mp3�����豸 */
//volatile uint8_t Isread=0;           /* DMA������ɱ�־ */
//static uint8_t bufflag=0;          /* ���ݻ�����ѡ���־ */

//uint32_t led_delay=0;

uint8_t inputbuf[INPUTBUF_SIZE]={0};        /* �������뻺������1940�ֽ�Ϊ���MP3֡��С  */
short outbuffer[2][MP3BUFFER_SIZE];  /* ���������������Ҳ��I2S�������ݣ�ʵ��ռ���ֽ�����RECBUFFER_SIZE*2 */

//FIL file;											/* file objects */
//FRESULT result;

/*���������ֻ����������ֽڶ���*/
extern AT_NONCACHEABLE_SECTION_ALIGN(uint8_t audioBuff[BUFFER_SIZE], 4);
extern AT_NONCACHEABLE_SECTION_ALIGN(uint8_t audioBuff2[BUFFER_SIZE], 4);
extern volatile FIL g_fileObject; //�ļ������ṹ��
/*���建������־λ*/
extern volatile bool buffer1_full;
extern volatile bool buffer2_full;

/*sai dma ���ա����;��*/
extern sai_edma_handle_t txHandle;
extern sai_edma_handle_t rxHandle;

/*����״̬*/
extern volatile bool istxFinished;

UINT bw; /* ��¼�ļ���ȡ������ */

#define list_head_length 10
uint8_t list_head[list_head_length] = {0}; //�����ǩͷ����Ҫ���ڼ���mp3�ļ���ǩͷ�Ĵ�С
int list_length = 0;                       //���ڱ���mp3 �����ļ���ǩ����
static uint8_t bufflag=0;          /* ���ݻ�����ѡ���־ */
volatile uint8_t Isread=0;           /* DMA������ɱ�־ */


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


void delay(uint32_t count);

sai_transfer_t xfer_buffer1 = {0};

/**
  * @brief   MP3��ʽ��Ƶ����������
  * @param  ��
  * @retval ��
  */
int times = 0;
void mp3PlayerDemo(const char *mp3file)
{
  uint8_t* read_ptr=inputbuf;
	uint32_t frames=0;
	int err=0, i=0, outputSamps=0;	
	int	read_offset = 0;				  /* ��ƫ��ָ�� */
	int	bytes_left = 0;					  /* ʣ���ֽ��� */	
  int result ;
	
//	mp3player.ucFreq = SAI_AUDIOFREQ_DEFAULT;
//	mp3player.ucStatus = STA_IDLE;
//	mp3player.ucVolume = 40;
	
  /*���ļ�*/
	err=f_open(&g_fileObject,mp3file,FA_READ);
	if(err!=FR_OK)
	{
		PRINTF("Open mp3file :%s fail!!!->%d\r\n",mp3file,err);
		result = f_close (&g_fileObject);
		return;	/* ֹͣ���� */
	}
	PRINTF("��ǰ�����ļ� -> %s\n",mp3file);
  
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
	if(Mp3Decoder==0)
	{
		PRINTF("��ʼ��helix������豸\n");
		return;	/* ֹͣ���� */
	}
	PRINTF("��ʼ����...\n");
	
  delay(90000);
	// Delay_ms(10);	/* �ӳ�һ��ʱ�䣬�ȴ�I2S�жϽ��� */
// 	wm8978_Reset();		/* ��λWM8978����λ״̬ */
// 	/* ����WM8978оƬ������ΪDAC�����Ϊ���� */
// 	wm8978_CfgAudioPath(DAC_ON, EAR_LEFT_ON | EAR_RIGHT_ON);

// 	/* ����������������ͬ���� */
// 	wm8978_SetOUT1Volume(mp3player.ucVolume);

// 	/* ����WM8978��Ƶ�ӿ�Ϊ�����ֱ�׼I2S�ӿڣ�16bit */
// 	wm8978_CfgAudioIF(SAI_I2S_STANDARD, 16);
	
// 	/*  ��ʼ��������I2S  */
//   SAI_Play_Stop();
// 	SAI_GPIO_Config();
// //	SAIxA_Tx_Config(SAI_I2S_STANDARD,SAI_PROTOCOL_DATASIZE_16BIT,mp3player.ucFreq);	
// //	SAIA_TX_DMA_Init();	
	
	bufflag=0;
	Isread=0;
	
	mp3player.ucStatus = STA_PLAYING;		/* ����״̬ */
  result=f_read(&g_fileObject,inputbuf,INPUTBUF_SIZE,&bw);
	if(result!=FR_OK)
	{
		PRINTF("��ȡ%sʧ�� -> %d\r\n",mp3file,result);
		MP3FreeDecoder(Mp3Decoder);
		return;
	}
	read_ptr=inputbuf;
	bytes_left=bw;
	/* ����������ѭ���� */
	while(mp3player.ucStatus == STA_PLAYING)
	{
		read_offset = MP3FindSyncWord(read_ptr, bytes_left);	//Ѱ��֡ͬ�������ص�һ��ͬ���ֵ�λ��
		if(read_offset < 0)										//û���ҵ�ͬ����
		{
			result=f_read(&g_fileObject,inputbuf,INPUTBUF_SIZE,&bw);
			if(result!=FR_OK)
			{
				PRINTF("��ȡ%sʧ�� -> %d\r\n",mp3file,result);
				break;
			}
			read_ptr=inputbuf;
			bytes_left=bw;
			continue;		//����ѭ��2���ص�ѭ��1	
		}
		read_ptr += read_offset;				//ƫ����ͬ���ֵ�λ��
		bytes_left -= read_offset;				//ͬ����֮������ݴ�С	
		if(bytes_left < 1024)							//��������
		{
			/* ע������ط���Ϊ���õ���DMA��ȡ������һ��Ҫ4�ֽڶ���  */
			i=(uint32_t)(bytes_left)&3;			//�ж϶�����ֽ�
			if(i) i=4-i;						//��Ҫ������ֽ�
			memcpy(inputbuf+i, read_ptr, bytes_left);	//�Ӷ���λ�ÿ�ʼ����
			read_ptr = inputbuf+i;										//ָ�����ݶ���λ��
			result = f_read(&g_fileObject, inputbuf+bytes_left+i, INPUTBUF_SIZE-bytes_left-i, &bw);//��������
			if(result!=FR_OK)
			{
				PRINTF("��ȡ%sʧ�� -> %d\r\n",mp3file,result);
				break;
			}
			bytes_left += bw;		//��Ч��������С
		}
    
    
		err = MP3Decode(Mp3Decoder, &read_ptr, &bytes_left, outbuffer[bufflag], 0);	//bufflag��ʼ���� ������mp3����ṹ�塢������ָ�롢��������С�������ָ�롢���ݸ�ʽ               ������������̫��4����ͨ��vo�ܲ���53����56		frames++;	
		if (err != ERR_MP3_NONE)	//������
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
					// ������֡
					if (bytes_left > 0)
					{
						bytes_left --;
						read_ptr ++;
					}	
					break;
			}
			Isread=1;
		}
		else		//�����޴���׼�������������PCM
		{
			 MP3GetLastFrameInfo(Mp3Decoder, &Mp3FrameInfo);		//��ȡ������Ϣ				
			 /* �����DAC */
			 outputSamps = Mp3FrameInfo.outputSamps;							//PCM���ݸ���
			 if (outputSamps > 0)
			 {
         xfer_buffer1.data = (uint8_t*)outbuffer[bufflag];
         xfer_buffer1.dataSize = outputSamps;
         istxFinished = false;
         SAI_TransferSendEDMA(DEMO_SAI, &txHandle, &xfer_buffer1);
         while(istxFinished == false);
         
         /*ִ�з���*/
         
//			 	if (Mp3FrameInfo.nChans == 1)	//������
//			 	{
//			 		//������������Ҫ����һ�ݵ���һ������
//			 		for (i = outputSamps - 1; i >= 0; i--)
//			 		{
//			 			outbuffer[bufflag][i * 2] = outbuffer[bufflag][i];
//			 			outbuffer[bufflag][i * 2 + 1] = outbuffer[bufflag][i];
//			 		}
//			 		outputSamps *= 2;
//			 	}//if (Mp3FrameInfo.nChans == 1)	//������
			 }//if (outputSamps > 0)
			
			// /* ���ݽ�����Ϣ���ò����� */
			// if (Mp3FrameInfo.samprate != mp3player.ucFreq)	//������ 
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
			// 	if(mp3player.ucFreq >= SAI_AUDIOFREQ_DEFAULT)	//I2S_AudioFreq_Default = 2��������֡��ÿ�ζ�Ҫ������
			// 	{
          
			// 		SAIxA_Tx_Config(SAI_I2S_STANDARD,SAI_PROTOCOL_DATASIZE_16BIT,mp3player.ucFreq);						//���ݲ������޸�iis����
      //     SAIA_TX_DMA_Init((uint32_t)(&outbuffer[0]),(uint32_t)&outbuffer[1],outputSamps);
			// 	}

			// 	SAI_Play_Start();
			
		}//else ��������
		
// 		if(file.fptr==file.fsize) 		//mp3�ļ���ȡ��ɣ��˳�
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
// 			//Input_scan();		//�ȴ�DMA������ɣ��˼�������а���ɨ�輰�����¼�
// 		}
// 		Isread=0;
	}
	// SAI_Play_Stop();
	// mp3player.ucStatus=STA_IDLE;
	// MP3FreeDecoder(Mp3Decoder);
	// f_close(&file);	
}



































// /**
//   * @brief   MP3��ʽ��Ƶ����������
//   * @param  ��
//   * @retval ��
//   */
// int times = 0;
// void mp3PlayerDemo(const char *mp3file)
// {
//   int result = -1; //���󷵻�ֵ

//   int read_offset = 0; //��ƫ��ָ��
//   int bytes_left = 0;  //ʣ���ֽ���
//   uint32_t test = 0;

//   sai_transfer_t xfer_buffer1 = {0};
//   sai_transfer_t xfer_buffer2 = {0};

//   /*���ļ���������Ƿ�ɹ�*/
//   result = f_open(&g_fileObject, mp3file, FA_READ);
//   if (result != FR_OK)
//   {
//     PRINTF("Open mp3file :%s fail!!!->%d\r\n", mp3file, result);
//     result = f_close(&g_fileObject);
//     return; /* ֹͣ���� */
//   }
//   PRINTF("��ǰ�����ļ� -> %s\n", mp3file);

//   /*��ȡmp3 ��ǩͷ*/
//   result = f_read(&g_fileObject, list_head, list_head_length, &bw);
//   if ((result != FR_OK) || (list_head_length != bw))
//   {
//     PRINTF("��ȡ%sʧ�� -> %d\r\n", mp3file, result);
//     return;
//   }
//   list_length = mp3_GetID3V2_Size(list_head); //��ȡ mp3 �ļ���ǩ���ȡ�Ϊ�����ļ�ƫ����׼��

//   /*���ö���дƫ��ֵ������ mp3 ��ǩ���������ô������⣬�ǲ���Ҫ�ر��ļ��������ó��ȣ���list_length �����Ƿ����
//   *��ǩͷ��10�ֽڣ�������ʾû�а�������ʱֱ�������ļ���дָ�롣��*/
//   result = f_lseek(&g_fileObject, list_length);
//   if (result != FR_OK)
//   {
//     PRINTF("set f_lseek failed !\r\n");
//     result = f_close(&g_fileObject);
//     return;
//   }

//   /*��ʼ��MP3������*/
//   Mp3Decoder = MP3InitDecoder();
//   if (Mp3Decoder == 0)
//   {
//     PRINTF("��ʼ��helix������豸\n");
//     return; /* ֹͣ���� */
//   }
//   PRINTF("��ʼ����...\n");

//   delay(90000);
//   /*��ȡ�ļ����ݵ�inputbuf*/
//   result = f_read(&g_fileObject, inputbuf, INPUTBUF_SIZE, &bw);
//   if (result != FR_OK)
//   {
//     PRINTF("��ȡ%sʧ�� -> %d\r\n", mp3file, result);
//     MP3FreeDecoder(Mp3Decoder);
//     return;
//   }
//   else
//   {
//     /* code */
//     PRINTF("��ȡ%s�ɹ� -> %d\r\n", mp3file, result);
//   }

//   /*��ȡ����֡ƫ��*/
//   read_offset = MP3FindSyncWord(inputbuf, bw); //Ѱ��֡ͬ�������ص�һ��ͬ���ֵ�λ��
//   if (read_offset < 0)                         //û���ҵ�ͬ����
//   {
//     result = f_read(&g_fileObject, inputbuf, INPUTBUF_SIZE, &bw);
//     if (result != FR_OK)
//     {
//       PRINTF("��ȡ%sʧ�� -> %d\r\n", mp3file, result);
//     }
//   }

//   // bytes_left = bw - read_offset;

//   //bufflag��ʼ���� ������mp3����ṹ�塢������ָ�롢��������С�������ָ�롢���ݸ�ʽ
//   result = MP3Decode(Mp3Decoder, (char **)&inputbuf, &bytes_left, (short *)audioBuff, 0);
//   if (result != ERR_MP3_NONE) //������
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

//   /*��ѭ��*/
//   while (1)
//   {
//     /*buffer1 ������*/
//     if (buffer1_full)
//     {
//       /*ִ�з���buffer1*/
//       SAI_TransferSendEDMA(DEMO_SAI, &txHandle, &xfer_buffer1);
//       while (istxFinished == false)
//       {
//         ;
//       }
//     }

//     if (buffer1_full == false)
//     {
//       /*������*/
//       /*��ȡ�ļ����ݵ�inputbuf*/
//       result = f_read(&g_fileObject, inputbuf, INPUTBUF_SIZE, &bw);
//       if (result != FR_OK)
//       {
//         PRINTF("��ȡ%sʧ�� -> %d\r\n", mp3file, result);
//         MP3FreeDecoder(Mp3Decoder);
//         return;
//       }

//       //bufflag��ʼ���� ������mp3����ṹ�塢������ָ�롢��������С�������ָ�롢���ݸ�ʽ
//       result = MP3Decode(Mp3Decoder, (char **)&inputbuf, &bytes_left, (short *)audioBuff, 0);
//       if (result != ERR_MP3_NONE) //������
//       {
//         PRINTF("error \r\n");
//       }
//       xfer_buffer1.data = audioBuff;
//       xfer_buffer1.dataSize = BUFFER_SIZE;
//       buffer1_full = true;
//       /*����ת��*/
//     }
//     /* code */
//     ;
//   }
// }

// ///* DMA��������жϻص����� */
// ///* �����������Ѿ�������ɣ���Ҫ�л��������������»��������ݲ���
// //   ͬʱ��ȡWAV�ļ�������䵽�Ѳ�������  */
// //void MusicPlayer_SAI_DMA_TX_Callback(void)
// //{

// //  if(DMA_Instance->CR&(1<<19)) //��ǰʹ��Memory1����
// //  {
// //    bufflag=0;                       //���Խ����ݶ�ȡ��������0
// //  }
// //  else                               //��ǰʹ��Memory0����
// //  {
// //    bufflag=1;                       //���Խ����ݶ�ȡ��������1
// //  }
// //  Isread=1;                          // DMA������ɱ�־
// //
// //}

// /***************************** (END OF FILE) *********************************/
