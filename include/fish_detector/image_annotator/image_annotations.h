/// @file
/// @brief Defines classes that store annotation information for images.

#ifndef IMAGE_ANNOTATIONS_H
#define IMAGE_ANNOTATIONS_H

#include <map>
#include <string>
#include <vector>
#include <functional>

#include <boost/property_tree/ptree.hpp>

#include "fish_detector/common/serialization.h"

namespace fish_detector { namespace image_annotator {

namespace pt = boost::property_tree;

/// @brief Defines annotation information for one image.
class ImageAnnotation : public Serialization {
public:
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

  /// @brief Writes to a property tree.
  ///
  /// @return Property tree constructed from the object.
  pt::ptree write() const;

  /// @brief Reads from a property tree.
  ///
  /// @param tree Property tree to be read.
  void read(const pt::ptree &tree);
private:
  std::string image_file_; ///< Name of the image file (not including path).
  std::string species_; ///< Species of the individual.
  std::string subspecies_; ///< Subspecies of the individual.
  uint64_t id_; ///< ID of the individual within the image.
  Rect rect_; ///< Rectangle defining the annotation. 
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

  /// @brief Writes annotations to file.
  ///
  /// Writes one file per image.  Multiple annotations for the same image
  /// are written to the same file.
  void write() const;

  /// @brief Reads annotations from a given list of input files.
  ///
  /// @param filenames Vector of strings containing the full path to an
  ///        image.
  void read(const std::vector<std::string> &filenames);
private:
  std::vector<ImageAnnotation> list_; ///< Vector of image annotations.

  /// @brief Map between image filename and reference to image annotations.
  std::multimap<
    std::string, 
    std::reference_wrapper<ImageAnnotation>> by_file_;

  /// @brief Map between species, subspecies, and reference to image 
  ///        annotations.
  std::multimap<
    std::string, 
    std::multimap<
      std::string,
      std::reference_wrapper<ImageAnnotation>>> by_species_;
}

}} // namespace fish_detector::image_annotator

#endif // IMAGE_ANNOTATIONS_H
