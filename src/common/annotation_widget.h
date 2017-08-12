/// @file
/// @brief Defines AnnotationWidget class.

#ifndef ANNOTATION_WIDGET_H
#define ANNOTATION_WIDGET_H

#include <memory>

#include "annotation_widget.h"
#include "ui_annotation_widget.h"

namespace fish_annotator {

enum AnnotationType {
  kBox,
  kLine,
  kDot
};

class AnnotationWidget : public QWidget {
  Q_OBJECT
public:
  /// @brief Constructor.
  ///
  /// @param parent Parent widget.
  explicit AnnotationWidget(QWidget *parent = 0);

private slots:
  /// @brief Emits signal to make a box annotation.
  ///
  /// @param Whether the button is toggled on or off.
  void on_makeBox_toggled(bool checked);

  /// @brief Emits signal to make a line annotation.
  ///
  /// @param Whether the button is toggled on or off.
  void on_makeLine_toggled(bool checked);

  /// @brief Emits signal to make a dot annotation.
  ///
  /// @param Whether the button is toggled on or off.
  void on_makeDot_toggled(bool checked);

signals:
  /// @brief Requests enabling a particular annotation tool.
  void setAnnotation(AnnotationType type);

private:
  /// @brief Widget loaded from ui file.
  std::unique_ptr<Ui::AnnotationWidget> ui_;
};

} // namespace fish_annotator

#endif // ANNOTATION_WIDGET_H
