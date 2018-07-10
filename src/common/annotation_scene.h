/// @file
/// @brief Defines AnnotationScene class.

#ifndef ANNOTATION_SCENE_H
#define ANNOTATION_SCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QGraphicsRectItem>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <QTransform>

#include "annotation_widget.h"

namespace fish_annotator {

/// Defines mouse behavior based on current drawing state.
enum Mode {
  kDoNothing,
  kSelect,
  kDraw
};

/// Scene that implements special mouse event handlers for
///        drawing annotations.
class AnnotationScene : public QGraphicsScene {
  Q_OBJECT
public:
  /// Constructor.
  ///
  /// @param parent Parent widget.
  /// @param continual Whether annotation should be continual.
  AnnotationScene(QObject *parent = 0, bool continual = true);

  /// Sets the widget that specifies the active annotation type.
  ///
  /// @param widget AnnotationWidget object.
  void setToolWidget(AnnotationWidget *widget);
public slots:
  /// Sets the current drawing mode.
  ///
  /// @param mode New mode.
  void setMode(Mode mode);

  /// Sets the annotation type.
  void setAnnotationType(AnnotationType type);
signals:
  /// Notifies that a box annotation has been drawn.
  ///
  /// @param rect Bounding rect of completed annotation.
  void boxFinished(const QRectF &rect);

  /// Notifies that a line annotation has been drawn.
  ///
  /// @param line Start and end of completed annotation.
  void lineFinished(const QLineF &line);

  /// Notifies that a dot annotation has been drawn.
  ///
  /// @param dot Location of completed annotation.
  void dotFinished(const QPointF &dot);

  /// Notifies that an item has been made active.
  ///
  /// @param item Pointer to active item.
  void itemActivated(const QGraphicsItem &item);
protected:
  /// Initiates drawing of an annotation if mode is kDraw.
  ///
  /// @param event Pointer to Qt mouse event.
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override final;

  /// Modifies the shape of the annotation if mode is kDraw.
  ///
  /// @param event Pointer to Qt mouse event.
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override final;

  /// Completes drawing of the annotation if mode is kDraw.
  ///
  /// @param event Pointer to Qt mouse event.
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override final;

  /// Cancels drawing mode if escape is pressed.
  ///
  /// @param event Pointer to Qt key event.
  void keyPressEvent(QKeyEvent *event) override final;
private:
  /// Current drawing mode.
  Mode mode_;

  /// Current annotation type.
  AnnotationType type_;

  /// Starting point of a new annotation.
  QPointF start_pos_;

  /// Rect item.
  QGraphicsRectItem *rect_item_;

  /// Line item.
  QGraphicsLineItem *line_item_;

  /// Dot item.
  QGraphicsEllipseItem *dot_item_;

  /// QGraphicsItem
  QGraphicsItem *active_item_;

  /// Whether annotation should be continual.
  const bool continual_;

  ///Makes current items movable and selectable.
  ///
  /// @param controllable True to make items controllable, false otherwise.
  void makeItemsControllable(bool controllable);
};

} // namespace fish_annotator

#endif // ANNOTATION_SCENE_H
