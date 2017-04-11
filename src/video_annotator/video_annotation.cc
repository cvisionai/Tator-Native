#include <boost/property_tree/json_parser.hpp>
#include <boost/algorithm/string.hpp>

#include <QProgressDialog>
#include <QMessageBox>

#include "fish_annotator/video_annotator/video_annotation.h"

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
  const std::string &subspecies,
  uint64_t frame_added,
  CountLabel count_label)
  : id_(id)
  , species_(species)
  , subspecies_(subspecies)
  , frame_added_(frame_added)
  , count_label_(count_label) {
}

TrackAnnotation::TrackAnnotation()
  : id_(0)
  , species_()
  , subspecies_() 
  , frame_added_(0)
  , count_label_(kIgnore) {
}

bool TrackAnnotation::operator==(const TrackAnnotation &rhs) const {
  if(id_ != rhs.id_) return false;
  if(species_ != rhs.species_) return false;
  if(subspecies_ != rhs.subspecies_) return false;
  if(frame_added_ != rhs.frame_added_) return false;
  if(count_label_ != rhs.count_label_) return false;
  return true;
}

bool TrackAnnotation::operator!=(const TrackAnnotation &rhs) const {
  return !operator==(rhs);
}

std::string TrackAnnotation::write(double fps) const {
  std::string csv_row;
  double time_added = static_cast<double>(frame_added_) / fps;
  csv_row += ","; csv_row += std::to_string(id_);
  csv_row += ","; csv_row += species_;
  csv_row += ","; csv_row += subspecies_;
  csv_row += ","; csv_row += std::to_string(frame_added_);
  csv_row += ","; csv_row += std::to_string(time_added);
  switch(count_label_) {
    case kIgnore: csv_row += ",Ignore"; break;
    case kEntering: csv_row += ",Entering"; break;
    case kExiting: csv_row += ",Exiting"; break;
  }
  return csv_row;
}

void TrackAnnotation::read(const std::string &csv_row) {
  std::vector<std::string> vals;
  boost::split(vals, csv_row, boost::is_any_of(","));
  if(vals.size() < 8) {
    QMessageBox err;
    err.setText("Invalid number of columns in track annotation file (csv)!");
    err.exec();
    return;
  }
  id_ = std::stoull(vals[4]);
  species_ = vals[5];
  subspecies_ = vals[6];
  frame_added_ = std::stoull(vals[7]);
  if(vals.size() > 9) {
    if(vals[9].find("Ignore") != std::string::npos) {
      count_label_ = kIgnore;
    }
    else if(vals[9].find("Entering") != std::string::npos) {
      count_label_ = kEntering;
    }
    else if(vals[9].find("Exiting") != std::string::npos) {
      count_label_ = kExiting;
    }
  }
}

VideoAnnotation::VideoAnnotation() 
  : detection_list_()
  , track_list_()
  , detections_by_frame_()
  , detections_by_id_()
  , tracks_by_id_()
  , tracks_by_species_()
  , tracks_by_frame_added_() 
  , degraded_by_frame_() {
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
  tracks_by_frame_added_.insert(
    {annotation->frame_added_, track_list_.begin()});
}

