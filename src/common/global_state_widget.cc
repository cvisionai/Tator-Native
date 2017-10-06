#include <fstream>

#include "global_state_widget.h"
#include "ui_global_state_widget.h"

namespace fish_annotator {

GlobalStateWidget::GlobalStateWidget(QWidget *parent)
  : QWidget(parent)
  , ui_(new Ui::GlobalStateWidget)
  , states_(nullptr) {
  ui_->setupUi(this);
}

void GlobalStateWidget::setStates(
  std::shared_ptr<GlobalStateAnnotation> states) {
  QLayoutItem* child;
  while(ui_->stateCheckboxes->count() != 0) {
    child = ui_->stateCheckboxes->takeAt(0);
    if(child->widget() != 0) {
        delete child->widget();
    }
    delete child;
  }
  for(auto state : states->states_) {
    auto *chkbox = new QCheckBox(state.first.c_str(), this);
    chkbox->setChecked(state.second);
    QObject::connect(chkbox, &QCheckBox::stateChanged, this,
       &GlobalStateWidget::updateGlobalState);
    ui_->stateCheckboxes->addWidget(chkbox);
  }
  states_ = states;
}

void GlobalStateWidget::updateGlobalState(int checked) {
  QCheckBox *sender = qobject_cast<QCheckBox *>(QObject::sender());
  std::string name = sender->text().toStdString();
  if(checked == Qt::Unchecked) {
    states_->states_[name] = false;
  }
  else if (checked == Qt::Checked) {
    states_->states_[name] = true;
  }
}

} // namespace fish_annotator
