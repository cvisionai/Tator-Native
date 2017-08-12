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

#include "species_controls.h"
#include "annotation_widget.h"
#include "annotation_view.h"
#include "annotation_scene.h"
#include "metadata.h"
#include "image_annotation.h"
#include "ui_mainwindow.h"

#ifndef NO_TESTING
class TestImageAnnotator;
#endif

namespace fish_annotator { namespace image_annotator {

class MainWindow : public QMainWindow {
  Q_OBJECT
#ifndef NO_TESTING
  friend class ::TestImageAnnotator;
#endif
public:
  /// @brief Constructor.
  ///
  /// @param parent Parent widget.
  explicit MainWindow(QWidget *parent = 0);
protected:
  /// @brief Resizes the video and scene.
  ///
  /// @param event Qt resize event pointer.
  void resizeEvent(QResizeEvent *event) override final;
private slots:
  /// @brief Moves to next image.
  void on_next_clicked();

  /// @brief Moves to previous image.
  void on_prev_clicked();

  /// @brief Brings up a dialog to open a directory containing images.
  void on_loadImageDir_triggered();

  /// @brief Saves annotations.
  void on_saveAnnotations_triggered();

  /// @brief Sets metadata for the annotations.
  void on_setMetadata_triggered();

  /// @brief Moves to the image indicated by image slider.
  void on_imageSlider_valueChanged();

  /// @brief Updates display to show/hide annotations.
  void on_showAnnotations_stateChanged();

  /// @brief Updates species and subspecies for selected ID.
  void on_idSelection_activated(const QString &id);

  /// @brief Updates the current annotation with a new species.
  ///
  /// @param text Selected species.
  void on_typeMenu_activated(const QString &text);

  /// @brief Updates the current annotation with a new subspecies.
  ///
  /// @param text Selected subspecies.
  void on_subTypeMenu_activated(const QString &text);

  /// @brief Removes currently selected annotation.
  void on_removeAnnotation_clicked();

  /// @brief Adds an individual and enables bounding box drawing.
  void addIndividual(std::string species, std::string subspecies);

  /// @brief Adds a box annotation.
  ///
  /// @param rect Definition of the box.
  void addBoxAnnotation(const QRectF &rect);

  /// @brief Adds a line annotation.
  ///
  /// @param line Definition of the line.
  void addLineAnnotation(const QLineF &line);

  /// @brief Adds a dot annotation.
  ///
  /// @param dot Definition of the dot.
  void addDotAnnotation(const QPointF &dot);
private:
  /// @brief Annotations associated with this directory.
  std::unique_ptr<ImageAnnotationList> annotations_;

  /// @brief Image window.
  std::unique_ptr<AnnotationView> view_;

  /// @brief Scene for displaying images.
  std::unique_ptr<AnnotationScene> scene_;

  /// @brief Widget loaded from ui file.
  std::unique_ptr<Ui::MainWindow> ui_;

  /// @brief Species controls widget.
  std::unique_ptr<SpeciesControls> species_controls_;

  /// @brief Annotation widget.
  std::unique_ptr<AnnotationWidget> annotation_widget_;

  /// @brief Vector of image files in a directory.
  std::vector<boost::filesystem::path> image_files_;

  /// @brief Annotation metadata.
  Metadata metadata_;

  /// @brief Records species of recently added individual.
  std::string species_;

  /// @brief Records subspecies of recently added individual.
  std::string subspecies_;

  /// @brief Current annotations.
  std::vector<QGraphicsItem*> current_annotations_;

  /// @brief Runs when image directory loaded successfully.
  ///
  /// Defined as separate function for testing purposes.
  void onLoadDirectorySuccess(const QString &image_dir);

  /// @brief Gets the current annotation according to image and ID.
  std::shared_ptr<ImageAnnotation> currentAnnotation();

  /// @brief Updates type menus.
  void updateTypeMenus();

  /// @brief Draws annotations.
  void drawAnnotations();

  /// @brief Updates species counts.
  void updateSpeciesCounts();
};

}} // namespace fish_annotator::image_annotator

#endif //IMAGE_ANNOTATOR_MAINWINDOW_H
