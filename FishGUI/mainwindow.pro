CONFIG += qt
SOURCES = player.cpp \
		  MainWindow.cpp
HEADERS = player.h \
		  MainWindow.h
FORMS = mainwindow.ui

TARGET = fishguibuild
target.path = ~/FishDetector/FishGUI
INSTALLS += target
QT += widgets
LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_videoio -Wl,-rpath=/usr/local/lib