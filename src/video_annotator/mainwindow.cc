#include <vector>

#include <boost/filesystem.hpp>

#include <QtMath>
#include <QTime>

#include "fish_annotator/common/species_dialog.h"
#include "fish_annotator/video_annotator/mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>
std::ofstream out("BLAHBLAH.txt");
namespace fish_annotator { namespace video_annotator {

namespace fs = boost::filesystem;

MainWindow::MainWindow(QWidget *parent)
  : annotation_(new VideoAnnotation)
  , scene_(new QGraphicsScene(this))
  , pixmap_item_(nullptr)
  , last_frame_(nullptr)
  , last_displayed_frame_(0)
  , player_(new Player(this))
  , ui_(new Ui::MainWidget)
  , species_controls_(new SpeciesControls(this))
  , was_playing_(false) 
  , fish_id_(0) 
  , current_annotations_() {
  ui_->setupUi(this);
  ui_->videoWindow->setViewport(new QOpenGLWidget);
  ui_->videoWindow->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
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
  QObject::connect(player_.get(), 
      SIGNAL(processedImage(std::shared_ptr<QImage>, uint64_t)),
      this, SLOT(showFrame(std::shared_ptr<QImage>, uint64_t)));
  QObject::connect(species_controls_.get(),
      SIGNAL(individualAdded(std::string, std::string)),
      this, SLOT(addIndividual(std::string, std::string)));
  QObject::connect(player_.get(), SIGNAL(durationChanged(uint64_t)),
      this, SLOT(handlePlayerDurationChanged(uint64_t)));
  QObject::connect(player_.get(), SIGNAL(positionChanged(uint64_t)),
      this, SLOT(handlePlayerPositionChanged(uint64_t)));
  QObject::connect(player_.get(), SIGNAL(playbackRateChanged(double)),
      this, SLOT(handlePlayerPlaybackRateChanged(double)));
  QObject::connect(player_.get(), SIGNAL(error(const std::string&)),
      this, SLOT(handlePlayerError(const std::string&)));
  QObject::connect(player_.get(), SIGNAL(mediaLoaded()),
      this, SLOT(handlePlayerMedia()));
}

void MainWindow::resizeEvent(QResizeEvent *event) {
  ui_->videoWindow->fitInView(scene_->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::on_play_clicked() {
  if(player_->isStopped() == true) {
    player_->play();
    ui_->play->setText("Pause");
    ui_->reverse->setEnabled(true);
    ui_->plusOneFrame->setEnabled(false);
    ui_->minusOneFrame->setEnabled(false);
  }
  else {
    player_->stop();
    ui_->play->setText("Play");
    ui_->reverse->setEnabled(false);
    ui_->plusOneFrame->setEnabled(true);
    ui_->minusOneFrame->setEnabled(true);
  }
}

void MainWindow::on_reverse_stateChanged(int state) {
}

void MainWindow::on_faster_clicked() {
  player_->speedUp();
}

void MainWindow::on_slower_clicked() {
  player_->slowDown();
}

void MainWindow::on_plusOneFrame_clicked() {
}

void MainWindow::on_minusOneFrame_clicked() {
}

void MainWindow::on_loadVideo_clicked() {
  QString file_str = QFileDialog::getOpenFileName(
      this,
      tr("Open Video"), ".",
      tr("Video Files (*.avi *.mpg *.mp4 *.mkv)"));
  QFileInfo file(file_str);
  if(file.exists() && file.isFile()) {
    out << "ATTEMPTING TO LOAD VIDEO: " << file_str.toStdString() << std::endl;
    player_->loadVideo(file_str.toStdString());
    out << "LOADED THE VIDEO" << std::endl;
    fs::path csv(file_str.toStdString());
    csv.replace_extension(".csv");
    fs::path json(file_str.toStdString());
    json.replace_extension(".json");
    if(fs::exists(csv) && fs::exists(json)) {
      auto reply = QMessageBox::question(this,
          "Annotation File Found",
          "Found annotation file with same base path,"
          " would you like to load it?",
          QMessageBox::Yes | QMessageBox::No);
      if(reply == QMessageBox::Yes) {
        annotation_->read(csv.string());
        drawAnnotations();
      }
    }
  }
}

void MainWindow::on_loadAnnotationFile_clicked() {
  QString file_str = QFileDialog::getOpenFileName(
      this,
      tr("Open Annotation File"), ".",
      tr("Annotation Files (*.csv)"));
  QFileInfo file(file_str);
  if(file.exists() && file.isFile()) {
    annotation_->read(file_str.toStdString());
    drawAnnotations();
  }
}

void MainWindow::on_saveAnnotationFile_clicked() {
  fs::path vid_path(player_->getVideoPath());
  annotation_->write(
      vid_path.replace_extension(".csv"),
      ui_->tripIDValue->text().toStdString(),
      ui_->towIDValue->text().toStdString(),
      ui_->reviewerNameValue->text().toStdString(),
      ui_->towStatus->isChecked() ? "Open" : "Closed",
      player_->getFrameRate());
}

void MainWindow::on_writeImage_clicked() {
}

void MainWindow::on_videoSlider_sliderPressed() {
  was_playing_ = player_->isStopped() == false;
  player_->stop();
}

void MainWindow::on_videoSlider_sliderReleased() {
  if(was_playing_ == true) player_->play();
}

void MainWindow::on_videoSlider_valueChanged(int value) {
  player_->setFrame(value);
}

void MainWindow::on_typeMenu_currentTextChanged(const QString &text) {
  auto trk = annotation_->findTrack(fish_id_);
  if(trk != nullptr) {
    trk->species_ = text.toStdString();
  }
}

void MainWindow::on_subTypeMenu_currentTextChanged(const QString &text) {
  auto trk = annotation_->findTrack(fish_id_);
  if(trk != nullptr) {
    trk->subspecies_ = text.toStdString();
  }
}

void MainWindow::on_prevFish_clicked() {
  auto trk = annotation_->prevTrack(fish_id_);
  if(trk != nullptr) {
    fish_id_ = trk->id_;
    updateStats();
  }
}

void MainWindow::on_nextFish_clicked() {
  auto trk = annotation_->nextTrack(fish_id_);
  if(trk != nullptr) {
    fish_id_ = trk->id_;
    updateStats();
  }
}

void MainWindow::on_removeFish_clicked() {
  annotation_->remove(fish_id_);
  drawAnnotations();
}

void MainWindow::on_goToFrame_clicked() {
  auto frame = annotation_->trackFirstFrame(fish_id_);
  player_->setFrame(frame);
}

void MainWindow::on_goToFishVal_returnPressed() {
  fish_id_ = ui_->goToFishVal->text().toInt();
  updateStats();
}

void MainWindow::on_addRegion_clicked() {
  annotation_->insert(std::make_shared<DetectionAnnotation>(
        last_displayed_frame_,
        fish_id_,
        Rect(0, 0, 100, 100)));
  drawAnnotations();
}

void MainWindow::on_removeRegion_clicked() {
  annotation_->remove(last_displayed_frame_, fish_id_);
  drawAnnotations();
}

void MainWindow::on_nextAndCopy_clicked() {
  auto det = annotation_->findDetection(last_displayed_frame_, fish_id_);
  if(det != nullptr) {
    on_plusOneFrame_clicked();
    annotation_->insert(std::make_shared<DetectionAnnotation>(
          last_displayed_frame_,
          fish_id_,
          det->area_));
    drawAnnotations();
  }
  else {
    QMessageBox msgBox;
    msgBox.setText("Could not find region to copy!");
    msgBox.exec();
  }
}

void MainWindow::showFrame(std::shared_ptr<QImage> image, uint64_t frame) {
  out << "RECEIVED FRAME AT: " << frame << std::endl;
  last_frame_ = image;
  auto pixmap = QPixmap::fromImage(*image);
  if(pixmap_item_ == nullptr) {
    pixmap_item_ = scene_->addPixmap(pixmap);
    scene_->setSceneRect(0, 0, image->width(), image->height());
    ui_->videoWindow->setScene(scene_.get());
    ui_->videoWindow->fitInView(scene_->sceneRect(), Qt::KeepAspectRatio);
    ui_->videoWindow->show();
  }
  else {
    pixmap_item_->setPixmap(pixmap);
  }
  last_displayed_frame_ = frame;
  drawAnnotations();
}

void MainWindow::addIndividual(std::string species, std::string subspecies) {
  fish_id_ = annotation_->nextId();
  annotation_->insert(std::make_shared<TrackAnnotation>(
        fish_id_, species, subspecies));
  on_addRegion_clicked();
  updateStats();
  drawAnnotations();
}

void MainWindow::handlePlayerDurationChanged(uint64_t duration) {
  ui_->videoSlider->setRange(0, duration);
  ui_->videoSlider->setSingleStep(1000.0);
  ui_->videoSlider->setPageStep(10000.0);
}

void MainWindow::handlePlayerPositionChanged(uint64_t position) {
  ui_->videoSlider->setValue(position);
}

void MainWindow::handlePlayerPlaybackRateChanged(double rate) {
  ui_->currentSpeed->setText(QString("Current Speed: %1%").arg(rate * 100));
}

void MainWindow::handlePlayerError(const std::string &err) {
  QMessageBox msgBox;
  msgBox.setText(err.c_str());
  msgBox.exec();
}

void MainWindow::handlePlayerMedia() {
  ui_->videoSlider->setEnabled(true);
  ui_->play->setEnabled(true);
  ui_->faster->setEnabled(true);
  ui_->slower->setEnabled(true);
  ui_->minusOneFrame->setEnabled(true);
  ui_->plusOneFrame->setEnabled(true);
  ui_->loadVideo->setEnabled(true);
  ui_->loadAnnotationFile->setEnabled(true);
  ui_->saveAnnotationFile->setEnabled(true);
  ui_->writeImage->setEnabled(true);
  ui_->typeLabel->setEnabled(true);
  ui_->typeMenu->setEnabled(true);
  ui_->subTypeLabel->setEnabled(true);
  ui_->subTypeMenu->setEnabled(true);
  ui_->prevFish->setEnabled(true);
  ui_->nextFish->setEnabled(true);
  ui_->removeFish->setEnabled(true);
  ui_->goToFrame->setEnabled(true);
  ui_->goToFishLabel->setEnabled(true);
  ui_->goToFishVal->setEnabled(true);
  ui_->addRegion->setEnabled(true);
  ui_->removeRegion->setEnabled(true);
  ui_->nextAndCopy->setEnabled(true);
  ui_->currentSpeed->setText("Current Speed: 100%");
  this->setWindowTitle(player_->getVideoPath().c_str());
  annotation_->clear();
  scene_->clear();
  pixmap_item_ = nullptr;
  last_frame_ = nullptr;
}

void MainWindow::updateStats() {
  ui_->fishNumVal->setText(QString::number(fish_id_));
  ui_->totalFishVal->setText(QString::number(annotation_->getTotal()));
  ui_->frameCountedVal->setText(QString::number(
        annotation_->trackFirstFrame(fish_id_)));
}

void MainWindow::drawAnnotations() {
  for(auto ann : current_annotations_) {
    scene_->removeItem(ann);
  }
  current_annotations_.clear();
  for(auto ann : annotation_->getDetectionAnnotations(last_displayed_frame_)) {
    auto region = new AnnotatedRegion<DetectionAnnotation>(
        ann->id_, ann, pixmap_item_->pixmap().toImage().rect());
    scene_->addItem(region);
    current_annotations_.push_back(region);
  }
}

#include "../../include/fish_annotator/video_annotator/moc_mainwindow.cpp"

}} // namespace fish_annotator::video_annotator

