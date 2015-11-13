#ifndef ANNOTATEDREGION_H
#define ANNOTATEDREGION_H

#include <QGraphicsRectItem>

enum drag_t {
    DRAG_NONE = 0,
    DRAG_TOP,
    DRAG_BOTTOM,
    DRAG_LEFT,
    DRAG_RIGHT,
    DRAG_TOP_LEFT,
    DRAG_TOP_RIGHT,
    DRAG_BOTTOM_LEFT,
    DRAG_BOTTOM_RIGHT
};

class AnnotatedRegion: public QGraphicsRectItem
{
protected:
    drag_t drag;
public:
    AnnotatedRegion(QRectF area);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
//    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
};

#endif // ANNOTATEDREGION_H
