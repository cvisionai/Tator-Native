#include <boost/algorithm/string.hpp>

#include "global_state_annotation.h"

namespace fish_annotator {

std::string GetValueVisitor::operator()(bool val) const {
  return val ? "true" : "false";
}

std::string GetValueVisitor::operator()(std::string val) const {
  return val;
}

std::string GetCsvValueVisitor::operator()(bool val) const {
  return val ? "1" : "0";
}

std::string GetCsvValueVisitor::operator()(std::string val) const {
  return val;
}

std::string GetTypeVisitor::operator()(bool val) const {
  return "bool";
}

std::string GetTypeVisitor::operator()(std::string val) const {
  return "string";
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
    tree.put("value", boost::apply_visitor(
      GetValueVisitor(), 
      state.second));
    tree.put("header", headers_.at(state.first));
    tree.put("type", boost::apply_visitor(
      GetTypeVisitor(), 
      state.second));
    arr.push_back(std::make_pair("", tree));
  }
  return arr;
}

void GlobalStateAnnotation::read(const pt::ptree &tree) {
  for(auto val : tree) {
    pt::ptree elem = val.second.get_child("");
    auto new_state = elem.get<std::string>("state");
    boost::algorithm::to_lower(new_state);
    auto type_str = elem.get<std::string>("type");
    if(states_.find(new_state) == states_.end()) {
      if(type_str == "bool") {
        states_.insert(std::pair<std::string, bool>(
          new_state,
          elem.get<bool>("value")));
      }
      else if(type_str == "string") {
        states_.insert(std::pair<std::string, std::string>(
          new_state,
          elem.get<std::string>("value")));
      }
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
      if(type_str == "bool") {
        states_[new_state] = elem.get<bool>("value");
      }
      else if(type_str == "string") {
        states_[new_state] = elem.get<std::string>("value");
      }
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
    values += boost::apply_visitor(
      GetCsvValueVisitor(),
      state.second);
  }
  return values;
}

} // namespace fish_annotator
