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
  ui_sideBarLayout->addWidget(species_controls_.get());
  QObject::connect(species_controls_.get(),
      SIGNAL(individualAdded(std::string, std::string)),
      this, SLOT(addIndividual(std::string, std::string)));
}

void on_play_clicked() {
}

void on_reverse_clicked() {
}

void on_faster_clicked() {
}

void on_slower_clicked() {
}

void on_plusOneFrame_clicked() {
}

void on_minusOneFrame_clicked() {
}

void on_loadVideo_clicked() {
}

void on_loadAnnotationFile_clicked() {
}

void on_saveAnnotationFile_clicked() {
}

void on_writeImage_clicked() {
}

void on_videoSlider_sliderPressed() {
}

void on_videoSlider_sliderReleased() {
}

void on_videoSlider_valueChanged() {
}

void on_typeMenu_currentTextChanged() {
}

void on_subTypeMenu_currentTextChanged() {
}

void on_prevFish_clicked() {
}

void on_nextFish_clicked() {
}

void on_removeFish_clicked() {
}

void on_goToFrame_clicked() {
}

void on_goToFishVal_returnPressed() {
}

void on_addRegion_clicked() {
}

void on_removeRegion_clicked() {
}

void on_nextAndCopy_clicked() {
}

void addIndividual(std::string species, std::string subspecies) {
}

#include "../../include/fish_annotator/video_annotator/moc_mainwindow.cpp"

}} // namespace fish_annotator::video_annotator

