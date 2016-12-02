#include "fish_annotator/common/rect.h"

namespace fish_annotator {

Rect::Rect(const Rect &r) {
  x = r.x;
  y = r.y;
  w = r.w;
  h = r.h;
}

Rect::Rect(uint64_t x, uint64_t y, uint64_t w, uint64_t h) 
  : x(x)
  , y(y)
  , w(w)
  , h(h) {
}

} // namespace fish_annotator

