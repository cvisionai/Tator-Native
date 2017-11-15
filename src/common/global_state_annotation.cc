#include "global_state_annotation.h"

namespace fish_annotator {

GlobalStateAnnotation::GlobalStateAnnotation(
  std::map<std::string, bool> states,
  std::map<std::string, std::string> headers)
  : states_(states) 
  , headers_(headers) {
}

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
    states_.insert(std::pair<std::string, bool>(
      elem.get<std::string>("state"),
      elem.get<bool>("value")));
    auto opt_header = elem.get_optional<std::string>("header");
    if(opt_header != boost::none) {
      headers_.insert(std::pair<std::string, std::string>(
        elem.get<std::string>("state"),
        opt_header.get()));
    }
    else {
      headers_.insert(std::pair<std::string, std::string>(
        elem.get<std::string>("state"), ""));
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
