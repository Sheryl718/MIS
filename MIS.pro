#-------------------------------------------------
#
# Project created by QtCreator 2018-03-25T17:20:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MIS
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mis.cpp \
    fusion.cpp \
    sgmentation.cpp \
    filter.cpp \
    registration.cpp

HEADERS  += mis.h \
    fusion.h \
    sgmentation.h \
    filter.h \
    registration.h

FORMS    += mis.ui

INCLUDEPATH += D:\A_Graduation\opencv\BIN\install\include\opencv
INCLUDEPATH += D:\A_Graduation\opencv\BIN\install\include\opencv2
INCLUDEPATH += D:\A_Graduation\opencv\BIN\install\include

LIBS += D:\A_Graduation\opencv\BIN\install\x64\vc12\lib\opencv_xfeatures2d310d.lib
LIBS += D:\A_Graduation\opencv\BIN\install\x64\vc12\lib\opencv_aruco310d.lib
LIBS += D:\A_Graduation\opencv\BIN\install\x64\vc12\lib\opencv_bgsegm310d.lib
LIBS += D:\A_Graduation\opencv\BIN\install\x64\vc12\lib\opencv_calib3d310d.lib
LIBS += D:\A_Graduation\opencv\BIN\install\x64\vc12\lib\opencv_ccalib310d.lib
LIBS += D:\A_Graduation\opencv\BIN\install\x64\vc12\lib\opencv_core310d.lib
LIBS += D:\A_Graduation\opencv\BIN\install\x64\vc12\lib\opencv_datasets310d.lib
LIBS += D:\A_Graduation\opencv\BIN\install\x64\vc12\lib\opencv_dnn310d.lib
LIBS += D:\A_Graduation\opencv\BIN\install\x64\vc12\lib\opencv_dpm310d.lib
LIBS += D:\A_Graduation\opencv\BIN\install\x64\vc12\lib\opencv_face310d.lib
LIBS += D:\A_Graduation\opencv\BIN\install\x64\vc12\lib\opencv_features2d310d.lib
LIBS += D:\A_Graduation\opencv\BIN\install\x64\vc12\lib\opencv_flann310d.lib
LIBS += D:\A_Graduation\opencv\BIN\install\x64\vc12\lib\opencv_fuzzy310d.lib
LIBS += D:\A_Graduation\opencv\BIN\install\x64\vc12\lib\opencv_highgui310d.lib
LIBS += D:\A_Graduation\opencv\BIN\install\x64\vc12\lib\opencv_imgcodecs310d.lib
LIBS += D:\A_Graduation\opencv\BIN\install\x64\vc12\lib\opencv_imgproc310d.lib
LIBS += D:\A_Graduation\opencv\BIN\install\x64\vc12\lib\opencv_line_descriptor310d.lib
LIBS += D:\A_Graduation\opencv\BIN\install\x64\vc12\lib\opencv_ml310d.lib
LIBS += D:\A_Graduation\opencv\BIN\install\x64\vc12\lib\opencv_objdetect310d.lib
LIBS += D:\A_Graduation\opencv\BIN\install\x64\vc12\lib\opencv_optflow310d.lib
LIBS += D:\A_Graduation\opencv\BIN\install\x64\vc12\lib\opencv_photo310d.lib
LIBS += D:\A_Graduation\opencv\BIN\install\x64\vc12\lib\opencv_plot310d.lib
LIBS += D:\A_Graduation\opencv\BIN\install\x64\vc12\lib\opencv_reg310d.lib
LIBS += D:\A_Graduation\opencv\BIN\install\x64\vc12\lib\opencv_rgbd310d.lib
LIBS += D:\A_Graduation\opencv\BIN\install\x64\vc12\lib\opencv_saliency310d.lib
LIBS += D:\A_Graduation\opencv\BIN\install\x64\vc12\lib\opencv_shape310d.lib
LIBS += D:\A_Graduation\opencv\BIN\install\x64\vc12\lib\opencv_stereo310d.lib
LIBS += D:\A_Graduation\opencv\BIN\install\x64\vc12\lib\opencv_stitching310d.lib
LIBS += D:\A_Graduation\opencv\BIN\install\x64\vc12\lib\opencv_structured_light310d.lib
LIBS += D:\A_Graduation\opencv\BIN\install\x64\vc12\lib\opencv_superres310d.lib
LIBS += D:\A_Graduation\opencv\BIN\install\x64\vc12\lib\opencv_surface_matching310d.lib
LIBS += D:\A_Graduation\opencv\BIN\install\x64\vc12\lib\opencv_text310d.lib
LIBS += D:\A_Graduation\opencv\BIN\install\x64\vc12\lib\opencv_tracking310d.lib
LIBS += D:\A_Graduation\opencv\BIN\install\x64\vc12\lib\opencv_ts310d.lib
LIBS += D:\A_Graduation\opencv\BIN\install\x64\vc12\lib\opencv_video310d.lib
LIBS += D:\A_Graduation\opencv\BIN\install\x64\vc12\lib\opencv_videoio310d.lib
LIBS += D:\A_Graduation\opencv\BIN\install\x64\vc12\lib\opencv_videostab310d.lib
LIBS += D:\A_Graduation\opencv\BIN\install\x64\vc12\lib\opencv_ximgproc310d.lib
LIBS += D:\A_Graduation\opencv\BIN\install\x64\vc12\lib\opencv_xobjdetect310d.lib
LIBS += D:\A_Graduation\opencv\BIN\install\x64\vc12\lib\opencv_xphoto310d.lib

CONFIG   += c++11
