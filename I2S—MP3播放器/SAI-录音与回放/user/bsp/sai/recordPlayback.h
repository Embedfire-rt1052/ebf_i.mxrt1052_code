#ifndef __RECORDPLAYBACK_H
#define __RECORDPLAYBACK_H

#include "fsl_codec_common.h"
#include "fsl_wm8960.h"


/*定义缓冲区大小，总大小为SIZE * BUFFER_NUM */
#define SIZE (512)
#define BUFFER_NUM (4)
#define SAMPLE_RATE (kSAI_SampleRate16KHz)




void RecordPlayback(I2S_Type *base, uint32_t time_s);
void RecordSDCard(I2S_Type *base, uint32_t time_s, const char* FR_NO_PATH);
void wav_header(uint8_t *header, uint32_t sampleRate, uint32_t bitsPerFrame, uint8_t channels, uint32_t fileSize);
#endif /* __RECORDPLAYBACK_H */

