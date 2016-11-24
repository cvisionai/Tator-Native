#include "fish_detector/common/species_dialog.h"
#include "ui_species_dialog.h"

namespace fish_detector {

SpeciesDialog::SpeciesDialog(QWidget *parent)
  : QDialog(parent) 
  , ui_(new Ui::SpeciesDialog) {
  ui_->setupUi(this);
}

void SpeciesDialog::on_ok_clicked() {
}

void SpeciesDialog::on_cancel_clicked() {
}

void SpeciesDialog::on_removeSubspecies_clicked() {
}

void SpeciesDialog::on_addSubspecies_clicked() {
}

Species SpeciesDialog::getSpecies() {
  Species species;
  return species;
}

#include "../../include/fish_detector/common/moc_species_dialog.cpp"

} // namespace fish_detector
