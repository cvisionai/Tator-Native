#include <QtPlugin>

#include "test_video_annotator.h"
#include "fish_detector/video_annotator/mainwindow.h"

#ifdef _WIN32
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
#endif

void TestVideoAnnotator::testLoadVideo() {
  fish_detector::video_annotator::MainWindow mainwin;
  std::string name = "slow_motion_drop.mp4";
  QVERIFY(mainwin.player_->loadVideo(name));
}
void TestVideoAnnotator::testSaveAnnotationsNoFish() {
  fish_detector::video_annotator::MainWindow mainwin;
  std::string name = "slow_motion_drop.mp4";
  QVERIFY(mainwin.player_->loadVideo(name));
  mainwin.onLoadVideoSuccess(QString::fromUtf8(name.c_str()));
  mainwin.saveAnnotations("");
}

QTEST_MAIN(TestVideoAnnotator)
#include "test_video_annotator.moc"

