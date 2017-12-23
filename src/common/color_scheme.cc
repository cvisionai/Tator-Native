
#include <boost/algorithm/string.hpp>

#include "color_scheme.h"

namespace fish_annotator {

ColorScheme::ColorScheme(
    const std::string& name, 
    const QColor &color)
  : name_(name)
  , color_(color) {
  boost::algorithm::to_lower(name_);
}

ColorScheme::ColorScheme()
  : name_()
  , color_() {
}

std::string ColorScheme::getName() const {
  return name_;
}

QColor &ColorScheme::getColor() {
  return color_;
}

const QColor &ColorScheme::getColor() const {
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
  tree.put("species", name_);
  tree.put("red", color_.red());
  tree.put("green", color_.green());
  tree.put("blue", color_.blue());
  return tree;
}

void ColorScheme::read(const pt::ptree &tree) {
  name_ = tree.get<std::string>("species");
  boost::algorithm::to_lower(name_);
  color_ = QColor(
    tree.get<int>("red"),
    tree.get<int>("green"),
    tree.get<int>("blue")
  );
}

ColorSchemeList::ColorSchemeList() 
  : schemes_() {
}

std::map<std::string, QColor> ColorSchemeList::getColorScheme() {
  std::map<std::string, QColor> scheme;
  for(const auto &s : schemes_) {
    scheme.insert(std::make_pair(s.getName(), s.getColor()));
  }
  return scheme;
}

void ColorSchemeList::addScheme(
  const std::string &name, 
  const QColor &color) {
  schemes_.push_back(ColorScheme(name, color));
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
  pt::ptree pt, arr;
  for(const auto &color : schemes_) {
    pt::ptree elem = color.write();
    arr.push_back(std::make_pair("", elem));
  }
  pt.add_child("color_scheme", arr);
  return pt;
}

void ColorSchemeList::read(const pt::ptree &tree) {
  for(auto &val : tree.get_child("color_scheme")) {
    ColorScheme scheme;
    scheme.read(val.second.get_child(""));
    schemes_.push_back(scheme);
  }
}

} // namespace fish_annotator
