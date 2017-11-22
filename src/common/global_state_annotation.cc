#include "global_state_annotation.h"

namespace fish_annotator {

GlobalStateAnnotation::GlobalStateAnnotation()
  : states_() 
  , headers_() {
}

pt::ptree GlobalStateAnnotation::write() const {
  pt::ptree arr;
  for(auto state : states_) {
    pt::ptree tree;
    tree.put("state", state.first);
    tree.put("value", state.second);
    tree.put("header", headers_.at(state.first));
    tree.put("type", "bool");
    arr.push_back(std::make_pair("", tree));
  }
  return arr;
}

void GlobalStateAnnotation::read(const pt::ptree &tree) {
  for(auto val : tree) {
    pt::ptree elem = val.second.get_child("");
    auto new_state = elem.get<std::string>("state");
    if(states_.find(new_state) == states_.end()) {
      states_.insert(std::pair<std::string, bool>(
        new_state,
        elem.get<bool>("value")));
      auto opt_header = elem.get_optional<std::string>("header");
      if(opt_header != boost::none) {
        headers_.insert(std::pair<std::string, std::string>(
          new_state,
          opt_header.get()));
      }
      else {
        headers_.insert(std::pair<std::string, std::string>(
          new_state, 
          ""));
      }
    }
    else {
      states_[new_state] = elem.get<bool>("value");
    }
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
