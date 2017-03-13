#include "fish_annotator/common/rect.h"

namespace fish_annotator {

Rect::Rect(const Rect &r) {
  x = r.x;
  y = r.y;
  w = r.w;
  h = r.h;
}

Rect::Rect(int64_t x, int64_t y, int64_t w, int64_t h) 
  : x(x)
  , y(y)
  , w(w)
  , h(h) {
}

} // namespace fish_annotator

