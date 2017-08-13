#include <boost/property_tree/json_parser.hpp>
#include <boost/algorithm/string.hpp>

#include <QProgressDialog>
#include <QMessageBox>

#include "video_annotation.h"

namespace fish_annotator { namespace video_annotator {

namespace {
  template<typename L, typename R>
  boost::bimap<L, R> 
  makeBimap(std::initializer_list<
      typename boost::bimap<L, R>::value_type> list) {
    return boost::bimap<L, R>(list.begin(), list.end());
  }
  static const auto count_label_map = makeBimap<CountLabel, std::string>({
      {kIgnore, "ignore"},
      {kEntering, "entering"},
      {kExiting, "exiting"}});
} // namespace

namespace fs = boost::filesystem;
namespace pt = boost::property_tree;

DetectionAnnotation::DetectionAnnotation(
  uint64_t frame,
  uint64_t id,
  const Rect &rect,
  enum AnnotationType type,
  std::string species,
  double prob)
  : frame_(frame)
  , id_(id)
  , area_(rect)
  , type_(type)
  , species_(species)
  , prob_(prob) {
}

DetectionAnnotation::DetectionAnnotation()
  : frame_(0)
  , id_(0)
  , area_(0, 0, 0, 0)
  , type_(kBox)
  , species_()
  , prob_(0.0) {
}

bool DetectionAnnotation::operator==(const DetectionAnnotation &rhs) const {
  if(frame_ != rhs.frame_) return false;
  if(id_ != rhs.id_) return false;
  if(area_.x != rhs.area_.x) return false;
  if(area_.y != rhs.area_.y) return false;
  if(area_.w != rhs.area_.w) return false;
  if(area_.h != rhs.area_.h) return false;
  if(type_ != rhs.type_) return false;
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
  tree.put("species", species_);
  tree.put("prob", prob_);
  return tree;
}

void DetectionAnnotation::read(const pt::ptree &tree) {
  frame_ = tree.get<uint64_t>("frame");
  id_ = tree.get<uint64_t>("id");
  area_.x = tree.get<uint64_t>("x");
  area_.y = tree.get<uint64_t>("y");
  area_.w = tree.get<uint64_t>("w");
  area_.h = tree.get<uint64_t>("h");
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
  species_ = ""; // default
  auto opt_species = tree.get_optional<std::string>("species");
  if(opt_species != boost::none) {
    species_ = opt_species.get();
  }
  prob_ = 0.0; // default
  auto opt_prob = tree.get_optional<double>("prob");
  if(opt_prob != boost::none) {
    prob_ = opt_prob.get();
  }
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
  boost::algorithm::to_lower(species_);
  boost::algorithm::to_lower(subspecies_);
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

std::string TrackAnnotation::getSpecies() {
  return species_;
}

pt::ptree TrackAnnotation::write() const {
  pt::ptree tree;
  tree.put("id", id_);
  tree.put("species", species_);
  tree.put("subspecies", subspecies_);
  tree.put("frame_added", frame_added_);
  tree.put("count_label", count_label_map.left.at(count_label_));
  return tree;
}

void TrackAnnotation::read(const pt::ptree &tree) {
  id_ = tree.get<decltype(id_)>("id");
  species_ = tree.get<decltype(species_)>("species");
  subspecies_ = tree.get<decltype(subspecies_)>("subspecies");
  frame_added_ = tree.get<decltype(frame_added_)>("frame_added");
  auto count_label_str = count_label_map.right.at(
    tree.get<std::string>("count_label"));
}

std::string TrackAnnotation::write_csv(double fps) const {
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

void TrackAnnotation::read_csv(const std::string &csv_row) {
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
  boost::algorithm::to_lower(species_);
  subspecies_ = vals[6];
  boost::algorithm::to_lower(subspecies_);
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
VideoAnnotation::getDetectionAnnotationsByFrame(uint64_t frame) {
  std::vector<std::shared_ptr<DetectionAnnotation>> annotations;
  auto range = detections_by_frame_.left.equal_range(frame);
  for(auto it = range.first; it != range.second; ++it) {
    annotations.push_back(*(it->second));
  }
  return annotations;
}

std::vector<std::shared_ptr<DetectionAnnotation>>
VideoAnnotation::getDetectionAnnotationsById(uint64_t id) {
  std::vector<std::shared_ptr<DetectionAnnotation>> annotations;
  auto range = detections_by_id_.left.equal_range(id);
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

uint64_t VideoAnnotation::trackLastFrame(uint64_t id) {
  auto it = tracks_by_id_.left.find(id);
  if(it != tracks_by_id_.left.end()) {
    auto t = *(it->second);
    auto last_det = std::find_if(
      detections_by_frame_.left.rbegin(), 
      detections_by_frame_.left.rend(),
      [&t](const DetectionsByInteger::left_value_type &d) {
        return t->id_ == (*(d.second))->id_;
      });
    if(last_det != detections_by_frame_.left.rend()) {
      return last_det->first;
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

void VideoAnnotation::write(
    const boost::filesystem::path &json_path,
    const std::string &trip_id,
    const std::string &tow_number,
    const std::string &reviewer,
    const std::string &tow_type,
    double fps,
    bool with_csv) const {
  std::unique_ptr<QProgressDialog> dlg(new QProgressDialog(
    "Saving annotations...", "Abort", 0,
    static_cast<int>(track_list_.size() + detection_list_.size())));
  dlg->setWindowModality(Qt::WindowModal);
  dlg->show();
  int iter = 0;
  // csv file
  if(with_csv == true) {
    fs::path csv_path(json_path);
    csv_path.replace_extension(".csv");
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
      csv << (*(t.second))->write_csv(fps);
      csv << std::endl;
    }
  }
  // json file
  pt::ptree tree;
  pt::ptree tracks;
  pt::ptree detections;
  pt::ptree global_state;
  for(const auto &t : tracks_by_id_.left) {
    tracks.push_back(std::make_pair("", (*(t.second))->write()));
    dlg->setValue(++iter);
    if(dlg->wasCanceled()) break;
  }
  for(const auto &d : detections_by_frame_.left) {
    detections.push_back(std::make_pair("", (*(d.second))->write()));
    dlg->setValue(++iter);
    if(dlg->wasCanceled()) break;
  }
  for(const auto &d : degraded_by_frame_) {
    pt::ptree degraded;
    degraded.put("frame", d.first);
    degraded.put("state", "degraded");
    degraded.put("value", d.second ? 1.0 : 0.0);
    global_state.push_back(std::make_pair("", degraded));
  }
  tree.add_child("tracks", tracks);
  tree.add_child("detections", detections);
  tree.add_child("global_state", global_state);
  // video state information here
  pt::write_json(json_path.string(), tree);
}

void VideoAnnotation::read_v1(const boost::filesystem::path &json_path) {
  if(fs::exists(json_path) == false) {
    QMessageBox err;
    err.setText(std::string(
          std::string("Could not find path ") +
          json_path.string() +
          std::string("!")).c_str());
    err.exec();
  }
  else {
    pt::ptree tree;
    pt::read_json(json_path.string(), tree);
    auto it_trk = tree.find("tracks");
    auto it_det = tree.find("detections");
    auto it_gst = tree.find("global_state");
    if(it_trk == tree.not_found() ||
        it_det == tree.not_found() ||
        it_gst == tree.not_found()) {
      QMessageBox err;
      err.setText(
          "Invalid file format!\n\n"
          "If this file is legacy format, select \"Legacy Format (*.csv)\" \n"
          "when loading an annotation file.");
      err.exec();
    }
    else {
      for(auto &trk : tree.get_child("tracks")) {
        auto track = std::make_shared<TrackAnnotation>();
        track->read(trk.second.get_child(""));
        insert(track);
      }
      for(auto &det : tree.get_child("detections")) {
        auto detection = std::make_shared<DetectionAnnotation>();
        detection->read(det.second.get_child(""));
        insert(detection);
      }
      for(auto &gst : tree.get_child("global_state")) {
        auto state = gst.second.get_child("");
        if(state.get<std::string>("state") == "degraded") {
          setDegraded(
              state.get<uint64_t>("frame"),
              state.get<double>("value") > 0.5 ? true : false);
        }
      }
    }
  }
}

void VideoAnnotation::read_v0(const boost::filesystem::path &csv_path) {
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
    trk->read_csv(line);
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
