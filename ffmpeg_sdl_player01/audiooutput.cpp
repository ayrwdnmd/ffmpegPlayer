#include "audiooutput.h"

AudioOutput::AudioOutput(const AudioParams &audio_params, AVFrameQueue *frame_queue)
    :m_source_params(audio_params), m_frame_queue(frame_queue)
{

}

AudioOutput::~AudioOutput()
{

}

void fill_audio_pcm(void *udata, Uint8 *stream, int len) {
    AudioOutput *is = (AudioOutput *)udata;
    int len1 = 0;
    int audio_size = 0;

    while (len > 0) {
        if(is->audio_buf_index == is->audio_buf_size) {
            is->audio_buf_index = 0;
            AVFrame * frame = is->m_frame_queue->pop(10);   //第一次pop会失败，改为多线程后可能可以解决
            if(frame) {
                if(((frame->format != is->m_destination_params.fmt)
                     || (frame->sample_rate != is->m_destination_params.freq)
                     || (frame->channel_layout != is->m_destination_params.channel_layout))
                    && (!is->swr_ctx_)) {
                    is->swr_ctx_ = swr_alloc_set_opts(NULL, //申请重采样实例的内存
                                                      is->m_destination_params.channel_layout,
                                                      (enum AVSampleFormat)is->m_destination_params.fmt,
                                                      is->m_destination_params.freq,
                                                      frame->channel_layout,
                                                      (enum AVSampleFormat)frame->format,
                                                      frame->sample_rate,
                                                      0,NULL);
                    if (!is->swr_ctx_ || swr_init(is->swr_ctx_) < 0) {  //初始化重采样实例
                        qDebug("channel_layout:%d,fmt:%d,freq:%d,channel_layout:%d,format:%d,sample_rate:%d",
                               is->m_destination_params.channel_layout,
                               (enum AVSampleFormat)is->m_destination_params.fmt,
                               is->m_destination_params.freq,
                               frame->channel_layout,
                               (enum AVSampleFormat)frame->format,
                               frame->sample_rate);
                        swr_free((SwrContext **)(&is->swr_ctx_));
                        return;
                    }
                }
                if(is->swr_ctx_) { // 重采样
                    const uint8_t **in = (const uint8_t **)frame->extended_data;    //指向音频数据的指针
                    uint8_t **out = &is->audio_buf1_;
                    int out_samples = frame->nb_samples * is->m_destination_params.freq/frame->sample_rate + 256;   //采样数*采样率的比值
                    int out_bytes = av_samples_get_buffer_size(NULL,is->m_destination_params.channels,out_samples,is->m_destination_params.fmt,0);  //根据音频参数计算缓冲区大小
                    av_fast_malloc(&is->audio_buf1_,&is->audio_buf1_size,out_bytes);    //分配缓冲区buf1

                    int len2 = swr_convert(is->swr_ctx_,out,out_samples,in,frame->nb_samples);  //开始转换,返回样本数
                    is->audio_buf_ = is->audio_buf1_;
                    is->audio_buf_size = av_samples_get_buffer_size(NULL,is->m_destination_params.channels,len2,is->m_destination_params.fmt,1);

                } else { // 没有重采样
                    audio_size = av_samples_get_buffer_size(NULL,frame->channels,frame->nb_samples,(enum AVSampleFormat)frame->format,1);
                    av_fast_malloc(&is->audio_buf1_,&is->audio_buf1_size,audio_size);
                    is->audio_buf_ = is->audio_buf1_;
                    is->audio_buf_size = audio_size;
                    memcpy(is->audio_buf_,frame->data[0],audio_size);
                }
                av_frame_free(&frame);
            }else {
                //没有读到解码后的数据
                is->audio_buf_ = NULL;
                is->audio_buf_size = 512;
            }
        }
        len1 = is->audio_buf_size - is->audio_buf_index;
        if(len1 > len)
            len1 = len;
        if(!is->audio_buf_) {
            memset(stream,0,len1);
        } else {
            //真正拷贝有效数据
            memcpy(stream,is->audio_buf_ + is->audio_buf_index,len1);
        }
        len -= len1;
        stream += len1;
        is->audio_buf_index += len1;
    }
}

int AudioOutput::init()
{
    SDL_Init(SDL_INIT_AUDIO);
    SDL_AudioSpec wanted_spec;
    wanted_spec.channels = m_source_params.channels;//2
    wanted_spec.freq = m_source_params.freq;
    wanted_spec.format = AUDIO_S16SYS;
    wanted_spec.silence = 0;
    wanted_spec.callback = fill_audio_pcm;
    wanted_spec.userdata = this;
    wanted_spec.samples = m_source_params.frame_size;//1024
    SDL_OpenAudio(&wanted_spec,NULL);

    m_destination_params.channels = wanted_spec.channels;
    m_destination_params.fmt = AV_SAMPLE_FMT_S16;
    m_destination_params.freq = wanted_spec.freq;
    m_destination_params.channel_layout = av_get_default_channel_layout(2);//wanted_spec.channels
    m_destination_params.frame_size = wanted_spec.samples;//1024;//wanted_spec.frame_size;
    SDL_PauseAudio(0);
    return 0;
}



int AudioOutput::audioPause()
{
    SDL_PauseAudio(1);
}

int AudioOutput::audioContinue()
{
    SDL_PauseAudio(0);
}
