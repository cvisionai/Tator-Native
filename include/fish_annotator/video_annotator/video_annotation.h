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

#include "fish_annotator/common/serialization.h"
#include "fish_annotator/common/rect.h"
#include "fish_annotator/common/species.h"

#ifndef NO_TESTING
class TestVideoAnnotation;
#endif

namespace fish_annotator { namespace video_annotator {

namespace pt = boost::property_tree;

/// @brief Defines annotation information for one detection.
struct DetectionAnnotation : public Serialization {
  /// @brief Constructor.
  ///
  /// @param frame Frame associated with the annotation.
  /// @param id ID of the individual.
  /// @param rect Rectangle defining the annotation.
  DetectionAnnotation(
    uint64_t frame,
    uint64_t id,
    const Rect &rect);

  /// @brief Default constructor.
  DetectionAnnotation();

  /// @brief Equality operator.
  ///
  /// @param rhs Right hand side argument.
  /// @return Whether the object is equal to rhs.
  bool operator==(const DetectionAnnotation &rhs) const;

  /// @brief Inequality operator.
  ///
  /// @param rhs Right hand side argument.
  /// @return Whether the object is not equal to rhs.
  bool operator!=(const DetectionAnnotation &rhs) const;

  /// @brief Writes to a property tree.
  ///
  /// @return Property tree constructed from the object.
  pt::ptree write() const;

  /// @brief Reads from a property tree.
  ///
  /// @param tree Property tree to be read.
  void read(const pt::ptree &tree);

  uint64_t frame_; ///< Frame of this annotation.
  uint64_t id_; ///< ID of the individual.
  Rect area_; ///< Rectangle defining the annotation.
};

/// @brief List of detection annotations.
typedef std::list<std::shared_ptr<DetectionAnnotation>> DetectionList;

/// @brief Makes iterator sortable so it can be used as key in maps.
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

/// @brief Defines annotation information for a track.
struct TrackAnnotation {
  /// @brief Constructor.
  ///
  /// @param ID of the individual.
  /// @param species Species of the individual.
  /// @param subspecies Subspecies of the individual.
  /// @param frame_added Frame that individual was added.
  TrackAnnotation(
    uint64_t id,
    const std::string &species,
    const std::string &subspecies,
    uint64_t frame_added);

  /// @brief Default constructor.
  TrackAnnotation();

  /// @brief Equality operator.
  ///
  /// @param rhs Right hand side argument.
  /// @return Whether the object is equal to rhs.
  bool operator==(const TrackAnnotation &rhs) const;

  /// @brief Inequality operator.
  ///
  /// @param rhs Right hand side argument.
  /// @return Whether the object is not equal to rhs.
  bool operator!=(const TrackAnnotation &rhs) const;

  /// @brief Writes to a string containing comma separated values.
  ///
  /// @param fps Frames per second of the video.
  /// @return String containing comma separated values.
  std::string write(double fps) const;

  /// @brief Reads from a string containing comma separated values.
  ///
  /// @param csv_row String to be read.
  void read(const std::string &csv_row);

  uint64_t id_; ///< ID of the individual.
  std::string species_; ///< Species of the individual.
  std::string subspecies_; ///< Subspecies of the individual.
  uint64_t frame_added_; ///< Frame that individual was added.
};

/// @brief List of track annotations.
typedef std::list<std::shared_ptr<TrackAnnotation>> TrackList;

/// @brief Makes iterator sortable so it can be used as key in maps.
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

/// @brief Defines annotation information for a video.
class VideoAnnotation {
#ifndef NO_TESTING
  friend class ::TestVideoAnnotation;
#endif
public:
  /// @brief Constructor.
  VideoAnnotation();

  /// @brief Inserts a detection annotation.
  ///
  /// @param annotation Annotation to be inserted.
  void insert(std::shared_ptr<DetectionAnnotation> annotation);

  /// @brief Inserts a track annotation.
  ///
  /// @param annotation Annotation to be inserted.
  void insert(std::shared_ptr<TrackAnnotation> annotation);

  /// @brief Removes a detection annotation.
  ///
  /// @param frame Frame of the annotation.
  /// @param id ID of the individual associated with the detection.
  void remove(uint64_t frame, uint64_t id);

  /// @brief Removes a track annotation.
  ///
  /// Also removes all detections with the same ID.
  ///
  /// @param id ID of the individual associated with the track.
  void remove(uint64_t id);

  /// @brief Gets next assignable ID for a video.
  ///
  /// @return Next assignable ID.
  uint64_t nextId();

  /// @brief Gets annotations for a given frame.
  ///
  /// @param frame Frame in the video.
  /// @return Annotations for the given frame.
  std::vector<std::shared_ptr<DetectionAnnotation>>
    getDetectionAnnotations(uint64_t frame);

  /// @brief Gets counts for each species in a video.
  ///
  /// @return Counts for each species in the annotations.
  std::map<std::string, uint64_t> getCounts();

  /// @brief Gets all species in the annotations.
  ///
  /// @return All species in annotations.
  std::vector<Species> getAllSpecies();

