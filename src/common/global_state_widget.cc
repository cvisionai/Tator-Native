#include "global_state_widget.h"
#include "ui_global_state_widget.h"

namespace fish_annotator {

GlobalStateWidget::GlobalStateWidget(QWidget *parent)
  : QWidget(parent)
  , ui_(new Ui::GlobalStateWidget)
  , states_(nullptr) {
  ui_->setupUi(this);
}

void setStates(std::shared_ptr<GlobalStateAnnotation> states) {
  QLayoutItem* child;
  int i = 0;
  for(auto state : states.states_) {
    child = ui_->stateCheckboxes->takeAt(i);
    // @TODO just delete everything instead of this and remake it
    if(child == 0) {
      ui_->stateCheckboxes->addWidget(
          new GlobalStateCheckBox(state.first.c_str(), this));
      child = ui_->stateCheckboxes->takeAt(i);
      // @TODO connect stateChanged signal to an internal slot
    }
    child->widget()->setChecked(state.second);
    ++i;
  }
}

// @TODO add internal slot that updates states_ whenever checkbox changed.

} // namespace fish_annotator
