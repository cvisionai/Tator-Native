/// @file
/// @brief Defines MainWindow class for image annotator.

#ifndef IMAGE_ANNOTATOR_MAINWINDOW_H
#define IMAGE_ANNOTATOR_MAINWINDOW_H

#include <memory>

#include <vector>
#include <string>
#include <memory>

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QMenu>

#include "fish_detector/common/species_widget.h"
#include "fish_detector/image_annotator/image_annotation.h"
#include "ui_mainwindow.h"

namespace fish_detector { namespace image_annotator {

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
  void on_imageSlider_valueChanged();

  /// @brief Clears all species widgets after asking for confirmation.
  void on_clearAllSpeciesWidgets_triggered();

  /// @brief Clears a specific species widget.
  void clearSpeciesWidget();

  /// @brief Edits a specific species widget.
  void editSpeciesWidget();

private:
  /// @brief Annotations associated with this directory.
  std::unique_ptr<ImageAnnotationList> annotations_;

  /// @brief Scene for displaying images.
  std::unique_ptr<QGraphicsScene> scene_;

  /// @brief Pixel map from current image.
  std::unique_ptr<QGraphicsPixmapItem> pixmap_;

  /// @brief Widget loaded from ui file.
  std::unique_ptr<Ui::MainWidget> ui_;

  /// @brief List of species widget pointers.
  std::list<std::unique_ptr<SpeciesWidget>> species_widgets_;

  /// @brief Drop down menu for editing species.
  std::unique_ptr<QMenu> edit_species_menu_;

  /// @brief Drop down menu for clearing species.
  std::unique_ptr<QMenu> clear_species_menu_;

  /// @brief Vector of image files in a directory.
  std::vector<std::string> image_files_;

  /// @brief Runs when image directory loaded successfully.
  void onLoadDirectorySuccess(const QString &image_dir);

  /// @brief Clears all species widgets.
  void clearAllSpeciesWidgets();
};

}} // namespace fish_detector::image_annotator

#endif //IMAGE_ANNOTATOR_MAINWINDOW_H
