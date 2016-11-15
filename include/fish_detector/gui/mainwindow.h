#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <ostream>
#include <memory>
#include <string>

#include <QWidget>
#include <QStringList>
#include <QFileDialog>
#include <QMessageBox>
#include <QTime>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QProgressDialog>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

#include "fish_detector/gui/fish.h"
#include "fish_detector/gui/annotatedregion.h"
#include "fish_detector/gui/document.h"
#include "fish_detector/gui/player.h"

namespace fish_detector { namespace gui {

namespace Ui {
  class MainWidget;
}

class MainWindow : public QWidget
{
  Q_OBJECT

  public:
  explicit MainWindow(QWidget *parent = 0);
//   ~MainWindow();

  void nextFrame();
  void prevFrame();

 private slots:
  void updatePlayerUI(QImage img);
  void updateSubTypeMenu(int typeIndex);
  void goToFish();
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
  void on_minusOneSecond_clicked();
  void on_minusThreeSecond_clicked();
  void on_minusOneFrame_clicked();
  void on_plusOneFrame_clicked();
  void on_addRound_clicked();
  void on_addFlat_clicked();
  void on_addSkate_clicked();
  void on_addOther_clicked();
  void on_prevFish_clicked();
  void on_nextFish_clicked();
  void on_goToFrame_clicked();
  void on_removeFish_clicked();
  void on_addRegion_clicked();
  void on_removeRegion_clicked();
  void on_nextAndCopy_clicked();
  void on_typeMenu_currentIndexChanged(int tIdx);
  void on_subTypeMenu_currentIndexChanged(int sIdx);
  void on_writeImage_clicked();
public:
  void updateImage(const QImage &image);
  std::unique_ptr<Player> &getPlayer() { return player; }
  
private:
  void updateAnnotations();
  void addFish(FishTypeEnum fType);
  void updateVecIndex();
  FishTypeEnum getFishType (std::string const &inString);
  int getFishSpecies (FishTypeEnum fType, std::string const &sString);
  std::string getFishTypeString (FishTypeEnum fType);
  std::string getFishSpeciesString (FishTypeEnum fType, int species);
  void disableControls();
  void enableControls();
  void keyPressEvent(QKeyEvent *e);
  void removeRegion(std::uint64_t id, std::uint64_t frame);
  void processAnnotations(uint64_t frame);
  void rewind_video(int seconds_to_rewind);
  void updateTypeMenu();
  void format_progress_dialog(QProgressDialog &progress_dialog);
  QProgressDialog * gen_progress_dialog(QString dialog_text, QProgressBar * myBar);

private:
  std::unique_ptr<Document> document;
  std::unique_ptr<Ui::MainWidget> ui;
  std::unique_ptr<Player> player;
  std::vector<Fish> myFishList;
  std::vector<Fish>::iterator listPos;
  std::unique_ptr<QGraphicsScene> scene;
  int fIndex;
  int nextID;
  QGraphicsPixmapItem *displayImage;
  std::list<AnnotatedRegion *> currentAnnotations;
  QString images_save_path_;
  QString progress_bar_stylesheet_;
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

}} // namespace fish_detector::gui

#endif // MAINWINDOW_H