void VideoAnnotation::remove(uint64_t frame, uint64_t id) {
  auto range = detections_by_frame_.left.equal_range(frame);
  for(auto it = range.first; it != range.second; ++it) {
    if((*(it->second))->id_ == id) {
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
    tracks_by_frame_added_.right.erase(tracks_by_frame_added_.right.find(
          it->second));
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

uint64_t VideoAnnotation::getTotal() {
  return track_list_.size();
}

void VideoAnnotation::clear() {
  track_list_.clear();
  detections_by_frame_.clear();
  detections_by_id_.clear();
  tracks_by_id_.clear();
  tracks_by_species_.clear();
  tracks_by_frame_added_.clear();
}

std::shared_ptr<DetectionAnnotation> 
VideoAnnotation::findDetection(uint64_t frame, uint64_t id) {
  auto range = detections_by_frame_.left.equal_range(frame);
  for(auto it = range.first; it != range.second; ++it) {
    if((*(it->second))->id_ == id) {
      return *(it->second);
    }
  }
  return std::shared_ptr<DetectionAnnotation>(nullptr);
}

std::shared_ptr<TrackAnnotation> VideoAnnotation::findTrack(uint64_t id) {
  auto it = tracks_by_id_.left.find(id);
  if(it != tracks_by_id_.left.end()) {
    return *(it->second);
  }
  return std::shared_ptr<TrackAnnotation>(nullptr);
}

std::shared_ptr<TrackAnnotation> VideoAnnotation::nextTrack(uint64_t id) {
  auto id_it = tracks_by_id_.left.find(id);
  if(id_it != tracks_by_id_.left.end()) {
    auto frame_it_right = tracks_by_frame_added_.right.find(id_it->second);
    if(frame_it_right != tracks_by_frame_added_.right.end()) {
      auto frame_it_left = tracks_by_frame_added_.project_left(frame_it_right);
      if(std::next(frame_it_left) != tracks_by_frame_added_.left.end()) {
        return *(std::next(frame_it_left)->second);
      }
    }
  }
  return std::shared_ptr<TrackAnnotation>(nullptr);
}

std::shared_ptr<TrackAnnotation> VideoAnnotation::prevTrack(uint64_t id) {
  auto id_it = tracks_by_id_.left.find(id);
  if(id_it != tracks_by_id_.left.end()) {
    auto frame_it_right = tracks_by_frame_added_.right.find(id_it->second);
    if(frame_it_right != tracks_by_frame_added_.right.end()) {
      auto frame_it_left = tracks_by_frame_added_.project_left(frame_it_right);
      if(std::prev(frame_it_left) != tracks_by_frame_added_.left.end()) {
        return *(std::prev(frame_it_left)->second);
      }
    }
  }
  return std::shared_ptr<TrackAnnotation>(nullptr);
}

uint64_t VideoAnnotation::trackFirstFrame(uint64_t id) {
  auto it = tracks_by_id_.left.find(id);
  if(it != tracks_by_id_.left.end()) {
    auto t = *(it->second);
    auto first_det = std::find_if(
      detections_by_frame_.left.begin(), 
      detections_by_frame_.left.end(),
      [&t](const DetectionsByInteger::left_value_type &d) {
        return t->id_ == (*(d.second))->id_;
      });
    if(first_det != detections_by_frame_.left.end()) {
      return first_det->first;
    }
  }
  return 0;
}

uint64_t VideoAnnotation::earliestTrackID() {
  if(tracks_by_id_.left.size() == 0) {
    return 0;
  }
  else {
    return (*(tracks_by_id_.left.begin()->second))->id_;
  }
}

void VideoAnnotation::setDegraded(uint64_t frame, bool degraded) {
  degraded_by_frame_[frame] = degraded;
}

bool VideoAnnotation::isDegraded(uint64_t frame) {
  if(degraded_by_frame_.size() == 0) return false;
  auto it = degraded_by_frame_.upper_bound(frame);
  if(it != degraded_by_frame_.begin()) {
    --it;
    return it->second;
  }
  else {
    return false;
  }
}

bool VideoAnnotation::operator==(VideoAnnotation &rhs) {
  if(track_list_.size() != rhs.track_list_.size()) return false;
  auto it = tracks_by_id_.left.begin();
  auto it_rhs = rhs.tracks_by_id_.left.begin();
  for(; 
    it != tracks_by_id_.left.end() && 
    it_rhs != rhs.tracks_by_id_.left.end(); 
    ++it, ++it_rhs) {
    if(**(it->second) != **(it_rhs->second)) return false;
  }
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
    const std::string &trip_id,
    const std::string &tow_number,
    const std::string &reviewer,
    const std::string &tow_type,
    double fps) const {
  std::unique_ptr<QProgressDialog> dlg(new QProgressDialog(
    "Saving annotations...", "Abort", 0,
    static_cast<int>(track_list_.size() + detection_list_.size())));
  dlg->setWindowModality(Qt::WindowModal);
  dlg->show();
  // Track file
  int iter = 0;
  std::string meta;
  meta += trip_id; meta += ",";
  meta += tow_number; meta += ",";
  meta += reviewer; meta += ",";
  meta += tow_type;
  std::ofstream csv(csv_path.string());
  csv << "Trip_ID,Tow_Number,Reviewer,Tow_Type,";
  csv << "Fish_Number,Fish_Type,Species,Frame,Time_In_Video";
  csv << std::endl;
  for(const auto &t : tracks_by_id_.left) {
    csv << meta;
    csv << (*(t.second))->write(fps);
    csv << std::endl;
    dlg->setValue(++iter);
    if(dlg->wasCanceled()) break;
  }
  // Detection file
  pt::ptree tree;
  pt::ptree ann_array;
  for(const auto &d : detections_by_frame_.left) {
    pt::ptree ann;
    pt::ptree ann_val = (*(d.second))->write();
    ann.add_child("annotation", ann_val);
    ann_array.push_back(std::make_pair("", ann));
    dlg->setValue(++iter);
    if(dlg->wasCanceled()) break;
  }
  tree.add_child("Annotation Array", ann_array);
  fs::path json_path(csv_path);
  json_path.replace_extension(".json");
  pt::write_json(json_path.string(), tree);
  // Degraded state file
  fs::path csv1_path(csv_path);
  csv1_path.replace_extension(".csv1");
  std::ofstream csv1(csv1_path.string());
  csv1 << "Frame,Degraded_State" << std::endl;
  for(const auto &d : degraded_by_frame_) {
    csv1 << std::to_string(d.first) << ",";
    csv1 << (d.second ? "degraded" : "visible");
    csv1 << std::endl;
  }
}

void VideoAnnotation::read(const boost::filesystem::path &csv_path) {
  std::ifstream f(csv_path.string());
  int num_lines = std::count(
      std::istreambuf_iterator<char>(f),
      std::istreambuf_iterator<char>(),
      '\n');
  f.close();
  std::unique_ptr<QProgressDialog> dlg(new QProgressDialog(
    "Loading annotations...", "Abort", 0, 2 * num_lines));
  dlg->setWindowModality(Qt::WindowModal);
  dlg->show();
  // Track file
  int iter = 0;
  std::ifstream csv(csv_path.string());
  std::string line;
  std::getline(csv, line);
  for(; std::getline(csv, line);) {
    auto trk = std::make_shared<TrackAnnotation>();
    trk->read(line);
    insert(trk);
    dlg->setValue(++iter);
    if(dlg->wasCanceled()) break;
  }
  // Detection file
  fs::path json_path(csv_path);
  json_path.replace_extension(".json");
  if(fs::exists(json_path)) {
    pt::ptree tree;
    pt::read_json(json_path.string(), tree);
    auto it = tree.find("Annotation Array");
    if(it != tree.not_found()) {
      for(auto &val : tree.get_child("Annotation Array")) {
        auto annotation = std::make_shared<DetectionAnnotation>();
        annotation->read(val.second.get_child("annotation"));
        insert(annotation);
        if(dlg->wasCanceled()) break;
      }
      dlg->setValue(2 * num_lines);
    }
  }
  // Degraded state file
  fs::path csv1_path(csv_path);
  csv1_path.replace_extension(".csv1");
  if(fs::exists(csv1_path)) {
    std::ifstream csv1(csv1_path.string());
    std::string line1;
    std::getline(csv1, line1);
    for(; std::getline(csv1, line1);) {
      std::vector<std::string> tokens;
      boost::split(tokens, line1, boost::is_any_of(","));
      if(tokens[1] == "degraded") {
        setDegraded(std::stoull(tokens[0]), true);
      }
      else if(tokens[1] == "visible") {
        setDegraded(std::stoull(tokens[0]), false);
      }
    }
  }
}

}} // namespace fish_annotator::video_annotator

