#include "annotatedregion.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QDebug>
#include <QCursor>

#include <iostream>

AnnotatedRegion::AnnotatedRegion(std::shared_ptr<FishDetector::AnnotationLocation> annotation, QRectF area)
{
    this->annotation = annotation;
    setRect(area);
    setAcceptHoverEvents(true);
    setFlags(ItemIsMovable | ItemIsSelectable);
}

void AnnotatedRegion::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QPointF pos = event->pos();
    qreal from_right = pos.x() - rect().right();
    qreal from_left = pos.x() - rect().left();
    qreal from_top = pos.y() - rect().top();
    qreal from_bottom = pos.y() - rect().bottom();

    qreal margin = 5;
    if (abs(from_top) < margin) {
        if (abs(from_left) < margin) {
            drag = DRAG_TOP_LEFT;
            setCursor(QCursor(Qt::SizeFDiagCursor));
        } else if (abs(from_right) < margin) {
            drag = DRAG_TOP_RIGHT;
            setCursor(QCursor(Qt::SizeBDiagCursor));
        } else {
            drag = DRAG_TOP;
            setCursor(QCursor(Qt::SizeVerCursor));
        }
    } else if (abs(from_bottom) < margin) {
        if (abs(from_left) < margin) {
            drag = DRAG_BOTTOM_LEFT;
            setCursor(QCursor(Qt::SizeBDiagCursor));
        } else if (abs(from_right) < margin) {
            drag = DRAG_BOTTOM_RIGHT;
            setCursor(QCursor(Qt::SizeFDiagCursor));
        } else {
            drag = DRAG_BOTTOM;
            setCursor(QCursor(Qt::SizeVerCursor));
        }
    } else if (abs(from_left) < margin) {
        drag = DRAG_LEFT;
        setCursor(QCursor(Qt::SizeHorCursor));
    } else if (abs(from_right) < margin) {
        drag = DRAG_RIGHT;
        setCursor(QCursor(Qt::SizeHorCursor));
    } else {
        drag = DRAG_NONE;
        setCursor(QCursor());
    }

    if (drag != DRAG_NONE) {
        setFlag(ItemIsMovable, false);
    } else {
        setFlag(ItemIsMovable, true);
    }

    QGraphicsItem::hoverMoveEvent(event);
}

void AnnotatedRegion::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(QCursor());
    QGraphicsItem::hoverLeaveEvent( event );
}

void AnnotatedRegion::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QRectF area = rect();

    QPointF pos = event->pos();
//    qreal from_right = pos.x() - rect().right();
    qreal from_left = pos.x() - rect().left();
    qreal from_top = pos.y() - rect().top();
    qreal from_bottom = pos.y() - rect().bottom();

    qreal margin = 5;
    switch (drag) {
    case DRAG_TOP: {
        qreal new_height = area.height() - from_top;
        if (new_height > 2*margin) {
            area.setHeight(area.height() - from_top);
            area.translate(0, from_top);
        }
        break;
    }
    case DRAG_BOTTOM: {
        qreal new_height = pos.y() - area.top();
        if (new_height > 2*margin) {
            area.setHeight(new_height);
        }
        break;
    }
    case DRAG_LEFT: {
        qreal new_width = area.width() - from_left;
        if (new_width > 2*margin) {
            area.setWidth(area.width() - from_left);
            area.translate(from_left, 0);
        }
        break;
    }
    case DRAG_RIGHT: {
        qreal new_width = pos.x() - area.left();
        if (new_width > 2*margin) {
            area.setWidth(new_width);
        }
        break;
    }
    default:
        QGraphicsRectItem::mouseMoveEvent(event);
        auto pos = QGraphicsRectItem::pos();
        annotation->area.x = pos.x();
        annotation->area.y = pos.y();

        return;
    }

    annotation->area.x = area.x();
    annotation->area.y = area.y();
    annotation->area.w = area.width();
    annotation->area.h = area.height();

    setRect(area);
    prepareGeometryChange();
    update();
}


void AnnotatedRegion::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                            QWidget *widget)
{
    painter->setFont(QFont("Helvetica", 14));

    // draw main rectangle
    painter->setPen(Qt::DashLine);
    painter->setPen(QPen(QColor(255,0,0)));
    painter->drawRect(rect());

    // draw UID
    QString text("UID");
    QFontMetrics fm = painter->fontMetrics();
    int width = fm.width(text);
    QBrush brush;
    brush.setColor(Qt::gray);
    QRectF text_area = QRectF(rect().right() - width, rect().bottom() - fm.height(), width, fm.height());
    painter->fillRect(text_area, QBrush(QColor(64, 64, 64, 128)));

    painter->setPen(QPen(QColor(255, 0, 0)));
    painter->drawText(text_area, "UID");
}
