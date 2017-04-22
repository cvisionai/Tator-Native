#include "fish_annotator/common/annotation_widget.h"
#include "ui_annotation_widget.h"

namespace fish_annotator {

AnnotationWidget::AnnotationWidget(QWidget *parent)
  : QWidget(parent)
  , ui_(new Ui::AnnotationWidget) {
  ui_->setupUi(this);
  ui_->makeBox->setIcon(
      QIcon(":/icons/annotation_tools/box_tool.svg"));
  ui_->makeBox->setIcon(
      QIcon(":/icons/annotation_tools/line_tool.svg"));
  ui_->makeBox->setIcon(
      QIcon(":/icons/annotation_tools/dot_tool.svg"));
}

void AnnotationWidget::on_makeBox_clicked() {
  emit makeAnnotation(kBox);
}

void AnnotationWidget::on_makeLine_clicked() {
  emit makeAnnotation(kLine);
}

void AnnotationWidget::on_makeDot_clicked() {
  emit makeAnnotation(kDot);
}

} // namespace fish_annotator

