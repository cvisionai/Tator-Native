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

void GlobalStateWidget::loadGlobalFile(const QString &in_file)
{
  std::ifstream inputFile(in_file.toStdString(),std::ifstream::in);
  std::vector<std::string> fileList;
  std::string line;
  while(inputFile >> line)
  {
    add_checkbox(new QCheckBox(QString::fromStdString(line),this));
    //global_states_.push_back(std::move(std::unique_ptr<QCheckBox>(
    //  new QCheckBox(line))));
  }
  global_states_ = ui_->entry_layout->findChildren<QCheckBox *>();
  //inputFile.close();
}

} // namespace fish_annotator
