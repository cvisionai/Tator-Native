#include <QtPlugin>
#include <QApplication>
#include <QFontDatabase>

#include "fish_annotator/video_slicer/mainwindow.h"

#ifdef _WIN32
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
Q_IMPORT_PLUGIN(QICOPlugin)
#endif

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
#if __unix__
  QFontDatabase::addApplicationFont(":/fonts/DejaVuSansCondensed.ttf");
#endif
  fish_annotator::video_slicer::MainWindow w;
  w.show();
  return a.exec();
}
