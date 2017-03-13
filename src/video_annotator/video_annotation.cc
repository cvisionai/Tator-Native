#include <boost/property_tree/json_parser.hpp>
#include <boost/algorithm/string/split.hpp>

#include <QProgressDialog>

#include "fish_annotator/video_annotator/video_annotation.h"

namespace fish_annotator { namespace video_annotator {

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
  const std::string &subspecies)
  : id_(id),
  , species_(species),
  , subspecies_(subspecies) {
}

TrackAnnotation::TrackAnnotation()
  : id_(0)
  , species_()
  , subspecies() {
}

bool TrackAnnotation::operator==(const TrackAnnotation &rhs) const {
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
  csv_row << "," << id_;
  csv_row << "," << species_;
  csv_row << "," << subspecies;
}

void TrackAnnotation::read(const std::string &csv_row) {
  std::vector<std::string> vals;
  boost::split(vals, csv_row, boost::is_any_of(","));
  id_ = std::stoi(vals[4]);
  species_ = vals[5];
  subspecies_ = vals[6];
}

VideoAnnotation::VideoAnnotation() {
}

void VideoAnnotation::insert(std::shared_ptr<DetectionAnnotation> annotation) {
}

void VideoAnnotation::remove(uint64_t frame, uint64_t id) {
}

uint64_t VideoAnnotation::nextId(uint64_t frame) {
}

std::vector<std::shared_ptr<DetectionAnnotation>>
VideoAnnotation::getDetectionAnnotations(uint64_t frame) {
}

std::map<uint64_t, uint64_t> VideoAnnotation::getCounts(uint64_t frame) {
}

std::vector<Species> VideoAnnotation::getAllSpecies() {
}

bool VideoAnnotation::operator==(VideoAnnotation &rhs) {
}

bool VideoAnnotation::operator!=(VideoAnnotation &rhs) {
}

void VideoAnnotation::write(const boost::filesystem::path &csv_path) const {
}

void VideoAnnotation::read(const boost::filesystem::path &csv_path) {
}


}} // namespace fish_annotator::video_annotator

