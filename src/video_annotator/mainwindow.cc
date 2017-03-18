#include <vector>

#include <boost/filesystem.hpp>

#include <QtMath>

#include "fish_annotator/common/species_dialog.h"
#include "fish_annotator/video_annotator/mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>
std::ofstream out("BLAHBLAH.txt");
namespace fish_annotator { namespace video_annotator {

namespace fs = boost::filesystem;

MainWindow::MainWindow(QWidget *parent)
  : annotation_(new VideoAnnotation)
  , scene_(new QGraphicsScene)
  , player_(new QMediaPlayer(this, QMediaPlayer::VideoSurface))
  , ui_(new Ui::MainWidget)
  , species_controls_(new SpeciesControls(this))
  , video_file_() 
  , was_playing_(false) 
  , fish_id_(0) {
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
  QGraphicsVideoItem *item = new QGraphicsVideoItem;
  player_->setVideoOutput(item);
  player_->setNotifyInterval(250);
  scene_->addItem(item);
  ui_->videoWindow->setScene(scene_.get());
  ui_->videoWindow->show();
  QObject::connect(species_controls_.get(),
      SIGNAL(individualAdded(std::string, std::string)),
      this, SLOT(addIndividual(std::string, std::string)));
  QObject::connect(player_.get(), SIGNAL(durationChanged(qint64)),
      this, SLOT(handlePlayerDurationChanged(qint64)));
  QObject::connect(player_.get(), SIGNAL(positionChanged(qint64)),
      this, SLOT(handlePlayerPositionChanged(qint64)));
  QObject::connect(player_.get(), SIGNAL(playbackRateChanged(qreal)),
      this, SLOT(handlePlayerPlaybackRateChanged(qreal)));
  QObject::connect(player_.get(), SIGNAL(error(QMediaPlayer::Error)),
      this, SLOT(handlePlayerError()));
  QObject::connect(player_.get(), 
      SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
      this, SLOT(handlePlayerMedia(QMediaPlayer::MediaStatus)));
}

void MainWindow::on_play_clicked() {
  if(player_->state() != QMediaPlayer::PlayingState) {
    player_->play();
    ui_->play->setText("Pause");
    ui_->reverse->setEnabled(true);
  }
  else {
    player_->pause();
    ui_->play->setText("Play");
    ui_->reverse->setEnabled(false);
  }
}

void MainWindow::on_reverse_stateChanged(int state) {
  qreal rate = qFabs(player_->playbackRate());
  if(state == Qt::Checked) {
    player_->setPlaybackRate(-rate);
  }
  else if(state == Qt::Unchecked) {
    player_->setPlaybackRate(rate);
  }
}

void MainWindow::on_faster_clicked() {
  qreal rate = player_->playbackRate() * 2.0;
  player_->setPlaybackRate(rate);
}

void MainWindow::on_slower_clicked() {
  qreal rate = player_->playbackRate() / 2.0;
  player_->setPlaybackRate(rate);
}

void MainWindow::on_plusOneFrame_clicked() {
  qreal fps = player_->metaData(QMediaMetaData::VideoFrameRate).toReal();
  player_->setPosition(player_->position() + 1000.0 / fps);
}

void MainWindow::on_minusOneFrame_clicked() {
  qreal fps = player_->metaData(QMediaMetaData::VideoFrameRate).toReal();
  player_->setPosition(player_->position() - 1000.0 / fps);
}

void MainWindow::on_loadVideo_clicked() {
  QString file_str = QFileDialog::getOpenFileName(
      this,
      tr("Open Video"), ".",
      tr("Video Files (*.avi *.mpg *.mp4 *.mkv)"));
  QFileInfo file(file_str);
  if(file.exists() && file.isFile()) {
    player_->setMedia(QUrl::fromLocalFile(file_str));
  }
}

void MainWindow::on_loadAnnotationFile_clicked() {
  QString file_str = QFileDialog::getOpenFileName(
      this,
      tr("Open Annotation File"), ".",
      tr("Annotation Files (*.csv)"));
  QFileInfo file(file_str);
  if(file.exists() && file.isFile()) {
    out << file_str.toStdString() << std::endl;
    annotation_->read(file_str.toStdString());
    handlePlayerPositionChanged(player_->position());
  }
}

void MainWindow::on_saveAnnotationFile_clicked() {
  fs::path vid_path(
      player_->media().canonicalUrl().toLocalFile().toStdString());
  annotation_->write(
      vid_path.replace_extension(".csv"),
      ui_->tripIDValue->text().toStdString(),
      ui_->towIDValue->text().toStdString(),
      ui_->reviewerNameValue->text().toStdString(),
      ui_->towStatus->isChecked() ? "Open" : "Closed",
      player_->metaData(QMediaMetaData::VideoFrameRate).toReal());
}

void MainWindow::on_writeImage_clicked() {
}

void MainWindow::on_videoSlider_sliderPressed() {
  was_playing_ = player_->state() == QMediaPlayer::PlayingState;
  player_->pause();
}

void MainWindow::on_videoSlider_sliderReleased() {
  if(was_playing_ == true) player_->play();
}

void MainWindow::on_videoSlider_valueChanged(int value) {
  player_->setPosition(value);
}

void MainWindow::on_typeMenu_currentTextChanged(const QString &text) {
}

void MainWindow::on_subTypeMenu_currentTextChanged(const QString &text) {
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
  annotation_->insert(std::make_shared<DetectionAnnotation>(
        currentFrame(),
        fish_id_,
        Rect(0, 0, 100, 100)));
}

void MainWindow::on_removeRegion_clicked() {
  annotation_->remove(currentFrame(), fish_id_);
}

void MainWindow::on_nextAndCopy_clicked() {
  auto det = annotation_->findDetection(currentFrame(), fish_id_);
  if(det != nullptr) {
    on_plusOneFrame_clicked();
    annotation_->insert(std::make_shared<DetectionAnnotation>(
          currentFrame(),
          fish_id_,
          det->area_));
  }
  else {
    QMessageBox msgBox;
    msgBox.setText("Could not find region to copy!");
    msgBox.exec();
  }
}

void MainWindow::addIndividual(std::string species, std::string subspecies) {
  fish_id_ = annotation_->nextId();
  annotation_->insert(std::make_shared<TrackAnnotation>(
        fish_id_, species, subspecies));
}

void MainWindow::handlePlayerDurationChanged(qint64 duration) {
  ui_->videoSlider->setRange(0, duration);
  ui_->videoSlider->setSingleStep(1000.0);
  ui_->videoSlider->setPageStep(10000.0);
}

void MainWindow::handlePlayerPositionChanged(qint64 position) {
  ui_->videoSlider->setValue(position);
}

void MainWindow::handlePlayerPlaybackRateChanged(qreal rate) {
  ui_->currentSpeed->setText(QString("Current Speed: %1%").arg(rate * 100));
}

void MainWindow::handlePlayerError() {
  QMessageBox msgBox;
  msgBox.setText(player_->errorString());
  msgBox.exec();
}

void MainWindow::handlePlayerMedia(QMediaPlayer::MediaStatus status) {
  out << "STATUS: " << static_cast<int>(status) << std::endl;
  if(status == QMediaPlayer::LoadedMedia) {
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
    this->setWindowTitle(player_->media().canonicalUrl().toLocalFile());
    annotation_.reset(new VideoAnnotation);
    on_play_clicked();
  }
}

uint64_t currentFrame() {
  qreal fps = player_->metaData(QMediaMetaData::VideoFrameRate).toReal();
  return std::round(static_cast<double>(player_->position() * fps) / 1000.0);
}

void updateStats() {
  ui_->fishNumVal->setText(QString::number(fish_id_));
  ui_->totalFishVal->setText(QString::number(annotation_->getTotal()));
  ui_->frameCountedVal->setText(QString::number(currentFrame()));
}

#include "../../include/fish_annotator/video_annotator/moc_mainwindow.cpp"

}} // namespace fish_annotator::video_annotator

