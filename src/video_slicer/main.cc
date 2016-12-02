#include <QtPlugin>
#include <QApplication>

#include "fish_annotator/video_slicer/mainwindow.h"

#ifdef _WIN32
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
#elif __APPLE__
Q_IMPORT_PLUGIN(QCocoaIntegrationPlugin)
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    fish_annotator::video_slicer::MainWindow w;
    w.show();

    return a.exec();
}
