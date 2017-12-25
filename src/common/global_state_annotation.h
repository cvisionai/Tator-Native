/// @file
/// @brief Defines global state annotation class.

#ifndef GLOBAL_STATE_ANNOTATION_H
#define GLOBAL_STATE_ANNOTATION_H

#include <map>
#include <memory>

#include <boost/property_tree/ptree.hpp>
#include <boost/variant.hpp>

#include "serialization.h"

namespace fish_annotator {

namespace pt = boost::property_tree;

/// Gets the value of the state as a string.
class GetValueVisitor : public boost::static_visitor<std::string> {
public:
  /// Specialization for bool.
  /// @param val Value.
  std::string operator()(bool val) const;

  /// Specialization for string.
  /// @param val Value.
  std::string operator()(std::string val) const;
};

/// Gets the value of the state as a string for csv.
class GetCsvValueVisitor : public boost::static_visitor<std::string> {
public:
  /// Specialization for bool.
  /// @param val Value.
  std::string operator()(bool val) const;

  /// Specialization for string.
  /// @param val Value.
  std::string operator()(std::string val) const;
};

/// Gets the type of the state as a string.
class GetTypeVisitor : public boost::static_visitor<std::string> {
public:
  /// Specialization for bool.
  /// @param val Value.
  std::string operator()(bool val) const;

  /// Specialization for string.
  /// @param val Value.
  std::string operator()(std::string val) const;
};

/// Defines global state information for one image/frame.
struct GlobalStateAnnotation : public Serialization {
  /// Constructor.
  GlobalStateAnnotation();

  /// Writes to a property tree.
  ///
  /// @return Property tree constructed from the object.
  pt::ptree write() const override final;

  /// Reads from a property tree.
  ///
  /// @param tree Property tree to be read.
  void read(const pt::ptree &tree) override final;

  /// Writes csv header.
  ///
  /// @return Header names for global state.
  std::string writeCsvHeader();

  /// Writes csv values.
  ///
  /// @return Values for global state.
  std::string writeCsvValues();

  /// Global states of this image/frame.
  std::map<std::string, boost::variant<bool, std::string>> states_;

  /// Headers for global states.
  std::map<std::string, std::string> headers_;
};

} // namespace fish_annotator

#endif // GLOBAL_STATE_ANNOTATION_H
