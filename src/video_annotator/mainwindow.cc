#include <vector>

#include <QtMath>

#include "fish_annotator/common/species_dialog.h"
#include "fish_annotator/video_annotator/mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>
std::ofstream out("BLAHBLAH.txt");
namespace fish_annotator { namespace video_annotator {

MainWindow::MainWindow(QWidget *parent)
  : annotation_(new VideoAnnotation)
  , scene_(new QGraphicsScene)
  , player_(new QMediaPlayer(this, QMediaPlayer::VideoSurface))
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
  QGraphicsVideoItem *item = new QGraphicsVideoItem;
  player_->setVideoOutput(item);
  scene_->addItem(item);
  ui_->videoWindow->setScene(scene_.get());
  ui_->videoWindow->show();
  QObject::connect(species_controls_.get(),
      SIGNAL(individualAdded(std::string, std::string)),
      this, SLOT(addIndividual(std::string, std::string)));
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
  qreal rate = player_->metaData(QMediaMetaData::VideoFrameRate).toReal();
  player_->setPosition(player_->position() + 1000.0 / rate);
}

void MainWindow::on_minusOneFrame_clicked() {
  qreal rate = player_->metaData(QMediaMetaData::VideoFrameRate).toReal();
  player_->setPosition(player_->position() - 1000.0 / rate);
}

void MainWindow::on_loadVideo_clicked() {
  QString file_str = QFileDialog::getOpenFileName(
      this,
      tr("Open Video"), ".",
      tr("Video Files (*.avi *.mpg *.mp4 *.mkv)"));
  QFileInfo file(file_str);
  if(file.exists() && file.isFile()) {
    player_->setMedia(QUrl::fromLocalFile(file_str));
    player_->setNotifyInterval(1);
  }
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
    this->setWindowTitle(player_->media().canonicalUrl().path());
    ui_->play->setFocus();
    ui_->videoSlider->setRange(0, player_->duration());
    annotation_.reset(new VideoAnnotation);
    on_play_clicked();
  }
}

#include "../../include/fish_annotator/video_annotator/moc_mainwindow.cpp"

}} // namespace fish_annotator::video_annotator

