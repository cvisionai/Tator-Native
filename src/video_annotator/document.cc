#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

#include "fish_detector/video_annotator/document.h"

namespace fish_detector { namespace video_annotator {

namespace pt = boost::property_tree;

AnnotationLocation::AnnotationLocation(uint64_t frame, Rect area)
  : frame(frame)
  , area(area) {
}

pt::ptree AnnotationLocation::write() const {
  pt::ptree node;
  node.add("annotation.frame", frame);
  node.add("annotation.x", area.x);
  node.add("annotation.y", area.y);
  node.add("annotation.w", area.w);
  node.add("annotation.h", area.h);
  return node;
}

void AnnotationLocation::read(const pt::ptree &tree) {
  frame = tree.get("annotation.frame",0);
  area.x = tree.get("annotation.x",0);
  area.y = tree.get("annotation.y",0);
  area.w = tree.get("annotation.w",0);
  area.h = tree.get("annotation.h",0);
}

void FrameAnnotations::add(ptr_t annotation) {
  annotations.push_back(annotation);
}

void FrameAnnotations::removeAnnotation(uint64_t id) {
  auto it = find_if(
    annotations.begin(), annotations.end(), 
    [&id](const std::pair<uint64_t, 
          std::shared_ptr<AnnotationLocation>>& obj) {
      return obj.first == id;
    });
  if (it != annotations.end()) {
      annotations.erase(it);
  }
}

Annotation::Annotation(uint64_t id) {
  this->id = id;
}

void Annotation::addLocation(std::shared_ptr<AnnotationLocation> location) {
  locations.push_back(location);
}

std::shared_ptr<AnnotationLocation> Annotation::addLocation(uint64_t frame, Rect area) {
  auto loc = std::make_shared<AnnotationLocation>(frame, area);
  locations.push_back(loc);
  return loc;
}

void Annotation::copyLastLocation(uint64_t frame) {
  auto lastLoc = locations.back();
  addLocation(frame, lastLoc->area);
}

bool Annotation::frameHasAnn(uint64_t frame) {
  auto it = find_if(locations.begin(), locations.end(), 
    [&frame](const std::shared_ptr<AnnotationLocation>& obj) {
      return obj->frame == frame;
    } );
  return (it != locations.end());
}

void Annotation::removeFrameAnn(uint64_t frame) {
  auto it = find_if(locations.begin(), locations.end(), 
    [&frame](const std::shared_ptr<AnnotationLocation>& obj) {
      return obj->frame == frame;
    } );
  if (it != locations.end()) {
      locations.erase(it);
  }
}

Document::Document() {
}

void Document::addAnnotation(uint64_t id) {
  auto annotation = std::make_shared<Annotation>(id);
  annotations[id] = annotation;
}

void Document::makeAnnotation(uint64_t id) {
  auto annotation = std::make_shared<Annotation>(id);
  annotations[id] = annotation;
}

std::shared_ptr<AnnotationLocation> Document::addAnnotationLocation(uint64_t id, uint64_t frame, Rect area) {
  auto loc = annotations[id]->addLocation(frame, area);
  annotationsByFrame[frame].add(std::make_pair(id,loc));
  return loc;
}

void Document::addAnnotationLocation(uint64_t id, std::shared_ptr<AnnotationLocation> newLoc) {
  annotations[id]->addLocation(newLoc);
  annotationsByFrame[newLoc->frame].add(std::make_pair(id, newLoc));
}

void Document::copyAnnotation(uint64_t id, uint64_t frame, Rect area) {
  //annotations[id]->addLocation(frame, area);
  auto loc = annotations[id]->addLocation(frame, area);
  auto pos = annotationsByFrame.find(frame);
  if (pos == annotationsByFrame.end()) {
    annotationsByFrame[frame] = FrameAnnotations();
    pos = annotationsByFrame.find(frame);
  }
  pos->second.add(std::make_pair(id,loc));
}

void Document::removeFrameAnnotation(uint64_t id) {
  for (auto const &ann : annotationsByFrame) {
    annotationsByFrame[ann.first].removeAnnotation(id);
  }
}

FrameAnnotations Document::getAnnotations(uint64_t frame) {
  return annotationsByFrame[frame];
}

pt::ptree Document::write() const {
  pt::ptree document;
  pt::ptree children;
  for (auto const &map_value : getAnnotations())
  {
    auto annotation = map_value.second;
    for (auto const &location : annotation->getLocations())
    {
      pt::ptree node = location->write();
      node.add("annotation.id", annotation->getId());
      children.push_back(std::make_pair("", node));
    }
  }
  document.add_child("Annotation Array", children);
  return document;
}

void Document::read(const pt::ptree &tree) {
  uint64_t id;
  annotations.clear();
  annotationsByFrame.clear();
  BOOST_FOREACH(const pt::ptree::value_type &v, tree.get_child("Annotation Array")) {
    id = v.second.get("annotation.id",0);
    auto newLoc = std::make_shared< AnnotationLocation>( 0, Rect( 0, 0, 0, 0 ) );
    newLoc->read(v.second);
    if(!keyExists(id)) {
      makeAnnotation(id);
    }
    addAnnotationLocation(id,newLoc);
  }
}

}} // namespace fish_detector::video_annotator

