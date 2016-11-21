/// @file
/// @brief Defines MainWindow class for image annotator.

#ifndef IMAGE_ANNOTATOR_MAINWINDOW_H
#define IMAGE_ANNOTATOR_MAINWINDOW_H

#include <memory>

#include <QWidget>

#include "fish_detector/image_annotator/image_annotation.h"

namespace fish_detector { namespace image_annotator {

namespace Ui {
  class MainWidget;
}

class MainWindow : public QWidget {
  Q_OBJECT
#ifndef NO_TESTING
  friend class TestImageAnnotator;
#endif
public:
  /// @brief Constructor.
  ///
  /// @param parent Parent widget.
  explicit MainWindow(QWidget *parent = 0);

private slots:
  /// @brief Brings up a dialog box to add a species.
  void on_addSpecies_clicked();

  /// @brief Brings up a dialog box to load a species file.
  void on_loadSpecies_clicked();

  /// @brief Brings up a dialog box to save a species file.
  void on_saveSpecies_clicked();

  /// @brief Moves to next image.
  void on_next_clicked();

  /// @brief Moves to previous image.
  void on_prev_clicked();

  /// @brief Brings up a dialog to open a directory containing images.
  void on_loadImageDir_clicked();

  /// @brief Saves annotations.
  void on_saveAnnotations_clicked();

  /// @brief Moves to the image indicated by image slider.
  void on_imageSlider_sliderMoved();

private:
  /// @brief Annotations associated with this directory.
  std::unique_ptr<ImageAnnotationList> annotations_;

  /// @brief Widget loaded from ui file.
  std::unique_ptr<Ui::MainWidget> ui_;
};

}} // namespace fish_detector::image_annotator

#endif //IMAGE_ANNOTATOR_MAINWINDOW_H
