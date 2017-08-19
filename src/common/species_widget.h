/// @file
/// @brief Defines the SpeciesWidget class.

#ifndef SPECIES_WIDGET_H
#define SPECIES_WIDGET_H

#include <memory>

#include <QWidget>
#include <QMenu>

#include "species.h"
#include "ui_species_widget.h"

namespace fish_annotator {

/// Widget containing all controls associated with a single species.
class SpeciesWidget : public QWidget {
  Q_OBJECT
public:
  /// Constructor.
  ///
  /// @param species Species object.
  /// @param parent Parent widget.
  explicit SpeciesWidget(const Species &species, QWidget *parent = 0);

  /// Gets const reference to species used to construct this widget.
  const Species &getSpecies() const {return species_;}

  /// Sets widget values according to input Species object.
  void setSpecies(const Species &species);

  /// Updates individual counts.
  void setCount(uint64_t count);

  /// Gets color used to represent this species.
  QColor getColor();

private slots:
  /// Adds an individual of this species.
  void on_addIndividual_clicked();

  /// Changes the color used to represent this species.
  void on_changeColor_clicked();

  /// Adds an individual of this species and subspecies.
  void onSubspeciesClicked();

signals:
  /// Signal for individual added.
  void individualAdded(std::string species, std::string subspecies);

  /// Signal for species color changed.
  void colorChanged();

private:
  /// Widget loaded from ui file.
  std::unique_ptr<Ui::SpeciesWidget> ui_;

  /// Subspecies menu.
  std::unique_ptr<QMenu> subspecies_menu_;

  /// Species used to construct this widget.
  Species species_;

  /// Color used to represent this species.
  QColor color_;

  /// Updates button color.
  void updateButtonColor();
};

} // namespace fish_annotator

#endif // SPECIES_WIDGET_H
