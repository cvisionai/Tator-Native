#include <QMessageBox>

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

void SpeciesControls::on_addSpecies_clicked() {
  SpeciesDialog *dlg = new SpeciesDialog(this);
  if(dlg->exec()) {
    Species species = dlg->getSpecies();
    if(!species.getName().empty()) {
      species_widgets_.push_back(std::move(std::unique_ptr<SpeciesWidget>(
              new SpeciesWidget(species, this))));
      ui_->speciesLayout->insertWidget(
          static_cast<int>(species_widgets_.size()) - 1, 
          species_widgets_.back().get());
    }
    QAction *edit = edit_species_menu_->addAction(species.getName().c_str());
    QObject::connect(edit, SIGNAL(triggered()), 
        this, SLOT(editSpeciesWidget()));
    QAction *clear = clear_species_menu_->addAction(species.getName().c_str());
    QObject::connect(clear, SIGNAL(triggered()), 
        this, SLOT(clearSpeciesWidget()));
  }
  delete dlg;
}

void SpeciesControls::on_loadSpecies_clicked() {
}

void SpeciesControls::on_saveSpecies_clicked() {
}
void SpeciesControls::on_clearAllSpeciesWidgets_triggered() {
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
      this, SLOT(on_clearAllSpeciesWidgets_triggered()));
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
}

#include "../../include/fish_detector/common/moc_species_controls.cpp"

} // namespace fish_detector

