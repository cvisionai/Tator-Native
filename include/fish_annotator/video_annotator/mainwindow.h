/// @file
/// @brief Defines MainWindow class for video annotator.

#ifndef VIDEO_ANNOTATOR_MAINWINDOW_H
#define VIDEO_ANNOTATOR_MAINWINDOW_H

#include <memory>
#include <atomic>

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsVideoItem>
#include <QFileDialog>
#include <QMessageBox>
#include <QOpenGLWidget>

#include "fish_annotator/common/species_controls.h"
#include "fish_annotator/common/annotatedregion.h"
#include "fish_annotator/video_annotator/video_annotation.h"
#include "fish_annotator/video_annotator/player.h"
#include "ui_mainwindow.h"

#ifndef NO_TESTING
class TestVideoAnnotator;
#endif

namespace fish_annotator { namespace video_annotator {

/// @brief Video annotation GUI.
class MainWindow : public QWidget {
  Q_OBJECT
#ifndef NO_TESTING
  friend class ::TestVideoAnnotator;
#endif
public:
  /// @brief Constructor.
  ///
  /// @param parent Parent widget.
  explicit MainWindow(QWidget *parent = 0);

protected:
  /// @brief Resizes the video and scene.
  void resizeEvent(QResizeEvent *event) override final;

private slots:
  /// @brief Plays/pauses the video.
  void on_play_clicked();

  /// @brief Changes the playback direction of the video.
  ///
  /// @param state State of the checkbox.
  void on_reverse_stateChanged(int state);

  /// @brief Increases the playback speed of the video by a factor of two.
  void on_faster_clicked();

  /// @brief Decreases the playback speed of the video by a factor of two.
  void on_slower_clicked();

  /// @brief Increments the video by one frame.
  void on_plusOneFrame_clicked();

  /// @brief Decrements the video by one frame.
  void on_minusOneFrame_clicked();

  /// @brief Loads a video file.
  void on_loadVideo_clicked();

  /// @brief Loads an annotation file.
  void on_loadAnnotationFile_clicked();

  /// @brief Saves an annotation file.
  void on_saveAnnotationFile_clicked();

  /// @brief Writes the current frame to file.
  void on_writeImage_clicked();

  /// @brief Pauses the video and stores the play/pause state.
  void on_videoSlider_sliderPressed();

  /// @brief Continues the video if it was previously playing.
  void on_videoSlider_sliderReleased();

  /// @brief Sets the current frame to a new position.
  ///
  /// @param value New value of the 
  void on_videoSlider_valueChanged(int value);

  /// @brief Updates the current fish with a new species and updates the
  ///        subtype options.
  ///
  /// @param text Selected species.
  void on_typeMenu_currentTextChanged(const QString &text);

  /// @brief Updates the current fish with a new subspecies.
  ///
  /// @param text Selected subspecies.
  void on_subTypeMenu_currentTextChanged(const QString &text);

  /// @brief Sets current fish to next lowest ID.
  void on_prevFish_clicked();

  /// @brief Sets current fish to next highest ID.
  void on_nextFish_clicked();

  /// @brief Removes the current fish.
  void on_removeFish_clicked();

  /// @brief Goes to the first frame where a detection for this fish
  ///        occurs, if any.
  void on_goToFrame_clicked();

  /// @brief Updates the current fish to the specified ID.
  void on_goToFishVal_returnPressed();

  /// @brief Adds a region for the current fish.  If a region already 
  ///        exists for this fish and frame, an error is raised.
  void on_addRegion_clicked();

  /// @brief Removes a region for the current fish and frame.  If a 
  ///        region does not exist an error is raised.
  void on_removeRegion_clicked();

  /// @brief Goes to next frame and copies the region corresponding to
  ///        the current fish and frame.
  void on_nextAndCopy_clicked();

  /// @brief Displays a video frame.
  ///
  /// @param image Video frame to display.
  /// @param frame Frame of the image.
  void showFrame(std::shared_ptr<QImage> image, uint64_t frame);

  /// @brief Adds an individual and enables bounding box drawing.
  void addIndividual(std::string species, std::string subspecies);

  /// @brief Handles player duration change.
  void handlePlayerDurationChanged(uint64_t duration);

  /// @brief Handles player position change.
  void handlePlayerPositionChanged(uint64_t position);

  /// @brief Handles player playback rate change.
  void handlePlayerPlaybackRateChanged(double rate);

  /// @brief Handles media player errors.
  void handlePlayerError(const std::string &err);

  /// @brief Handles new media loaded.
  void handlePlayerMedia();

private:
  /// @brief Annotations associated with this video.
  std::unique_ptr<VideoAnnotation> annotation_;

  /// @brief Scene for displaying video.
  std::unique_ptr<QGraphicsScene> scene_;

  /// @brief Pixmap item for displaying video frames.
  QGraphicsPixmapItem *pixmap_item_;

  /// @brief Last frame displayed by player.
  std::shared_ptr<QImage> last_frame_;

  /// @brief Last displayed video position (frames).
  uint64_t last_displayed_frame_;

  /// @brief Media player.
  std::unique_ptr<Player> player_;

  /// @brief Widget loaded from the ui file.
  std::unique_ptr<Ui::MainWidget> ui_;

  /// @brief Species controls widget.
  std::unique_ptr<SpeciesControls> species_controls_;

  /// @brief Used to store play state when slider moved.
  bool was_playing_;

  /// @brief Currently selected fish ID.
  uint64_t fish_id_;

  /// @brief Current annotations.
  std::list<AnnotatedRegion<DetectionAnnotation>*> current_annotations_;

  /// @brief Updates displayed fish statistics.
  void updateStats();

  /// @brief Draws annotations for the last displayed frame.
  void drawAnnotations();
};

}} // namespace fish_annotator::video_annotator

#endif //VIDEO_ANNOTATOR_MAINWINDOW_H
