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
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

#include "ui_mainwindow.h"
#include "fish.h"
#include "annotatedregion.h"
#include "document.h"

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

    void nextFrame();
    void prevFrame();
    void gotoFrame();
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
    void on_addRegion_clicked();
    void on_typeMenu_currentIndexChanged(int tIdx);
    void on_subTypeMenu_currentIndexChanged(int sIdx);
public:
    void updateImage(const QImage &image);
    std::unique_ptr<Player> &getPlayer() { return player; }
    
 private:
    void updateAnnotations();
    void addFish(FishTypeEnum fType);
    void updateVecIndex();
    FishTypeEnum getFishType (string const &inString);
    int getFishSpecies (FishTypeEnum fType, string const &sString);
    string getFishTypeString (FishTypeEnum fType);
    string getFishSpeciesString (FishTypeEnum fType, int species);
    void disableControls();
    void enableControls();
    void keyPressEvent(QKeyEvent *e);
 private:
    std::unique_ptr<FishDetector::Document> document;

    std::unique_ptr<Ui::MainWidget> ui;
    std::unique_ptr<Player> player;
//	fishSerialize::FishList* fList;
	vector<Fish> myFishList;
    vector<Fish>::iterator listPos;
    std::unique_ptr<QGraphicsScene> scene;
    int fIndex;

//    void convertFishToSerialize();
//    void convertSerialToFish();

//    QImage displayImage;
    QGraphicsPixmapItem *displayImage;
    std::list<AnnotatedRegion *> currentAnnotations;
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
