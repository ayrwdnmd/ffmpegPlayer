#include "demuxthread.h"

DemuxThread::DemuxThread(AVPacketQueue *video_queue,AVPacketQueue *audio_queue)
    :m_video_queue(video_queue),m_audio_queue(audio_queue)
{

}

DemuxThread::~DemuxThread()
{
    stop();
}

int DemuxThread::init(const char *url)
{
    m_format_context = avformat_alloc_context();                //分配解封装器上下文
    avformat_open_input(&m_format_context, url, NULL, NULL);    //打开音视频文件
    avformat_find_stream_info(m_format_context,NULL);           //获取视频文件基本信息
    av_dump_format(m_format_context,0,url,0);                   //打印输出数据详情
    m_audio_index = av_find_best_stream(m_format_context, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    m_video_index = av_find_best_stream(m_format_context, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    return 0;
}

void DemuxThread::run()
{
    AVPacket packet;
    while(true){
        av_read_frame(m_format_context, &packet);
        if(packet.stream_index == m_video_index){
            m_video_queue->push(&packet);
            // qDebug("video");
        }else if(packet.stream_index == m_audio_index){
            m_audio_queue->push(&packet);
            // qDebug("audio");
        }else{
            av_packet_unref(&packet);
            qDebug("av_read_frame packet failed");
        }
    }
    // av_packet_free(&packet);
}

int DemuxThread::stop()
{
    avformat_close_input(&m_format_context);   //关闭解复用器
    return 0;
}

AVCodecParameters *DemuxThread::AudioCodecParameters()
{
    if(m_audio_index != -1) {
        return m_format_context->streams[m_audio_index]->codecpar;
    } else {
        return NULL;
    }
}

AVCodecParameters *DemuxThread::VideoCodecParameters()
{
    if(m_video_index != -1) {
        return m_format_context->streams[m_video_index]->codecpar;
    } else {
        return NULL;
    }
}

AVRational DemuxThread::AudioStreamTimebase()
{
    if(m_audio_index != -1) {
        return m_format_context->streams[m_audio_index]->time_base;
    } else {
        return AVRational{0,0};
    }
}

AVRational DemuxThread::VideoStreamTimebase()
{
    if(m_video_index != -1) {
        return m_format_context->streams[m_video_index]->time_base;
    } else {
        return AVRational{0,0};
    }
}
