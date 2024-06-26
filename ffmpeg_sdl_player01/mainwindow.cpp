#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButton,&QPushButton::clicked,this,&MainWindow::fileChoose);
    connect(ui->pushButton_pause,&QPushButton::clicked,this,&MainWindow::stopPlaying);
    connect(ui->pushButton_continue,&QPushButton::clicked,this,&MainWindow::continuePlay);
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::fileChoose()
{
    QFileDialog * fd = new QFileDialog(this);
    fd->setWindowTitle("选择数据文件");
    fd->setViewMode(QFileDialog::Detail);
    filePath = "";
    QStringList list_str;
    if(fd->exec() == QDialog::Accepted)
    {
        list_str = fd->selectedFiles();
        filePath = list_str[0];
        ui->lineEdit->setText(filePath);
        output();
    }
    return 0;
}

int MainWindow::output()
{
    AVPacketQueue video_packet_queue;
    AVPacketQueue audio_packet_queue;
    AVFrameQueue video_frame_queue;
    AVFrameQueue audio_frame_queue;

    DemuxThread *demux_thread = new DemuxThread(&video_packet_queue, &audio_packet_queue);
    const char *str = filePath.toStdString().c_str();
    demux_thread->init(str);
    demux_thread->start();

    DecodeThread *video_decode_thread = new DecodeThread(&video_packet_queue, &video_frame_queue);
    video_decode_thread->init(demux_thread->VideoCodecParameters());
    video_decode_thread->start();
    DecodeThread *audio_decode_thread = new DecodeThread(&audio_packet_queue, &audio_frame_queue);
    audio_decode_thread->init(demux_thread->AudioCodecParameters());
    audio_decode_thread->start();

    AudioParams audio_params;
    memset(&audio_params,0,sizeof(AudioParams));
    audio_params.channels = demux_thread->AudioCodecParameters()->channels;
    audio_params.channel_layout = demux_thread->AudioCodecParameters()->channel_layout;
    audio_params.fmt = (enum AVSampleFormat)demux_thread->AudioCodecParameters()->format;
    audio_params.freq = demux_thread->AudioCodecParameters()->sample_rate;
    audio_params.frame_size = demux_thread->AudioCodecParameters()->frame_size;

    audio_output = new AudioOutput(audio_params, &audio_frame_queue);
    audio_output->init();

    int video_width = demux_thread->VideoCodecParameters()->width;
    int video_height = demux_thread->VideoCodecParameters()->height;

    SDL_Init(SDL_INIT_VIDEO);
    imgLabel = new QLabel(this);
    imgLabel->resize(video_width,video_height);

    // SDL_Window *window = SDL_CreateWindowFrom((void *)imgLabel->winId());
    SDL_Window *window = SDL_CreateWindow("player", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          video_width, video_height,
                                          SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture *texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_IYUV,SDL_TEXTUREACCESS_STREAMING,video_width, video_height);

    video_output = new VideoOutput(&video_frame_queue, video_width, video_height);
    video_output->init(window,renderer,texture);
    video_output->play();
    return 0;
}

int MainWindow::stopPlaying()
{
    audio_output->audioPause();
    video_output->videoPause();
    return 0;
}

int MainWindow::continuePlay()
{
    audio_output->audioContinue();
    video_output->videoContinue();
    return 0;
}
