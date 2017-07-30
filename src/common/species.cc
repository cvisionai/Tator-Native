#include <boost/algorithm/string.hpp>

#include "fish_annotator/common/species.h"

namespace fish_annotator {

Species::Species(const std::string& name)
  : name_()
  , subspecies_() {
  std::string lower_name = name;
  boost::algorithm::to_lower(lower_name);
  name_ = lower_name;
}

Species::Species()
  : name_()
  , subspecies_() {
}

std::vector<std::string> &Species::getSubspecies() {
  return subspecies_;
}

const std::vector<std::string> &Species::getSubspecies() const {
  return subspecies_;
}

bool Species::operator==(Species &rhs) {
  if(name_ != rhs.name_) return false;
  if(subspecies_.size() != rhs.subspecies_.size()) return false;
  for(uint32_t n=0; n < subspecies_.size(); n++) {
    if(subspecies_[n] != rhs.subspecies_[n]) return false;
  }
  return true;
}

bool Species::operator!=(Species &rhs) {
  return !operator==(rhs);
}

pt::ptree Species::write() const {
  pt::ptree tree;
  tree.put("name", name_);
  for(const auto &subname : subspecies_) {
    tree.add("subspecies_list.subspecies", subname);
  }
  return tree;
}

void Species::read(const pt::ptree &tree) {
  name_ = tree.get<std::string>("name");
  if(tree.count("subspecies_list") > 0) {
    for(auto &val : tree.get_child("subspecies_list")) {
      subspecies_.push_back(val.second.data());
    }
  }
}

SpeciesList::SpeciesList() 
  : species_() {
}

std::vector<Species> &SpeciesList::getSpecies() {
  return species_;
}

bool SpeciesList::operator==(SpeciesList &rhs) {
  if(species_.size() != rhs.species_.size()) return false;
  for(uint32_t n=0; n < species_.size(); n++) {
    if(species_[n] != rhs.species_[n]) return false;
  }
  return true;
}

bool SpeciesList::operator!=(SpeciesList &rhs) {
  return !operator==(rhs);
}

pt::ptree SpeciesList::write() const {
  pt::ptree tree;
  for(const auto &species : species_) {
    tree.add_child("species_list.species", species.write());
  }
  return tree;
}

void SpeciesList::read(const pt::ptree &tree) {
  for(auto &val : tree.get_child("species_list")) {
    Species species;
    species.read(val.second);
    species_.push_back(species);
  }
}

} // namespace fish_annotator
