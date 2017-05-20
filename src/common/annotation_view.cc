#include "fish_annotator/common/annotation_view.h"

namespace fish_annotator {

AnnotationView::AnnotationView(QWidget *parent)
  : QGraphicsView(parent)
  , bounding_rect_() 
  , zoom_(0) {
}

void AnnotationView::fitInView() {
  if(bounding_rect_.isNull() == false) {
    auto unity = transform().mapRect(QRectF(0, 0, 1, 1));
    scale(1.0 / unity.width(), 1.0 / unity.height());
    auto view_rect = viewport()->rect();
    auto scene_rect = transform().mapRect(bounding_rect_);
    auto factor_w = view_rect.width() / scene_rect.width();
    auto factor_h = view_rect.height() / scene_rect.height();
    auto factor = factor_w < factor_h ? factor_w : factor_h;
    scale(factor, factor);
    centerOn(bounding_rect_.center());
    zoom_ = 0;
  }
}

void AnnotationView::wheelEvent(QWheelEvent *event) {
  if(bounding_rect_.isNull() == false) {
    double factor;
    if(event->delta() > 0) {
      factor = 1.25;
      zoom_ += 1;
    }
    else {
      factor = 0.8;
      zoom_ -= 1;
    }
    if(zoom_ > 0) {
      scale(factor, factor);
    }
    else if(zoom_ == 0) {
      fitInView();
    }
    else {
      zoom_ = 0;
    }
  }
}

void AnnotationView::setBoundingRect(const QRectF &rect) {
  bounding_rect_ = rect;
}

#include "../../include/fish_annotator/common/moc_annotation_view.cpp"

} // namespace fish_annotator

