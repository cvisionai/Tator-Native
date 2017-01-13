#include "fish_annotator/common/species_widget.h"
#include "ui_species_widget.h"

namespace fish_annotator {

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
    QAction *action = new QAction(subspecies.c_str(), this);
    QObject::connect(action, SIGNAL(triggered()), 
        this, SLOT(onSubspeciesClicked()));
    subspecies_menu_->addAction(action);
  }
}

void SpeciesWidget::on_addIndividual_clicked() {
  emit individualAdded(species_.getName(), "");
}

void SpeciesWidget::onSubspeciesClicked() {
  QAction *action = qobject_cast<QAction*>(QObject::sender());
  emit individualAdded(species_.getName(), action->text().toStdString());
}

void SpeciesWidget::setCount(uint64_t count) {
  ui_->count->setText(std::to_string(count).c_str());
}

#include "../../include/fish_annotator/common/moc_species_widget.cpp"

} // namespace fish_annotator
