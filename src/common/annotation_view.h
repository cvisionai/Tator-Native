/// @file
/// @brief Defines AnnotationView class.

#ifndef ANNOTATION_VIEW_H
#define ANNOTATION_VIEW_H

#include <QGraphicsView>
#include <QWheelEvent>

namespace fish_annotator {

/// View that implements zoom.
class AnnotationView : public QGraphicsView {
  Q_OBJECT
public:
  /// Constructor.
  ///
  /// @param parent Parent widget.
  AnnotationView(QWidget *parent = 0);

  /// Fits view to current bounding rect.
  void fitInView();

  /// Reimplementation of wheelEvent.
  void wheelEvent(QWheelEvent *event) override final;

  /// Sets bounding rect.
  void setBoundingRect(const QRectF &rect);

private:
  /// Bounding rect for this view.
  QRectF bounding_rect_;

  /// Current zoom level.
  int32_t zoom_;
};

} // namespace fish_annotator

#endif // ANNOTATION_VIEW_H
