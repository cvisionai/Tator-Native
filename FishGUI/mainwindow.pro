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
LIBS += -L/usr/local/Cellar -lprotobuf -pthread -lpthread 
LIBS += -L/usr/local/opt/opencv3/lib -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_videoio 
INCLUDEPATH += /usr/local/include
INCLUDEPATH += /usr/local/opt/opencv3/include
