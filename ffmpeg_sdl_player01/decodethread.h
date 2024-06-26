#ifndef DECODETHREAD_H
#define DECODETHREAD_H

#include "avpacketqueue.h"
#include "avframequeue.h"
#include <QThread>
#ifdef __cplusplus
extern "C"
{
#include "libavutil/avutil.h"
#include "libavformat/avformat.h"
}
#endif
class DecodeThread : public QThread
{
public:
    DecodeThread(AVPacketQueue *packet_queue, AVFrameQueue *frame_queue);
    ~DecodeThread();
    int init(AVCodecParameters *codec_parameter);
    void run();
    int stop();
private:
    AVCodecContext *m_codec_context = NULL;
    AVPacketQueue *m_packet_queue = NULL;
    AVFrameQueue *m_frame_queue = NULL;
};

#endif // DECODETHREAD_H
