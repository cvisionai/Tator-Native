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

#include "fish_annotator/common/annotatedregion.h"
#include "fish_annotator/video_annotator/fish.h"
#include "fish_annotator/video_annotator/document.h"
#include "fish_annotator/video_annotator/player.h"
#include "ui_mainwindow.h"

namespace fish_annotator { namespace video_annotator {

class MainWindow : public QWidget
{
  Q_OBJECT
#ifndef NO_TESTING
  friend class TestVideoAnnotator;
#endif
public:
  /// @brief Mainwindow constructor.
  ///
  /// @param parent The parent widget for mainwindow.
  explicit MainWindow(QWidget *parent = 0);

  /// @brief Retrieves next frame from video player and adds annotations to 
  ///        frame.
  void nextFrame();

  /// @brief Retrieves previous frame from video player and adds annotations 
  ///        to frame.
  void prevFrame();

private slots:
  /// @brief Updates the GUI window with image img.
  ///
  /// @param img The image to display in the GUI window
  void updatePlayerUI(QImage img);
  void updateSubTypeMenu(int typeIndex);

  /// @brief Retrieves information for fish number currently in goToFishVal.
  void goToFish();

  /// @brief Converts time in seconds into hours, minutes, seconds.
  ///
  /// @param timeInSeconds The time from the start of the video in seconds.
  /// @return QString with formatted time.
  QString getFormattedTime(int timeInSeconds);

  /// @brief Stops the video player for seeking within video using slider.
  void on_videoSlider_sliderPressed();

  /// @brief Starts the video player after seeking within video using slider.
  void on_videoSlider_sliderReleased();

  /// @brief Sets the frame that the video player starts playing at within 
  ///        the video after seeking using the slider.
  ///
  /// @param position The position of the video slider.
  void on_videoSlider_sliderMoved(int position);

  /// @brief Starts video player.
  void on_Play_clicked();

  /// @brief Loads a video file user chooses with popup dialog.
  void on_LoadVideo_clicked();
  void on_loadAnnotate_clicked();
  void on_saveAnnotate_clicked();

  /// @brief Speeds up the playback rate of the video player.
  void on_SpeedUp_clicked();

  /// @brief Slows down the playback rate of the video player.
  void on_SlowDown_clicked();

  /// @brief Rewinds the video by 1 second.
  void on_minusOneSecond_clicked();

  /// @brief Rewinds the video by 3 seconds.
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
private:
  void onLoadVideoSuccess(const QFileInfo &name);
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

  /// @brief Adds a region.
  ///
  /// @return True if success, false if failure.
  bool addRegion();
  void removeRegion(uint64_t id, uint64_t frame);

  /// @brief Saves annotations to a given directory.
  ///
  /// @param dir_name Directory to save files.
  void saveAnnotations(const QString &dir_name);

  /// @brief Retrieves annotations associated with frame.
  ///
  /// @param frame The frame for which you want to retrieve annotations.
  void processAnnotations(uint64_t frame);

  /// @brief Rewinds the video by seconds_to_rewind seconds.
  ///
  /// @param seconds_to_rewind The number of seconds in the video to rewind.
  void rewindVideo(int seconds_to_rewind);
  void updateTypeMenu();
  QProgressDialog * genProgressDialog(QString dialog_text);

private:
  std::unique_ptr<Document> document_;
  std::unique_ptr<Ui::MainWidget> ui_;
  std::unique_ptr<Player> player_;
  std::vector<Fish> my_fish_list_;
  std::vector<Fish>::iterator list_pos_;
  std::unique_ptr<QGraphicsScene> scene_;
  int f_index_;
  int next_id_;
  QGraphicsPixmapItem *display_image_;
  std::list<AnnotatedRegion<AnnotationLocation> *> current_annotations_;
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

}} // namespace fish_annotator::video_annotator

#endif // MAINWINDOW_H

