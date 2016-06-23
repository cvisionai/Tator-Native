#-------------------------------------------------
#
# Project created by QtCreator 2016-06-14T19:36:23
#
#-------------------------------------------------
CONFIG += qt
CONFIG += c++11

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Video_Slicer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

LIBS += -LD:\\OpenCV\\build\\lib\\Debug
LIBS += -lopencv_core300d -lopencv_imgproc300d -lopencv_highgui300d -lopencv_videoio300d

INCLUDEPATH += D:\\OpenCV\\opencv\\build\\include
INCLUDEPATH += D:\\boost_1_60_0
