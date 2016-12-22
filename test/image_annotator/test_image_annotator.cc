#include <QtPlugin>

#include <boost/filesystem.hpp>

#include "test_image_annotator.h"
#include "fish_annotator/image_annotator/mainwindow.h"

#ifdef _WIN32
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
#elif __APPLE__
Q_IMPORT_PLUGIN(QCocoaIntegrationPlugin)
#elif __unix__
Q_IMPORT_PLUGIN(QXcbIntegrationPlugin)
#endif

#ifdef _WIN32
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
#elif __APPLE__
Q_IMPORT_PLUGIN(QICOPlugin)
#endif

namespace fs = boost::filesystem;

void TestImageAnnotator::testLoadImageDir() {
  fish_annotator::image_annotator::MainWindow mainwin;
  fs::path cwd(fs::current_path());
  fs::path full = cwd / fs::path("crashed");
  mainwin.onLoadDirectorySuccess(full.string().c_str());
}

void TestImageAnnotator::testLoadSpeciesFile() {
  fish_annotator::image_annotator::MainWindow mainwin;
  fs::path cwd(fs::current_path());
  fs::path full = cwd / fs::path("blah.json");
  mainwin.species_controls_->loadSpeciesFile(full.string().c_str());
}

void TestImageAnnotator::testAddRegionAndNext() {
  fish_annotator::image_annotator::MainWindow mainwin;
  fs::path cwd(fs::current_path());
  fs::path full = cwd / fs::path("crashed");
  mainwin.onLoadDirectorySuccess(full.string().c_str());
  mainwin.addIndividual("sahsdogiha", "jdhbfkaj");
  mainwin.on_next_clicked();
}

QTEST_MAIN(TestImageAnnotator)
#include "test_image_annotator.moc"

