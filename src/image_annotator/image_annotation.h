/// @file
/// @brief Defines classes that store annotation information for images.

#ifndef IMAGE_ANNOTATION_H
#define IMAGE_ANNOTATION_H

#include <string>
#include <vector>
#include <list>
#include <map>
#include <memory>
#include <fstream>

#include <boost/property_tree/ptree.hpp>
#include <boost/bimap.hpp>
#include <boost/bimap/multiset_of.hpp>
#include <boost/filesystem.hpp>

#include "serialization.h"
#include "rect.h"
#include "species.h"
#include "annotation_scene.h"

#ifndef NO_TESTING
class TestImageAnnotation;
#endif

namespace fish_annotator { namespace image_annotator {

namespace pt = boost::property_tree;

/// Defines annotation information for one detection in an image.
struct ImageAnnotation : public Serialization {
  /// Constructor.
  ///
  /// @param image_file Name of the image file (not including path).
  /// @param species Species of the individual.
  /// @param subspecies Subspecies of the individual.
  /// @param id ID of the individual within the image.
  /// @param rect Rectangle defining the annotation.
  /// @param type Type of annotation.
  ImageAnnotation(const std::string& image_file, 
                  const std::string& species,
                  const std::string& subspecies,
                  uint64_t id, 
                  const Rect &rect,
                  enum AnnotationType type);

  /// Default constructor.
  ImageAnnotation();

  /// Writes to a property tree.
  ///
  /// @return Property tree constructed from the object.
  pt::ptree write() const;

  /// Writes to a csv.  Does not include header.
  ///
  /// @param csv_file Output file stream.
  void write_csv(std::ofstream &csv_file) const;

  /// Equality operator.
  ///
  /// @param rhs Right hand side argument.
  /// @return Whether the object is equal to rhs.
  bool operator==(const ImageAnnotation &rhs) const;

  /// Inequality operator.
  ///
  /// @param rhs Right hand side argument.
  /// @return Whether the object is not equal to rhs.
  bool operator!=(const ImageAnnotation &rhs) const;

  /// Reads from a property tree.
  ///
  /// @param tree Property tree to be read.
  void read(const pt::ptree &tree);

  std::string image_file_; ///< Name of the image file (not including path).
  std::string species_; ///< Species of the individual.
  std::string subspecies_; ///< Subspecies of the individual.
  uint64_t id_; ///< ID of the individual within the image.
  Rect area_; ///< Rectangle defining the annotation. 
  enum AnnotationType type_; ///< Type of annotation.
};

/// List of image annotations.
typedef std::list<std::shared_ptr<ImageAnnotation>> List;

/// Makes iterator sortable so it can be used as key in maps.
///
/// Works by converting both iterators to pointers and applying operator<.
///
/// @param lhs Left hand side argument.
/// @param rhs Right hand side argument.
inline bool operator<(const List::iterator& lhs, const List::iterator& rhs) {
  return &(*lhs) < &(*rhs);
}

/// Defines annotation information for a series of images.
class ImageAnnotationList {
#ifndef NO_TESTING
  friend class ::TestImageAnnotation;
#endif
public:
  /// Constructor.
  ImageAnnotationList();

  /// Inserts an annotation.
  ///
  /// @param annotation Annotation to be inserted.
  void insert(std::shared_ptr<ImageAnnotation> annotation);

  /// Removes an annotation.
  ///
  /// @param image_file Image file path.
  /// @param id ID of the individual within the image.
  void remove(const boost::filesystem::path &image_file, uint64_t id);

  /// Gets next ID for a given image.
  ///
  /// @param image_file Image file path.
  uint64_t nextId(const boost::filesystem::path &image_file);

  /// Gets annotations for a given image.
  ///
  /// @param image_file Image file path.
  std::vector<std::shared_ptr<ImageAnnotation>> 
    getImageAnnotations(const boost::filesystem::path &image_file);

  /// Gets counts for each species in a given image.
  std::map<std::string, uint64_t> getCounts(const std::string &image_file);

  /// Gets all species in the annotations.
  std::vector<Species> getAllSpecies();

  /// Equality operator.
  ///
  /// @param rhs Right hand side argument.
  /// @return Whether the object is equal to rhs.
  bool operator==(ImageAnnotationList &rhs);

  /// Inequality operator.
  ///
  /// @param rhs Right hand side argument.
  /// @return Whether the object is not equal to rhs.
  bool operator!=(ImageAnnotationList &rhs);

  /// Writes annotations to file.
  ///
  /// Writes one file per image.  Multiple annotations for the same image
  /// are written to the same file.
  ///
  /// @param filenames Vector of paths, each containing the full path 
  ///        to an image.
  void write(const std::vector<boost::filesystem::path> &filenames) const;

  /// Reads annotations from a given list of input files.
  ///
  /// @param filenames Vector of paths, each containing the full path 
  ///        to an image.
  void read(const std::vector<boost::filesystem::path> &filenames);
private:
  /// For mapping strings to image annotations.
  typedef boost::bimap<
    boost::bimaps::multiset_of<std::string>, 
    boost::bimaps::set_of<List::iterator>> ByString;

  /// For mapping pair of strings to image annotations.
  typedef boost::bimap<
    boost::bimaps::multiset_of<std::pair<std::string, std::string>>,
    boost::bimaps::set_of<List::iterator>> ByStringPair;

  /// List of image annotations.
  List list_; 

  /// Map between image filename and reference to image annotations.
  ByString by_file_;

  /// Map between species/subspecies and reference to image annotations.
  ByStringPair by_species_;
};

}} // namespace fish_annotator::image_annotator

#endif // IMAGE_ANNOTATION_H
