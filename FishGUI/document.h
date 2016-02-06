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

namespace FishDetector {

template <class T>
struct Serialization {
    static ptree write(const T &obj) {}
    static T read(const ptree &node) {}
};

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
    int writeJSON(const std::string& filename);
    std::shared_ptr<Annotation> addAnnotation(); //std::uint64_t frame, Rect area);
    void makeAnnotation(std::uint64_t id);
    std::shared_ptr<AnnotationLocation> addAnnotationLocation(std::uint64_t id, std::uint64_t frame, Rect area);
    void addAnnotationLocation(std::uint64_t id, std::shared_ptr<AnnotationLocation> newLoc);
    void copyAnnotation(std::uint64_t id, std::uint64_t frame, Rect area);
    std::uint64_t getIDCounter() {return id_counter; }
    FrameAnnotations getAnnotations(std::uint64_t frame);
    bool keyExists(std::uint64_t id) { return (annotations.find( id ) != annotations.end());}
    annotation_map_t &getAnnotations() { return annotations; }
    const annotation_map_t &getAnnotations() const { return annotations; }
private:
    std::uint64_t id_counter;
    std::map<std::uint64_t, FrameAnnotations> annotationsByFrame;
    annotation_map_t annotations;
};

template <>
struct Serialization<AnnotationLocation> {
    static ptree write(const AnnotationLocation &obj) {
        ptree node;
        node.add("annotation.frame", obj.frame);
        node.add("annotation.x", obj.area.x);
        node.add("annotation.y", obj.area.y);
        node.add("annotation.w", obj.area.w);
        node.add("annotation.h", obj.area.h);
        return node;
    }

    static std::shared_ptr<AnnotationLocation> read(ptree &node) {
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
};

template <>
struct Serialization<Document> {
    static ptree write(const Document &obj) {
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

    static Document read(const ptree &document) {
        std::uint64_t id;
        Document newDoc = Document();
        BOOST_FOREACH(const ptree::value_type &v, document.get_child("Annotation Array")) {
            id = v.second.get("id",0);
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
};

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

}


#endif // DOCUMENT_H
