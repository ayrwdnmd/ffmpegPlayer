QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    audiooutput.cpp \
    avframequeue.cpp \
    avpacketqueue.cpp \
    decodethread.cpp \
    demuxthread.cpp \
    main.cpp \
    mainwindow.cpp \
    videooutput.cpp

HEADERS += \
    audiooutput.h \
    avframequeue.h \
    avpacketqueue.h \
    decodethread.h \
    demuxthread.h \
    mainwindow.h \
    queue.h \
    videooutput.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += $$PWD/ffmpeg-4.2.2-win64-dev/include \
            $$PWD/SDL2-2.30.3/include

LIBS += -L$$PWD/"ffmpeg-4.2.2-win64-dev/lib" -lavcodec \
        -L$$PWD/"ffmpeg-4.2.2-win64-dev/lib" -lavdevice \
        -L$$PWD/"ffmpeg-4.2.2-win64-dev/lib" -lavfilter \
        -L$$PWD/"ffmpeg-4.2.2-win64-dev/lib" -lavformat \
        -L$$PWD/"ffmpeg-4.2.2-win64-dev/lib" -lavutil \
        -L$$PWD/"ffmpeg-4.2.2-win64-dev/lib" -lpostproc \
        -L$$PWD/"ffmpeg-4.2.2-win64-dev/lib" -lswresample \
        -L$$PWD/"ffmpeg-4.2.2-win64-dev/lib" -lswscale \

LIBS += -L$$PWD/"SDL2-2.30.3/lib/x64" -lSDL2
