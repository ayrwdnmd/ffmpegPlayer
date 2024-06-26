#ifndef AUDIOOUTPUT_H
#define AUDIOOUTPUT_H

#ifdef __cplusplus
extern "C"
{
#include "SDL.h"
#include "libswresample/swresample.h"
}
#endif

#include "avframequeue.h"
typedef struct AudioParams
{
    int freq;   //采样率
    int channels;//通道数
    int64_t channel_layout;
    // int64_t channel_layout;
    enum AVSampleFormat fmt;
    int frame_size;
}AudioParams;

class AudioOutput
{
public:
    AudioOutput(const AudioParams &audio_params, AVFrameQueue *frame_queue);
    ~AudioOutput();
    int init();
    int audioPause();
    int audioContinue();
    int DeInit();
// private:
	int64_t pts_ = AV_NOPTS_VALUE;
    
    AudioParams m_source_params;        //解码后产生的参数
    AudioParams m_destination_params;   //SDL实际输出的参数
    AVFrameQueue *m_frame_queue = NULL;

    struct SwrContext *swr_ctx_ = NULL; //重采样上下文

    
    uint8_t *audio_buf_ = NULL;
    uint8_t *audio_buf1_ = NULL;
    uint32_t audio_buf_size = 0;
    uint32_t audio_buf1_size = 0;
    uint32_t audio_buf_index = 0;
};

#endif // AUDIOOUTPUT_H
