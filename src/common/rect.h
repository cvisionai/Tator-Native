/// @file
/// @brief Defines geometry for a rectangle.

#ifndef RECT_H
#define RECT_H

#include <cstdint>

namespace fish_annotator {

/// Geometric definition of a rectangle.
struct Rect {
  /// Constructor.
  ///
  /// @param x Horizontal coordinate of top-left corner.
  /// @param y Vertical coordinate of top-left corner.
  /// @param w Width.
  /// @param h Height.
  Rect(int64_t x, int64_t y, int64_t w, int64_t h);

  /// Copy constructor.
  ///
  /// @param r Rect object to be copied.
  Rect(const Rect &r);

  int64_t x; ///< Horizontal coordinate of top-left corner.
  int64_t y; ///< Vertical coordinate of top-left corner.
  int64_t w; ///< Width.
  int64_t h; ///< Height.
};

} // namespace fish_annotator

#endif // RECT_H


