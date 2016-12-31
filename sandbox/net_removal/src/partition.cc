
#include "partition.h"

namespace net_removal {

Partition::Partition(const char *filename)
  : m_image(af::loadImage(filename))
{
}

}
