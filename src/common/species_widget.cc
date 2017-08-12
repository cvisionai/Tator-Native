#include <QColorDialog>

#include "species_widget.h"
#include "ui_species_widget.h"

namespace fish_annotator {

namespace {
  static const std::vector<QColor> default_colors = {
    QColor(0x2F, 0x15, 0x11),
    QColor(0x52, 0x2D, 0x3A),
    QColor(0x5B, 0x52, 0x6D),
    QColor(0x43, 0x7E, 0x92),
    QColor(0x3B, 0xA9, 0x94),
    QColor(0x86, 0xCC, 0x79),
    QColor(0xF1, 0xE2, 0x64)
  };
  static uint32_t color_index = 0;
} // namespace

SpeciesWidget::SpeciesWidget(const Species &species, QWidget *parent)
  : QWidget(parent)
  , ui_(new Ui::SpeciesWidget)
  , subspecies_menu_(new QMenu(this))
  , species_(species)
  , color_(default_colors[color_index]) {
  ui_->setupUi(this);
  ui_->count->setText("0");
  ui_->addIndividualSubspecies->setMenu(subspecies_menu_.get());
  setSpecies(species);
  ++color_index;
  color_index < default_colors.size() ? color_index : 0;
  updateButtonColor();
}

void SpeciesWidget::setSpecies(const Species &species) {
  species_ = species;
  ui_->name->setText(species.getName().c_str());
  subspecies_menu_->clear();
  for(const auto &subspecies : species.getSubspecies()) {
    QAction *action = new QAction(subspecies.c_str(), this);
    QObject::connect(action, SIGNAL(triggered()), 
        this, SLOT(onSubspeciesClicked()));
    subspecies_menu_->addAction(action);
  }
}

void SpeciesWidget::on_addIndividual_clicked() {
  emit individualAdded(species_.getName(), "");
}

void SpeciesWidget::on_changeColor_clicked() {
  auto dlg = new QColorDialog(this);
  dlg->setModal(true);
  dlg->setCurrentColor(color_);
  if(dlg->exec()) {
    color_ = dlg->selectedColor();
    updateButtonColor();
  }
}

void SpeciesWidget::onSubspeciesClicked() {
  QAction *action = qobject_cast<QAction*>(QObject::sender());
  emit individualAdded(species_.getName(), action->text().toStdString());
}

void SpeciesWidget::setCount(uint64_t count) {
  ui_->count->setText(std::to_string(count).c_str());
}

QColor SpeciesWidget::getColor() {
  return color_;
}

void SpeciesWidget::updateButtonColor() {
  QString qss = QString("background-color:%1").arg(color_.name());
  ui_->changeColor->setStyleSheet(qss);
  emit colorChanged();
}

#include "moc_species_widget.cpp"

} // namespace fish_annotator
