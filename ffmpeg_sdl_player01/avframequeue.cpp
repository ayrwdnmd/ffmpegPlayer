#include "avframequeue.h"

int AVFrameQueue::push(AVFrame *frame)
{
    AVFrame *temp_frame = av_frame_alloc();
    av_frame_move_ref(temp_frame, frame);
    return m_queue.push(temp_frame);
}

AVFrame *AVFrameQueue::pop(const int timeout)
{
    return m_queue.pop(timeout);
}

AVFrame *AVFrameQueue::front()
{
    return m_queue.front();
}

int AVFrameQueue::size()
{
    return m_queue.size();
}