  /// @brief Gets total tracks in the video.
  ///
  /// @return Total tracks in entire video.
  uint64_t getTotal();

  /// @brief Clears all annotations.
  void clear();

  /// @brief Find detection for a given frame and ID.
  ///
  /// @param frame Detection frame.
  /// @param id Track ID.
  /// @return Shared pointer to detection annotation, nullptr if not found.
  std::shared_ptr<DetectionAnnotation> 
  findDetection(uint64_t frame, uint64_t id);

  /// @brief Find track for a given ID.
  ///
  /// @param id Track ID.
  /// @return Shared pointer to track annotation, nullptr if not found.
  std::shared_ptr<TrackAnnotation> findTrack(uint64_t id);

  /// @brief Retrieves next track after given ID.
  ///
  /// @param id Track ID.
  /// @return Shared pointer to track annotation, nullptr if not found.
  std::shared_ptr<TrackAnnotation> nextTrack(uint64_t id);

  /// @brief Retrieves previous track before given ID.
  ///
  /// @param id Track ID.
  /// @return Shared pointer to track annotation, nullptr if not found.
  std::shared_ptr<TrackAnnotation> prevTrack(uint64_t id);

  /// @brief Gets first frame where a track occurs.
  ///
  /// @param id Track ID.
  /// @return Frame of first occurrence, zero if not found or no detections.
  uint64_t trackFirstFrame(uint64_t id);

  /// @brief Gets ID of earliest track.
  ///
  /// @return ID of earliest track.
  uint64_t earliestTrackID();

  /// @brief Sets degraded state for a particular frame.
  ///
  /// Subsequent frames have the same degraded state until another 
  /// degraded state is inserted.
  ///
  /// @param frame Frame for which the degraded state is specified.
  /// @param degraded True if degraded, false otherwise.
  void setDegraded(uint64_t frame, bool degraded);

  /// @brief Gets recent degraded state.
  ///
  /// @param frame Current frame.
  /// @return Most recent degraded state.
  bool isDegraded(uint64_t frame);

  /// @brief Equality operator.
  ///
  /// @param rhs Right hand side argument.
  /// @return Whether the object is equal to rhs.
  bool operator==(VideoAnnotation &rhs);

  /// @brief Inequality operator.
  ///
  /// @param rhs Right hand side argument.
  /// @return Whether the object is not equal to rhs.
  bool operator!=(VideoAnnotation &rhs);

  /// @brief Writes annotations to file.
  ///
  /// Writes two files, a csv and a json file.  The json file contains
  /// position information within the video, and the csv file contains
  /// the species and subspecies information.  The json file has the same
  /// path as the input csv path with different extension.
  ///
  /// @param csv_path Path to csv file.
  /// @param trip_id Trip ID.
  /// @param tow_number Tow number.
  /// @param reviewer Name of reviewer.
  /// @param tow_type Tow type.
  /// @param fps Native frames per second of the video.
  void write(const boost::filesystem::path &csv_path,
    const std::string &trip_id,
    const std::string &tow_number,
    const std::string &reviewer,
    const std::string &tow_type,
    double fps) const;

  /// @brief Reads annotations from csv and json files.
  ///
  /// Path to json file is assumed to be the same as the input csv path
  /// with different extension.
  ///
  /// @param csv_path Path to csv file.
  void read(const boost::filesystem::path &csv_path);
private:
  /// @brief For mapping integers to detection annotations.
  typedef boost::bimap<
    boost::bimaps::multiset_of<uint64_t>,
    boost::bimaps::multiset_of<DetectionList::iterator>> DetectionsByInteger;

  /// @brief For mapping unique integers to track annotations.
  typedef boost::bimap<
    uint64_t,
    boost::bimaps::multiset_of<TrackList::iterator>> TracksByUniqueInteger;

  /// @brief For mapping integers to track annotations.
  typedef boost::bimap<
    boost::bimaps::multiset_of<uint64_t>,
    boost::bimaps::multiset_of<TrackList::iterator>> TracksByInteger;

  /// @brief For mapping pair of strings to track annotations.
  typedef boost::bimap<
    boost::bimaps::multiset_of<std::pair<std::string, std::string>>,
    boost::bimaps::multiset_of<TrackList::iterator>> TracksByStringPair;

  /// @brief List of detection annotations.
  DetectionList detection_list_;

  /// @brief List of track annotations.
  TrackList track_list_;

  /// @brief Map between frame and iterator to detection annotations.
  DetectionsByInteger detections_by_frame_;

  /// @brief Map between id and iterator to detection annotations.
  DetectionsByInteger detections_by_id_;

  /// @brief Map between id and iterator to track annotations.
  TracksByUniqueInteger tracks_by_id_;

  /// @brief Map between species/subspecies and iterator to track annotations. 
  TracksByStringPair tracks_by_species_;

  /// @brief Map between frame added and iterator to track annotations.
  TracksByInteger tracks_by_frame_added_;

  /// @brief Degraded state by frame.
  std::map<uint64_t, bool> degraded_by_frame_;
};

}} // namespace fish_annotator::video_annotator

#endif // VIDEO_ANNOTATION_H
