#include <boost/filesystem.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "fish_detector/image_annotator/image_annotation.h"

namespace fish_detector { namespace image_annotator {

ImageAnnotation::ImageAnnotation(const std::string& image_file, 
                                 const std::string& species,
                                 const std::string& subspecies,
                                 uint64_t id, 
                                 const Rect &rect)
  : image_file_(image_file)
  , species_(species)
  , subspecies_(subspecies)
  , id_(id)
  , rect_(rect) {
}

ImageAnnotation::ImageAnnotation()
  : image_file_()
  , species_()
  , subspecies_()
  , id_(0)
  , rect_(0, 0, 0, 0) {
}

pt::ptree ImageAnnotation::write() const {
  pt::ptree tree;
  tree.put("image_file", image_file_);
  tree.put("species", species_);
  tree.put("subspecies", subspecies_);
  tree.put("id", id_);
  tree.put("x", rect_.x);
  tree.put("y", rect_.y);
  tree.put("w", rect_.w);
  tree.put("h", rect_.h);
  return tree;
}

void ImageAnnotation::read(const pt::ptree &tree) {
  image_file_ = tree.get<std::string>("image_file");
  species_ = tree.get<std::string>("species");
  subspecies_ = tree.get<std::string>("subspecies");
  id_ = tree.get<uint64_t>("id");
  uint64_t x = tree.get<uint64_t>("x");
  uint64_t y = tree.get<uint64_t>("y");
  uint64_t w = tree.get<uint64_t>("w");
  uint64_t h = tree.get<uint64_t>("h");
  rect_ = Rect(x, y, w, h);
}

ImageAnnotationList::ImageAnnotationList()
  : list_()
  , by_file_()
  , by_species_() {
}

void ImageAnnotationList::insert(const ImageAnnotation &annotation) {
  list_.push_front(annotation);
  by_file_.insert({annotation.image_file_, list_.begin()});
  by_species_.insert({{annotation.species_, annotation.subspecies_}, 
                      list_.begin()});
}

void ImageAnnotationList::remove(const std::string &image_file, uint64_t id) {
  auto it = by_file_.left.find(image_file);
  for(; it != by_file_.left.end(); ++it) {
    if(it->second->id_ == id) {
      list_.erase(it->second);
      by_species_.right.erase(by_species_.right.find(it->second));
      by_file_.left.erase(it);
      break;
    }
  }
}

void ImageAnnotationList::write(
  const std::vector<std::string> &filenames) const {
  for(auto &image_file : filenames) {
    pt::ptree tree;
    auto it = by_file_.left.find(image_file);
    for(; it != by_file_.left.end(); ++it) {
      tree.add_child("annotation_list.annotation", it->second->write());
    }
    boost::filesystem::path json_file(image_file);
    json_file.replace_extension(".json");
    pt::write_json(json_file.string(), tree);
  }
}

void ImageAnnotationList::read(const std::vector<std::string> &filenames) {
}

}} // namespace fish_detector::image_annotator
