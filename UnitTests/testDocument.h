//#include <QtTest/QtTest>
#include <QObject>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>


//#include <memory>
#include "../FishGUI/document.h"
// unit test class for Document
using namespace std;

using boost::property_tree::ptree;
using boost::property_tree::json_parser::write_json;
using boost::property_tree::json_parser::read_json;

using namespace FishDetector;

struct TestClass {
    int x, y;

    TestClass(int x, int y) {
        this->x = x;
        this->y = y;
    }
};

template <>
struct Serialization<TestClass> {
    static ptree write(const TestClass &obj) {
        ptree document;
        ptree children;
        ptree child;

        document.add("x", obj.x);
        document.add("y", obj.y);
        return document;
    }

    static TestClass read(const ptree &document) {
        TestClass result(0, 0);

        result.x = document.get("x", 0);
        result.y = document.get("y", 0);
        return result;
    }
};


class TestDocument : public QObject {
    Q_OBJECT
private slots:
    void testSerialize();
    void testDeserialize();
    void testReadJSON();
//    void writeJSON();
};
