#include "document.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <iostream>

namespace FishDetector {

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


void FrameAnnotations::add(std::shared_ptr<AnnotationLocation> annotation)
{
    annotations.push_back(annotation);
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

Document::Document() {
    id_counter = 0;
}

std::shared_ptr<Annotation> Document::addAnnotation() { //std::uint64_t frame, Rect area) {
    auto id = id_counter++;
    auto annotation = std::make_shared<Annotation>(id);
    annotations[id] = annotation;
//    annotationLocations.push_back

    return annotation;
}

void Document::makeAnnotation(std::uint64_t id) {
    auto annotation = std::make_shared<Annotation>(id);
    annotations[id] = annotation;
}

std::shared_ptr<AnnotationLocation> Document::addAnnotationLocation(std::uint64_t id, std::uint64_t frame, Rect area) {
    auto loc = annotations[id]->addLocation(frame, area);
    annotationsByFrame[frame].add(loc);
    return loc;
}

void Document::addAnnotationLocation(std::uint64_t id, std::shared_ptr<AnnotationLocation> newLoc) {
    annotations[id]->addLocation(newLoc);
    annotationsByFrame[newLoc->frame].add(newLoc);
}

void Document::copyAnnotation(std::uint64_t id, std::uint64_t frame, Rect area) {
    annotations[id]->addLocation(frame, area);

    auto loc = annotations[id]->addLocation(frame, area);
    auto pos = annotationsByFrame.find(frame);
    if (pos == annotationsByFrame.end()) {
        annotationsByFrame[frame] = FrameAnnotations();
        pos = annotationsByFrame.find(frame);
    }
    pos->second.add(loc);
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

}
