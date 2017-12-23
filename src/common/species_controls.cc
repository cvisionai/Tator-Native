#include <QMessageBox>
#include <QFileDialog>

#include "color_scheme.h"
#include "species_dialog.h"
#include "species_controls.h"
#include "ui_species_controls.h"

namespace fish_annotator {

SpeciesControls::SpeciesControls(QWidget *parent)
  : ui_(new Ui::SpeciesControls)
  , species_widgets_()
  , edit_species_menu_(new QMenu(this))
  , clear_species_menu_(new QMenu(this)) {
  ui_->setupUi(this);
  ui_->addSpecies->setIcon(
      QIcon(":/icons/species_controls/add_species.svg"));
  ui_->editSpecies->setIcon(
      QIcon(":/icons/species_controls/edit_species.svg"));
  ui_->clearSpecies->setIcon(
      QIcon(":/icons/species_controls/clear_species.svg"));
  ui_->loadSpecies->setIcon(
      QIcon(":/icons/species_controls/load_species.svg"));
  ui_->saveSpecies->setIcon(
      QIcon(":/icons/species_controls/save_species.svg"));
  ui_->loadColors->setIcon(
      QIcon(":/icons/species_controls/load_colors.svg"));
  ui_->saveColors->setIcon(
      QIcon(":/icons/species_controls/save_colors.svg"));
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
    QObject::connect(
        species_widgets_.back().get(), &SpeciesWidget::colorChanged,
        this, &SpeciesControls::onColorChanged);
    onColorChanged();
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
      "Specify input species file", QString(), "Species File (*.species)");
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

void SpeciesControls::onColorChanged() {
  QMap<QString, QColor> colors;
  for(const auto& widget : species_widgets_) {
    colors.insert(
        widget->getSpecies().getName().c_str(), 
        widget->getColor());
  }
  emit colorChanged(colors);
}

void SpeciesControls::loadFromVector(const std::vector<Species> &vec) {
  for(const auto &species : vec) {
    insertSpeciesWidget(species);
  }
}

std::vector<Species> SpeciesControls::getSpecies() {
  std::vector<Species> species;
  for(const auto &w : species_widgets_) {
    species.push_back(w->getSpecies());
  }
  return species;
}

void SpeciesControls::on_saveSpecies_clicked() {
  QString out_file = QFileDialog::getSaveFileName(this, 
      "Specify output species file", QString(), "Species File (*.species)");
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

void SpeciesControls::on_loadColors_clicked() {
  QString in_file = QFileDialog::getOpenFileName(this,
      "Specify input colors file", QString(), "Colors File (*.colors)");
  if(!in_file.isEmpty()) {
    loadColorsFile(in_file);
  }
}

void SpeciesControls::loadColorsFile(const QString &in_file) {
  ColorSchemeList list;
  if(deserialize(list, in_file.toStdString())) {
    auto mapping = list.getColorScheme();
    for(auto &widget : species_widgets_) {
      auto species = widget->getSpecies().getName();
      if(mapping.find(species) != mapping.end()) {
        widget->setColor(mapping[species]);
      }
    }
  }
}

void SpeciesControls::on_saveColors_clicked() {
  QString out_file = QFileDialog::getSaveFileName(this, 
      "Specify output colors file", QString(), "Colors File (*.colors)");
  if(!out_file.isEmpty()) {
    ColorSchemeList list;
    for(auto &widget : species_widgets_) {
      list.addScheme(widget->getSpecies().getName(), widget->getColor());
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

void SpeciesControls::resetCounts() {
  for(auto &widget : species_widgets_) {
    widget->setCount(0);
  }
}

void SpeciesControls::setCount(uint64_t count, const std::string &species) {
  for(auto &widget : species_widgets_) {
    if(widget->getSpecies().getName() == species) {
      widget->setCount(count);
    }
  }
}

#include "moc_species_controls.cpp"

} // namespace fish_annotator

