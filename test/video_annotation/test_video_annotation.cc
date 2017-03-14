#include <boost/filesystem.hpp>

#include "fish_annotator/common/rect.h"
#include "fish_annotator/video_annotator/video_annotation.h"
#include "test_video_annotation.h"

#ifdef _WIN32
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
#endif

namespace va = fish_annotator::video_annotator;

void TestVideoAnnotation::testSerialize() {
  va::VideoAnnotation ann;
  ann.insert(std::make_shared<va::TrackAnnotation>(
    ann.nextId(), "ahasdjk", "ahashdjkl"));
  ann.insert(std::make_shared<va::TrackAnnotation>(
    ann.nextId(), "iopjdfg", "ghjasdf aga"));
  ann.insert(std::make_shared<va::TrackAnnotation>(
    ann.nextId(), "abghj", "iuohysdfg"));
  ann.insert(std::make_shared<va::DetectionAnnotation>(
    1231, 0, fish_annotator::Rect(1, 2, 3, 4)));
  ann.insert(std::make_shared<va::DetectionAnnotation>(
    1151, 0, fish_annotator::Rect(5, 2, 34, 2)));
  ann.insert(std::make_shared<va::DetectionAnnotation>(
    1151, 0, fish_annotator::Rect(15, 12, 3, 5)));
  ann.insert(std::make_shared<va::DetectionAnnotation>(
    1151, 2, fish_annotator::Rect(5, 2, 34, 2)));
  QVERIFY(ann.getAllSpecies().size() == 3);
  QVERIFY(ann.detections_by_frame_.left.count(0) == 2);
  QVERIFY(ann.detections_by_frame_.left.count(1) == 0);
  QVERIFY(ann.detections_by_frame_.left.count(2) == 1);
  boost::filesystem::path csv_path("vid_ann.csv");
  ann.write(csv_path, 15, 23, "Joe Mango", "Open", 24.0);
  va::VideoAnnotation deser_ann;
  deser_ann.read(csv_path);
  QVERIFY(ann == deser_ann);
  ann.remove(1151, 0);
  ann.write(csv_path, 15, 23, "Joe Mango", "Open", 24.0);
  va::VideoAnnotation deser_ann_1;
  deser_ann_1.read(csv_path);
  QVERIFY(ann == deser_ann_1);
  QVERIFY(ann.detections_by_frame_.left.count(0) == 1);
}

QTEST_MAIN(TestVideoAnnotation)
#include "test_video_annotation.moc"
  
