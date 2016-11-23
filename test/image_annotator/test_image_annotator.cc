#include <QtPlugin>

#include "test_image_annotator.h"
#include "fish_detector/image_annotator/mainwindow.h"

#ifdef _WIN32
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
#endif

void TestImageAnnotator::testLoadImageDir() {
  fish_detector::image_annotator::MainWindow mainwin;
  mainwin.onLoadDirectorySuccess("crashed");
}

QTEST_MAIN(TestImageAnnotator)
#include "test_image_annotator.moc"

