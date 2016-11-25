/// @file
/// @brief Defines SpeciesDialog class.

#ifndef SPECIES_DIALOG_H
#define SPECIES_DIALOG_H

#include <memory>

#include <QWidget>
#include <QDialog>

#include "fish_detector/common/species.h"

namespace Ui {
  class SpeciesDialog;
}

namespace fish_detector {

class SpeciesDialog : public QDialog {
  Q_OBJECT
public:
  /// @brief Constructor.
  ///
  /// @param parent Parent widget.
  explicit SpeciesDialog(QWidget *parent = 0);

  /// @brief Returns a Species object corresponding to the dialog values.
  ///
  /// @return Species object corresponding to the dialog values.
  Species getSpecies();

private slots:
  /// @brief Calls inherited accept function.
  void on_ok_clicked();

  /// @brief Calls inherited reject function.
  void on_cancel_clicked();

  /// @brief Removes currently selected subspecies.
  void on_removeSubspecies_clicked();

  /// @brief Adds a new subspecies.
  void on_addSubspecies_clicked();

private:
  /// @brief Widget loaded from ui file.
  std::unique_ptr<Ui::SpeciesDialog> ui_;
};

} // namespace fish_detector

#endif // SPECIES_DIALOG_H