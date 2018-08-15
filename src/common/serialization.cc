
#include <boost/property_tree/json_parser.hpp>

#include "serialization.h"

namespace tator {

namespace pt = boost::property_tree;

void serialize(const Serialization &obj, std::ostream &out) {
  pt::write_json(out, obj.write());
}

void deserialize(Serialization &obj, std::istream &in) {
  pt::ptree tree;
  pt::json_parser::read_json(in, tree);
  obj.read(tree);
}

bool serialize(const Serialization &obj, const std::string &out) {
  std::ofstream stream(out, std::ios::out);
  if(stream.fail()) return false;
  serialize(obj, stream);
  return true;
}

bool deserialize(Serialization &obj, const std::string &in) {
  std::ifstream stream(in, std::ios::in);
  if(stream.fail()) return false;
  deserialize(obj, stream);
  return true;
}

} // namespace tator

