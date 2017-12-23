/// @file
/// @brief Defines ColorScheme class which contains color assignments for
///        species.

#ifndef COLOR_SCHEME_H
#define COLOR_SCHEME_H

#include <string>
#include <vector>
#include <map>

#include <boost/property_tree/ptree.hpp>

#include <QColor>

#include "serialization.h"

namespace fish_annotator {

namespace pt = boost::property_tree;

/// Stores color assignment for a species.
class ColorScheme : public Serialization {
public:
  /// Constructor.
  ///
  /// @param name Name of the species.
  /// @param color Color used to represent the species.
  ColorScheme(
    const std::string& name, 
    const QColor& color);

  /// Default constructor.
  ColorScheme();

  /// Gets the species name.
  std::string getName() const;

  /// Gets a reference to the RGB color.
  ///
  /// @return Reference to the RGB color.
  QColor &getColor();

  /// Gets a const reference to the RGB color.
  ///
  /// @return Const reference to the RGB color.
  const QColor &getColor() const;

  /// Equality operator.
  ///
  /// @param rhs Right hand side argument.
  /// @return Whether the object is equal to rhs.
  bool operator==(ColorScheme &rhs);

  /// Inequality operator.
  ///
  /// @param rhs Right hand side argument.
  /// @return Whether the object is not equal to rhs.
  bool operator!=(ColorScheme &rhs);

  /// Writes to a property tree.
  ///
  /// @return Property tree constructed from the object.
  pt::ptree write() const override final;

  /// Reads from a property tree.
  ///
  /// @param tree Property tree to be read.
  void read(const pt::ptree &tree) override final;
private:
  /// Name of the species.
  std::string name_;

  /// RGB color of the species.
  QColor color_;
};

/// List of color schemes.
class ColorSchemeList : public Serialization {
public:
  /// Constructor.
  ColorSchemeList();

  /// Returns a map between species names and colors.
  ///
  /// @return Map between species names and colors.
  std::map<std::string, QColor> getColorScheme();

  /// Adds a scheme to the scheme list.
  void addScheme(const std::string& name, const QColor& color);

  /// Equality operator.
  ///
  /// @param rhs Right hand side argument.
  /// @return Whether the object is equal to rhs.
  bool operator==(ColorSchemeList &rhs);

  /// Inequality operator.
  ///
  /// @param rhs Right hand side argument.
  /// @return Whether the object is not equal to rhs.
  bool operator!=(ColorSchemeList &rhs);

  /// Writes to a property tree.
  ///
  /// @return Property tree constructed from the object.
  pt::ptree write() const override final;

  /// Reads from a property tree.
  ///
  /// @param tree Property tree to be read.
  void read(const pt::ptree &tree) override final;
private:
  /// List of species.
  std::vector<ColorScheme> schemes_;
};

} // namespace fish_annotator

#endif // COLOR_SCHEME_H

