/// @file
/// @brief Defines classes needed for recording and loading annotations.

#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <cstdint>
#include <list>
#include <vector>
#include <map>
#include <memory>

#include <iostream>
#include <sstream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

using boost::property_tree::ptree;
using boost::property_tree::json_parser::write_json;
using boost::property_tree::json_parser::read_json;

namespace fish_detector { namespace gui {


/// @brief Class for serializing data.
///
/// Converts a generic type to and from a Boost property tree. 
///
/// @tparam T Type to be serialized.
template <class T>
struct Serialization {
  /// @brief Writes the data to property tree.
  ///
  /// @param obj The object to be written.
  /// @return Property tree containing the data.
  static ptree write(const T &obj) {}

  /// @brief Reads the data from property tree.
  ///
  /// @param node Property tree containing data.
  /// @return An object containing the data.
  static T read(const ptree &node) {}
};

/// @brief Geometric definition of a rectangle.
struct Rect {
  /// @brief Constructor.
  ///
  /// @param x Horizontal coordinate of top-left corner.
  /// @param y Vertical coordinate of top-left corner.
  /// @param w Width.
  /// @param h Height.
  Rect(std::uint64_t x, std::uint64_t y, std::uint64_t w, std::uint64_t h);

  /// @brief Copy constructor.
  ///
  /// @param r Rect object to be copied.
  Rect(const Rect &r);

  std::uint64_t x; ///< Horizontal coordinate of top-left corner.
  std::uint64_t y; ///< Vertical coordinate of top-left corner.
  std::uint64_t w; ///< Width.
  std::uint64_t h; ///< Height.
};

/// @brief Location of an annotation.
///
/// There is one of these for each annotation created by the user in the GUI.
struct AnnotationLocation {
  /// @brief Constructor.
  ///
  /// @param frame The video frame in which this annotation was made.
  /// @param area Rectangle bounding the annotation.
  AnnotationLocation(std::uint64_t frame, Rect area);

  std::uint64_t frame; ///< Video frame in which this annotation was made.
  Rect area; ///< Rectangle bounding the annotation.
};

/// @brief Contains all annotation information for a video.
class Annotation {
public:
  /// @brief List of pointers to annotation locations.
  ///
  /// Each element points to a unique region specified by the user.
  typedef std::list<std::shared_ptr<AnnotationLocation> > list_t;
public:
  /// @brief Constructor.
  Annotation(std::uint64_t id);

  /// @brief Adds another location to this annotation.
  ///
  /// @param location Shared pointer to the annotation location to be added.
  void addLocation(std::shared_ptr<AnnotationLocation> location);

  /// @brief Adds another location to this annotation.
  ///
  /// @param frame The video frame for which this location is added.
  /// @param area Definition of the bounding box for this location.
  /// @return Shared pointer to the resulting AnnotationLocation object.
  std::shared_ptr<AnnotationLocation> addLocation(std::uint64_t frame, Rect area);

  /// @brief Copies the last location for the given frame.
  ///
  /// @param frame Video frame for which the last location is requested.
  void copyLastLocation(std::uint64_t frame);

  /// @brief Indicates whether a given frame has an annotation.
  ///
  /// @param frame Video frame of interest.
  /// @return Whether the frame has an annotation.
  bool frameHasAnn(uint64_t frame);

  /// @brief Removes annotations from a given frame.
  ///
  /// @param frame The frame for which annotations will be removed.
  void removeFrameAnn(uint64_t frame);

  /// @brief Accessor for the annotation ID.
  ///
  /// @return ID for this annotation.
  std::uint64_t getId() { return id; }

  /// @brief Accessor for the annotation locations.
  ///
  /// @return List of pointers to locations in this annotation.
  list_t &getLocations() { return locations; }
private:
  std::uint64_t id; ///< Unique identifier for this annotation.
  list_t locations; ///< Locations of this annotation.
};

/// @brief Annotations for a particular video frame.
class FrameAnnotations {
public:
  /// @brief Mapping from annotation ID to pointer to annotation location.
    typedef std::pair<std::uint64_t, std::shared_ptr<AnnotationLocation>> ptr_t;

  /// @brief List of annotation ID to location mappings.
  typedef std::list<ptr_t> list_t;

