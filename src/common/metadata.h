/// @file
/// @brief Defines Metadata struct.

#ifndef METADATA_H
#define METADATA_H

#include <string>

namespace tator {

/// Stores video annotation metadata.
struct Metadata {

  /// Base file name.
  std::string file_name_;

  /// Reviewer name.
  std::string reviewer_name_;

  /// Trip ID.
  uint32_t trip_id_;

  /// Tow number.
  uint32_t tow_number_;

  /// Tow status.
  bool tow_status_;

};

} // namespace tator

#endif // METADATA_H

