#include <boost/property_tree/json_parser.hpp>
#include <boost/algorithm/string.hpp>

#include <QProgressDialog>

#include "fish_annotator/video_annotator/video_annotation.h"
#include <iostream>
namespace fish_annotator { namespace video_annotator {

namespace fs = boost::filesystem;
namespace pt = boost::property_tree;

DetectionAnnotation::DetectionAnnotation(
  uint64_t frame,
  uint64_t id,
  const Rect &rect)
  : frame_(frame)
  , id_(id)
  , area_(rect) {
}

DetectionAnnotation::DetectionAnnotation() 
  : frame_(0)
  , id_(0)
  , area_(0, 0, 0, 0) {
}

bool DetectionAnnotation::operator==(const DetectionAnnotation &rhs) const {
  std::cout << "DETECTION 1: " << std::endl;
  std::cout << id_ << std::endl;
  std::cout << frame_ << std::endl;
  std::cout << area_.x << std::endl;
  std::cout << area_.y << std::endl;
  std::cout << area_.w << std::endl;
  std::cout << area_.h << std::endl;
  std::cout << "DETECTION 2: " << std::endl;
  std::cout << rhs.id_ << std::endl;
  std::cout << rhs.frame_ << std::endl;
  std::cout << rhs.area_.x << std::endl;
  std::cout << rhs.area_.y << std::endl;
  std::cout << rhs.area_.w << std::endl;
  std::cout << rhs.area_.h << std::endl;
  if(frame_ != rhs.frame_) return false;
  if(id_ != rhs.id_) return false;
  if(area_.x != rhs.area_.x) return false;
  if(area_.y != rhs.area_.y) return false;
  if(area_.w != rhs.area_.w) return false;
  if(area_.h != rhs.area_.h) return false;
  return true;
}

bool DetectionAnnotation::operator!=(const DetectionAnnotation &rhs) const {
  return !operator==(rhs);
}

pt::ptree DetectionAnnotation::write() const {
  pt::ptree tree;
  tree.put("frame", frame_);
  tree.put("id", id_);
  tree.put("x", area_.x);
  tree.put("y", area_.y);
  tree.put("w", area_.w);
  tree.put("h", area_.h);
  return tree;
}

void DetectionAnnotation::read(const pt::ptree &tree) {
  frame_ = tree.get<uint64_t>("frame");
  id_ = tree.get<uint64_t>("id");
  area_.x = tree.get<uint64_t>("x");
  area_.y = tree.get<uint64_t>("y");
  area_.w = tree.get<uint64_t>("w");
  area_.h = tree.get<uint64_t>("h");
}

TrackAnnotation::TrackAnnotation(
  uint64_t id,
  const std::string &species,
  const std::string &subspecies)
  : id_(id)
  , species_(species)
  , subspecies_(subspecies) {
}

TrackAnnotation::TrackAnnotation()
  : id_(0)
  , species_()
  , subspecies_() {
}

bool TrackAnnotation::operator==(const TrackAnnotation &rhs) const {
  std::cout << "TRACK 1: " << std::endl;
  std::cout << id_ << std::endl;
  std::cout << species_ << std::endl;
  std::cout << subspecies_ << std::endl;
  std::cout << "TRACK 2: " << std::endl;
  std::cout << rhs.id_ << std::endl;
  std::cout << rhs.species_ << std::endl;
  std::cout << rhs.subspecies_ << std::endl;
  if(id_ != rhs.id_) return false;
  if(species_ != rhs.species_) return false;
  if(subspecies_ != rhs.subspecies_) return false;
  return true;
}

bool TrackAnnotation::operator!=(const TrackAnnotation &rhs) const {
  return !operator==(rhs);
}

std::string TrackAnnotation::write() const {
  std::string csv_row;
  csv_row += ","; csv_row += std::to_string(id_);
  csv_row += ","; csv_row += species_;
  csv_row += ","; csv_row += subspecies_;
  return csv_row;
}

void TrackAnnotation::read(const std::string &csv_row) {
  std::vector<std::string> vals;
  boost::split(vals, csv_row, boost::is_any_of(","));
  id_ = std::stoull(vals[4]);
  species_ = vals[5];
  subspecies_ = vals[6];
}

VideoAnnotation::VideoAnnotation() 
  : detection_list_()
  , track_list_()
  , detections_by_frame_()
  , detections_by_id_()
  , tracks_by_id_()
  , tracks_by_species_() {
}

void VideoAnnotation::insert(std::shared_ptr<DetectionAnnotation> annotation) {
  remove(annotation->frame_, annotation->id_);
  detection_list_.push_front(annotation);
  detections_by_frame_.insert({annotation->frame_, detection_list_.begin()});
  detections_by_id_.insert({annotation->id_, detection_list_.begin()});
}

void VideoAnnotation::insert(std::shared_ptr<TrackAnnotation> annotation) {
  track_list_.push_front(annotation);
  tracks_by_id_.insert({annotation->id_, track_list_.begin()});
  tracks_by_species_.insert({
    {annotation->species_, annotation->subspecies_}, track_list_.begin()});
}

void VideoAnnotation::remove(uint64_t frame, uint64_t id) {
  auto range = detections_by_frame_.left.equal_range(frame);
  for(auto it = range.first; it != range.second; ++it) {
    if((*(it->second))->id_ == id) {
      std::cout << "REMOVING FRAME " << frame << ", ID " << id << std::endl;
      detection_list_.erase(it->second);
      detections_by_id_.right.erase(
        detections_by_id_.right.find(it->second));
      detections_by_frame_.left.erase(it);
      break;
    }
  }
}

void VideoAnnotation::remove(uint64_t id) {
  auto range = tracks_by_id_.left.equal_range(id);
  for(auto it = range.first; it != range.second; ++it) {
    track_list_.erase(it->second);
    tracks_by_species_.right.erase(tracks_by_species_.right.find(it->second));
    tracks_by_id_.left.erase(it);
    break;
  }
  auto drange = detections_by_id_.left.equal_range(id);
  for(auto it = drange.first; it != drange.second; ++it) {
    detection_list_.erase(it->second);
    detections_by_frame_.right.erase(
      detections_by_frame_.right.find(it->second));
  }
  detections_by_id_.left.erase(drange.first, drange.second);
}

uint64_t VideoAnnotation::nextId() {
  uint64_t max_id = 0;
  for(auto const &t : tracks_by_id_.left) {
    if(t.first > max_id) {
      max_id = t.first;
    }
  }
  return max_id + 1;
}

std::vector<std::shared_ptr<DetectionAnnotation>>
VideoAnnotation::getDetectionAnnotations(uint64_t frame) {
  std::vector<std::shared_ptr<DetectionAnnotation>> annotations;
  auto range = detections_by_frame_.left.equal_range(frame);
  for(auto it = range.first; it != range.second; ++it) {
    annotations.push_back(*(it->second));
  }
  return annotations;
}

std::map<std::string, uint64_t> VideoAnnotation::getCounts() {
  std::map<std::string, uint64_t> counts;
  for(auto const &t : tracks_by_species_.left) {
    const std::string &species = t.first.first;
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

std::vector<Species> VideoAnnotation::getAllSpecies() {
  std::vector<Species> vec;
  for(auto const &t : tracks_by_species_.left) {
    const std::string &species = t.first.first;
    const std::string &subspecies = t.first.second;
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

bool VideoAnnotation::operator==(VideoAnnotation &rhs) {
  std::cout << "COMPARING TRACK LIST SIZE..." << std::endl;
  std::cout << "SIZE 1: " << track_list_.size() << std::endl;
  std::cout << "SIZE 2: " << rhs.track_list_.size() << std::endl;
  if(track_list_.size() != rhs.track_list_.size()) return false;
  std::cout << " PASS!" << std::endl;
  std::cout << "COMPARING TRACKS...";
  auto it = tracks_by_id_.left.begin();
  auto it_rhs = rhs.tracks_by_id_.left.begin();
  for(; 
    it != tracks_by_id_.left.end() && 
    it_rhs != rhs.tracks_by_id_.left.end(); 
    ++it, ++it_rhs) {
    std::cout << "GOT HERE" << std::endl;
    std::cout << *(it->second) << std::endl;
    std::cout << *(it_rhs->second) << std::endl;
    if(**(it->second) != **(it_rhs->second)) return false;
  }
  std::cout << " PASS!" << std::endl;
  std::cout << "COMPARING DETECTION LIST SIZE..." << std::endl;;
  std::cout << "SIZE 1: " << detection_list_.size() << std::endl;
  std::cout << "SIZE 2: " << rhs.detection_list_.size() << std::endl;
  if(detection_list_.size() != rhs.detection_list_.size()) return false;
  auto dit = detections_by_frame_.left.begin();
  auto dit_rhs = rhs.detections_by_frame_.left.begin();
  for(; 
    dit != detections_by_frame_.left.end() && 
    dit_rhs != rhs.detections_by_frame_.left.end(); 
    ++dit, ++dit_rhs) {
    if(**(dit->second) != **(dit_rhs->second)) return false;
  }
  return true;
}

bool VideoAnnotation::operator!=(VideoAnnotation &rhs) {
  return !operator==(rhs);
}

void VideoAnnotation::write(const boost::filesystem::path &csv_path,
    uint64_t trip_id,
    uint64_t tow_number,
    const std::string &reviewer,
    const std::string &tow_type,
    double fps) const {
  std::unique_ptr<QProgressDialog> dlg(new QProgressDialog(
    "Saving annotations...", "Abort", 0,
    static_cast<int>(track_list_.size() + detection_list_.size())));
  dlg->setWindowModality(Qt::WindowModal);
  dlg->show();
  int iter = 0;
  std::string meta;
  meta += std::to_string(trip_id); meta += ",";
  meta += std::to_string(tow_number); meta += ",";
  meta += reviewer; meta += ",";
  meta += tow_type;
  std::ofstream csv(csv_path.string());
  csv << "Trip_ID,Tow_Number,Reviewer,Tow_Type,";
  csv << "Fish_Number,Fish_Type,Species,Frame,Time_In_Video";
  csv << std::endl;
  for(const auto &t : tracks_by_id_.left) {
    auto first_det = std::find_if(
      detections_by_frame_.left.begin(), 
      detections_by_frame_.left.end(),
      [&t](const DetectionsByInteger::left_value_type &d) {
        return t.first == (*(d.second))->id_;
      });
    csv << meta;
    csv << (*(t.second))->write();
    csv << ",";
    if(first_det == detections_by_frame_.left.end()) {
      csv << 0 << ",";
      csv << 0.0;
    }
    else {
      csv << first_det->first << ",";
      csv << static_cast<double>(first_det->first) / fps;
    }
    csv << std::endl;
    dlg->setValue(++iter);
    if(dlg->wasCanceled()) break;
  }
  pt::ptree tree;
  for(const auto &d : detections_by_frame_.left) {
    tree.add_child("Annotation Array.annotation", (*(d.second))->write());
    dlg->setValue(++iter);
    if(dlg->wasCanceled()) break;
  }
  fs::path json_path(csv_path);
  json_path.replace_extension(".json");
  pt::write_json(json_path.string(), tree);
}

void VideoAnnotation::read(const boost::filesystem::path &csv_path) {
  std::ifstream csv(csv_path.string());
  std::string line;
  std::getline(csv, line);
  for(; std::getline(csv, line);) {
    std::vector<std::string> tokens;
    boost::split(tokens, line, boost::is_any_of(","));
    insert(std::make_shared<TrackAnnotation>(
      std::stoull(tokens[4]), tokens[5], tokens[6]));
  }
  fs::path json_path(csv_path);
  json_path.replace_extension(".json");
  if(fs::exists(json_path)) {
    pt::ptree tree;
    pt::read_json(json_path.string(), tree);
    auto it = tree.find("Annotation Array");
    if(it != tree.not_found()) {
      for(auto &val : tree.get_child("Annotation Array")) {
        auto annotation = std::make_shared<DetectionAnnotation>();
        annotation->read(val.second);
        insert(annotation);
      }
    }
  }
}

}} // namespace fish_annotator::video_annotator

