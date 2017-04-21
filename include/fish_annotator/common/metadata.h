/// @file
/// @brief Defines Metadata struct.

#ifndef METADATA_H
#define METADATA_H

#include <string>

namespace fish_annotator {

/// @brief Stores video annotation metadata.
struct Metadata {

  /// @brief Base file name.
  std::string file_name_;

  /// @brief Reviewer name.
  std::string reviewer_name_;

  /// @brief Trip ID.
  uint32_t trip_id_;

  /// @brief Tow number.
  uint32_t tow_number_;

};

} // namespace fish_annotator

#endif // METADATA_H

