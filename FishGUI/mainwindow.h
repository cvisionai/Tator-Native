#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QWidget>
#include <QStringList>
#include <QFileDialog>
#include <QMessageBox>
#include <QTime>
#include <QGraphicsPixmapItem>
#include <vector>
#include <algorithm>
#include <player.h>
#include <iostream>
#include "ui_mainwindow.h"
#include "fish.h"
#include "fishSerialize.pb.h"

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
	void updateSubTypeMenu(int typeIndex);
    void goToFish();
    void setTowType(bool towOpenStatus);
	QString getFormattedTime(int timeInSeconds);
	void on_videoSlider_sliderPressed();
	void on_videoSlider_sliderReleased();
	void on_videoSlider_sliderMoved(int position);
  	void on_Play_clicked();
  	void on_LoadVideo_clicked();
    void on_loadAnnotate_clicked();
	void on_SpeedUp_clicked();
	void on_SlowDown_clicked();
	void on_minusOneFrame_clicked();
	void on_plusOneFrame_clicked();
	void on_addRound_clicked();
	void on_addFlat_clicked();
	void on_addSkate_clicked();
	void on_addOther_clicked();
    void on_prevFish_clicked();
    void on_nextFish_clicked();
    void on_goToFrame_clicked();
    void on_updateFishFrame_clicked();
    void on_typeMenu_currentIndexChanged(int tIdx);
    void on_subTypeMenu_currentIndexChanged(int sIdx);
 private:
 	Ui::MainWidget *ui;
 	Player* myPlayer;
	fishSerialize::FishList* fList;
	vector<Fish> myFishList;
    vector<Fish>::iterator listPos;
	QGraphicsPixmapItem* imgPointer;
	QGraphicsScene *scene;
    int m_fIndex;
	void addFish(FishTypeEnum fType);
    void addFishSerialize(fishSerialize::FishEntry* newFish, FishTypeEnum newFType, int frame);
    void updateVecIndex();
    void convertFishToSerialize();
    void convertSerialToFish();
};
#endif
