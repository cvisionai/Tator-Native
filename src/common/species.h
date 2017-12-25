/// @file
/// @brief Defines Species class which contains relevant info about
///        a species.

#ifndef SPECIES_H
#define SPECIES_H

#include <vector>
#include <string>

#include <boost/property_tree/ptree.hpp>

#include "serialization.h"

namespace fish_annotator {

namespace pt = boost::property_tree;

/// Stores relevant info about a species.
class Species : public Serialization {
public:
  /// Constructor.
  ///
  /// @param name Name of the species.
  Species(const std::string& name);

  /// Default constructor.
  Species();

  /// Gets the species name.
  std::string getName() const {return name_;}

  /// Gets a reference to the subspecies list.
  ///
  /// @return Reference to the subspecies list.
  std::vector<std::string> &getSubspecies();

  /// Gets a const reference to the subspecies list.
  ///
  /// @return Const reference to the subspecies list.
  const std::vector<std::string> &getSubspecies() const;

  /// Equality operator.
  ///
  /// @param rhs Right hand side argument.
  /// @return Whether the object is equal to rhs.
  bool operator==(Species &rhs);

  /// Inequality operator.
  ///
  /// @param rhs Right hand side argument.
  /// @return Whether the object is not equal to rhs.
  bool operator!=(Species &rhs);

  /// Writes to a property tree.
  ///
  /// @return Property tree constructed from the object.
  pt::ptree write() const;

  /// Reads from a property tree.
  ///
  /// @param tree Property tree to be read.
  void read(const pt::ptree &tree);
private:
  /// Name of the species.
  std::string name_;

  /// List of subspecies.
  std::vector<std::string> subspecies_;
};

/// List of species.
class SpeciesList : public Serialization {
public:
  /// Constructor.
  SpeciesList();

  /// Get a reference to species list.
  ///
  /// @return Reference to species list.
  std::vector<Species> &getSpecies();

  /// Equality operator.
  ///
  /// @param rhs Right hand side argument.
  /// @return Whether the object is equal to rhs.
  bool operator==(SpeciesList &rhs);

  /// Inequality operator.
  ///
  /// @param rhs Right hand side argument.
  /// @return Whether the object is not equal to rhs.
  bool operator!=(SpeciesList &rhs);

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
  std::vector<Species> species_;
};

} // namespace fish_annotator

#endif // SPECIES_H


