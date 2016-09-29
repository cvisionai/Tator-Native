CONFIG += qt
CONFIG += c++11


SOURCES = player.cpp \
	  mainwindow.cpp \
          fish.cpp \
          annotatedregion.cpp \
          navigatorwidget.cpp \
          document.cpp \
          mainwindowmenus.cpp \
          mainwindowbuttons.cpp
HEADERS = player.h \
	  mainwindow.h \
          fish.h \
          annotatedregion.h \
          navigatorwidget.h \
          document.h
FORMS = mainwindow.ui \
        navigatorwidget.ui

TARGET = fishguibuild

win32{
    target.path = D:\\Documents\\GitHub\\FishDetector\\FishGUI
    INSTALLS += target
    QT += widgets testlib
    CONFIG(release, debug|release) {
        LIBS += -LD:\\OpenCV\\build\\lib\\Release
        LIBS += -lopencv_core300 -lopencv_imgproc300 -lopencv_highgui300 -lopencv_videoio300
    }

    CONFIG(debug, debug|release) {
        LIBS += -LD:\\OpenCV\\build\\lib\\Debug
        LIBS += -lopencv_core300d -lopencv_imgproc300d -lopencv_highgui300d -lopencv_videoio300d
    }

    INCLUDEPATH += D:\\OpenCV\\opencv\\build\\include
    INCLUDEPATH += D:\\boost_1_60_0
}

macx{
    target.path = ~/FishDetector/FishGUI
    INSTALLS += target
    QT += widgets
    LIBS += -L/Users/bwoodward/Projects/opencv-3.1.0/build/lib/ #This is your opencv install location
    #LIBS += -L/usr/local/lib/
    LIBS += -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_videoio
    INCLUDEPATH += /usr/local/Cellar/qt5/5.5.1_2/include/
    INCLUDEPATH += /Users/bwoodward/Projects/opencv-3.1.0/build/include/
    INCLUDEPATH += /usr/local/include/
}

