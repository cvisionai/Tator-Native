/// @file
/// @brief Defines SpeciesDialog class.

#ifndef SPECIES_DIALOG_H
#define SPECIES_DIALOG_H

#include <memory>

#include <QWidget>
#include <QDialog>

#include "species.h"
#include "ui_species_dialog.h"

namespace tator {

/// Dialog for adding a new species.
class SpeciesDialog : public QDialog {
  Q_OBJECT
public:
  /// Constructor.
  ///
  /// @param parent Parent widget.
  explicit SpeciesDialog(QWidget *parent = 0);

  /// Returns a Species object corresponding to the dialog values.
  ///
  /// @return Species object corresponding to the dialog values.
  Species getSpecies();

  /// Sets dialog values to values corresponding to input species.
  ///
  /// @param species Input Species object.
  void setSpecies(const Species &species);

private slots:
  /// Calls inherited accept function.
  void on_ok_clicked();

  /// Calls inherited reject function.
  void on_cancel_clicked();

  /// Removes currently selected subspecies.
  void on_removeSubspecies_clicked();

  /// Adds a new subspecies.
  void on_addSubspecies_clicked();

private:
  /// Widget loaded from ui file.
  std::unique_ptr<Ui::SpeciesDialog> ui_;
};

} // namespace tator

#endif // SPECIES_DIALOG_H
