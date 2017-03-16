#include <vector>

#include "fish_annotator/common/species_dialog.h"
#include "fish_annotator/video_annotator/mainwindow.h"
#include "ui_mainwindow.h"

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
  item->setPos(100,100);
  ui_->videoWindow->setScene(scene_.get());
  scene_->addItem(item);
  QString filename = "C:/local/FishDData/GP062080_clip_1080p_crop.mp4";
  player_->setMedia(QUrl::fromLocalFile(filename));
  player_->play();
  player_->setVideoOutput(item);
  //player_->setNotifyInterval(1);
  //ui_->videoWindow->fitInView(scene_->sceneRect(), Qt::KeepAspectRatio);
  //ui_->videoWindow->setSceneRect(item->boundingRect());
  //ui_->videoWindow->setScene(scene_.get());
  ui_->videoWindow->show();
  QObject::connect(species_controls_.get(),
      SIGNAL(individualAdded(std::string, std::string)),
      this, SLOT(addIndividual(std::string, std::string)));
  QObject::connect(player_.get(), SIGNAL(positionChanged(int)),
      ui_->videoSlider, SLOT(setValue(int)));
  QObject::connect(player_.get(), SIGNAL(error(QMediaPlayer::Error)),
      this, SLOT(handlePlayerError()));
}

void MainWindow::on_play_clicked() {
  if(player_->state() != QMediaPlayer::PlayingState) {
    player_->pause();
  }
  else {
    player_->play();
  }
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
  QString file = QFileDialog::getOpenFileName(
      this,
      tr("Open Video"), ".",
      tr("Video Files (*.avi *.mpg *.mp4 *.mkv)"));
  if(!file.isEmpty()) {
    player_->setMedia(QUrl::fromLocalFile(file));
    if(player_->mediaStatus() == QMediaPlayer::InvalidMedia) {
    }
    else {
      onLoadVideoSuccess(file);
    }
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
  ui_->videoSlider->setEnabled(true);
  ui_->play->setEnabled(true);
  ui_->reverse->setEnabled(true);
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
  this->setWindowTitle(video_path);
  ui_->currentSpeed->setText("Current Speed: 100%");
  ui_->play->setFocus();
  ui_->videoSlider->setRange(0, player_->duration());
  annotation_.reset(new VideoAnnotation);
}

void MainWindow::handlePlayerError() {
  QMessageBox msgBox;
  msgBox.setText(player_->errorString());
  msgBox.exec();
}

#include "../../include/fish_annotator/video_annotator/moc_mainwindow.cpp"

}} // namespace fish_annotator::video_annotator

