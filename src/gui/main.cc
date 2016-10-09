#include <QtPlugin>
#include <QApplication>

#include "fish_detector/gui/mainwindow.h"

#ifdef _WIN32
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
#elif __APPLE__
Q_IMPORT_PLUGIN(QCocoaIntegrationPlugin)
#endif

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
  fish_detector::gui::MainWindow* w = new fish_detector::gui::MainWindow();
	w->setAttribute(Qt::WA_DeleteOnClose, true);

	w->show();

	return a.exec();
}
 
