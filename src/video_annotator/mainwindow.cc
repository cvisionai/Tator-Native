#include <vector>

#include "fish_annotator/common/species_dialog.h"
#include "fish_annotator/video_annotator/mainwindow.h"
#include "ui_mainwindow.h"

namespace fish_annotator { namespace video_annotator {

namespace { //anonymous

static const std::vector<std::string> kVideoExtensions = {
  ".mp4", ".avi",
  ".MP4", ".AVI"};

} // anonymous namespace

MainWindow::MainWindow(QWidget *parent)
  : annotation_(new VideoAnnotation)
  , scene_(new QGraphicsScene)
  , player_(new QMediaPlayer)
  , ui_(new Ui::MainWidget)
  , species_controls_(new SpeciesControls(this))
  , video_file_() {
  ui_->setupUi(this);
#ifdef _WIN32
  setWindowIcon(QIcon(":/icons/FishAnnotator.ico"));
#endif
  setStyleSheet("QPushButton { background-color: rgb(230, 230, 230);"
	  "border-style: outset; border-radius: 5px; border-width: 2px; "
    "border-color: grey; padding: 6px;}"
	  "QPushButton:pressed{background-color: rgb(190, 190, 190); "
    "border-style: outset; border-radius: 5px;"
	  "border-width: 2px; border-color: grey; padding: 6px;}");
  ui_->sideBarLayout->addWidget(species_controls_.get());
  QObject::connect(species_controls_.get(),
      SIGNAL(individualAdded(std::string, std::string)),
      this, SLOT(addIndividual(std::string, std::string)));
}

void MainWindow::on_play_clicked() {
}

void MainWindow::on_reverse_clicked() {
}

void MainWindow::on_faster_clicked() {
}

void MainWindow::on_slower_clicked() {
}

void MainWindow::on_plusOneFrame_clicked() {
}

void MainWindow::on_minusOneFrame_clicked() {
}

void MainWindow::on_loadVideo_clicked() {
}

void MainWindow::on_loadAnnotationFile_clicked() {
}

void MainWindow::on_saveAnnotationFile_clicked() {
}

void MainWindow::on_writeImage_clicked() {
}

void MainWindow::on_videoSlider_sliderPressed() {
}

void MainWindow::on_videoSlider_sliderReleased() {
}

void MainWindow::on_videoSlider_valueChanged() {
}

void MainWindow::on_typeMenu_currentTextChanged() {
}

void MainWindow::on_subTypeMenu_currentTextChanged() {
}

void MainWindow::on_prevFish_clicked() {
}

void MainWindow::on_nextFish_clicked() {
}

void MainWindow::on_removeFish_clicked() {
}

void MainWindow::on_goToFrame_clicked() {
}

void MainWindow::on_goToFishVal_returnPressed() {
}

void MainWindow::on_addRegion_clicked() {
}

void MainWindow::on_removeRegion_clicked() {
}

void MainWindow::on_nextAndCopy_clicked() {
}

void MainWindow::addIndividual(std::string species, std::string subspecies) {
}

void MainWindow::onLoadVideoSuccess(const QString &video_path) {
}

#include "../../include/fish_annotator/video_annotator/moc_mainwindow.cpp"

}} // namespace fish_annotator::video_annotator

