
#include <boost/property_tree/json_parser.hpp>

#include "fish_detector/common/serialization.h"

namespace fish_detector {

namespace pt = boost::property_tree;

void serialize(const Serialization &obj, std::ostream &out) {
  pt::write_json(out, obj.write(), false);
}

void deserialize(Serialization &obj, std::istream &in) {
  pt::ptree tree;
  pt::json_parser::read_json(in, tree);
  obj.read(tree);
}

} // namespace fish_detector

