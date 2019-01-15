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
#include <QMap>
#include <QColor>
#include <QCheckBox>

#include "species_controls.h"
#include "annotation_widget.h"
#include "global_state_widget.h"
#include "annotation_view.h"
#include "annotation_scene.h"
#include "metadata.h"
#include "image_annotation.h"
#include "ui_mainwindow.h"

#ifndef NO_TESTING
class TestImageAnnotator;
#endif

namespace tator { namespace image_annotator {

/// Main window for image annotator.
class MainWindow : public QMainWindow {
  Q_OBJECT
#ifndef NO_TESTING
  friend class ::TestImageAnnotator;
#endif
public:
  /// Constructor.
  ///
  /// @param parent Parent widget.
  explicit MainWindow(QWidget *parent = 0);
protected:
  /// Resizes the video and scene.
  ///
  /// @param event Qt resize event pointer.
  void resizeEvent(QResizeEvent *event) override final;
private slots:
  /// Moves to next image.
  void on_next_clicked();

  /// Moves to previous image.
  void on_prev_clicked();

  /// Brings up a dialog to open a directory containing images.
  void on_loadImageDir_triggered();

  /// Saves annotations.
  void on_saveAnnotations_triggered();

  /// Saves an annotated image.
  void on_saveAnnotatedImage_triggered();

  /// Saves series of annotated images.
  void on_saveAnnotatedImageBatch_triggered();

  /// Sets metadata for the annotations.
  void on_setMetadata_triggered();

  /// Moves to the image indicated by image slider.
  void on_imageSlider_sliderReleased();

  /// Updates display to show/hide annotations.
  void on_showAnnotations_stateChanged();

  /// Updates species and subspecies for selected ID.
  void on_idSelection_activated(const QString &id);

  /// Updates the current annotation with a new species.
  ///
  /// @param text Selected species.
  void on_typeMenu_activated(const QString &text);

  /// Updates the current annotation with a new subspecies.
  ///
  /// @param text Selected subspecies.
  void on_subTypeMenu_activated(const QString &text);

  /// Removes currently selected annotation.
  void on_removeAnnotation_clicked();

public slots:
  /// Adds an individual and enables bounding box drawing.
  void addIndividual(std::string species, std::string subspecies);

  /// Handles color change for a species.
  ///
  /// @param color_map Map between species names and colors.
  void colorChanged(QMap<QString, QColor> color_map);

  /// Adds a box annotation.
  ///
  /// @param rect Definition of the box.
  void addBoxAnnotation(const QRectF &rect);

  /// Adds a line annotation.
  ///
  /// @param line Definition of the line.
  void addLineAnnotation(const QLineF &line);

  /// Adds a dot annotation.
  ///
  /// @param dot Definition of the dot.
  void addDotAnnotation(const QPointF &dot);
private:
  /// Annotations associated with this directory.
  std::unique_ptr<ImageAnnotationList> annotations_;

  /// Image window.
  std::unique_ptr<AnnotationView> view_;

  /// Scene for displaying images.
  std::unique_ptr<AnnotationScene> scene_;

  /// Widget loaded from ui file.
  std::unique_ptr<Ui::MainWindow> ui_;

  /// Species controls widget.
  std::unique_ptr<SpeciesControls> species_controls_;

  /// Annotation widget.
  std::unique_ptr<AnnotationWidget> annotation_widget_;

  /// Global state widget
  std::unique_ptr<GlobalStateWidget> global_state_widget_;

  /// Vector of image files in a directory.
  std::vector<boost::filesystem::path> image_files_;

  /// Annotation metadata.
  Metadata metadata_;

  /// Records species of recently added individual.
  std::string species_;

  /// Records subspecies of recently added individual.
  std::string subspecies_;

  /// Current annotations.
  std::vector<std::pair<uint64_t, QGraphicsItem*>> current_annotations_;

  /// Map of species names to colors.
  QMap<QString, QColor> color_map_;

  /// Runs when image directory loaded successfully.
  ///
  /// Defined as separate function for testing purposes.
  void onLoadDirectorySuccess(const QString &image_dir);

  /// Gets the current annotation according to image and ID.
  std::shared_ptr<ImageAnnotation> currentAnnotation();

  /// Updates type menus.
  void updateTypeMenus();

  /// Draws annotations.
  void drawAnnotations();

  /// Updates species counts.
  void updateSpeciesCounts();

  /// Sets current active annotation
  void setItemActive(const QGraphicsItem &item);

  /// Deletes current active annotation
  void deleteCurrentAnn();

  /// Updates image.
  void updateImage();

  /// Enables or disables controls.
  void setEnabled(bool enable);
};

}} // namespace tator::image_annotator

#endif //IMAGE_ANNOTATOR_MAINWINDOW_H
