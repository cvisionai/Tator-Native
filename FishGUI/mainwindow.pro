CONFIG += qt
SOURCES = player.cpp \
	  mainwindow.cpp \
	  fish.cpp \
	  fishSerialize.pb.cc
HEADERS = player.h \
	  mainwindow.h \
	  fish.h \
	  fishSerialize.pb.h
FORMS = mainwindow.ui

TARGET = fishguibuild
target.path = ~/FishDetector/FishGUI
INSTALLS += target
QT += widgets
LIBS += -lprotobuf 
LIBS += -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_videoio 
INCLUDEPATH += /usr/local/include
