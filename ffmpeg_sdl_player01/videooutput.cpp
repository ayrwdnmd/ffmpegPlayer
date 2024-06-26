#include "videooutput.h"

VideoOutput::VideoOutput(AVFrameQueue *frame_queue, int width, int height)
    : m_frame_queue(frame_queue), m_width(width), m_height(height)
{

}

VideoOutput::~VideoOutput()
{

}

int VideoOutput::init(SDL_Window *window,SDL_Renderer *renderer,SDL_Texture *texture)
{
    // SDL_Init(SDL_INIT_VIDEO);
    m_window = window;
    m_renderer = renderer;
    m_texture = texture;
    // m_window = SDL_CreateWindow("player", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
    //                  m_width, m_height,
    //                  SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    // m_renderer = SDL_CreateRenderer(m_window, -1, 0);
    // m_texture = SDL_CreateTexture(m_renderer,SDL_PIXELFORMAT_IYUV,SDL_TEXTUREACCESS_STREAMING,m_width, m_height);
    return 0;
}

int VideoOutput::play()
{
    SDL_Event event;
    while(true)
    {
        SDL_PumpEvents();
        SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT);
        _sleep(50);
        if(play_handle)
            videoRefresh();
        switch(event.type){
        case SDL_KEYDOWN:
            if(event.key.keysym.sym == SDLK_ESCAPE)
                return 1;
        case SDL_QUIT:
            return 1;
        }
    }
    return 0;
}

int VideoOutput::videoPause()
{
    play_handle = false;
    return 0;
}

int VideoOutput::videoContinue()
{
    play_handle = true;
    return 0;
}

int VideoOutput::videoRefresh()
{
    SDL_Rect rect_;
    rect_.x = 0;
    rect_.y = 0;
    rect_.w = m_width;
    rect_.h = m_height;
    AVFrame *frame = m_frame_queue->pop(10);
    SDL_UpdateYUVTexture(m_texture,&rect_,frame->data[0],frame->linesize[0],
                         frame->data[1],frame->linesize[1],
                         frame->data[2],frame->linesize[2]);
    SDL_RenderClear(m_renderer);
    SDL_RenderCopy(m_renderer,m_texture,NULL,&rect_);
    SDL_RenderPresent(m_renderer);

    return 0;
}
