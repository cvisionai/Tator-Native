#include <QtPlugin>

#include "test_video_annotator.h"
#include "fish_detector/video_annotator/mainwindow.h"

#ifdef _WIN32
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
#endif

void TestVideoAnnotator::testLoadVideo() {
  fish_detector::video_annotator::MainWindow mainwin;
  std::string name = "C:/local/FishDetector/test/video_annotator/slow_motion_drop.mp4";
  QVERIFY(mainwin.player_->loadVideo(name));
}
void TestVideoAnnotator::testSaveAnnotationsNoFish() {
  fish_detector::video_annotator::MainWindow mainwin;
  std::string filename = "slow_motion_drop.mp4";
  QVERIFY(mainwin.player_->loadVideo(filename));
  mainwin.onLoadVideoSuccess(QString::fromUtf8(filename.c_str()));
}

QTEST_MAIN(TestVideoAnnotator)
#include "test_video_annotator.moc"

