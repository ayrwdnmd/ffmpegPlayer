#ifndef VIDEOOUTPUT_H
#define VIDEOOUTPUT_H

#include "avframequeue.h"
#include <QLabel>
#ifdef __cplusplus
extern "C"
{
#include "SDL.h"
}
#endif
class VideoOutput
{
public:
    VideoOutput(AVFrameQueue *frame_queue, int width, int height);
    ~VideoOutput();
    int init(SDL_Window *window,SDL_Renderer *renderer,SDL_Texture *texture);
    int play();
    int videoPause();
    int videoContinue();
    int videoRefresh();
private:
    AVFrameQueue *m_frame_queue;
    int m_width;
    int m_height;
    bool play_handle = true;
    SDL_Window *m_window = NULL;
    SDL_Renderer *m_renderer = NULL;
    SDL_Texture *m_texture = NULL;
};

#endif // VIDEOOUTPUT_H
