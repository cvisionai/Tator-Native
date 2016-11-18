/// @file
/// @brief Defines geometry for a rectangle.

#ifndef RECT_H
#define RECT_H

#include <cstdint>

namespace fish_detector {

/// @brief Geometric definition of a rectangle.
struct Rect {
  /// @brief Constructor.
  ///
  /// @param x Horizontal coordinate of top-left corner.
  /// @param y Vertical coordinate of top-left corner.
  /// @param w Width.
  /// @param h Height.
  Rect(uint64_t x, uint64_t y, uint64_t w, uint64_t h);

  /// @brief Copy constructor.
  ///
  /// @param r Rect object to be copied.
  Rect(const Rect &r);

  uint64_t x; ///< Horizontal coordinate of top-left corner.
  uint64_t y; ///< Vertical coordinate of top-left corner.
  uint64_t w; ///< Width.
  uint64_t h; ///< Height.
};

} // namespace fish_detector

#endif // RECT_H


