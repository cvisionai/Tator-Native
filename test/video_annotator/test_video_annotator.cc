#include <QtPlugin>

#include "test_video_annotator.h"
#include "fish_annotator/video_annotator/mainwindow.h"

#ifdef _WIN32
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
#endif

void TestVideoAnnotator::testLoadVideo() {
  fish_annotator::video_annotator::MainWindow mainwin;
  std::string name = "slow_motion_drop.mp4";
  QVERIFY(mainwin.player_->loadVideo(name));
}
void TestVideoAnnotator::testSaveAnnotationsNoFish() {
  fish_annotator::video_annotator::MainWindow mainwin;
  std::string name = "slow_motion_drop.mp4";
  QVERIFY(mainwin.player_->loadVideo(name));
  mainwin.onLoadVideoSuccess(QString::fromUtf8(name.c_str()));
  mainwin.saveAnnotations("");
}

void TestVideoAnnotator::testAddRegionNoFish() {
  fish_annotator::video_annotator::MainWindow mainwin;
  std::string name = "slow_motion_drop.mp4";
  QVERIFY(mainwin.player_->loadVideo(name));
  mainwin.onLoadVideoSuccess(QString::fromUtf8(name.c_str()));
  QVERIFY(!mainwin.addRegion());
  mainwin.on_removeRegion_clicked();
  mainwin.on_nextAndCopy_clicked();
}

void TestVideoAnnotator::testSaveAnnotations() {
  fish_annotator::video_annotator::MainWindow mainwin;
  std::string name = "slow_motion_drop.mp4";
  QVERIFY(mainwin.player_->loadVideo(name));
  mainwin.onLoadVideoSuccess(QString::fromUtf8(name.c_str()));
  mainwin.on_addFlat_clicked();
  mainwin.on_addRound_clicked();
  for(int n=0;n<4;n++) {
    mainwin.on_addRegion_clicked();
    mainwin.on_prevFish_clicked();
  }
  for(int n=0;n<5;n++) mainwin.on_nextAndCopy_clicked();
  for(int n=0;n<6;n++) mainwin.prevFrame();
  for(int n=0;n<3;n++) mainwin.on_addOther_clicked();
  for(int n=0;n<5;n++) mainwin.on_nextAndCopy_clicked();
  for(int n=0;n<10;n++) mainwin.on_addSkate_clicked();
  for(int n=0;n<10;n++) {
    mainwin.on_addRegion_clicked();
    mainwin.on_nextFish_clicked();
  }
  for(int n=0;n<3;n++) mainwin.on_nextAndCopy_clicked();
  mainwin.saveAnnotations("");
}

QTEST_MAIN(TestVideoAnnotator)
#include "test_video_annotator.moc"

