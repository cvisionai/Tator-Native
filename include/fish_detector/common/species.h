/// @file
/// @brief Defines Species class which contains relevant info about
///        a species.

#ifndef SPECIES_H
#define SPECIES_H

#include <vector>
#include <string>

#include <boost/property_tree/ptree.hpp>

#include "fish_detector/common/serialization.h"

namespace fish_detector {

namespace pt = boost::property_tree;

/// @brief Stores relevant info about a species.
class Species : public Serialization {
public:
  /// @brief Constructor.
  ///
  /// @param name Name of the species.
  Species(const std::string& name);

  /// @brief Gets a reference to the subspecies list.
  ///
  /// @return Reference to the subspecies list.
  std::vector<std::string> &getSubspecies();

  /// @brief Writes to a property tree.
  ///
  /// @return Property tree constructed from the object.
  pt::ptree write() const;

  /// @brief Reads from a property tree.
  ///
  /// @param tree Property tree to be read.
  void read(const pt::ptree &tree);
private:
  /// @brief Name of the species.
  std::string name_;

  /// @brief List of subspecies.
  std::vector<std::string> subspecies_;
}

/// @brief List of species.
class SpeciesList : public Serialization {
public:
  /// @brief Constructor.
  SpeciesList();

  /// @brief Writes to a property tree.
  ///
  /// @return Property tree constructed from the object.
  pt::ptree write() const;

  /// @brief Reads from a property tree.
  ///
  /// @param tree Property tree to be read.
  void read(const pt::ptree &tree);
private:
  /// @brief List of species.
  std::vector<Species> species_;
}

} // namespace fish_detector

#endif // SPECIES_H


