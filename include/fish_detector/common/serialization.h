/// @file
/// @brief Defines a virtual interface for serializable types.

#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include <boost/property_tree/ptree.hpp>

namespace fish_detector {

/// @brief Class for serializing data.
///
/// Abstract class for types that are convertible to and from a boost
/// property tree.
struct Serialization {

  /// @brief Writes an object to a property tree.
  ///
  /// @return Property tree constructed from the object.
  virtual boost::property_tree::ptree write() const = 0;

  /// @brief Reads a document from a property tree.
  ///
  /// @param tree Property tree to be read.
  virtual void read(const boost::property_tree::ptree &tree) = 0;
};

/// @brief Serializes data to a stream.
///
/// @param obj Object to be serialized.
/// @param out Output stream.
void serialize(const Serialization &obj, std::ostream &out);

/// @brief Deserializes data from a stream.
///
/// @param obj Deserialized object.
void deserialize(Serialization &obj, std::istream &in);

} // namespace fish_detector

#endif // SERIALIZATION_H

