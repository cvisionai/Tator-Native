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