#include <QtPlugin>
#include <QApplication>

#include "fish_detector/video_slicer/mainwindow.h"

Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    fish_detector::video_slicer::MainWindow w;
    w.show();

    return a.exec();
}
