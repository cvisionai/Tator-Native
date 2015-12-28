#include "document.h"

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

std::shared_ptr<AnnotationLocation> Document::addAnnotationLocation(std::uint64_t id, std::uint64_t frame, Rect area) {
    auto loc = annotations[id]->addLocation(frame, area);
    annotationsByFrame[frame].add(loc);
    return loc;
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

}
