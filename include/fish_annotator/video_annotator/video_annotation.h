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

/// @brief Defines annotation information for one frame.
struct FrameAnnotation : public Serialization {
  /// @brief Constructor.
  ///
  /// @param frame Frame associated with the annotation.
  /// @param species Species of the individual.
  /// @param subspecies Subspecies of the individual.
  /// @param id ID of the individual.
  /// @param rect Rectangle defining the annotation.
  FrameAnnotation(uint64_t frame,
                  const std::string& species,
                  const std::string& subspecies,
                  uint64_t id,
                  const Rect &rect);

  /// @brief Default constructor.
  FrameAnnotation();

  /// @brief Writes to a property tree.
  ///
  /// @return Property tree constructed from the object.
  pt::ptree write() const;

  /// @brief Equality operator.
  ///
  /// @param rhs Right hand side argument.
  /// @return Whether the object is equal to rhs.
  bool operator==(const FrameAnnotation &rhs) const;

  /// @brief Inequality operator.
  ///
  /// @param rhs Right hand side argument.
  /// @return Whether the object is not equal to rhs.
  bool operator!=(const FrameAnnotation &rhs) const;

  /// @brief Reads from a property tree.
  ///
  /// @param tree Property tree to be read.
  void read(const pt::ptree &tree);

  uint64_t frame_; ///< Frame of this annotation.
  std::string species_; ///< Species of the individual.
  std::string subspecies_; ///< Subspecies of the individual.
  uint64_t id_; ///< ID of the individual.
  Rect area_; ///< Rectangle defining the annotation.
};

/// @brief List of video annotations.
typedef std::list<std::shared_ptr<FrameAnnotation>> List;

/// @brief Makes iterator sortable so it can be used as key in maps.
///
/// Works by converting both iterators to pointers and applying operator<.
///
/// @param lhs Left hand side argument.
/// @param rhs Right hand side argument.
inline bool operator<(const List::iterator& lhs, const List::iterator& rhs) {
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

  /// @brief Inserts an annotation.
  ///
  /// @param annotation Annotation to be inserted.
  void insert(std::shared_ptr<FrameAnnotation> annotation);

  /// @brief Removes an annotation.
  ///
  /// @param frame Frame of the annotation.
  /// @param id ID of the individual within the frame.
  void remove(uint64_t frame, uint64_t id);

  /// @brief Gets next ID for a given frame.
  ///
  /// @param frame Frame in the video.
  uint64_t nextId(uint64_t frame);

  /// @brief Gets annotations for a given frame.
  ///
  /// @param frame Frame in the video.
  std::vector<std::shared_ptr<FrameAnnotation>>
    getFrameAnnotations(uint64_t frame);

  /// @brief Gets counts for each species in a given frame.
  std::map<uint64_t, uint64_t> getCounts(uint64_t frame);

  /// @brief Gets all species in the annotations.
  std::vector<Species> getAllSpecies();

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
  void write(const boost::filesystem::path &csv_path) const;

  /// @brief Reads annotations from csv and json files.
  ///
  /// Path to json file is assumed to be the same as the input csv path
  /// with different extension.
  ///
  /// @param csv_path Path to csv file.
  void read(const boost::filesystem::path &csv_path);
private:
  /// @brief For mapping integers to frame annotations.
  typedef boost::bimap<
    boost::bimaps::multiset_of<uint64_t>,
    boost::bimaps::multiset_of<List::iterator>> ByInteger;

  /// @brief For mapping pair of strings to frame annotations.
  typedef boost::bimap<
    boost::bimaps::multiset_of<std::pair<std::string, std::string>>,
    boost::bimaps::multiset_of<List::iterator>> ByStringPair;

  /// @brief List of frame annotations.
  List list_;

  /// @brief Map between frame and reference to frame annotations.
  ByInteger by_frame_;

  /// @brief Map between id and reference to frame annotations.
  ByInteger by_id_;

  /// @brief Map between species/subspecies and reference to frame annotations.
  ByStringPair by_species_;
};

}} // namespace fish_annotator::video_annotator

#endif // VIDEO_ANNOTATION_H
