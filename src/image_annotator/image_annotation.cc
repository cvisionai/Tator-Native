
#include "fish_detector/image_annotator/image_annotation.h"

namespace fish_detector { namespace image_annotator {

ImageAnnotation::ImageAnnotation(const std::string& image_file, 
                                 const std::string& species,
                                 const std::string& subspecies,
                                 uint64_t id, 
                                 const Rect &rect)
  : image_file_(image_file)
  , species_(species)
  , subspecies_(subspecies)
  , id_(id)
  , rect_(rect) {
}

ImageAnnotation::ImageAnnotation()
  : image_file_()
  , species_()
  , subspecies_()
  , id_(0)
  , rect_(0, 0, 0, 0) {
}

pt::ptree ImageAnnotation::write() const {
  pt::ptree tree;
  return tree;
}

void ImageAnnotation::read(const pt::ptree &tree) {
}

ImageAnnotationList::ImageAnnotationList()
  : list_()
  , by_file_()
  , by_species_() {
}

void ImageAnnotationList::insert(const ImageAnnotation &annotation) {
}

void ImageAnnotationList::remove(const std::string &image_file, uint64_t id) {
}

void ImageAnnotationList::write() const {
}

void ImageAnnotationList::read(const std::vector<std::string> &filenames) {
}

}} // namespace fish_detector::image_annotator
