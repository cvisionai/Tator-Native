/// @file
/// @brief Defines AnnotationView class.

#ifndef ANNOTATION_VIEW_H
#define ANNOTATION_VIEW_H

#include <QGraphicsView>
#include <QWheelEvent>

namespace fish_annotator {

/// @brief View that implements zoom.
class AnnotationView : public QGraphicsView {
  Q_OBJECT
public:
  /// @brief Constructor.
  ///
  /// @param parent Parent widget.
  AnnotationView(QWidget *parent = 0);

  /// @brief Fits view to current bounding rect.
  void fitInView();

  /// @brief Reimplementation of wheelEvent.
  void wheelEvent(QWheelEvent *event) override final;

  /// @brief Sets bounding rect.
  void setBoundingRect(const QRectF &rect);

private:
  /// @brief Bounding rect for this view.
  QRectF bounding_rect_;

  /// @brief Current zoom level.
  uint32_t zoom_;
};

} // namespace fish_annotator

#endif // ANNOTATION_VIEW_H
