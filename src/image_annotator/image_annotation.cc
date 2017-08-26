#include <boost/property_tree/json_parser.hpp>
#include <boost/algorithm/string.hpp>

#include <QProgressDialog>
#include <QMessageBox>

#include "image_annotation.h"

namespace fish_annotator { namespace image_annotator {

namespace fs = boost::filesystem;

ImageAnnotation::ImageAnnotation(
  const std::string& image_file, 
  const std::string& species,
  const std::string& subspecies,
  uint64_t id, 
  const Rect &rect,
  enum AnnotationType type)
  : image_file_(image_file)
  , species_(species)
  , subspecies_(subspecies)
  , id_(id)
  , area_(rect)
  , type_(type) {
  boost::algorithm::to_lower(species_);
  boost::algorithm::to_lower(subspecies_);
}

ImageAnnotation::ImageAnnotation()
  : image_file_()
  , species_()
  , subspecies_()
  , id_(0)
  , area_(0, 0, 0, 0)
  , type_(kBox) {
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
  if(type_ != rhs.type_) return false;
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
  switch(type_) {
    case kBox:
      tree.put("type", "box");
      break;
    case kLine:
      tree.put("type", "line");
      break;
    case kDot:
      tree.put("type", "dot");
      break;
  }
  return tree;
}

void ImageAnnotation::write_csv(std::ofstream &csv) const {
  csv << image_file_ << ",";
  csv << species_ << ",";
  csv << subspecies_ << ",";
  csv << id_ << ",";
  csv << area_.x << ",";
  csv << area_.y << ",";
  csv << area_.w << ",";
  csv << area_.h << ",";
  switch(type_) {
    case kBox:
      csv << "Box,0.0";
      break;
    case kLine: {
      double xdiff = static_cast<double>(area_.x - area_.w);
      double ydiff = static_cast<double>(area_.y - area_.h);
      double length = std::sqrt(xdiff * xdiff + ydiff * ydiff);
      csv << "Line,";
      csv << length;
      break;
    }
    case kDot:
      csv << "Dot,0.0";
      break;
  }
  csv << std::endl;
}

void ImageAnnotation::read(const pt::ptree &tree) {
  image_file_ = tree.get<std::string>("image_file");
  species_ = tree.get<std::string>("species");
  boost::algorithm::to_lower(species_);
  subspecies_ = tree.get<std::string>("subspecies");
  boost::algorithm::to_lower(subspecies_);
  id_ = tree.get<uint64_t>("id");
  uint64_t x = tree.get<uint64_t>("x");
  uint64_t y = tree.get<uint64_t>("y");
  uint64_t w = tree.get<uint64_t>("w");
  uint64_t h = tree.get<uint64_t>("h");
  area_ = Rect(x, y, w, h);
  type_ = kBox; // default
  auto opt_type_str = tree.get_optional<std::string>("type");
  if(opt_type_str != boost::none) {
    auto type_str = opt_type_str.get();
    if(type_str == "box") {
      type_ = kBox;
    }
    else if(type_str == "line") {
      type_ = kLine;
    }
    else if(type_str == "dot") {
      type_ = kDot;
    }
  }
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

void ImageAnnotationList::remove(const fs::path &image_file, uint64_t id) {
  auto range = by_file_.left.equal_range(image_file.filename().string());
  for(auto it = range.first; it != range.second; ++it) {
    if((*(it->second))->id_ == id) {
      list_.erase(it->second);
      by_species_.right.erase(by_species_.right.find(it->second));
      by_file_.left.erase(it);
      break;
    }
  }
}

uint64_t ImageAnnotationList::nextId(const fs::path &image_file) {
  auto range = by_file_.left.equal_range(image_file.filename().string());
  uint64_t max_id = 0;
  for(auto it = range.first; it != range.second; ++it) {
    if((*(it->second))->id_ > max_id) {
      max_id = (*(it->second))->id_;
    }
  }
  return max_id + 1;
}

std::vector<std::shared_ptr<ImageAnnotation>> 
ImageAnnotationList::getImageAnnotations(const fs::path &image_file) {
  std::vector<std::shared_ptr<ImageAnnotation>> annotations;
  auto range = by_file_.left.equal_range(image_file.filename().string());
  for(auto it = range.first; it != range.second; ++it) {
    annotations.push_back(*(it->second));
  }
  return annotations;
}

std::map<std::string, uint64_t> 
ImageAnnotationList::getCounts(const std::string &image_file) {
  std::map<std::string, uint64_t> counts;
  auto range = by_file_.left.equal_range(image_file);
  for(auto it = range.first; it != range.second; ++it) {
    std::string species = (*(it->second))->species_;
    auto count_it = counts.find(species);
    if(count_it != counts.end()) {
      count_it->second++;
    }
    else {
      counts.insert({species, 1});
    }
  }
  return counts;
}

std::vector<Species> ImageAnnotationList::getAllSpecies() {
  std::vector<Species> vec;
  for(const auto &elem : by_species_.left) {
    const std::string &species = elem.first.first;
    const std::string &subspecies = elem.first.second;
    auto it = std::find_if(vec.begin(), vec.end(), 
        [&species](const Species &s) {
          return species == s.getName();
        });
    if(it == vec.end()) {
      vec.push_back(Species(species));
      if(!subspecies.empty()) {
        vec.back().getSubspecies().push_back(subspecies);
      }
    }
    else if(!subspecies.empty()) {
      auto &subs = it->getSubspecies();
      auto sub_it = std::find_if(subs.begin(), subs.end(),
        [&subspecies](const std::string &s) {
          return subspecies == s;
        });
      if(sub_it == subs.end()) {
        subs.push_back(subspecies);
      }
    }
  }
  return vec;
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
  const std::vector<fs::path> &filenames) const {
  std::unique_ptr<QProgressDialog> dlg(new QProgressDialog(
    "Saving annotations...", 
    "Abort", 
    0,
    static_cast<int>(filenames.size() - 1),
    nullptr,
    Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint));
  dlg->setCancelButton(0);
  dlg->setWindowTitle("Save Annotations");
  dlg->setMinimumDuration(10);
  int iter = 0;
  fs::path sum_file(filenames[0]);
  sum_file = sum_file.parent_path();
  sum_file /= "_summary.csv";
  std::ofstream sum(sum_file.string());
  sum << "Image File,Species,Subspecies,ID,Top,Left,Width,Height,Type,Length";
  sum << std::endl;
  for(const auto &image_file : filenames) {
    fs::path csv_file(image_file);
    csv_file.replace_extension(".csv");
    std::ofstream csv(csv_file.string());
    csv << "Image File,Species,Subspecies,ID,Top,Left,Width,Height,Type,Length";
    csv << std::endl;
    pt::ptree tree;
    pt::ptree detections;
    auto range = by_file_.left.equal_range(image_file.filename().string());
    for(auto it = range.first; it != range.second; ++it) {
      (*(it->second))->write_csv(csv);
      (*(it->second))->write_csv(sum);
      detections.push_back(std::make_pair("", (*(it->second))->write()));
    }
    tree.add_child("detections", detections);
    fs::path json_file(image_file);
    json_file.replace_extension(".json");
    pt::write_json(json_file.string(), tree);
    dlg->setValue(iter++);
    if(dlg->wasCanceled()) {
      break;
    }
  }
}

void ImageAnnotationList::read(
  const std::vector<fs::path> &filenames) {
  for(const auto &image_file : filenames) {
    fs::path json_file(image_file);
    json_file.replace_extension(".json");
    if(fs::exists(json_file)) {
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
      else {
        auto it_new = tree.find("detections");
        if(it_new != tree.not_found()) {
          for(auto &val : tree.get_child("detections")) {
            auto annotation = std::make_shared<ImageAnnotation>();
            annotation->read(val.second.get_child(""));
            insert(annotation);
          }
        }
      }
    }
  }
}

}} // namespace fish_annotator::image_annotator
