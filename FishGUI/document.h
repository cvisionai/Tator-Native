#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <cstdint>
#include <list>
#include <vector>
#include <map>
#include <memory>

namespace FishDetector {

struct Rect {
    std::uint64_t x, y;
    std::uint64_t w, h;

    Rect(const Rect &r);
    Rect(std::uint64_t x, std::uint64_t y, std::uint64_t w, std::uint64_t h);
};

struct AnnotationLocation {
    std::uint64_t frame;
    Rect area;

    AnnotationLocation(std::uint64_t frame, Rect area);
};

class Annotation {
public:
    typedef std::list<std::shared_ptr<AnnotationLocation> > list_t;
public:
    Annotation(std::uint64_t id);
    void addLocation(std::shared_ptr<AnnotationLocation> location);
    std::shared_ptr<AnnotationLocation> addLocation(std::uint64_t frame, Rect area);

    std::uint64_t getId() { return id; }

    list_t &getLocations() { return locations; }
private:
    std::uint64_t id;
    list_t locations;
};

class FrameAnnotations {
public:
    typedef std::shared_ptr<AnnotationLocation> ptr_t;
    typedef std::list<ptr_t> list_t;
    typedef list_t::iterator iterator;
    typedef list_t::const_iterator const_iterator;
public:
    void add(ptr_t annotation);

    iterator begin() { return annotations.begin(); }
    iterator end() { return annotations.end(); }
    const_iterator cbegin() { return annotations.cbegin(); }
    const_iterator cend() { return annotations.cend(); }
private:
    list_t annotations;
};

// contains a pointer to video we're annotating and annotations
class Document {
public:
    typedef std::map<std::uint64_t, std::shared_ptr<Annotation> > annotation_map_t;
public:
    Document();

    std::shared_ptr<Annotation> addAnnotation(); //std::uint64_t frame, Rect area);
    std::shared_ptr<AnnotationLocation> addAnnotationLocation(std::uint64_t id, std::uint64_t frame, Rect area);
    void copyAnnotation(std::uint64_t id, std::uint64_t frame, Rect area);
    std::uint64_t getIDCounter() {return id_counter; }
    FrameAnnotations getAnnotations(std::uint64_t frame);
    annotation_map_t &getAnnotations() { return annotations; }
private:
    std::uint64_t id_counter;
//    std::vector<std::shared_ptr<AnnotationLocation>> annotationLocations;
//    std::list<std::shared_ptr<Annotation>> annotations;
    std::map<std::uint64_t, FrameAnnotations> annotationsByFrame;
    annotation_map_t annotations;
};

}


#endif // DOCUMENT_H
