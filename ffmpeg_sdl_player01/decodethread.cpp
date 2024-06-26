#include "decodethread.h"

DecodeThread::DecodeThread(AVPacketQueue *packet_queue, AVFrameQueue *frame_queue)
    : m_packet_queue(packet_queue), m_frame_queue(frame_queue)
{

}

DecodeThread::~DecodeThread()
{

}

int DecodeThread::init(AVCodecParameters *codec_parameter)
{
    m_codec_context = avcodec_alloc_context3(NULL);                     //分配编解码器上下文(创建对象）
    avcodec_parameters_to_context(m_codec_context, codec_parameter);    //根据编解码器参数分配上下文
    AVCodec *codec = avcodec_find_decoder(codec_parameter->codec_id);   //根据id查找编解码器
    avcodec_open2(m_codec_context, codec, NULL);
    return 0;
}

void DecodeThread::run()
{
    AVPacket *packet = av_packet_alloc();
    AVFrame *frame = av_frame_alloc();
    while(m_packet_queue->front()){
        packet = m_packet_queue->pop(1);
        avcodec_send_packet(m_codec_context, packet);
        avcodec_receive_frame(m_codec_context, frame);
        m_frame_queue->push(frame);
    }
    av_packet_free(&packet);
    av_frame_free(&frame);
}

int DecodeThread::stop()
{

}
