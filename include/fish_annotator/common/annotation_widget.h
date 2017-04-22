/// @file
/// @brief Defines AnnotationWidget class.

#ifndef ANNOTATION_WIDGET_H
#define ANNOTATION_WIDGET_H

#include <memory>

#include "fish_annotator/common/annotation_widget.h"
#include "../../src/common/ui_annotation_widget.h"

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
  void on_makeBox_clicked();

  /// @brief Emits signal to make a line annotation.
  void on_makeLine_clicked();

  /// @brief Emits signal to make a dot annotation.
  void on_makeDot_clicked();

signals:
  /// @brief Requests enabling a particular annotation tool.
  void makeAnnotation(AnnotationType type);

private:
  /// @brief Widget loaded from ui file.
  std::unique_ptr<Ui::AnnotationWidget> ui_;
};

} // namespace fish_annotator

#endif // ANNOTATION_WIDGET_H
