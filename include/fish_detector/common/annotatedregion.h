#ifndef ANNOTATEDREGION_H
#define ANNOTATEDREGION_H

#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>

namespace fish_detector {

enum Drag {
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


/// @brief Displays bounding boxes drawn by user.
///
/// @tparam Info Contains data associated with a bounding box.
template<typename Info>
class AnnotatedRegion: public QGraphicsRectItem
{
public:

    /// @brief Constructor.
    AnnotatedRegion(uint64_t uid, 
                    std::shared_ptr<Info> annotation, 
                    QRectF area);

    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    void updateFrame(uint64_t frame);
    std::shared_ptr<Info> getAnnotation() {return annotation_;}
    uint64_t getUID() {return uid_;}
protected:
    /// @brief Pointer to the annotation location.
    std::shared_ptr<Info> annotation_;

    /// @brief ID associated with this object.
    uint64_t uid_;

    /// @brief Type of move event.
    Drag drag_;
};

//
// Implementations
//

template<typename Info>
AnnotatedRegion<Info>::AnnotatedRegion(
    uint64_t uid,
    std::shared_ptr<Info> annotation,
    QRectF area )
    : annotation_(annotation)
    , uid_(uid)
    , drag_() {
    setRect(area);
    setAcceptHoverEvents(true);
    setFlags(ItemIsMovable | ItemIsSelectable);
}

template<typename Info>
void AnnotatedRegion<Info>::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
    QPointF pos = event->pos();
    qreal from_right = pos.x() - rect().right();
    qreal from_left = pos.x() - rect().left();
    qreal from_top = pos.y() - rect().top();
    qreal from_bottom = pos.y() - rect().bottom();

    qreal margin = 5;
    if (std::abs(from_top) < margin) {
        if (std::abs(from_left) < margin) {
            drag_ = DRAG_TOP_LEFT;
            setCursor(QCursor(Qt::SizeFDiagCursor));
        } else if (std::abs(from_right) < margin) {
            drag_ = DRAG_TOP_RIGHT;
            setCursor(QCursor(Qt::SizeBDiagCursor));
        } else {
            drag_ = DRAG_TOP;
            setCursor(QCursor(Qt::SizeVerCursor));
        }
    } else if (std::abs(from_bottom) < margin) {
        if (std::abs(from_left) < margin) {
            drag_ = DRAG_BOTTOM_LEFT;
            setCursor(QCursor(Qt::SizeBDiagCursor));
        } else if (std::abs(from_right) < margin) {
            drag_ = DRAG_BOTTOM_RIGHT;
            setCursor(QCursor(Qt::SizeFDiagCursor));
        } else {
            drag_ = DRAG_BOTTOM;
            setCursor(QCursor(Qt::SizeVerCursor));
        }
    } else if (std::abs(from_left) < margin) {
        drag_ = DRAG_LEFT;
        setCursor(QCursor(Qt::SizeHorCursor));
    } else if (std::abs(from_right) < margin) {
        drag_ = DRAG_RIGHT;
        setCursor(QCursor(Qt::SizeHorCursor));
    } else {
        drag_ = DRAG_NONE;
        setCursor(QCursor());
    }

    if (drag_ != DRAG_NONE) {
        setFlag(ItemIsMovable, false);
    } else {
        setFlag(ItemIsMovable, true);
    }

    QGraphicsItem::hoverMoveEvent(event);
}

template<typename Info>
void AnnotatedRegion<Info>::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    setCursor(QCursor());
    QGraphicsItem::hoverLeaveEvent( event );
}

template<typename Info>
void AnnotatedRegion<Info>::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    QRectF area = rect();
    QPointF pos = event->pos();
    qreal from_left = pos.x() - rect().left();
    qreal from_top = pos.y() - rect().top();
    qreal margin = 5;
    switch (drag_) {
    case DRAG_TOP: {
        qreal new_height = area.height() - from_top;
        if (new_height > 2*margin) {
            annotation_->area.y = sceneBoundingRect().y() + 2;
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
            annotation_->area.x = sceneBoundingRect().x() + 2;
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
    case DRAG_TOP_LEFT: {
        qreal new_width = area.width() - from_left;
        if (new_width > 2*margin) {
            annotation_->area.x = sceneBoundingRect().x() + 2;
            area.setWidth(area.width() - from_left);
            area.translate(from_left, 0);
        }
        qreal new_height = area.height() - from_top;
        if (new_height > 2*margin) {
            annotation_->area.y = sceneBoundingRect().y() + 2;
            area.setHeight(area.height() - from_top);
            area.translate(0, from_top);

        }
        break;
    }
    case DRAG_TOP_RIGHT: {
        qreal new_height = area.height() - from_top;
        if (new_height > 2*margin) {
            annotation_->area.y = sceneBoundingRect().y() + 2;
            area.setHeight(area.height() - from_top);
            area.translate(0, from_top);

        }
        qreal new_width = pos.x() - area.left();
        if (new_width > 2*margin) {
            area.setWidth(new_width);
        }
        break;
    }
    case DRAG_BOTTOM_LEFT: {
        qreal new_height = pos.y() - area.top();
        if (new_height > 2*margin) {
            area.setHeight(new_height);
        }
        qreal new_width = area.width() - from_left;
        if (new_width > 2*margin) {
            annotation_->area.x = sceneBoundingRect().x() + 2;
            area.setWidth(area.width() - from_left);
            area.translate(from_left, 0);
        }
        break;
    }
    case DRAG_BOTTOM_RIGHT: {
        qreal new_height = pos.y() - area.top();
        if (new_height > 2*margin) {
            area.setHeight(new_height);
        }
        qreal new_width = pos.x() - area.left();
        if (new_width > 2*margin) {
            area.setWidth(new_width);
        }
        break;
    }
    default:
        QGraphicsRectItem::mouseMoveEvent(event);
        auto pos = this->pos();
        annotation_->area.x = area.left() + pos.x();
        annotation_->area.y = area.top() + pos.y();
        setRect(area);
        return;
    }
    setRect(area);
    annotation_->area.w = area.width();
    annotation_->area.h = area.height();
    prepareGeometryChange();
    update();
}

template<typename Info>
void AnnotatedRegion<Info>::paint(QPainter *painter, 
  const QStyleOptionGraphicsItem *option, QWidget *widget) {
  painter->setFont(QFont("Helvetica", 16));
  // draw main rectangle
  painter->setPen(Qt::DashLine);
  painter->setPen(QPen(QColor(255,0,0)));
  painter->drawRect(rect());
  // draw UID
  QString text("0000");
  QFontMetrics fm = painter->fontMetrics();
  int width = fm.width(text);
  QBrush brush;
  brush.setColor(Qt::gray);
  QRectF text_area = QRectF(
    rect().right() - width, 
    rect().bottom() - fm.height(), 
    width, 
    fm.height()
    );
  painter->fillRect(text_area, QBrush(QColor(64, 64, 64, 64)));
  painter->setPen(QPen(QColor(255, 0, 0)));
  painter->drawText(text_area, QString::number(uid_));
}

} // namespace fish_detector

#endif // ANNOTATEDREGION_H

