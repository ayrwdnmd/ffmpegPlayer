#include "avpacketqueue.h"

int AVPacketQueue::push(AVPacket *packet)
{
    AVPacket *temp_packet = av_packet_alloc();
    av_packet_move_ref(temp_packet,packet);
    return m_queue.push(temp_packet);;
}

AVPacket *AVPacketQueue::pop(const int timeout)
{
    return m_queue.pop(timeout);
}

AVPacket *AVPacketQueue::front()
{
    return m_queue.front();
}

int AVPacketQueue::size()
{
    return m_queue.size();
}
