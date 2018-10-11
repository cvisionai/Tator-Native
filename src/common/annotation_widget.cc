#include "annotation_widget.h"
#include "ui_annotation_widget.h"

namespace tator {

AnnotationWidget::AnnotationWidget(QWidget *parent)
  : QWidget(parent)
  , ui_(new Ui::AnnotationWidget) {
  ui_->setupUi(this);
  ui_->makeBox->setIcon(
      QIcon(":/icons/annotation_tools/box_tool.svg"));
  ui_->makeLine->setIcon(
      QIcon(":/icons/annotation_tools/line_tool.svg"));
  ui_->makeDot->setIcon(
      QIcon(":/icons/annotation_tools/dot_tool.svg"));
}

void AnnotationWidget::on_makeBox_toggled(bool checked) {
  bool line_checked = ui_->makeLine->isChecked();
  bool dot_checked = ui_->makeDot->isChecked();
  if(line_checked == false && dot_checked == false) {
    ui_->makeBox->setChecked(true);
  }
  else {
    ui_->makeLine->setChecked(false);
    ui_->makeDot->setChecked(false);
  }
  emit setAnnotation(kBox);
}

void AnnotationWidget::on_makeLine_toggled(bool checked) {
  bool box_checked = ui_->makeBox->isChecked();
  bool dot_checked = ui_->makeDot->isChecked();
  if(box_checked == false && dot_checked == false) {
    ui_->makeLine->setChecked(true);
  }
  else {
    ui_->makeBox->setChecked(false);
    ui_->makeDot->setChecked(false);
  }
  emit setAnnotation(kLine);
}

void AnnotationWidget::on_makeDot_toggled(bool checked) {
  bool line_checked = ui_->makeLine->isChecked();
  bool box_checked = ui_->makeBox->isChecked();
  if(line_checked == false && box_checked == false) {
    ui_->makeDot->setChecked(true);
  }
  else {
    ui_->makeLine->setChecked(false);
    ui_->makeBox->setChecked(false);
  }
  emit setAnnotation(kDot);
}

#include "moc_annotation_widget.cpp"

} // namespace tator

