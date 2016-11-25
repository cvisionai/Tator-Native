/// @file
/// @brief Defines the SpeciesWidget class.

#ifndef SPECIES_WIDGET_H
#define SPECIES_WIDGET_H

#include <memory>

#include <QWidget>

#include "fish_detector/common/species.h"

namespace Ui {
  class SpeciesWidget;
}

namespace fish_detector {

class SpeciesWidget : public QWidget {
  Q_OBJECT
public:
  /// @brief Constructor.
  ///
  /// @param species Species object.
  /// @param parent Parent widget.
  explicit SpeciesWidget(const Species &species, QWidget *parent = 0);

  /// @brief Gets const reference to species used to construct this widget.
  const Species &getSpecies() const {return species_;}

private slots:
  /// @brief Adds an individual of this species.
  void on_addIndividual_clicked();

private:
  /// @brief Adds an individual of this species and subspecies.
  void onSubspeciesClicked();

  /// @brief Widget loaded from ui file.
  std::unique_ptr<Ui::SpeciesWidget> ui_;

  /// @brief Species used to construct this widget.
  Species species_;
};

} // namespace fish_detector

#endif // SPECIES_WIDGET_H
