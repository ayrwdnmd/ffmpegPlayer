#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <thread>
#include <QLabel>

#include <QMainWindow>
#include <QApplication>
#include <QDebug>
#include <QFileDialog>
#include <chrono>
#include "avpacketqueue.h"
#include "avframequeue.h"
#include "demuxthread.h"
#include "decodethread.h"
#include "audiooutput.h"
#include "videooutput.h"
#ifdef __cplusplus
extern "C"
{
#include "SDL.h"
#include "libavcodec/avcodec.h"
// #include "libavdevice/avdevice.h"
// #include "libavfilter/avfilter.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
// #include "libpostproc/postprocess.h"
// #include "libswresample/swresample.h"
// #include "libswscale/swscale.h"
}
#endif

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



    int fileChoose();
    int output();
    int stopPlaying();
    int continuePlay();
private:
    QString filePath;
    QLabel *imgLabel;
    SDL_Window *window;
    SDL_Renderer *render;
    AudioOutput *audio_output = NULL;
    VideoOutput *video_output = NULL;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
