#include "global_state_widget.h"
#include "ui_global_state_widget.h"

namespace fish_annotator {

GlobalStateWidget::GlobalStateWidget(QWidget *parent) :
    QWidget(parent),
    ui_(new Ui::GlobalStateWidget)
{
    ui_->setupUi(this);
}

void GlobalStateWidget::add_checkbox(QCheckBox *checkbox)
{
  ui_->entry_layout->addWidget(checkbox);
}

} // namespace fish_annotator
