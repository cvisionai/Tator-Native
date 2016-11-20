#include "fish_detector/common/species.h"

namespace fish_detector {

Species::Species(const std::string& name)
  : name_(name)
  , subspecies_() {
}

Species::Species()
  : name_()
  , subspecies_() {
}

std::vector<std::string> &Species::getSubspecies() {
  return subspecies_;
}

pt::ptree Species::write() const {
  pt::ptree tree;
  tree.add("name", name_);
  for(const auto &subname : subspecies_) {
    tree.add("subspecies_list.subspecies", subname);
  }
  return tree;
}

void Species::read(const pt::ptree &tree) {
  name_ = tree.get("name", 0);
  for(auto &val : tree.get_child("subspecies_list")) {
    subspecies_.push_back(val.second.data());
  }
}

SpeciesList::SpeciesList() 
  : species_() {
}

pt::ptree SpeciesList::write() const {
  pt::ptree tree;
  for(const auto &species : species_) {
    tree.add_child("species_list.species", species.write());
  }
  return tree;
}

void SpeciesList::read(const pt::ptree &tree) {
  Species species;
  for(auto &val : tree.get_child("species_list")) {
    species.read(val.second);
    species_.push_back(species);
  }
}

} // namespace fish_detector
