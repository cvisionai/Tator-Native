#include "fish_detector/common/rect.h"
#include "fish_detector/image_annotator/image_annotation.h"
#include "test_image_annotation.h"

namespace ia = fish_detector::image_annotator;

void TestImageAnnotation::testSerialize() {
  std::vector<std::string> image_list = {
    "ceres/ceres1.png",
    "ceres/ceres2.jpg",
    "ceres/ceres3.jpg",
    "ceres/ceres4.jpg"};
  ia::ImageAnnotationList list;
  list.insert(std::make_shared<ia::ImageAnnotation>(
    image_list[0], "asdfa", "ljklg", 0, 
    fish_detector::Rect(1, 2, 3, 4)));
  list.insert(std::make_shared<ia::ImageAnnotation>(
    image_list[0], "ahsdifuasega", "ljsdasdhaklg", 1, 
    fish_detector::Rect(3, 2, 1, 9)));
  list.insert(std::make_shared<ia::ImageAnnotation>(
    image_list[1], "ahspda8", "asha", 0, 
    fish_detector::Rect(4, 3, 2, 8)));
  list.insert(std::make_shared<ia::ImageAnnotation>(
    image_list[3], "ahspda8", "ashaahsgha", 0, 
    fish_detector::Rect(4, 3, 2, 8)));
  QVERIFY(list.getAllSpecies().size() == 3);
  QVERIFY(list.by_file_.left.count(image_list[0]) == 2);
  list.write(image_list);
  ia::ImageAnnotationList deserialized_list;
  deserialized_list.read(image_list);
  QVERIFY(list == deserialized_list);
  list.remove(image_list[0], 1);
  list.write(image_list);
  ia::ImageAnnotationList deserialized_list_1;
  deserialized_list_1.read(image_list);
  QVERIFY(list == deserialized_list_1);
  QVERIFY(list.by_file_.left.count(image_list[0]) == 1);
}

QTEST_MAIN(TestImageAnnotation)
#include "test_image_annotation.moc"

