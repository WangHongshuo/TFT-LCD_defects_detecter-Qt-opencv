#-------------------------------------------------
#
# Project created by QtCreator 2017-11-16T15:26:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = defect_detect_opencv
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    detecter.cpp \
    ImageWidget-Qt/ImageWidget.cpp \
    ImageWidget-Qt/selectrect.cpp \
    QImageMatConvert/mat_qimage_convert.cpp

HEADERS += \
        mainwindow.h \
    detecter.h \
    ImageWidget-Qt/ImageWidget.h \
    ImageWidget-Qt/selectrect.h \
    QImageMatConvert/mat_qimage_convert.h

FORMS += \
        mainwindow.ui

## MinGW32

#INCLUDEPATH += D:/opencv4.1.0/opencv_qt730/include

#LIBS += D:/opencv4.1.0/opencv_qt730/lib/libopencv_*.a

 MSVC

INCLUDEPATH += D:/opencv4.1.0/opencv/build/include

CONFIG(debug, debug|release) {
LIBS += D:/opencv4.1.0/opencv/build/x86/vc16/lib/*410d.lib
} else {
LIBS += D:/opencv4.1.0/opencv/build/x86/vc16/lib/*410.lib
}

