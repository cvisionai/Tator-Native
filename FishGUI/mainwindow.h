#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QWidget>
#include <QStringList>
#include <QFileDialog>
#include <QMessageBox>
#include <QTime>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <vector>
#include <algorithm>
#include <player.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <ostream>
#include <memory>
#include <string>
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
// 	~MainWindow();

 private slots:
  	void updatePlayerUI(QImage img);
	void updateSubTypeMenu(int typeIndex);
    void goToFish();
    //void setTowType(bool towOpenStatus);
	QString getFormattedTime(int timeInSeconds);
	void on_videoSlider_sliderPressed();
	void on_videoSlider_sliderReleased();
	void on_videoSlider_sliderMoved(int position);
  	void on_Play_clicked();
  	void on_LoadVideo_clicked();
    void on_loadAnnotate_clicked();
    void on_saveAnnotate_clicked();
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
    std::unique_ptr<Ui::MainWidget> ui;
    std::unique_ptr<Player> myPlayer;
//	fishSerialize::FishList* fList;
	vector<Fish> myFishList;
    vector<Fish>::iterator listPos;
	QGraphicsPixmapItem* imgPointer;
    std::unique_ptr<QGraphicsScene> scene;
    int m_fIndex;
	void addFish(FishTypeEnum fType);
    void updateVecIndex();
    FishTypeEnum getFishType (string const& inString);
    int getFishSpecies (FishTypeEnum fType, string const& sString);
    string getFishTypeString (FishTypeEnum fType);
    string getFishSpeciesString (FishTypeEnum fType, int species);
    void disableControls();
    void enableControls();
    void keyPressEvent(QKeyEvent* e);

//    void convertFishToSerialize();
//    void convertSerialToFish();
};

template<class T>
T base_name(T const & path, T const & delims = "/\\")
{
  return path.substr(path.find_last_of(delims) + 1);
}

template<class T>
T remove_extension(T const & filename)
{
  typename T::size_type const p(filename.find_last_of('.'));
  return p > 0 && p != T::npos ? filename.substr(0, p) : filename;
}

template<class T>
T remove_reviewer(T const & filename)
{
  typename T::size_type const p(filename.find_last_of('_'));
  return p > 0 && p != T::npos ? filename.substr(0, p) : filename;
}
#endif
