#include "global_state_annotation.h"
#include <fstream>
std::ofstream debug3("DEBUG3.TXT");
namespace fish_annotator {

GlobalStateAnnotation::GlobalStateAnnotation(
  std::map<std::string, bool> states)
  : states_(states) {
}

GlobalStateAnnotation::GlobalStateAnnotation()
  : states_() {
}

pt::ptree GlobalStateAnnotation::write() const {
  pt::ptree arr;
  for(auto state : states_) {
    pt::ptree tree;
    tree.put("state", state.first);
    tree.put("value", state.second);
    tree.put("type", "bool");
    arr.push_back(std::make_pair("", tree));
  }
  return arr;
}

void GlobalStateAnnotation::read(const pt::ptree &tree) {
  states_.clear();
  for(auto val : tree) {
    pt::ptree elem = val.second.get_child("");
    debug3 << "STATE: " << elem.get<std::string>("state") << std::endl;
    debug3 << "VALUE: " << (elem.get<std::string>("value") == "true") << std::endl;
    states_.insert(std::pair<std::string, bool>(
      elem.get<std::string>("state"),
      elem.get<std::string>("value") == "true"));
  }
}

std::string GlobalStateAnnotation::writeCsvHeader() {
  std::string header;
  for(auto state : states_) {
    header += ",";
    header += state.first;
  }
  return header;
}

std::string GlobalStateAnnotation::writeCsvValues() {
  std::string values;
  for (auto state : states_) {
    values += ",";
    values += state.second ? "1" : "0";
  }
  return values;
}

} // namespace fish_annotator
