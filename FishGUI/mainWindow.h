#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <player.h>
#include "ui_mainwindow.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QWidget, private Ui::MainWindow
{
	Q_OBJECT

 public:
 	MainWindow(QWidget *parent = 0);

 private slots:
  	void updatePlayerUI(QImage img);
  	void on_pushButton_clicked();
  	void on_pushButton2_clicked();
 private:
 	Ui::MainWindow *ui;
 	Player* myPlayer;
};
