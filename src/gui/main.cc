#include <QtPlugin>
#include <QApplication>

#include "fish_detector/gui/mainwindow.h"

Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
  fish_detector::gui::MainWindow* w = new fish_detector::gui::MainWindow();
	w->setAttribute(Qt::WA_DeleteOnClose, true);

	w->show();

	return a.exec();
}
 
