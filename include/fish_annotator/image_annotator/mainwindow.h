/// @file
/// @brief Defines MainWindow class for image annotator.

#ifndef IMAGE_ANNOTATOR_MAINWINDOW_H
#define IMAGE_ANNOTATOR_MAINWINDOW_H

#include <memory>
#include <vector>
#include <string>

#include <boost/filesystem.hpp>

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

#include "fish_annotator/common/species_controls.h"
#include "fish_annotator/common/annotatedregion.h"
#include "fish_annotator/image_annotator/image_annotation.h"
#include "ui_mainwindow.h"

#ifndef NO_TESTING
class TestImageAnnotator;
#endif

namespace fish_annotator { namespace image_annotator {

class MainWindow : public QWidget {
  Q_OBJECT
#ifndef NO_TESTING
  friend class ::TestImageAnnotator;
#endif
public:
  /// @brief Constructor.
  ///
  /// @param parent Parent widget.
  explicit MainWindow(QWidget *parent = 0);

private slots:
  /// @brief Moves to next image.
  void on_next_clicked();

  /// @brief Moves to previous image.
  void on_prev_clicked();

  /// @brief Brings up a dialog to open a directory containing images.
  void on_loadImageDir_clicked();

  /// @brief Saves annotations.
  void on_saveAnnotations_clicked();

  /// @brief Moves to the image indicated by image slider.
  void on_imageSlider_valueChanged();

  /// @brief Updates display to show/hide annotations.
  void on_showAnnotations_stateChanged();

  /// @brief Updates species and subspecies for selected ID.
  void on_idSelection_currentIndexChanged(const QString &id);

  /// @brief Removes currently selected annotation.
  void on_removeAnnotation_clicked();

  /// @brief Adds an individual and enables bounding box drawing.
  void addIndividual(std::string species, std::string subspecies);

private:
  /// @brief Annotations associated with this directory.
  std::unique_ptr<ImageAnnotationList> annotations_;

  /// @brief Scene for displaying images.
  std::unique_ptr<QGraphicsScene> scene_;

  /// @brief Widget loaded from ui file.
  std::unique_ptr<Ui::MainWidget> ui_;

  /// @brief Species controls widget.
  std::unique_ptr<SpeciesControls> species_controls_;

  /// @brief Vector of image files in a directory.
  std::vector<boost::filesystem::path> image_files_;

  /// @brief Runs when image directory loaded successfully.
  void onLoadDirectorySuccess(const QString &image_dir);
};

}} // namespace fish_annotator::image_annotator

#endif //IMAGE_ANNOTATOR_MAINWINDOW_H
