/// @file
/// @brief Defines global state annotation class.

#ifndef GLOBAL_STATE_ANNOTATION_H
#define GLOBAL_STATE_ANNOTATION_H

namespace fish_annotator {

/// Defines global state information for one image/frame.
struct GlobalStateAnnotation : public Serialization {
  /// Constructor.
  ///
  /// @param states Global states of this image/frame.
  GlobalStateAnnotation(std::map<std::string, bool> states);

  /// Default constructor.
  GlobalStateAnnotation();

  /// Writes to a property tree.
  ///
  /// @return Property tree constructed from the object.
  pt::ptree write() const override final;

  /// Reads from a property tree.
  ///
  /// @param tree Property tree to be read.
  void read(const pt::ptree &tree);

  /// Global states of this image/frame.
  std::map<std::string, bool> states_;
};

} // namespace fish_annotator

#endif // GLOBAL_STATE_ANNOTATION_H