  /// @brief Iterator for mapping list.
  typedef list_t::iterator iterator;

  /// @brief Const iterator for mapping list.
  typedef list_t::const_iterator const_iterator;
public:
  /// @brief Adds a mapping to the current list.
  void add(ptr_t annotation);

  /// @brief Returns iterator to start of mapping list.
  iterator begin() { return annotations.begin(); }

  /// @brief Returns iterator to end of mapping list.
  iterator end() { return annotations.end(); }

  /// @brief Returns const iterator to start of mapping list.
  const_iterator cbegin() { return annotations.cbegin(); }

  /// @brief Returns const iterator to end of mapping list.
  const_iterator cend() { return annotations.cend(); }

  /// @brief Removes an annotation from the current list.
  void removeAnnotation(std::uint64_t id);
private:
  /// @brief List of mappings between annotation ID and shared pointers
  ///        to annotation locations.
  list_t annotations;
};

/// @todo I'll let ben do this one..
/// @brief Contains a pointer to video we're annotating and annotations.
class Document {
public:
    typedef std::map<std::uint64_t, std::shared_ptr<Annotation> > annotation_map_t;
public:
    Document();
    int writeJSON(const std::string& filename);
    void addAnnotation(std::uint64_t id);

    void makeAnnotation(std::uint64_t id);

    std::shared_ptr<AnnotationLocation> addAnnotationLocation(std::uint64_t id, std::uint64_t frame, Rect area);
    void addAnnotationLocation(std::uint64_t id, std::shared_ptr<AnnotationLocation> newLoc);

    void copyAnnotation(std::uint64_t id, std::uint64_t frame, Rect area);
    bool keyExists(std::uint64_t id) { return (annotations.find( id ) != annotations.end());}

    FrameAnnotations getAnnotations(std::uint64_t frame);
    std::shared_ptr<Annotation> getAnnotation(std::uint64_t id) { return annotations[id];}
    annotation_map_t &getAnnotations() { return annotations; }
    const annotation_map_t &getAnnotations() const { return annotations; }

    void removeFrameAnnotation(std::uint64_t id, std::uint64_t frame) {annotationsByFrame[frame].removeAnnotation(id);}
    void removeFrameAnnotation(std::uint64_t id);
    void removeAnnotation(std::uint64_t id) {annotations.erase(id);}
private:
    std::map<std::uint64_t, FrameAnnotations> annotationsByFrame;
    annotation_map_t annotations;
};

/// @brief This specialization serializes AnnotationLocation objects.
template <>
struct Serialization<AnnotationLocation> {
  /// @brief Writes an annotation to a property tree.
  ///
  /// @param obj AnnotationLocation object to be written.
  /// @return Property tree constructed from AnnotationLocation object.
  static ptree write(const AnnotationLocation &obj);

  /// @brief Reads an annotation from a property tree.
  ///
  /// @param node Property tree to be read.
  /// @return Shared pointer to annotation location constructed from 
  ///         property tree.
  static std::shared_ptr<AnnotationLocation> read(ptree &node);
};

/// @brief This specialization serializes Document objects.
template <>
struct Serialization<Document> {
  /// @brief Writes a document to a property tree.
  ///
  /// @param obj Document object to be written.
  /// @return Property tree constructed from Document object.
  static ptree write(const Document &obj);

  /// @brief Reads a document from a property tree.
  ///
  /// @param document Property tree to be read.
  /// @return Document object constructed form property tree.
  static Document read(const ptree &document);
};

/// @brief Serializes data to a stream.
///
/// @tparam T Type of object to be serialized.
/// @param obj Object to be serialized.
/// @param out Output stream.
template <typename T>
void serialize(const T &obj, std::ostream &out);

/// @brief Deserializes data from a stream.
///
/// @tparam T Type of object to be deserialized.
/// @param in Input stream.
/// @return Deserialized object.
template <typename T>
T deserialize(std::istream &in);

//
// Implementations
// 

template <typename T>
void serialize(const T &obj, std::ostream &out) {
    ptree document = Serialization<T>::write(obj);
    write_json(out, document, false);
}

template <typename T>
T deserialize(std::istream &in) {
  ptree document;

  read_json(in, document);
  return Serialization<T>::read(document);
}

}} // namespace fish_detector::gui

#endif // DOCUMENT_H

