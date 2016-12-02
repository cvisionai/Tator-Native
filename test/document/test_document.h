#include <QObject>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "fish_annotator/video_annotator/document.h"
// unit test class for Document
using namespace std;

using boost::property_tree::ptree;
using boost::property_tree::json_parser::write_json;
using boost::property_tree::json_parser::read_json;

struct TestClass : public fish_annotator::Serialization {
  int x, y;

  TestClass(int x, int y) 
    : x(x)
    , y(y) {
  }

  ptree write() const {
    ptree document;
    document.add("x", x);
    document.add("y", y);
    return document;
  }

  void read(const ptree &tree) {
    x = tree.get("x", 0);
    y = tree.get("y", 0);
  }
    
};

class TestDocument : public QObject {
    Q_OBJECT
private slots:
    void testSerialize();
    void testDeserialize();
    void testReadJSON();
};
