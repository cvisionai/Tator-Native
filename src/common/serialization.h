/// @file
/// @brief Defines a virtual interface for serializable types.

#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include <string>

#include <boost/property_tree/ptree.hpp>

namespace fish_annotator {

/// Class for serializing data.
///
/// Abstract class for types that are convertible to and from a boost
/// property tree.
struct Serialization {

  /// Writes an object to a property tree.
  ///
  /// @return Property tree constructed from the object.
  virtual boost::property_tree::ptree write() const = 0;

  /// Reads a document from a property tree.
  ///
  /// @param tree Property tree to be read.
  virtual void read(const boost::property_tree::ptree &tree) = 0;
};

/// Serializes data to a stream.
///
/// @param obj Object to be serialized.
/// @param out Output stream.
void serialize(const Serialization &obj, std::ostream &out);

/// Deserializes data from a stream.
///
/// @param obj Deserialized object.
/// @param in Input stream.
void deserialize(Serialization &obj, std::istream &in);

/// Serializes data to a file.
///
/// @param obj Object to be serialized.
/// @param out Output file name.
/// @return True if successful, false otherwise.
bool serialize(const Serialization &obj, const std::string &out);

/// Deserializes data from a file.
///
/// @param obj Deserialized object.
/// @param in Input file name.
/// @return True if successful, false otherwise.
bool deserialize(Serialization &obj, const std::string &in);

} // namespace fish_annotator

#endif // SERIALIZATION_H

