#ifndef AVFRAMEQUEUE_H
#define AVFRAMEQUEUE_H
#include "queue.h"

#ifdef __cplusplus
extern "C"{
#include "libavcodec/avcodec.h"
}
#endif
class AVFrameQueue
{
public:
    AVFrameQueue(){}
    ~AVFrameQueue(){}
    int push(AVFrame *frame);
    AVFrame *pop(const int timeout);
    AVFrame *front();
    int size();
private:
    Queue<AVFrame *> m_queue;

};

#endif // AVFRAMEQUEUE_H
