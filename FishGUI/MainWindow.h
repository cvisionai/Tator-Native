#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <player.h>
#include "ui_mainwindow.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

 public:
 	explicit MainWindow(QWidget *parent = 0);
 	~MainWindow();

 private slots:
  	void updatePlayerUI(QImage img);
  	void on_Play_clicked();
  	void on_LoadVideo_clicked();
 private:
 	Ui::MainWindow *ui;
 	Player* myPlayer;
};
