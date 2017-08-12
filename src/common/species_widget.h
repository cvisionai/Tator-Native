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

  /// @brief Sets widget values according to input Species object.
  void setSpecies(const Species &species);

  /// @brief Updates individual counts.
  void setCount(uint64_t count);

  /// @brief Gets color used to represent this species.
  QColor getColor();

private slots:
  /// @brief Adds an individual of this species.
  void on_addIndividual_clicked();

  /// @brief Changes the color used to represent this species.
  void on_changeColor_clicked();

  /// @brief Adds an individual of this species and subspecies.
  void onSubspeciesClicked();

signals:
  /// @brief Signal for individual added.
  void individualAdded(std::string species, std::string subspecies);

  /// @brief Signal for species color changed.
  void colorChanged();

private:
  /// @brief Widget loaded from ui file.
  std::unique_ptr<Ui::SpeciesWidget> ui_;

  /// @brief Subspecies menu.
  std::unique_ptr<QMenu> subspecies_menu_;

  /// @brief Species used to construct this widget.
  Species species_;

  /// @brief Color used to represent this species.
  QColor color_;

  /// @brief Updates button color.
  void updateButtonColor();
};

} // namespace fish_annotator

#endif // SPECIES_WIDGET_H
