#include <QtPlugin>
#include <QApplication>
#include <QFontDatabase>

#include "mainwindow.h"

#ifdef _WIN32
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
Q_IMPORT_PLUGIN(QODBCDriverPlugin)
Q_IMPORT_PLUGIN(QICOPlugin)
#endif

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
#if __unix__
  QFontDatabase::addApplicationFont(":/fonts/DejaVuSansCondensed.ttf");
#endif
  fish_annotator::db_uploader::MainWindow* w = 
    new fish_annotator::db_uploader::MainWindow();
  w->setAttribute(Qt::WA_DeleteOnClose, true);
  w->show();
  return a.exec();
}
 
