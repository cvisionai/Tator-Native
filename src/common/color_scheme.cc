
#include <boost/algorithm/string.hpp>

#include "color_scheme.h"

namespace fish_annotator {

ColorScheme::ColorScheme(
    const std::string& name, 
    const std::array<int, 3> &color)
  : name_(name)
  , color_(color) {
  boost::algorithm::to_lower(name_);
}

ColorScheme::ColorScheme()
  : name_()
  , color_() {
}

std::array<int, 3> &ColorScheme::getColor() {
  return color_;
}

const std::array<int, 3> &ColorScheme::getColor() const {
  return color_;
}

bool ColorScheme::operator==(ColorScheme &rhs) {
  if(name_ != rhs.name_) return false;
  if(color_ != rhs.color_) return false;
  return true;
}

bool ColorScheme::operator!=(ColorScheme &rhs) {
  return !operator==(rhs);
}

pt::ptree ColorScheme::write() const {
  pt::ptree tree;
  tree.put("name", name_);
  tree.put("red", color_[0]);
  tree.put("green", color_[1]);
  tree.put("blue", color_[2]);
  return tree;
}

void ColorScheme::read(const pt::ptree &tree) {
  name_ = tree.get<std::string>("name");
  boost::algorithm::to_lower(name_);
  color_ = {
    tree.get<int>("red"),
    tree.get<int>("green"),
    tree.get<int>("blue")
  };
}

ColorSchemeList::ColorSchemeList() 
  : schemes_() {
}

std::vector<ColorScheme> &ColorSchemeList::getColorScheme() {
  return schemes_;
}

bool ColorSchemeList::operator==(ColorSchemeList &rhs) {
  if(schemes_.size() != rhs.schemes_.size()) return false;
  for(uint32_t n=0; n < schemes_.size(); n++) {
    if(schemes_[n] != rhs.schemes_[n]) return false;
  }
  return true;
}

bool ColorSchemeList::operator!=(ColorSchemeList &rhs) {
  return !operator==(rhs);
}

pt::ptree ColorSchemeList::write() const {
  pt::ptree arr;
  for(const auto &color : schemes_) {
    pt::ptree elem;
    elem.put("species", color.getName());
    elem.put("red", color.getColor()[0]);
    elem.put("green", color.getColor()[1]);
    elem.put("blue", color.getColor()[2]);
    arr.push_back(std::make_pair("", elem));
  }
  return arr;
}

void ColorSchemeList::read(const pt::ptree &tree) {
  for(auto &val : tree) {
    ColorScheme scheme;
    scheme.read(val.second.get_child(""));
    schemes_.push_back(scheme);
  }
}

} // namespace fish_annotator
