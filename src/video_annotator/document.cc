#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

#include "fish_detector/video_annotator/document.h"

namespace fish_detector { namespace video_annotator {

Rect::Rect(const Rect &r) {
    x = r.x;
    y = r.y;
    w = r.w;
    h = r.h;
}

Rect::Rect(std::uint64_t x, std::uint64_t y, std::uint64_t w, std::uint64_t h) {
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
}

AnnotationLocation::AnnotationLocation(std::uint64_t frame, Rect area):
    frame(frame),
    area(area)
{}


void FrameAnnotations::add(ptr_t annotation)
{
    annotations.push_back(annotation);
}

void FrameAnnotations::removeAnnotation(std::uint64_t id) {
    auto it = find_if(annotations.begin(), annotations.end(), [&id](const std::pair<std::uint64_t, std::shared_ptr<AnnotationLocation>>& obj) {return obj.first == id;});
    if (it != annotations.end()) {
        annotations.erase(it);
    }
}

Annotation::Annotation(std::uint64_t id) {
    this->id = id;
}

void Annotation::addLocation(std::shared_ptr<AnnotationLocation> location) {
    locations.push_back(location);
}

std::shared_ptr<AnnotationLocation> Annotation::addLocation(std::uint64_t frame, Rect area) {
    auto loc = std::make_shared<AnnotationLocation>(frame, area);
    locations.push_back(loc);
    return loc;
}

void Annotation::copyLastLocation(std::uint64_t frame) {
    auto lastLoc = locations.back();
    addLocation(frame, lastLoc->area);
}

bool Annotation::frameHasAnn(uint64_t frame) {
    auto it = find_if(locations.begin(), locations.end(), [&frame](const std::shared_ptr<AnnotationLocation>& obj) {return obj->frame == frame;});
    return (it != locations.end());
}

void Annotation::removeFrameAnn(uint64_t frame) {
    auto it = find_if(locations.begin(), locations.end(), [&frame](const std::shared_ptr<AnnotationLocation>& obj) {return obj->frame == frame;});
    if (it != locations.end()) {
        locations.erase(it);
    }
}

Document::Document() {
}

void Document::addAnnotation(std::uint64_t id) {
    auto annotation = std::make_shared<Annotation>(id);
    annotations[id] = annotation;
}

void Document::makeAnnotation(std::uint64_t id) {
    auto annotation = std::make_shared<Annotation>(id);
    annotations[id] = annotation;
}

std::shared_ptr<AnnotationLocation> Document::addAnnotationLocation(std::uint64_t id, std::uint64_t frame, Rect area) {
    auto loc = annotations[id]->addLocation(frame, area);
    annotationsByFrame[frame].add(std::make_pair(id,loc));
    return loc;
}

void Document::addAnnotationLocation(std::uint64_t id, std::shared_ptr<AnnotationLocation> newLoc) {
    annotations[id]->addLocation(newLoc);
    annotationsByFrame[newLoc->frame].add(std::make_pair(id, newLoc));
}

void Document::copyAnnotation(std::uint64_t id, std::uint64_t frame, Rect area) {
    //annotations[id]->addLocation(frame, area);
    auto loc = annotations[id]->addLocation(frame, area);
    auto pos = annotationsByFrame.find(frame);
    if (pos == annotationsByFrame.end()) {
        annotationsByFrame[frame] = FrameAnnotations();
        pos = annotationsByFrame.find(frame);
    }
    pos->second.add(std::make_pair(id,loc));
}

void Document::removeFrameAnnotation(std::uint64_t id) {
    for (auto const &ann : annotationsByFrame) {
        annotationsByFrame[ann.first].removeAnnotation(id);
    }
}

FrameAnnotations Document::getAnnotations(std::uint64_t frame) {
    return annotationsByFrame[frame];
}

int Document::writeJSON(const std::string& filename)
{
    using boost::property_tree::ptree;
    using boost::property_tree::json_parser::write_json;
    ptree pt;
    ptree children;

    // Iterate over the modules in the set and put them in the
    // property tree. Note that the put function places the new
    // key at the end of the list of keys. This is fine most of
    // the time. If you want to place an item at some other place
    // (i.e. at the front or somewhere in the middle), this can
    // be achieved using a combination of the insert and put_own
    // functions.

    for (auto const &map_value : annotations)
    {
        auto annotation = map_value.second;
        for (auto const &location : annotation->getLocations())
        {
            ptree child;
            child.add("annotation.id",annotation->getId());
            child.add("annotation.frame",location->frame);
            child.add("annotation.x",location->area.x);
            child.add("annotation.y",location->area.y);
            child.add("annotation.h",location->area.h);
            child.add("annotation.w",location->area.w);
            children.push_back(std::make_pair("", child));
        }
    }
    
    pt.add_child("Annotation Array", children);
    // Write the property tree to the JSON file.
    write_json(filename, pt);
    return 0;
}

ptree Serialization< AnnotationLocation >::write( 
    const AnnotationLocation &obj ) {
  ptree node;
  node.add("annotation.frame", obj.frame);
  node.add("annotation.x", obj.area.x);
  node.add("annotation.y", obj.area.y);
  node.add("annotation.w", obj.area.w);
  node.add("annotation.h", obj.area.h);
  return node;
}

std::shared_ptr< AnnotationLocation > 
Serialization< AnnotationLocation >::read( ptree &node ) {
  std::uint64_t frame,x,y,w,h;
  frame = node.get("annotation.frame",0);
  x = node.get("annotation.x",0);
  y = node.get("annotation.y",0);
  w = node.get("annotation.w",0);
  h = node.get("annotation.h",0);
  Rect newRect = Rect(x,y,w,h);
  auto newLoc = std::make_shared<AnnotationLocation>(frame,newRect);
  return newLoc;
}

ptree Serialization< Document >::write( const Document &obj ) {
  ptree document;
  ptree children;
  for (auto const &map_value : obj.getAnnotations())
  {
    auto annotation = map_value.second;
    for (auto const &location : annotation->getLocations())
    {
      ptree node = Serialization<AnnotationLocation>::write(*location);
      node.add("annotation.id", annotation->getId());
      children.push_back(std::make_pair("", node));
    }
  }
  document.add_child("Annotation Array", children);
  return document;
}

Document Serialization< Document >::read( const ptree &document ) {
  std::uint64_t id;
  Document newDoc = Document();
  BOOST_FOREACH(const ptree::value_type &v, document.get_child("Annotation Array")) {
    id = v.second.get("annotation.id",0);
    ptree newAnnotation = v.second;
    auto newLoc = Serialization<AnnotationLocation>::read(newAnnotation);
    if (newDoc.keyExists(id))
    {
      newDoc.addAnnotationLocation(id,newLoc);
    }
    else
    {
      newDoc.makeAnnotation(id);
      newDoc.addAnnotationLocation(id,newLoc);
    }
  }
  return newDoc;
}

}} // namespace fish_detector::video_annotator

