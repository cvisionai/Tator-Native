CONFIG += qt
SOURCES = player.cpp \
	  mainwindow.cpp \
	  fish.cpp \
HEADERS = player.h \
	  mainwindow.h \
	  fish.h \
FORMS = mainwindow.ui

TARGET = fishguibuild
target.path = ~/FishDetector/FishGUI
INSTALLS += target
QT += widgets
LIBS += -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_videoio 
INCLUDEPATH += /usr/local/include
