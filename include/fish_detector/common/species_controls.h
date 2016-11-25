/// @file
/// @brief Defines SpeciesControls class.

#ifndef SPECIES_CONTROLS_H
#define SPECIES_CONTROLS_H

#include <memory>
#include <list>

#include <QMenu>

#include "fish_detector/common/species_widget.h"

namespace Ui {
  class SpeciesControls;
}

namespace fish_detector {

class SpeciesControls : public QWidget {
  Q_OBJECT
public:
  /// @brief Constructor.
  ///
  /// @param parent Parent widget.
  explicit SpeciesControls(QWidget *parent = 0);

private slots:
  /// @brief Brings up a dialog box to add a species.
  void on_addSpecies_clicked();

  /// @brief Brings up a dialog box to load a species file.
  void on_loadSpecies_clicked();

  /// @brief Brings up a dialog box to save a species file.
  void on_saveSpecies_clicked();

  /// @brief Clears all species widgets after asking for confirmation.
  void on_clearAllSpeciesWidgets_triggered();

  /// @brief Clears a specific species widget.
  void clearSpeciesWidget();

  /// @brief Edits a specific species widget.
  void editSpeciesWidget();

private:
  /// @brief Widget loaded from ui file.
  std::unique_ptr<Ui::SpeciesControls> ui_;

  /// @brief List of species widget pointers.
  std::list<std::unique_ptr<SpeciesWidget>> species_widgets_;

  /// @brief Drop down menu for editing species.
  std::unique_ptr<QMenu> edit_species_menu_;

  /// @brief Drop down menu for clearing species.
  std::unique_ptr<QMenu> clear_species_menu_;

  /// @brief Clears all species widgets.
  void clearAllSpeciesWidgets();
};

} // namespace fish_detector

#endif // SPECIES_CONTROLS_H
