/// @file
/// @brief Defines SpeciesControls class.

#ifndef SPECIES_CONTROLS_H
#define SPECIES_CONTROLS_H

#include <memory>
#include <list>

#include <QMenu>
#include <QMap>

#include "species_widget.h"
#include "ui_species_controls.h"

#ifndef NO_TESTING
class TestImageAnnotator;
#endif

namespace fish_annotator {

/// Widget containing all species controls.
class SpeciesControls : public QWidget {
  Q_OBJECT
#ifndef NO_TESTING
  friend class ::TestImageAnnotator;
#endif
public:
  /// Constructor.
  ///
  /// @param parent Parent widget.
  explicit SpeciesControls(QWidget *parent = 0);

  /// Resets all counts to zero.
  void resetCounts();

  /// Sets count for a given species.
  ///
  /// @param count Number of individuals of this species.
  /// @param species Name of the species.
  void setCount(uint64_t count, const std::string &species);

  /// Loads species from an external source.
  ///
  /// @param vec Vector of species used to insert widgets.
  void loadFromVector(const std::vector<Species> &vec);

  /// Gets species for every species widget.
  ///
  /// @return Species for every species widget.
  std::vector<Species> getSpecies();

  /// Loads species file.
  ///
  /// @param in_file Path to input species file.
  void loadSpeciesFile(const QString &in_file);

public slots:
  /// Handles color change in a species widget.
  void onColorChanged();

private slots:
  /// Brings up a dialog box to add a species.
  void on_addSpecies_clicked();

  /// Brings up a dialog box to load a species file.
  void on_loadSpecies_clicked();

  /// Brings up a dialog box to save a species file.
  void on_saveSpecies_clicked();

  /// Clears all species widgets after asking for confirmation.
  void onClearAllSpeciesWidgetsTriggered();

  /// Clears a specific species widget.
  void clearSpeciesWidget();

  /// Edits a specific species widget.
  void editSpeciesWidget();

signals:
  /// Propagates add individual signal from species widgets.
  ///
  /// @param species Species name.
  /// @param subspecies Subspecies name.
  void individualAdded(std::string species, std::string subspecies);

  /// Emits map between species name and color.
  void colorChanged(QMap<QString, QColor>);

private:
  /// Widget loaded from ui file.
  std::unique_ptr<Ui::SpeciesControls> ui_;

  /// List of species widget pointers.
  std::list<std::unique_ptr<SpeciesWidget>> species_widgets_;

  /// Drop down menu for editing species.
  std::unique_ptr<QMenu> edit_species_menu_;

  /// Drop down menu for clearing species.
  std::unique_ptr<QMenu> clear_species_menu_;

  /// Clears all species widgets.
  void clearAllSpeciesWidgets();

  /// Inserts a new species widget.
  ///
  /// @param species Species object used to construct the widget.
  void insertSpeciesWidget(const Species &species);
};

} // namespace fish_annotator

#endif // SPECIES_CONTROLS_H
