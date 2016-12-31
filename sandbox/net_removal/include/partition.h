#ifndef PARTITION_H
#define PARTITION_H

#include <arrayfire.h>

namespace net_removal {

class Partition {
public:
  /// @brief Constructor.
  /// @param filename Full qualified path to image file.
  Partition(const char *filename);
private:
  af::array m_image;
};

} // namespace net_removal

#endif //PARTITION_H
