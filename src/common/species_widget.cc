#include "fish_detector/common/species_widget.h"
#include "ui_species_widget.h"

namespace fish_detector {

SpeciesWidget::SpeciesWidget(const Species &species, QWidget *parent)
  : QWidget(parent)
  , ui_(new Ui::SpeciesWidget)
  , subspecies_menu_(new QMenu(this))
  , species_(species) {
  ui_->setupUi(this);
  ui_->count->setText("0");
  ui_->addIndividualSubspecies->setMenu(subspecies_menu_.get());
  setSpecies(species);
}

void SpeciesWidget::setSpecies(const Species &species) {
  species_ = species;
  ui_->name->setText(species.getName().c_str());
  subspecies_menu_->clear();
  for(const auto &subspecies : species.getSubspecies()) {
    QAction *action = new QAction(subspecies.c_str());
    QObject::connect(action, SIGNAL(triggered()), 
        this, SLOT(onSubspeciesClicked()));
    subspecies_menu_->addAction(action);
  }
}

void SpeciesWidget::on_addIndividual_clicked() {
}

void SpeciesWidget::onSubspeciesClicked() {
}

#include "../../include/fish_detector/common/moc_species_widget.cpp"

} // namespace fish_detector
