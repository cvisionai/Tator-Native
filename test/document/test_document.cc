#include "test_document.h"
#include <QtTest>
#include <memory>
#include <iostream>
#include <sstream>

void TestDocument::testSerialize() {
    std::string expected = "{\"x\":\"1\",\"y\":\"2\"}\n";
    std::ostringstream out;
    TestClass test(1, 2);

    fish_detector::serialize(test, out);
    QCOMPARE(expected, out.str());
}

void TestDocument::testDeserialize() {
    std::string data = "{\"x\":\"1\",\"y\":\"2\"}\n";
    std::istringstream input(data);
    TestClass test(0, 0);
    fish_detector::deserialize(test, input);
    QCOMPARE(test.x, 1);
    QCOMPARE(test.y, 2);
}

void TestDocument::testReadJSON()
{
    std::string data = "{\"Annotation Array\": [{\"annotation\": {\"id\": \"0\",\
                      \"frame\":\"1\",\"x\":\"1\",\"y\":\"1\",\"h\":\"5\",\"w\":\"5\"\
                      }}]}\n";
    std::istringstream input(data);
    fish_detector::video_annotator::Document test;
    fish_detector::deserialize(test, input);
    auto annotations = test.getAnnotations();
    auto annotation = annotations[0];
    auto locations = annotation->getLocations();
    auto location = locations.front();
    QCOMPARE(location->frame, uint64_t(1));
    QCOMPARE(location->area_.x,uint64_t(1));
    QCOMPARE(location->area_.y,uint64_t(1));
    QCOMPARE(location->area_.h,uint64_t(5));
    QCOMPARE(location->area_.w,uint64_t(5));
    QCOMPARE(test.keyExists(uint64_t(0)),true);
}

QTEST_MAIN(TestDocument)
#include "test_document.moc"
