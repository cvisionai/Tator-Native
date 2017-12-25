/// @file
/// @brief Defines classes that store annotation information for videos.

#ifndef VIDEO_ANNOTATION_H
#define VIDEO_ANNOTATION_H

#include <string>
#include <vector>
#include <list>
#include <map>
#include <memory>

#include <boost/property_tree/ptree.hpp>
#include <boost/bimap.hpp>
#include <boost/bimap/multiset_of.hpp>
#include <boost/filesystem.hpp>

#include "serialization.h"
#include "rect.h"
#include "species.h"
#include "annotation_scene.h"
#include "global_state_annotation.h"

#ifndef NO_TESTING
class TestVideoAnnotation;
#endif

namespace fish_annotator { namespace video_annotator {

namespace pt = boost::property_tree;

/// Gets a required field from a property tree.
///
/// This function creates an error message box on failure, and also
/// returns an error if the parent function should discontinue.
///
/// @tparam FieldType Type of the field.
/// @param tree Property tree to read from.
/// @param name Name of the field.
/// @param value Value is read into this variable.
template<typename FieldType>
void getRequired(const pt::ptree &tree, const char *name, FieldType &value) {
  auto it = tree.find(name);
  if(it == tree.not_found()) {
    QMessageBox err;
    err.setText(QString("Could not find required field %1.  A default "
          "value will be used instead.").arg(name));
    err.exec();
  }
  else {
    value = tree.get<FieldType>(name);
  }
}


/// Defines annotation information for one detection.
struct DetectionAnnotation : public Serialization {
  /// Constructor.
  ///
  /// @param frame Frame associated with the annotation.
  /// @param id ID of the individual.
  /// @param rect Rectangle defining the annotation.
  /// @param type Shape of the annotation.
  /// @param species Species identified by this detection.
  /// @param prob Probability associated with this detection.
  DetectionAnnotation(
    uint64_t frame,
    uint64_t id,
    const Rect &rect,
    enum AnnotationType type,
    std::string species,
    double prob);

  /// Default constructor.
  DetectionAnnotation();

  /// Equality operator.
  ///
  /// @param rhs Right hand side argument.
  /// @return Whether the object is equal to rhs.
  bool operator==(const DetectionAnnotation &rhs) const;

  /// Inequality operator.
  ///
  /// @param rhs Right hand side argument.
  /// @return Whether the object is not equal to rhs.
  bool operator!=(const DetectionAnnotation &rhs) const;

  /// Writes to a property tree.
  ///
  /// @return Property tree constructed from the object.
  pt::ptree write() const;

  /// Reads from a property tree.
  ///
  /// @param tree Property tree to be read.
  void read(const pt::ptree &tree);

  uint64_t frame_; ///< Frame of this annotation.
  uint64_t id_; ///< ID of the individual.
  Rect area_; ///< Rectangle defining the annotation.
  enum AnnotationType type_; ///< Annotation type.
  std::string species_; ///< Species.
  double prob_; ///< Detection probability.
};

/// List of detection annotations.
typedef std::list<std::shared_ptr<DetectionAnnotation>> DetectionList;

/// Makes iterator sortable so it can be used as key in maps.
///
/// Works by converting both iterators to pointers and applying operator<.
///
/// @param lhs Left hand side argument.
/// @param rhs Right hand side argument.
inline bool operator<(
  const DetectionList::iterator& lhs,
  const DetectionList::iterator& rhs) {
  return &(*lhs) < &(*rhs);
}

/// Specifies how a track should contribute to overall fish count.
enum CountLabel {
  kIgnore,
  kEntering,
  kExiting
};

/// Defines annotation information for a track.
struct TrackAnnotation {
  /// Constructor.
  ///
  /// @param id ID of the individual.
  /// @param species Species of the individual.
  /// @param subspecies Subspecies of the individual.
  /// @param frame_added Frame that individual was added.
  /// @param count_label How this track contributes to overall count.
  TrackAnnotation(
    uint64_t id,
    const std::string &species,
    const std::string &subspecies,
    uint64_t frame_added,
    CountLabel count_label);

  /// Default constructor.
  TrackAnnotation();

  /// Equality operator.
  ///
  /// @param rhs Right hand side argument.
  /// @return Whether the object is equal to rhs.
  bool operator==(const TrackAnnotation &rhs) const;

  /// Inequality operator.
  ///
  /// @param rhs Right hand side argument.
  /// @return Whether the object is not equal to rhs.
  bool operator!=(const TrackAnnotation &rhs) const;

