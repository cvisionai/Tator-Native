/// @file
/// @brief Defines ColorScheme class which contains color assignments for
///        species.

#ifndef COLOR_SCHEME_H
#define COLOR_SCHEME_H

#include <string>
#include <array>
#include <vector>

#include <boost/property_tree/ptree.hpp>

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
    const std::array<int, 3>& color);

  /// Default constructor.
  ColorScheme();

  /// Gets the species name.
  std::string getName() const;

  /// Gets a reference to the RGB color.
  ///
  /// @return Reference to the RGB color.
  std::array<int, 3> &getColor();

  /// Gets a const reference to the RGB color.
  ///
  /// @return Const reference to the RGB color.
  const std::array<int, 3> &getColor() const;

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
  std::array<int, 3> color_;
};

/// List of color schemes.
class ColorSchemeList : public Serialization {
public:
  /// Constructor.
  ColorSchemeList();

  /// Get a reference to species list.
  ///
  /// @return Reference to species list.
  std::vector<ColorScheme> &getColorScheme();

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

