#include <QGraphicsView>

#include "fish_annotator/common/annotation_scene.h"

namespace fish_annotator {

AnnotationScene::AnnotationScene(QObject *parent)
  : QGraphicsScene(parent)
  , mode_(kDoNothing)
  , type_(kBox)
  , start_pos_()
  , rect_item_(nullptr)
  , line_item_(nullptr)
  , dot_item_(nullptr) {
}

void AnnotationScene::setToolWidget(AnnotationWidget *widget) {
  QObject::connect(widget, &AnnotationWidget::setAnnotation,
      this, &AnnotationScene::setAnnotationType);
}

void AnnotationScene::setMode(Mode mode) {
  mode_ = mode;
  QGraphicsView::DragMode drag;
  if(mode == kDraw) {
    makeItemsControllable(false);
    drag = QGraphicsView::NoDrag;
  }
  else if(mode == kSelect) {
    makeItemsControllable(true);
    drag = QGraphicsView::RubberBandDrag;
  }
  auto view = views().at(0);
  if(view != nullptr) view->setDragMode(drag);
}

void AnnotationScene::setAnnotationType(AnnotationType type) {
  type_ = type;
}

void AnnotationScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  if(mode_ == kDraw) {
    start_pos_ = event->scenePos();
    switch(type_) {
      case kBox:
        rect_item_ = new QGraphicsRectItem(
              start_pos_.x(), start_pos_.y(), 0, 0);
        rect_item_->setPen(QPen(Qt::black, 7, Qt::SolidLine));
        addItem(rect_item_);
        break;
      case kLine:
        line_item_ = new QGraphicsLineItem(QLineF(
              start_pos_, start_pos_));
        line_item_->setPen(QPen(Qt::black, 7, Qt::SolidLine));
        addItem(line_item_);
        break;
      case kDot:
        dot_item_ = new QGraphicsEllipseItem(
              start_pos_.x(), start_pos_.y(), 0, 0);
        dot_item_->setPen(QPen(Qt::black, 7, Qt::SolidLine));
        addItem(dot_item_);
        break;
    }
  }
  QGraphicsScene::mousePressEvent(event);
}

void AnnotationScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  if(mode_ == kDraw) {
    auto update_pos = event->scenePos();
    switch(type_) {
      case kBox:
        if(rect_item_ != nullptr) {
          auto left = qMin(start_pos_.x(), update_pos.x());
          auto top = qMin(start_pos_.y(), update_pos.y());
          auto width = qAbs(start_pos_.x() - update_pos.x());
          auto height = qAbs(start_pos_.y() - update_pos.y());
          rect_item_->setRect(QRectF(left, top, width, height));
        }
        break;
      case kLine:
        if(line_item_ != nullptr) {
          line_item_->setLine(QLineF(start_pos_, update_pos));
        }
        break;
      case kDot:
        if(dot_item_ != nullptr) {
          dot_item_->setRect(QRectF(update_pos, update_pos));
        }
        break;
    }
  }
}

void AnnotationScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  if(mode_ == kDraw) {
    switch(type_) {
      case kBox:
        if(rect_item_ != nullptr) {
          emit boxFinished(rect_item_->rect());
        }
        break;
      case kLine:
        if(line_item_ != nullptr) {
          emit lineFinished(line_item_->line());
        }
        break;
      case kDot:
        if(dot_item_ != nullptr) {
          emit dotFinished(dot_item_->rect().topLeft());
        }
        break;
    }
  }
}

} // namespace fish_annotator
