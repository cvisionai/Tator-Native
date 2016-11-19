#include <QtPlugin>

#include "test_video_annotator.h"
#include "fish_detector/video_annotator/mainwindow.h"

#ifdef _WIN32
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
#endif

void TestVideoAnnotator::testLoadVideo() {
  fish_detector::video_annotator::MainWindow mainwin;
  mainwin.player_->loadVideo("slow_motion_drop.mp4");
}
void TestVideoAnnotator::testSaveAnnotationsNoFish() {
}

QTEST_MAIN(TestVideoAnnotator)
#include "test_video_annotator.moc"

