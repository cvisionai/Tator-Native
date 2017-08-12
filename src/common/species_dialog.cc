#include "species_dialog.h"
#include "ui_species_dialog.h"

namespace fish_annotator {

SpeciesDialog::SpeciesDialog(QWidget *parent)
  : QDialog(parent) 
  , ui_(new Ui::SpeciesDialog) {
  ui_->setupUi(this);
}

void SpeciesDialog::on_ok_clicked() {
  accept();
}

void SpeciesDialog::on_cancel_clicked() {
  reject();
}

void SpeciesDialog::on_removeSubspecies_clicked() {
  QListWidgetItem *current = ui_->subspeciesList->currentItem();
  if(current != nullptr) {
    delete current;
  }
}

void SpeciesDialog::on_addSubspecies_clicked() {
  QListWidgetItem *item = new QListWidgetItem("New subspecies");
  item->setFlags(item->flags() | Qt::ItemIsEditable);
  ui_->subspeciesList->addItem(item);
  ui_->subspeciesList->editItem(item);
}

Species SpeciesDialog::getSpecies() {
  Species species(ui_->species->text().toStdString());
  for(int row = 0; row < ui_->subspeciesList->count(); ++row) {
    QListWidgetItem *item = ui_->subspeciesList->item(row);
    species.getSubspecies().push_back(item->text().toStdString());
  }
  return species;
}

void SpeciesDialog::setSpecies(const Species &species) {
  ui_->species->setText(species.getName().c_str());
  for(auto &subspecies : species.getSubspecies()) {
    QListWidgetItem *item = new QListWidgetItem(subspecies.c_str());
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    ui_->subspeciesList->addItem(item);
  }
}

#include "moc_species_dialog.cpp"

} // namespace fish_annotator