  /// returns the species of the track.
  ///
  /// @return string containing species of track.
  std::string getSpecies();

  /// Writes to a property tree.
  ///
  /// @return Property tree constructed from the object.
  pt::ptree write() const;

  /// Reads from a property tree.
  ///
  /// @param tree Property tree to be read.
  void read(const pt::ptree &tree);

  /// Writes to a string containing comma separated values.
  ///
  /// @param fps Frames per second of the video.
  /// @return String containing comma separated values.
  std::string write_csv(double fps) const;

  /// Reads from a string containing comma separated values.
  ///
  /// @param csv_row String to be read.
  void read_csv(const std::string &csv_row);

  uint64_t id_; ///< ID of the individual.
  std::string species_; ///< Species of the individual.
  std::string subspecies_; ///< Subspecies of the individual.
  uint64_t frame_added_; ///< Frame that individual was added.
  CountLabel count_label_; ///< How this track contributes to overall count.
};

/// List of track annotations.
typedef std::list<std::shared_ptr<TrackAnnotation>> TrackList;

/// Makes iterator sortable so it can be used as key in maps.
///
/// Works by converting both iterators to pointers and applying operator<.
///
/// @param lhs Left hand side argument.
/// @param rhs Right hand side argument.
inline bool operator<(
  const TrackList::iterator& lhs,
  const TrackList::iterator& rhs) {
  return &(*lhs) < &(*rhs);
}

/// Defines annotation information for a video.
class VideoAnnotation {
#ifndef NO_TESTING
  friend class ::TestVideoAnnotation;
#endif
public:
  /// Constructor.
  VideoAnnotation();

  /// Sets length of video.
  ///
  /// @param video_length Length of video being annotated.
  void setVideoLength(uint64_t video_length);

  /// Inserts a detection annotation.
  ///
  /// @param annotation Annotation to be inserted.
  void insert(std::shared_ptr<DetectionAnnotation> annotation);

  /// Inserts a track annotation.
  ///
  /// @param annotation Annotation to be inserted.
  void insert(std::shared_ptr<TrackAnnotation> annotation);

  /// Removes a detection annotation.
  ///
  /// @param frame Frame of the annotation.
  /// @param id ID of the individual associated with the detection.
  void remove(uint64_t frame, uint64_t id);

  /// Removes a track annotation.
  ///
  /// Also removes all detections with the same ID.
  ///
  /// @param id ID of the individual associated with the track.
  void remove(uint64_t id);

  /// Gets next assignable ID for a video.
  ///
  /// @return Next assignable ID.
  uint64_t nextId();

  /// Gets annotations for a given frame.
  ///
  /// @param frame Frame in the video.
  /// @return Annotations for the given frame.
  std::vector<std::shared_ptr<DetectionAnnotation>>
    getDetectionAnnotationsByFrame(uint64_t frame);

  /// Gets annotations for a given ID.
  ///
  /// @param id Track ID.
  /// @return Annotations for the given frame.
  std::vector<std::shared_ptr<DetectionAnnotation>>
    getDetectionAnnotationsById(uint64_t id);

  /// Gets counts for each species in a video.
  ///
  /// @return Counts for each species in the annotations.
  std::map<std::string, uint64_t> getCounts();

  /// Gets all species in the annotations.
  ///
  /// @return All species in annotations.
  std::vector<Species> getAllSpecies();

  /// Gets total tracks in the video.
  ///
  /// @return Total tracks in entire video.
  uint64_t getTotal();

  /// Clears all annotations.
  void clear();

  /// Find detection for a given frame and ID.
  ///
  /// @param frame Detection frame.
  /// @param id Track ID.
  /// @return Shared pointer to detection annotation, nullptr if not found.
  std::shared_ptr<DetectionAnnotation>
  findDetection(uint64_t frame, uint64_t id);

  /// Find track for a given ID.
  ///
  /// @param id Track ID.
  /// @return Shared pointer to track annotation, nullptr if not found.
  std::shared_ptr<TrackAnnotation> findTrack(uint64_t id);

  /// Retrieves next track after given ID.
  ///
  /// @param id Track ID.
  /// @return Shared pointer to track annotation, nullptr if not found.
  std::shared_ptr<TrackAnnotation> nextTrack(uint64_t id);

  /// Retrieves previous track before given ID.
  ///
  /// @param id Track ID.
  /// @return Shared pointer to track annotation, nullptr if not found.
  std::shared_ptr<TrackAnnotation> prevTrack(uint64_t id);

