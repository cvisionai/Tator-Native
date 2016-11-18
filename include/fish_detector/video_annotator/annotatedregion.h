#ifndef ANNOTATEDREGION_H
#define ANNOTATEDREGION_H

#include <QGraphicsRectItem>
#include <QGraphicsView>

#include "fish_detector/video_annotator/document.h"

namespace fish_detector { namespace video_annotator {

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

class AnnotatedRegion: public QGraphicsRectItem
{
protected:
    /// @brief Pointer to the annotation location.
    std::shared_ptr<AnnotationLocation> annotation_;

    /// @brief ID associated with this object.
    std::uint64_t uid_;

    /// @brief Type of move event.
    Drag drag_;
public:

    /// @brief Constructor.
    AnnotatedRegion(std::uint64_t uid, 
                    std::shared_ptr<AnnotationLocation> annotation, 
                    QRectF area);

    /// @brief Executed 
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    void updateFrame(std::uint64_t frame);
    std::shared_ptr<AnnotationLocation> getAnnotation() {return annotation_;}
    std::uint64_t getUID() {return uid_;}
};

}} // namespace fish_detector::video_annotator

#endif // ANNOTATEDREGION_H

