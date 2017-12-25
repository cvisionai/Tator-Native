#include <fstream>

#include <QGroupBox>

#include "string_state_widget.h"
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
  std::map<std::string, QGroupBox*> group_map;
  for(auto header : states->headers_) {
    if(group_map.find(header.second) == group_map.end()) {
      group_map[header.second] = 
        new QGroupBox(header.second.c_str(), this);
      group_map[header.second]->setLayout(new QVBoxLayout);
      ui_->stateCheckboxes->addWidget(group_map[header.second]);
    }
  }
  for(auto state : states->states_) {
    std::string type_str = boost::apply_visitor(
      GetTypeVisitor(),
      state.second);
    if(type_str == "bool") {
      auto *chkbox = new QCheckBox(state.first.c_str(), this);
      chkbox->setChecked(boost::get<bool>(state.second));
      QObject::connect(chkbox, &QCheckBox::stateChanged, this,
         &GlobalStateWidget::updateBoolState);
      group_map[states->headers_[state.first]]->layout()->addWidget(chkbox);
    }
    else if(type_str == "string") {
      auto *widget = new StringStateWidget(
          state.first.c_str(),
          boost::get<std::string>(state.second).c_str());
      QObject::connect(widget, &StringStateWidget::valueChanged, this,
          &GlobalStateWidget::updateStringState);
      group_map[states->headers_[state.first]]->layout()->addWidget(widget);
    }
  }
  states_ = states;
}

void GlobalStateWidget::updateBoolState(int checked) {
  QCheckBox *sender = qobject_cast<QCheckBox*>(QObject::sender());
  std::string name = sender->text().toStdString();
  if(checked == Qt::Unchecked) {
    states_->states_[name] = false;
  }
  else if (checked == Qt::Checked) {
    states_->states_[name] = true;
  }
  emit stateChanged();
}

void GlobalStateWidget::updateStringState(
  const QPair<QString, QString> &name_and_value) {
  std::string name = name_and_value.first.toStdString();
  std::string value = name_and_value.second.toStdString();
  states_->states_[name] = value;
  emit stateChanged();
}

} // namespace fish_annotator
