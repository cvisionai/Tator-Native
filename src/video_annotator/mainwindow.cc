#include <vector>

#include <boost/filesystem.hpp>

#include <QtMath>
#include <QTime>

#include "fish_annotator/common/species_dialog.h"
#include "fish_annotator/video_annotator/mainwindow.h"
#include "ui_mainwindow.h"

namespace fish_annotator { namespace video_annotator {

namespace fs = boost::filesystem;

MainWindow::MainWindow(QWidget *parent)
  : QWidget(parent)
  , annotation_(new VideoAnnotation)
  , scene_(new QGraphicsScene)
  , pixmap_item_(nullptr)
  , ui_(new Ui::MainWidget)
  , species_controls_(new SpeciesControls)
  , video_path_()
  , width_(0)
  , height_(0)
  , last_frame_(nullptr)
  , last_position_(0)
  , stopped_(true) 
  , was_stopped_(true)
  , rate_(0.0)
  , native_rate_(0.0)
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
  ui_->videoWindow->setScene(scene_.get());
  QObject::connect(species_controls_.get(), &SpeciesControls::individualAdded,
      this, &MainWindow::addIndividual);
  Player *player = new Player();
  QThread *thread = new QThread();
  QObject::connect(player, &Player::processedImage, 
      this, &MainWindow::showFrame);
  QObject::connect(player, &Player::durationChanged, 
      this, &MainWindow::handlePlayerDurationChanged);
  QObject::connect(player, &Player::playbackRateChanged,
      this, &MainWindow::handlePlayerPlaybackRateChanged);
  QObject::connect(player, &Player::resolutionChanged,
      this, &MainWindow::handlePlayerResolutionChanged);
  QObject::connect(player, &Player::stateChanged,
      this, &MainWindow::handlePlayerStateChanged);
  QObject::connect(player, &Player::mediaLoaded,
      this, &MainWindow::handlePlayerMediaLoaded);
  QObject::connect(player, &Player::error,
      this, &MainWindow::handlePlayerError);
  QObject::connect(this, &MainWindow::requestLoadVideo, 
      player, &Player::loadVideo);
  QObject::connect(this, &MainWindow::requestPlay, 
      player, &Player::play);
  QObject::connect(this, &MainWindow::requestStop, 
      player, &Player::stop);
  QObject::connect(this, &MainWindow::requestSpeedUp, 
      player, &Player::speedUp);
  QObject::connect(this, &MainWindow::requestSlowDown, 
      player, &Player::slowDown);
  QObject::connect(this, &MainWindow::requestSetFrame,
      player, &Player::setFrame);
  QObject::connect(this, &MainWindow::requestNextFrame,
      player, &Player::nextFrame);
  QObject::connect(this, &MainWindow::requestPrevFrame,
      player, &Player::prevFrame);
  QObject::connect(thread, &QThread::finished,
      player, &Player::deleteLater);
  QObject::connect(thread, &QThread::finished,
      thread, &QThread::deleteLater);
  player->moveToThread(thread);
  thread->start();
}

void MainWindow::resizeEvent(QResizeEvent *event) {
  ui_->videoWindow->fitInView(scene_->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::on_play_clicked() {
  if(stopped_ == true) {
    emit requestPlay();
    ui_->play->setText("Pause");
    ui_->minusOneSecond->setEnabled(false);
    ui_->minusThreeSecond->setEnabled(false);
    ui_->plusOneFrame->setEnabled(false);
    ui_->minusOneFrame->setEnabled(false);
  }
  else {
    emit requestStop();
    ui_->play->setText("Play");
    ui_->minusOneSecond->setEnabled(true);
    ui_->minusThreeSecond->setEnabled(true);
    ui_->plusOneFrame->setEnabled(true);
    ui_->minusOneFrame->setEnabled(true);
  }
}

void MainWindow::on_faster_clicked() {
  emit requestSpeedUp();
}

void MainWindow::on_slower_clicked() {
  emit requestSlowDown();
}

void MainWindow::on_minusOneSecond_clicked() {
  qint64 frame = last_position_;
  frame -= static_cast<qint64>(std::round(rate_));
  emit requestSetFrame(frame);
}

void MainWindow::on_minusThreeSecond_clicked() {
  qint64 frame = last_position_;
  frame -= static_cast<qint64>(std::round(rate_ * 3.0));
  emit requestSetFrame(frame);
}

void MainWindow::on_plusOneFrame_clicked() {
  emit requestNextFrame();
}

void MainWindow::on_minusOneFrame_clicked() {
  emit requestPrevFrame();
}

void MainWindow::on_loadVideo_clicked() {
  QString file_str = QFileDialog::getOpenFileName(
      this,
      tr("Open Video"), 
      QFileInfo(video_path_).dir().canonicalPath(),
      tr("Video Files (*.avi *.mpg *.mp4 *.mkv)"));
  QFileInfo file(file_str);
  if(file.exists() && file.isFile()) {
    emit requestLoadVideo(file_str);
  }
}

void MainWindow::on_loadAnnotationFile_clicked() {
  QString file_str = QFileDialog::getOpenFileName(
      this,
      tr("Open Annotation File"),
      QFileInfo(video_path_).dir().canonicalPath(),
      tr("Annotation Files (*.csv)"));
  QFileInfo file(file_str);
  if(file.exists() && file.isFile()) {
    annotation_->read(file_str.toStdString());
    species_controls_->loadFromVector(annotation_->getAllSpecies());
    fish_id_ = annotation_->earliestTrackID();
    if(fish_id_ != 0) {
      updateSpeciesCounts();
      updateStats();
      drawAnnotations();
    }
  }
}

void MainWindow::on_saveAnnotationFile_clicked() {
  std::string filename = ui_->fileNameValue->text().toStdString();
  std::string reviewer = ui_->reviewerNameValue->text().toStdString();
  if(filename.empty() == true && reviewer.empty() == true) {
    filename = video_path_.toStdString();
  }
  else if(filename.empty() == true && reviewer.empty() == false) {
    fs::path vid_path(video_path_.toStdString());
    fs::path out_path = 
      vid_path.parent_path() / 
      fs::path(vid_path.stem().string() + 
      std::string("_") + 
      reviewer + 
      std::string(".csv"));
    filename = out_path.string();
  }
  else {
    filename = filename + std::string("_") + reviewer + std::string(".csv");
  }
  fs::path vid_path(filename);
  annotation_->write(
      vid_path.replace_extension(".csv"),
      ui_->tripIDValue->text().toStdString(),
      ui_->towIDValue->text().toStdString(),
      ui_->reviewerNameValue->text().toStdString(),
      ui_->towStatus->isChecked() ? "Open" : "Closed",
      native_rate_);
}

void MainWindow::on_writeImage_clicked() {
}

void MainWindow::on_videoSlider_sliderPressed() {
  was_stopped_ = stopped_;
  emit requestStop();
}

void MainWindow::on_videoSlider_sliderReleased() {
  emit requestSetFrame(ui_->videoSlider->sliderPosition());
  if(was_stopped_ == false) emit requestPlay();
}

void MainWindow::on_videoSlider_actionTriggered(int action) {
  emit requestSetFrame(ui_->videoSlider->sliderPosition());
}

void MainWindow::on_typeMenu_activated(const QString &text) {
  auto trk = annotation_->findTrack(fish_id_);
  if(trk != nullptr) {
    trk->species_ = text.toStdString();
    updateStats();
  }
}

void MainWindow::on_subTypeMenu_activated(const QString &text) {
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
  updateSpeciesCounts();
  drawAnnotations();
}

void MainWindow::on_goToFrame_clicked() {
  qint64 frame = annotation_->trackFirstFrame(fish_id_);
  emit requestSetFrame(frame);
}

void MainWindow::on_goToFishVal_returnPressed() {
  auto trk = annotation_->findTrack(ui_->goToFishVal->text().toInt());
  if(trk != nullptr) {
    fish_id_ = ui_->goToFishVal->text().toInt();
    updateStats();
  }
  else {
    handlePlayerError("Fish with that ID does not exist!");
  }
}

void MainWindow::on_addRegion_clicked() {
  if(annotation_->getTotal() < 1) {
    handlePlayerError("Please add a fish before adding a region!");
  }
  else {
    annotation_->insert(std::make_shared<DetectionAnnotation>(
          last_position_,
          fish_id_,
          Rect(0, 0, 100, 100)));
    drawAnnotations();
  }
}

void MainWindow::on_removeRegion_clicked() {
  annotation_->remove(last_position_, fish_id_);
  drawAnnotations();
}

void MainWindow::on_nextAndCopy_clicked() {
  auto det = annotation_->findDetection(last_position_, fish_id_);
  if(det != nullptr) {
    annotation_->insert(std::make_shared<DetectionAnnotation>(
          last_position_ + 1,
          fish_id_,
          det->area_));
    on_plusOneFrame_clicked();
  }
  else {
    QMessageBox msgBox;
    msgBox.setText("Could not find region to copy!");
    msgBox.exec();
  }
}

void MainWindow::on_degradedStatus_stateChanged(int state) {
  if(state == Qt::Checked) {
    annotation_->setDegraded(last_position_, true);
  }
  else if(state == Qt::Unchecked) {
    annotation_->setDegraded(last_position_, false);
  }
}

void MainWindow::showFrame(QImage image, qint64 frame) {
  last_frame_ = image;
  auto pixmap = QPixmap::fromImage(image);
  pixmap_item_->setPixmap(pixmap);
  ui_->videoWindow->fitInView(scene_->sceneRect(), Qt::KeepAspectRatio);
  last_position_ = frame;
  drawAnnotations();
  ui_->videoSlider->setValue(static_cast<int>(frame));
}

void MainWindow::addIndividual(std::string species, std::string subspecies) {
  fish_id_ = annotation_->nextId();
  annotation_->insert(std::make_shared<TrackAnnotation>(
        fish_id_, species, subspecies, last_position_));
  on_addRegion_clicked();
  updateSpeciesCounts();
  updateStats();
  drawAnnotations();
}

void MainWindow::handlePlayerDurationChanged(qint64 duration) {
  ui_->videoSlider->setRange(0, duration);
  ui_->videoSlider->setSingleStep(1);
  ui_->videoSlider->setPageStep(duration / 20);
}

void MainWindow::handlePlayerPlaybackRateChanged(double rate) {
  rate_ = rate;
  qint64 rate_percent = std::round(100.0 * rate_ / native_rate_);
  ui_->currentSpeed->setText(QString("Current Speed: %1%").arg(rate_percent));
}

void MainWindow::handlePlayerResolutionChanged(qint64 width, qint64 height) {
  width_ = width;
  height_ = height;
}

void MainWindow::handlePlayerStateChanged(bool stopped) {
  stopped_ = stopped;
}

void MainWindow::handlePlayerMediaLoaded(
  QString video_path, 
  qreal native_rate) {
  video_path_ = video_path;
  native_rate_ = native_rate;
  ui_->videoSlider->setEnabled(true);
  ui_->play->setEnabled(true);
  ui_->faster->setEnabled(true);
  ui_->slower->setEnabled(true);
  ui_->minusOneSecond->setEnabled(true);
  ui_->minusThreeSecond->setEnabled(true);
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
  this->setWindowTitle(video_path_);
  annotation_->clear();
  scene_->clear();
  QPixmap pixmap(width_, height_);
  pixmap_item_ = scene_->addPixmap(pixmap);
  scene_->setSceneRect(0, 0, width_, height_);
  ui_->videoWindow->show();
  emit requestNextFrame();
}

void MainWindow::handlePlayerError(QString err) {
  QMessageBox msgBox;
  msgBox.setText(err);
  msgBox.exec();
}

void MainWindow::updateSpeciesCounts() {
  auto counts = annotation_->getCounts();
  for(auto it = counts.begin(); it != counts.end(); it++) {
    species_controls_->setCount(it->second, it->first);
  }
}

void MainWindow::updateStats() {
  auto trk = annotation_->findTrack(fish_id_);
  ui_->fishNumVal->setText(QString::number(fish_id_));
  ui_->totalFishVal->setText(QString::number(annotation_->getTotal()));
  ui_->frameCountedVal->setText(QString::number(trk->frame_added_));
  ui_->typeMenu->clear();
  ui_->subTypeMenu->clear();
  auto species = species_controls_->getSpecies();
  for(auto &s : species) {
    ui_->typeMenu->addItem(s.getName().c_str());
    if(s.getName() == trk->species_) {
      ui_->typeMenu->setCurrentText(s.getName().c_str());
      auto subspecies = s.getSubspecies();
      for(auto &sub : subspecies) {
        ui_->subTypeMenu->addItem(sub.c_str());
        if(sub == trk->subspecies_) {
          ui_->subTypeMenu->setCurrentText(sub.c_str());
        }
      }
    }
  }
}

void MainWindow::drawAnnotations() {
  for(auto ann : current_annotations_) {
    scene_->removeItem(ann);
  }
  current_annotations_.clear();
  for(auto ann : annotation_->getDetectionAnnotations(last_position_)) {
    auto region = new AnnotatedRegion<DetectionAnnotation>(
        ann->id_, ann, pixmap_item_->pixmap().toImage().rect());
    scene_->addItem(region);
    current_annotations_.push_back(region);
  }
}

#include "../../include/fish_annotator/video_annotator/moc_mainwindow.cpp"

}} // namespace fish_annotator::video_annotator

