#include "fish_detector/common/species_widget.h"
#include "ui_species_widget.h"

namespace fish_detector {

SpeciesWidget::SpeciesWidget(const Species &species, QWidget *parent)
  : QWidget(parent)
  , ui_(new Ui::SpeciesWidget)
  , species_(species) {
  ui_->setupUi(this);
  ui_->name->setText(species.getName().c_str());
  ui_->count->setText("0");
}

void SpeciesWidget::on_addIndividual_clicked() {
}

void SpeciesWidget::onSubspeciesClicked() {
}

#include "../../include/fish_detector/common/moc_species_widget.cpp"

} // namespace fish_detector
