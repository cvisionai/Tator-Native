#include <fstream>

#include "global_state_widget.h"
#include "ui_global_state_widget.h"
std::ofstream debug1("DEBUG1.txt");
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
    debug1 << "STATE: " << state.first << std::endl;
    debug1 << "VALUE: " << state.second << std::endl;
    chkbox->setChecked(state.second);
    QObject::connect(chkbox, &QCheckBox::stateChanged, this,
       &GlobalStateWidget::updateGlobalState);
    ui_->stateCheckboxes->addWidget(chkbox);
  }
  states_ = states;
}

void GlobalStateWidget::updateGlobalState(int checked) {
  debug1 << "GOT TO UPDATEGLOBALSTATE" << std::endl;
  QCheckBox *sender = qobject_cast<QCheckBox *>(QObject::sender());
  debug1 << "GOT HERE1" << std::endl;
  std::string name = sender->text().toStdString();
  debug1 << "GOT HERE2" << std::endl;
  if(checked == Qt::Unchecked) {
    states_->states_[name] = false;
  debug1 << "GOT HERE3" << std::endl;
  }
  else if (checked == Qt::Checked) {
    states_->states_[name] = true;
  debug1 << "GOT 4" << std::endl;
  }
}

} // namespace fish_annotator
