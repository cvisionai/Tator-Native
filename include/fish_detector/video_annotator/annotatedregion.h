#ifndef ANNOTATEDREGION_H
#define ANNOTATEDREGION_H

#include <QGraphicsRectItem>
#include <QGraphicsView>

#include "fish_detector/video_annotator/document.h"

namespace fish_detector { namespace video_annotator {

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
    std::shared_ptr<AnnotationLocation> annotation;
    std::uint64_t uid;
    drag_t drag;
public:
    AnnotatedRegion(std::uint64_t uid, std::shared_ptr<AnnotationLocation> annotation, QRectF area);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    void updateFrame(std::uint64_t frame);
    std::shared_ptr<AnnotationLocation> getAnnotation() {return annotation;}
    std::uint64_t getUID() {return uid;}
};

}} // namespace fish_detector::video_annotator

#endif // ANNOTATEDREGION_H

