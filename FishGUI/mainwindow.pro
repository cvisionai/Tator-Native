CONFIG += qt
SOURCES = player.cpp \
	  mainwindow.cpp \
          fish.cpp
HEADERS = player.h \
	  mainwindow.h \
          fish.h
FORMS = mainwindow.ui

TARGET = fishguibuild
win32{
target.path = D:\\Documents\\GitHub\\FishDetector\\FishGUI
INSTALLS += target
QT += widgets
LIBS += -LD:\\OpenCV\\build\\lib\\Debug
LIBS += -lopencv_core300d -lopencv_imgproc300d -lopencv_highgui300d -lopencv_videoio300d
INCLUDEPATH += D:\\OpenCV\\opencv\\build\\include
}
macx{
target.path = ~/FishDetector/FishGUI
INSTALLS += target
QT += widgets
LIBS += -L/usr/local/lib -lprotobuf
LIBS += -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_videoio
INCLUDEPATH += /usr/local/include
}

