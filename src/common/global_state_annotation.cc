#include "global_state_annotation.h"

namespace fish_annotator {

GlobalStateAnnotation::GlobalStateAnnotation(std::map<std::string, bool> states)
  : states_(states) {
}

pt::ptree GlobalStateAnnotation::write() const {
  pt::ptree tree;
  for(auto state : states_) {
    tree.put(state.first, state.second ? 1 : 0);
  }
}

void GlobalStateAnnotation::read(const pt::ptree &tree) {
  states_.clear();
  for(auto it : tree) {
    states_.insert(std::pair<std::string, bool>(
      it.first,
      it.second.get<std::string>(it.first) == "1"));
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
}

} // namespace fish_annotator
