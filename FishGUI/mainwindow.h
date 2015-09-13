#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QTime>
#include <QGraphicsPixmapItem>
#include <vector>
#include <player.h>
#include <iostream>
#include "ui_mainwindow.h"
#include "fish.h"

using namespace std;
namespace Ui {
	class MainWidget;
}

class MainWindow : public QWidget
{
	Q_OBJECT

 public:
 	explicit MainWindow(QWidget *parent = 0);
 	~MainWindow();

 private slots:
  	void updatePlayerUI(QImage img);
	QString getFormattedTime(int timeInSeconds);
	void on_videoSlider_sliderPressed();
	void on_videoSlider_sliderReleased();
	void on_videoSlider_sliderMoved(int position);
  	void on_Play_clicked();
  	void on_LoadVideo_clicked();
	void on_SpeedUp_clicked();
	void on_SlowDown_clicked();
	void on_addFlounder_clicked();
	void on_addCod_clicked();
	void on_addHaddock_clicked();
	void on_addSkate_clicked();
 private:
 	Ui::MainWidget *ui;
 	Player* myPlayer;
	Fish* tempFish;
	vector<Fish> myFishList;
	QGraphicsPixmapItem* imgPointer;
	QGraphicsScene *scene;
};
#endif
