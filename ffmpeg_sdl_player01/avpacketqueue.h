#ifndef AVPACKETQUEUE_H
#define AVPACKETQUEUE_H
#include "queue.h"

#ifdef __cplusplus
extern "C"{
#include "libavcodec/avcodec.h"
}
#endif
class AVPacketQueue
{
public:
    AVPacketQueue(){}
    ~AVPacketQueue(){}
    int push(AVPacket *packet);
    AVPacket *pop(const int timeout);
    AVPacket *front();
    int size();
private:
    Queue<AVPacket*> m_queue;
};

#endif // AVPACKETQUEUE_H
