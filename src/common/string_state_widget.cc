#include "string_state_widget.h"
#include "ui_string_state_widget.h"

namespace fish_annotator {

StringStateWidget::StringStateWidget(
  const QString &name,
  const QString &value,
  QWidget *parent)
  : QWidget(parent)
  , ui_(new Ui::StringStateWidget) {
  ui_->setupUi(this);
}

void StringStateWidget::on_value_editingFinished() {
  emit valueChanged(qMakePair(
    ui_->label->text(), 
    ui_->value->text()));
}

} // namespace fish_annotator
