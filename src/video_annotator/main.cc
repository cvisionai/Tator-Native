#include <QtPlugin>
#include <QApplication>

#include "fish_annotator/video_annotator/mainwindow.h"

#ifdef _WIN32
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
Q_IMPORT_PLUGIN(QICOPlugin)
#elif __APPLE__
Q_IMPORT_PLUGIN(QCocoaIntegrationPlugin)
#endif

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	

	fish_annotator::video_annotator::MainWindow* w = new fish_annotator::video_annotator::MainWindow();
	w->setAttribute(Qt::WA_DeleteOnClose, true);

	w->show();

	return a.exec();
}
 
