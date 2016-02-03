#include "testDocument.h"
#include <QtTest/QtTest>
#include <QObject>
#include <memory>
#include "../FishGUI/document.h"

void TestDocument::writeJSON()
{
    std::unique_ptr<FishDetector::Document> testDocument (new FishDetector::Document);
    uint64_t x,y,h,w,frame;
    x = 1;
    y = 1;
    h = 5;
    w = 5;
    frame = 1;
    testDocument->addAnnotation();
    testDocument->addAnnotationLocation(std::uint64_t(0), frame, FishDetector::Rect(x,y,w,h));
    std::string filename = "/Users/bwoodward/Projects/FishDetector/UnitTests/test.json";
    QCOMPARE(testDocument->writeJSON(filename), 0);

}

QTEST_MAIN(TestDocument)
