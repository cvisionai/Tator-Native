#include <QGraphicsView>

#include "annotation_scene.h"

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
    QApplication::setOverrideCursor(QCursor(Qt::CrossCursor));
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

namespace {
// Define pen width in anonymous namespace.
static const int pen_width = 7;
}

void AnnotationScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  if(mode_ == kDraw && sceneRect().contains(event->scenePos()) == true) {
    start_pos_ = event->scenePos();
    switch(type_) {
      case kBox:
        rect_item_ = new QGraphicsRectItem(
              start_pos_.x(), start_pos_.y(), 0, 0);
        rect_item_->setPen(QPen(Qt::black, pen_width, Qt::SolidLine));
        addItem(rect_item_);
        break;
      case kLine:
        line_item_ = new QGraphicsLineItem(QLineF(
              start_pos_, start_pos_));
        line_item_->setPen(QPen(Qt::black, pen_width, Qt::SolidLine));
        addItem(line_item_);
        break;
      case kDot:
        dot_item_ = new QGraphicsEllipseItem(
              start_pos_.x() - pen_width, 
              start_pos_.y() - pen_width, 
              2 * pen_width, 2 * pen_width);
        dot_item_->setPen(QPen(Qt::black, pen_width, Qt::SolidLine));
        addItem(dot_item_);
        break;
    }
  }
  QGraphicsScene::mousePressEvent(event);
}

void AnnotationScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  if(mode_ == kDraw) {
    auto update_pos = event->scenePos();
    qreal margin;
    switch(type_) {
      case kBox: 
        margin = pen_width / 2.0; 
        break;
      case kLine:
        margin = pen_width; 
        break;
      case kDot:
        margin = 2.0 * pen_width; 
        break;
    }
    update_pos.setX(qMin(update_pos.x(), sceneRect().right() - margin));
    update_pos.setX(qMax(update_pos.x(), sceneRect().left() + margin));
    update_pos.setY(qMin(update_pos.y(), sceneRect().bottom() - margin));
    update_pos.setY(qMax(update_pos.y(), sceneRect().top() + margin));
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
          dot_item_->setRect(QRectF(
                update_pos.x() - pen_width,
                update_pos.y() - pen_width,
                2 * pen_width, 2 * pen_width));
        }
        break;
    }
  }
  QGraphicsScene::mouseMoveEvent(event);
}

void AnnotationScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  if(mode_ == kDraw) {
    switch(type_) {
      case kBox:
        if(rect_item_ != nullptr) {
          emit boxFinished(rect_item_->rect());
          removeItem(rect_item_);
          rect_item_ = nullptr;
        }
        break;
      case kLine:
        if(line_item_ != nullptr) {
          emit lineFinished(line_item_->line());
          removeItem(line_item_);
          line_item_ = nullptr;
        }
        break;
      case kDot:
        if(dot_item_ != nullptr) {
          emit dotFinished(QPointF(
              dot_item_->rect().topLeft().x() + pen_width,
              dot_item_->rect().topLeft().y() + pen_width));
          removeItem(dot_item_);
          dot_item_ = nullptr;
        }
        break;
    }
  }
  QGraphicsScene::mouseReleaseEvent(event);
}

void AnnotationScene::keyPressEvent(QKeyEvent *event) {
  if(mode_ == kDraw && event->key() == Qt::Key_Escape) {
    mode_ = kSelect;
    QApplication::restoreOverrideCursor();
  }
  QGraphicsScene::keyPressEvent(event);
}

void AnnotationScene::makeItemsControllable(bool controllable) {
  foreach(QGraphicsItem *item, items()) {
    item->setFlag(QGraphicsItem::ItemIsSelectable, controllable);
    item->setFlag(QGraphicsItem::ItemIsMovable, controllable);
  }
}

#include "moc_annotation_scene.cpp"

} // namespace fish_annotator
