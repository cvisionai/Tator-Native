#include <QtPlugin>

#include <boost/filesystem.hpp>

#include "test_image_annotator.h"
#include "fish_detector/image_annotator/mainwindow.h"

#ifdef _WIN32
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
#endif

Q_IMPORT_PLUGIN(QDDSPlugin)
Q_IMPORT_PLUGIN(QGifPlugin)
Q_IMPORT_PLUGIN(QICNSPlugin)
Q_IMPORT_PLUGIN(QICOPlugin)
Q_IMPORT_PLUGIN(QJpegPlugin)
Q_IMPORT_PLUGIN(QSvgPlugin)
Q_IMPORT_PLUGIN(QTgaPlugin)
Q_IMPORT_PLUGIN(QTiffPlugin)
Q_IMPORT_PLUGIN(QWbmpPlugin)
Q_IMPORT_PLUGIN(QWebpPlugin)

namespace fs = boost::filesystem;

void TestImageAnnotator::testLoadImageDir() {
  fish_detector::image_annotator::MainWindow mainwin;
  fs::path cwd(fs::current_path());
  fs::path full = cwd / fs::path("crashed");
  mainwin.onLoadDirectorySuccess(full.string().c_str());
}

void TestImageAnnotator::testLoadSpeciesFile() {
  fish_detector::image_annotator::MainWindow mainwin;
  fs::path cwd(fs::current_path());
  fs::path full = cwd / fs::path("blah.json");
  mainwin.species_controls_->loadSpeciesFile(full.string().c_str());
}

QTEST_MAIN(TestImageAnnotator)
#include "test_image_annotator.moc"

