/// @file
/// @brief Defines AnnotationScene class.

#ifndef ANNOTATION_SCENE_H
#define ANNOTATION_SCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsRectItem>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>

#include "fish_annotator/common/annotation_widget.h"

namespace fish_annotator {

/// @brief Defines mouse behavior based on current drawing state.
enum Mode {
  kDoNothing,
  kSelect,
  kDraw
};

/// @brief Scene that implements special mouse event handlers for
///        drawing annotations.
class AnnotationScene : public QGraphicsScene {
  Q_OBJECT
public:
  /// @brief Constructor.
  ///
  /// @param parent Parent widget.
  AnnotationScene(QObject *parent = 0);

  /// @brief Sets the widget that specifies the active annotation type.
  ///
  /// @param widget AnnotationWidget object.
  void setToolWidget(AnnotationWidget *widget);
public slots:
  /// @brief Sets the current drawing mode.
  ///
  /// @param mode New mode.
  void setMode(Mode mode);

  /// @brief Sets the annotation type.
  void setAnnotationType(AnnotationType type);
signals:
  /// @brief Notifies that a box annotation has been drawn.
  ///
  /// @param Bounding rect of completed annotation.
  void boxFinished(const QRectF &rect);

  /// @brief Notifies that a line annotation has been drawn.
  ///
  /// @param Bounding rect of completed annotation.
  void lineFinished(const QLineF &line);

  /// @brief Notifies that a dot annotation has been drawn.
  ///
  /// @param Bounding rect of completed annotation.
  void dotFinished(const QPointF &dot);
protected:
  /// @brief Initiates drawing of an annotation if mode is kDraw.
  ///
  /// @param event Pointer to Qt mouse event.
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override final;

  /// @brief Modifies the shape of the annotation if mode is kDraw.
  ///
  /// @param event Pointer to Qt mouse event.
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override final;

  /// @brief Completes drawing of the annotation if mode is kDraw.
  ///
  /// @param event Pointer to Qt mouse event.
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override final;
private:
  /// @brief Current drawing mode.
  Mode mode_;

  /// @brief Current annotation type.
  AnnotationType type_;

  /// @brief Starting point of a new annotation.
  QPointF start_pos_;

  /// @brief Rect item.
  QGraphicsRectItem *rect_item_;

  /// @brief Line item.
  QGraphicsLineItem *line_item_;

  /// @brief Dot item.
  QGraphicsEllipseItem *dot_item_;

  ///@brief Makes current items movable and selectable.
  ///
  /// @param controllable True to make items controllable, false otherwise.
  void makeItemsControllable(bool controllable);
};

} // namespace fish_annotator

#endif // ANNOTATION_SCENE_H
