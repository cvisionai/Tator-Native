#include <QtPlugin>
#include <QApplication>

#include "fish_annotator/image_annotator/mainwindow.h"

#ifdef _WIN32
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
Q_IMPORT_PLUGIN(QDDSPlugin)
Q_IMPORT_PLUGIN(QGifPlugin)
Q_IMPORT_PLUGIN(QICNSPlugin)
Q_IMPORT_PLUGIN(QICOPlugin)
Q_IMPORT_PLUGIN(QJpegPlugin)
Q_IMPORT_PLUGIN(QSvgPlugin)
Q_IMPORT_PLUGIN(QTgaPlugin)
Q_IMPORT_PLUGIN(QTiffPlugin)
Q_IMPORT_PLUGIN(QWbmpPlugin)
Q_IMPORT_PLUGIN(QWebpPlugin)
#elif __APPLE__
Q_IMPORT_PLUGIN(QCocoaIntegrationPlugin)
#elif __unix__
Q_IMPORT_PLUGIN(QXcbIntegrationPlugin)
#endif

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	fish_annotator::image_annotator::MainWindow* w = 
    new fish_annotator::image_annotator::MainWindow();
	w->setAttribute(Qt::WA_DeleteOnClose, true);
	w->show();
	return a.exec();
}
 