  /// Gets first frame where a track occurs.
  ///
  /// @param id Track ID.
  /// @return Frame of first occurrence, zero if not found or no detections.
  uint64_t trackFirstFrame(uint64_t id);

  /// Gets last frame where a track occurs.
  ///
  /// @param id Track ID.
  /// @return Frame of last occurrence, zero if not found or no detections.
  uint64_t trackLastFrame(uint64_t id);

  /// Gets ID of earliest track.
  ///
  /// @return ID of earliest track.
  uint64_t earliestTrackID();

  /// Inserts global state annotation.
  ///
  /// @param frame Frame to insert.
  /// @param ann Global state annotation for this frame.
  void insertGlobalStateAnnotation(
    uint64_t frame,
    const GlobalStateAnnotation &ann);

  /// Gets global state annotation corresponding to a given frame.
  ///
  /// @param frame Frame to retrieve.  If this frame does not exist in the 
  ///   map, it returns the annotation corresponding to the nearest frame
  ///   that is less than the requested one.
  GlobalStateAnnotation getGlobalStateAnnotation(const uint64_t frame);

  /// Equality operator.
  ///
  /// @param rhs Right hand side argument.
  /// @return Whether the object is equal to rhs.
  bool operator==(VideoAnnotation &rhs);

  /// Inequality operator.
  ///
  /// @param rhs Right hand side argument.
  /// @return Whether the object is not equal to rhs.
  bool operator!=(VideoAnnotation &rhs);

  /// Writes annotations to file.
  ///
  /// Writes a json file and optionally a csv file.  The json file contains
  /// all video annotation information, and the csv file contains
  /// a track level summary of the annotations.  The csv file has the same
  /// path as the input json path with different extension.
  ///
  /// @param json_path Path to json file.
  /// @param trip_id Trip ID.
  /// @param tow_number Tow number.
  /// @param reviewer Name of reviewer.
  /// @param tow_type Tow type.
  /// @param fps Native frames per second of the video.
  /// @param with_csv True to include csv file in output.
  void write(
    const boost::filesystem::path &json_path,
    const std::string &trip_id,
    const std::string &tow_number,
    const std::string &reviewer,
    const std::string &tow_type,
    double fps,
    bool with_csv) const;

  /// Reads annotations from json files.
  ///
  /// @param json_path Path to json file.
  void read(const boost::filesystem::path &json_path);

private:
  /// For mapping integers to detection annotations.
  typedef boost::bimap<
    boost::bimaps::multiset_of<uint64_t>,
    boost::bimaps::multiset_of<DetectionList::iterator>> DetectionsByInteger;

  /// For mapping unique integers to track annotations.
  typedef boost::bimap<
    uint64_t,
    boost::bimaps::multiset_of<TrackList::iterator>> TracksByUniqueInteger;

  /// For mapping integers to track annotations.
  typedef boost::bimap<
    boost::bimaps::multiset_of<uint64_t>,
    boost::bimaps::multiset_of<TrackList::iterator>> TracksByInteger;

  /// For mapping pair of strings to track annotations.
  typedef boost::bimap<
    boost::bimaps::multiset_of<std::pair<std::string, std::string>>,
    boost::bimaps::multiset_of<TrackList::iterator>> TracksByStringPair;

  /// List of detection annotations.
  DetectionList detection_list_;

  /// List of track annotations.
  TrackList track_list_;

  /// Map between frame and iterator to detection annotations.
  DetectionsByInteger detections_by_frame_;

  /// Map between id and iterator to detection annotations.
  DetectionsByInteger detections_by_id_;

  /// Map between id and iterator to track annotations.
  TracksByUniqueInteger tracks_by_id_;

  /// Map between species/subspecies and iterator to track annotations.
  TracksByStringPair tracks_by_species_;

  /// Map between frame added and iterator to track annotations.
  TracksByInteger tracks_by_frame_added_;

  /// Map between frame and global state annotations.
  std::map<uint64_t, GlobalStateAnnotation> global_states_;

  /// Length of the video being annotated.
  uint64_t video_length_;

  /// Verifies that frame number is in bounds.
  ///
  /// If a frame is out of bounds, it is capped so that it is in bounds.
  ///
  /// @param frame Frame number to check.
  void boundFrame(uint64_t &frame);
};

}} // namespace fish_annotator::video_annotator

#endif // VIDEO_ANNOTATION_H
