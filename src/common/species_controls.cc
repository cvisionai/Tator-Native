#include <QMessageBox>
#include <QFileDialog>

#include "fish_detector/common/species_dialog.h"
#include "fish_detector/common/species_controls.h"
#include "ui_species_controls.h"

namespace fish_detector {

SpeciesControls::SpeciesControls(QWidget *parent)
  : ui_(new Ui::SpeciesControls)
  , species_widgets_()
  , edit_species_menu_(new QMenu(this))
  , clear_species_menu_(new QMenu(this)) {
  ui_->setupUi(this);
  ui_->clearSpecies->setMenu(clear_species_menu_.get());
  ui_->editSpecies->setMenu(edit_species_menu_.get());
  clearAllSpeciesWidgets();
}

void SpeciesControls::insertSpeciesWidget(const Species &species) {
  bool widget_exists = false;
  for(auto &widget : species_widgets_) {
    if(widget->getSpecies().getName() == species.getName()) {
      widget_exists = true;
      break;
    }
  }
  if(!widget_exists) {
    species_widgets_.push_back(std::move(std::unique_ptr<SpeciesWidget>(
            new SpeciesWidget(species, this))));
    ui_->speciesLayout->insertWidget(
        static_cast<int>(species_widgets_.size()) - 1, 
        species_widgets_.back().get());
    QAction *edit = edit_species_menu_->addAction(species.getName().c_str());
    QObject::connect(edit, SIGNAL(triggered()), 
        this, SLOT(editSpeciesWidget()));
    QAction *clear = clear_species_menu_->addAction(species.getName().c_str());
    QObject::connect(clear, SIGNAL(triggered()), 
        this, SLOT(clearSpeciesWidget()));
    QObject::connect(
        species_widgets_.back().get(), 
        SIGNAL(individualAdded(std::string, std::string)),
        this, SIGNAL(individualAdded(std::string, std::string)));
  }
}

void SpeciesControls::on_addSpecies_clicked() {
  SpeciesDialog *dlg = new SpeciesDialog(this);
  if(dlg->exec()) {
    Species species = dlg->getSpecies();
    if(!species.getName().empty()) {
      insertSpeciesWidget(species);
    }
  }
  delete dlg;
}

void SpeciesControls::on_loadSpecies_clicked() {
  QString in_file = QFileDialog::getOpenFileName(this,
      "Specify input species file", QString(), "JSON (*.json)");
  if(!in_file.isEmpty()) {
    loadSpeciesFile(in_file);
  }
}

void SpeciesControls::loadSpeciesFile(const QString &in_file) {
  SpeciesList list;
  if(deserialize(list, in_file.toStdString())) {
    for(auto &species : list.getSpecies()) {
      insertSpeciesWidget(species);
    }
  }
  else {
    QMessageBox err;
    err.critical(0, "Error", std::string(
          std::string("Could not open file ")
        + in_file.toStdString()
        + std::string(".")).c_str());
  }
}

void SpeciesControls::on_saveSpecies_clicked() {
  QString out_file = QFileDialog::getSaveFileName(this, 
      "Specify output species file", QString(), "JSON (*.json)");
  if(!out_file.isEmpty()) {
    SpeciesList list;
    for(auto &widget : species_widgets_) {
      list.getSpecies().push_back(widget->getSpecies());
    }
    if(!serialize(list, out_file.toStdString())) {
      QMessageBox err;
      err.critical(0, "Error", std::string(
            std::string("Could not save file ")
          + out_file.toStdString()
          + std::string(".")).c_str());
    }
  }
}

void SpeciesControls::onClearAllSpeciesWidgetsTriggered() {
  QMessageBox::StandardButton reply = QMessageBox::question(
      this, "Clear Species", "Are you sure you want to clear all species?",
      QMessageBox::Yes | QMessageBox::No);
  if(reply == QMessageBox::Yes) {
    clearAllSpeciesWidgets();
  }
}

void SpeciesControls::clearAllSpeciesWidgets() {
  clear_species_menu_->clear();
  edit_species_menu_->clear();
  species_widgets_.clear();
  QAction *all = clear_species_menu_->addAction("All");
  QObject::connect(all, SIGNAL(triggered()), 
      this, SLOT(onClearAllSpeciesWidgetsTriggered()));
  clear_species_menu_->addSeparator();
}

void SpeciesControls::clearSpeciesWidget() {
  QAction *action = qobject_cast<QAction*>(QObject::sender());
  for(auto edit : edit_species_menu_->actions()) {
    if(edit->text() == action->text()) {
      edit_species_menu_->removeAction(edit);
      break;
    }
  }
  auto widget = species_widgets_.begin();
  for(; widget != species_widgets_.end(); ++widget) {
    if((*widget)->getSpecies().getName() == action->text().toStdString()) {
      species_widgets_.erase(widget);
      break;
    }
  }
  clear_species_menu_->removeAction(action);
}

void SpeciesControls::editSpeciesWidget() {
  QAction *action = qobject_cast<QAction*>(QObject::sender());
  SpeciesDialog *dlg = new SpeciesDialog(this);
  auto widget = species_widgets_.begin();
  for(; widget != species_widgets_.end(); ++widget) {
    if((*widget)->getSpecies().getName() == action->text().toStdString()) {
      dlg->setSpecies((*widget)->getSpecies());
      break;
    }
  }
  if(dlg->exec()) {
    (*widget)->setSpecies(dlg->getSpecies());
  }
}

#include "../../include/fish_detector/common/moc_species_controls.cpp"

} // namespace fish_detector

