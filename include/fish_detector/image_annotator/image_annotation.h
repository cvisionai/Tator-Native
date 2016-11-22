/// @file
/// @brief Defines classes that store annotation information for images.

#ifndef IMAGE_ANNOTATIONS_H
#define IMAGE_ANNOTATIONS_H

#include <map>
#include <string>
#include <vector>
#include <list>
#include <functional>

#include <boost/property_tree/ptree.hpp>
#include <boost/bimap.hpp>
#include <boost/bimap/multiset_of.hpp>

#include "fish_detector/common/serialization.h"
#include "fish_detector/common/rect.h"

namespace fish_detector { namespace image_annotator {

namespace pt = boost::property_tree;

/// @brief Defines annotation information for one image.
struct ImageAnnotation : public Serialization {
  /// @brief Constructor.
  ///
  /// @param image_file Name of the image file (not including path).
  /// @param species Species of the individual.
  /// @param subspecies Subspecies of the individual.
  /// @param id ID of the individual within the image.
  /// @param rect Rectangle defining the annotation.
  ImageAnnotation(const std::string& image_file, 
                  const std::string& species,
                  const std::string& subspecies,
                  uint64_t id, 
                  const Rect &rect);

  /// @brief Default constructor.
  ImageAnnotation();

  /// @brief Writes to a property tree.
  ///
  /// @return Property tree constructed from the object.
  pt::ptree write() const;

  /// @brief Equality operator.
  ///
  /// @param rhs Right hand side argument.
  /// @return Whether the object is equal to rhs.
  bool operator==(const ImageAnnotation &rhs) const;

  /// @brief Inequality operator.
  ///
  /// @param rhs Right hand side argument.
  /// @return Whether the object is not equal to rhs.
  bool operator!=(const ImageAnnotation &rhs) const;

  /// @brief Reads from a property tree.
  ///
  /// @param tree Property tree to be read.
  void read(const pt::ptree &tree);

  std::string image_file_; ///< Name of the image file (not including path).
  std::string species_; ///< Species of the individual.
  std::string subspecies_; ///< Subspecies of the individual.
  uint64_t id_; ///< ID of the individual within the image.
  Rect rect_; ///< Rectangle defining the annotation. 
};

/// @brief List of image annotations.
typedef std::list<ImageAnnotation> List;

/// @brief Makes iterator sortable so it can be used as key in maps.
///
/// Works by converting both iterators to pointers and applying operator<.
///
/// @param lhs Left hand side argument.
/// @param rhs Right hand side argument.
inline bool operator<(const List::iterator& lhs, const List::iterator& rhs) {
  return &(*lhs) < &(*rhs);
}

/// @brief Defines annotation information for a series of images.
class ImageAnnotationList {
public:
  /// @brief Constructor.
  ImageAnnotationList();

  /// @brief Inserts an annotation.
  ///
  /// @param annotation Annotation to be inserted.
  void insert(const ImageAnnotation &annotation);

  /// @brief Removes an annotation.
  ///
  /// @param image_file Image file name (not including path).
  /// @param id ID of the individual within the image.
  void remove(const std::string &image_file, uint64_t id);

  /// @brief Equality operator.
  ///
  /// @param rhs Right hand side argument.
  /// @return Whether the object is equal to rhs.
  bool operator==(ImageAnnotationList &rhs);

  /// @brief Inequality operator.
  ///
  /// @param rhs Right hand side argument.
  /// @return Whether the object is not equal to rhs.
  bool operator!=(ImageAnnotationList &rhs);

  /// @brief Writes annotations to file.
  ///
  /// Writes one file per image.  Multiple annotations for the same image
  /// are written to the same file.
  ///
  /// @param filenames Vector of strings, each containing the full path 
  ///        to an image.
  void write(const std::vector<std::string> &filenames) const;

  /// @brief Reads annotations from a given list of input files.
  ///
  /// @param filenames Vector of strings, each containing the full path 
  ///        to an image.
  void read(const std::vector<std::string> &filenames);
private:
  /// @brief For mapping strings to image annotations.
  typedef boost::bimap<
    boost::bimaps::multiset_of<std::string>, 
    boost::bimaps::set_of<List::iterator>> ByString;

  /// @brief For mapping pair of strings to image annotations.
  typedef boost::bimap<
    boost::bimaps::multiset_of<std::pair<std::string, std::string>>,
    boost::bimaps::set_of<List::iterator>> ByStringPair;

  std::list<ImageAnnotation> list_; ///< List of image annotations.

  /// @brief Map between image filename and reference to image annotations.
  ByString by_file_;

  /// @brief Map between species/subspecies and reference to image annotations.
  ByStringPair by_species_;
};

}} // namespace fish_detector::image_annotator

#endif // IMAGE_ANNOTATIONS_H
