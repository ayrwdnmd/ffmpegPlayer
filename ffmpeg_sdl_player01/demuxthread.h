#ifndef DEMUXTHREAD_H
#define DEMUXTHREAD_H

#include "avpacketqueue.h"
#include <QThread>
#ifdef __cplusplus
extern "C"
{
#include "libavutil/avutil.h"
#include "libavformat/avformat.h"
}
#endif

class DemuxThread : public QThread
{
public:
    DemuxThread(AVPacketQueue *video_queue,AVPacketQueue *audio_queue);
    ~DemuxThread();
    int init(const char *);
    void run();
    int stop();

    AVCodecParameters *VideoCodecParameters();
    AVCodecParameters *AudioCodecParameters();
    AVRational VideoStreamTimebase();
    AVRational AudioStreamTimebase();


private:
    AVPacketQueue *m_video_queue = NULL;
    AVPacketQueue *m_audio_queue = NULL;
    std::string m_url;  //文件路径
    AVFormatContext *m_format_context = NULL;   //解封装器上下文
    int m_video_index = -1;
    int m_audio_index = -1;
};

#endif // DEMUXTHREAD_H
