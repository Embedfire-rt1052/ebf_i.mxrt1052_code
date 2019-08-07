#ifndef __MP3PLAYER_H__
#define __MP3PLAYER_H__

#include <inttypes.h>


#define Delay_ms	HAL_Delay 	

//#define SAI_AUDIOFREQ_DEFAULT   SAI_AUDIO_FREQUENCY_8K

/* ?? */
enum
{
	STA_IDLE = 0,	/* ?????? */
	STA_PLAYING,	/* ?????? */
	STA_ERR,			/*  error  */
};

typedef struct
{
	uint8_t ucVolume;			/* ??????????? */
	uint8_t ucStatus;			/* ????0?????????1????????§µ?2 ???? */	
	uint32_t ucFreq;			/* ??????? */
}MP3_TYPE;	




void mp3PlayerDemo(const char *mp3file);
uint32_t mp3_GetID3V2_Size(unsigned char *buf);

//void MusicPlayer_SAI_DMA_TX_Callback(void);
#endif  /* __MP3PLAYER_H__   */

