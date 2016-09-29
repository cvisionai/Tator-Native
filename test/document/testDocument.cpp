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
    auto test = deserialize<TestClass>(input);
    QCOMPARE(test.x, 1);
    QCOMPARE(test.y, 2);
}

void TestDocument::testReadJSON()
{
    std:string data = "{\"Annotation Array\": [{\"annotation\": {\"id\": \"0\",\
                      \"frame\":\"1\",\"x\":\"1\",\"y\":\"1\",\"h\":\"5\",\"w\":\"5\"\
                      }}]}\n";
    std::istringstream input(data);
    auto test = deserialize<FishDetector::Document>(input);
    auto annotations = test.getAnnotations();
    auto annotation = annotations[0];
    auto locations = annotation->getLocations();
    auto location = locations.front();
    QCOMPARE(location->frame, std::uint64_t(1));
    QCOMPARE(location->area.x,std::uint64_t(1));
    QCOMPARE(location->area.y,std::uint64_t(1));
    QCOMPARE(location->area.h,std::uint64_t(5));
    QCOMPARE(location->area.w,std::uint64_t(5));
    QCOMPARE(test.keyExists(std::uint64_t(0)),true);
}

QTEST_MAIN(TestDocument)
