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

#include "fish_detector/common/rect.h"
#include "fish_detector/common/serialization.h"

namespace fish_detector { namespace video_annotator {

/// @brief Location of an annotation in a video.
///
/// There is one of these for each annotation created by the user in the GUI.
struct AnnotationLocation : public Serialization {
  /// @brief Constructor.
  ///
  /// @param frame The video frame in which this annotation was made.
  /// @param area Rectangle bounding the annotation.
  AnnotationLocation(uint64_t frame, Rect area);

  /// @brief Writes to a property tree.
  ///
  /// @return Property tree constructed from the object.
  boost::property_tree::ptree write() const;

  /// @brief Reads from a property tree.
  ///
  /// @param tree Property tree to be read.
  void read(const boost::property_tree::ptree &tree);

  uint64_t frame; ///< Video frame in which this annotation was made.
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
  Annotation(uint64_t id);

  /// @brief Adds another location to this annotation.
  ///
  /// @param location Shared pointer to the annotation location to be added.
  void addLocation(std::shared_ptr<AnnotationLocation> location);

  /// @brief Adds another location to this annotation.
  ///
  /// @param frame The video frame for which this location is added.
  /// @param area Definition of the bounding box for this location.
  /// @return Shared pointer to the resulting AnnotationLocation object.
  std::shared_ptr<AnnotationLocation> addLocation(uint64_t frame, 
                                                  Rect area);

  /// @brief Copies the last location for the given frame.
  ///
  /// @param frame Video frame for which the last location is requested.
  void copyLastLocation(uint64_t frame);

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
  uint64_t getId() { return id; }

  /// @brief Accessor for the annotation locations.
  ///
  /// @return List of pointers to locations in this annotation.
  list_t &getLocations() { return locations; }
private:
  uint64_t id; ///< Unique identifier for this annotation.
  list_t locations; ///< Locations of this annotation.
};

/// @brief Annotations for a particular video frame.
class FrameAnnotations {
public:
  /// @brief Mapping from annotation ID to pointer to annotation location.
    typedef std::pair<uint64_t, std::shared_ptr<AnnotationLocation>> ptr_t;

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
  void removeAnnotation(uint64_t id);
private:
  /// @brief List of mappings between annotation ID and shared pointers
  ///        to annotation locations.
  list_t annotations;
};

/// @todo I'll let ben do this one..
/// @brief Contains a pointer to video we're annotating and annotations.
class Document : public Serialization {
public:
  typedef std::map<uint64_t, std::shared_ptr<Annotation> > AnnotationMap;
public:
  Document();
  void addAnnotation(uint64_t id);

  void makeAnnotation(uint64_t id);

  std::shared_ptr<AnnotationLocation> addAnnotationLocation(uint64_t id, 
      uint64_t frame, Rect area);
  void addAnnotationLocation(uint64_t id, std::shared_ptr<AnnotationLocation> newLoc);

  void copyAnnotation(uint64_t id, uint64_t frame, Rect area);
  bool keyExists(uint64_t id) { return (annotations.find( id ) != annotations.end());}

  FrameAnnotations getAnnotations(uint64_t frame);
  std::shared_ptr<Annotation> getAnnotation(uint64_t id) { return annotations[id];}
  AnnotationMap &getAnnotations() { return annotations; }
  const AnnotationMap &getAnnotations() const { return annotations; }

  void removeFrameAnnotation(uint64_t id, uint64_t frame) {annotationsByFrame[frame].removeAnnotation(id);}
  void removeFrameAnnotation(uint64_t id);
  void removeAnnotation(uint64_t id) {annotations.erase(id);}

  /// @brief Writes to a property tree.
  ///
  /// @return Property tree constructed from the object.
  boost::property_tree::ptree write() const;

  /// @brief Reads from a property tree.
  ///
  /// @param tree Property tree to be read.
  void read(const boost::property_tree::ptree &tree);
private:
    std::map<uint64_t, FrameAnnotations> annotationsByFrame;
    AnnotationMap annotations;
};

}} // namespace fish_detector::video_annotator

#endif // DOCUMENT_H

