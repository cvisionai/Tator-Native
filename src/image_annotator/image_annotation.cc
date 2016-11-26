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
  , area_(rect) {
}

ImageAnnotation::ImageAnnotation()
  : image_file_()
  , species_()
  , subspecies_()
  , id_(0)
  , area_(0, 0, 0, 0) {
}

bool ImageAnnotation::operator==(const ImageAnnotation &rhs) const {
  if(image_file_ != rhs.image_file_) return false;
  if(species_ != rhs.species_) return false;
  if(subspecies_ != rhs.subspecies_) return false;
  if(id_ != rhs.id_) return false;
  if(area_.x != rhs.area_.x) return false;
  if(area_.y != rhs.area_.y) return false;
  if(area_.w != rhs.area_.w) return false;
  if(area_.h != rhs.area_.h) return false;
  return true;
}

bool ImageAnnotation::operator!=(const ImageAnnotation &rhs) const {
  return !operator==(rhs);
}

pt::ptree ImageAnnotation::write() const {
  pt::ptree tree;
  tree.put("image_file", image_file_);
  tree.put("species", species_);
  tree.put("subspecies", subspecies_);
  tree.put("id", id_);
  tree.put("x", area_.x);
  tree.put("y", area_.y);
  tree.put("w", area_.w);
  tree.put("h", area_.h);
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
  area_ = Rect(x, y, w, h);
}

ImageAnnotationList::ImageAnnotationList()
  : list_()
  , by_file_()
  , by_species_() {
}

void ImageAnnotationList::insert(std::shared_ptr<ImageAnnotation> annotation) {
  list_.push_front(annotation);
  by_file_.insert({annotation->image_file_, list_.begin()});
  by_species_.insert({{annotation->species_, annotation->subspecies_}, 
                      list_.begin()});
}

void ImageAnnotationList::remove(const std::string &image_file, uint64_t id) {
  auto range = by_file_.left.equal_range(image_file);
  for(auto it = range.first; it != range.second; ++it) {
    if((*(it->second))->id_ == id) {
      list_.erase(it->second);
      by_species_.right.erase(by_species_.right.find(it->second));
      by_file_.left.erase(it);
      break;
    }
  }
}

uint64_t ImageAnnotationList::nextId(const std::string &image_file) {
  auto range = by_file_.left.equal_range(image_file);
  uint64_t max_id = 1;
  for(auto it = range.first; it != range.second; ++it) {
    if((*(it->second))->id_ > max_id) {
      max_id = (*(it->second))->id_;
    }
  }
  return max_id + 1;
}

bool ImageAnnotationList::operator==(ImageAnnotationList &rhs) {
  if(list_.size() != rhs.list_.size()) return false;
  auto it = list_.begin();
  auto it_rhs = rhs.list_.begin();
  for(; it != list_.end() && it_rhs != rhs.list_.end(); ++it, ++it_rhs) {
    if(**it != **it_rhs) return false;
  }
  return true;
}

void ImageAnnotationList::write(
  const std::vector<std::string> &filenames) const {
  for(auto &image_file : filenames) {
    pt::ptree tree;
    auto range = by_file_.left.equal_range(image_file);
    for(auto it = range.first; it != range.second; ++it) {
      tree.add_child("annotation_list.annotation", (*(it->second))->write());
    }
    boost::filesystem::path json_file(image_file);
    json_file.replace_extension(".json");
    pt::write_json(json_file.string(), tree);
  }
}

void ImageAnnotationList::read(const std::vector<std::string> &filenames) {
  for(auto &image_file : filenames) {
    boost::filesystem::path json_file(image_file);
    json_file.replace_extension(".json");
    if(boost::filesystem::exists(json_file)) {
      pt::ptree tree;
      pt::read_json(json_file.string(), tree);
      auto it = tree.find("annotation_list");
      if(it != tree.not_found()) {
        for(auto &val : tree.get_child("annotation_list")) {
          auto annotation = std::make_shared<ImageAnnotation>();
          annotation->read(val.second);
          insert(annotation);
        }
      }
    }
  }
}

}} // namespace fish_detector::image_annotator
