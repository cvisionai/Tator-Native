#include "testDocument.h"
#include <QtTest/QtTest>
#include <QObject>
#include <memory>
#include <iostream>
#include <sstream>

#include "../FishGUI/document.h"

void TestDocument::testSerialize() {
    std::string expected = "{\"x\":\"1\",\"y\":\"2\"}\n";
    std::ostringstream out;
    TestClass test(1, 2);

    serialize(test, out);
    QCOMPARE(expected, out.str());
}

void TestDocument::testDeserialize() {
    std::string data = "{\"x\":\"1\",\"y\":\"2\"}\n";
    std::istringstream input(data);

    TestClass test = deserialize<TestClass>(input);
    QCOMPARE(test.x, 1);
    QCOMPARE(test.y, 2);
}

//void TestDocument::writeJSON()
//{
//    using namespace FishDetector;

//    Document testDocument;
//    uint64_t x = 1;
//    uint64_t y = 1;
//    uint64_t h = 5;
//    uint64_t w = 5;
//    uint64_t frame = 1;

//    testDocument.addAnnotation();
//    testDocument.addAnnotationLocation(std::uint64_t(0), frame, FishDetector::Rect(x,y,w,h));
////    std::string filename = "/Users/bwoodward/Projects/FishDetector/UnitTests/test.json";
////    QCOMPARE(testDocument->writeJSON(filename), 0);

//    std::ostringstream out;
//    Serialization<Document>::write(testDocument, out);
//    std::cout << "got: " << out.str() << std::endl;
//}

QTEST_MAIN(TestDocument)
