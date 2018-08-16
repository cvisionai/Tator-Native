#include <vector>

#include <boost/filesystem.hpp>

#include <QtMath>
#include <QTime>
#include <QCoreApplication>

#include "species_dialog.h"
#include "metadata_dialog.h"
#include "annotatedregion.h"
#include "annotated_line.h"
#include "annotated_dot.h"
#include "reassign_dialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

namespace tator { namespace video_annotator {

namespace fs = boost::filesystem;

namespace {

/// Used for colorization by track.
static const std::vector<QColor> track_colors {
  QColor(  0, 104, 132),
  QColor(  0, 144, 158),
  QColor(137, 219, 236),
  QColor(237,   0,  38),
  QColor(250, 157,   0),
  QColor(255, 208, 141),
  QColor(176,   0,  81),
  QColor(246, 131, 112),
  QColor(254, 171, 185),
  QColor(110,   0, 108),
  QColor(145,  39, 143),
  QColor(207, 151, 215),
  QColor(  0,   0,   0),
  QColor( 91,  91,  91),
  QColor(212, 212, 212)
};

} // namespace

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , annotation_(new VideoAnnotation)
  , view_(new AnnotationView)
  , scene_(new AnnotationScene(nullptr, false))
  , pixmap_item_(nullptr)
  , count_text_(nullptr)
  , ui_(new Ui::MainWindow)
  , species_controls_(new SpeciesControls)
  , annotation_widget_(new AnnotationWidget)
  , global_state_widget_(new GlobalStateWidget)
  , current_global_state_(new GlobalStateAnnotation)
  , load_progress_(nullptr)
  , video_path_()
  , width_(0)
  , height_(0)
  , last_frame_(nullptr)
  , last_position_(0)
  , stopped_(true)
  , was_stopped_(true)
  , rate_(0.0)
  , native_rate_(0.0)
  , track_id_(0)
  , current_annotations_()
  , metadata_()
  , color_map_()
  , zoom_reset_needed_(false)
  , write_image_enabled_(false) {
  ui_->setupUi(this);
  setWindowTitle("Video Annotator");
#ifdef _WIN32
  setWindowIcon(QIcon(":/icons/cvision/cvision_no_text.ico"));
#endif
  ui_->play->setIcon(
      QIcon(":/icons/video_controls/play.svg"));
  ui_->faster->setIcon(
      QIcon(":/icons/video_controls/faster.svg"));
  ui_->slower->setIcon(
      QIcon(":/icons/video_controls/slower.svg"));
  ui_->minusOneSecond->setIcon(
      QIcon(":/icons/video_controls/minus_one_second.svg"));
  ui_->minusThreeSecond->setIcon(
      QIcon(":/icons/video_controls/minus_three_seconds.svg"));
  ui_->plusOneFrame->setIcon(
      QIcon(":/icons/video_controls/plus_one_frame.svg"));
  ui_->minusOneFrame->setIcon(
      QIcon(":/icons/video_controls/minus_one_frame.svg"));
  ui_->goToFrame->setIcon(
      QIcon(":/icons/navigation/go_to_frame.svg"));
  ui_->removeTrack->setIcon(
      QIcon(":/icons/navigation/remove_track.svg"));
  ui_->nextTrack->setIcon(
      QIcon(":/icons/navigation/next_track.svg"));
  ui_->prevTrack->setIcon(
      QIcon(":/icons/navigation/prev_track.svg"));
  ui_->reassignTrack->setIcon(
      QIcon(":/icons/navigation/reassign_track.svg"));
  ui_->videoWindowLayout->addWidget(view_.get());
  ui_->speciesLayout->addWidget(annotation_widget_.get());
  ui_->speciesLayout->addWidget(species_controls_.get());
  ui_->globalStateLayout->addWidget(global_state_widget_.get());
  view_->setScene(scene_.get());
  tabifyDockWidget(
    ui_->globalStateDockWidget,
    ui_->navigationDockWidget);
  tabifyDockWidget(
    ui_->navigationDockWidget,
    ui_->speciesDockWidget);
  QObject::connect(species_controls_.get(), &SpeciesControls::individualAdded,
      this, &MainWindow::addIndividual);
  QObject::connect(species_controls_.get(), &SpeciesControls::colorChanged,
      this, &MainWindow::colorChanged);
  QObject::connect(global_state_widget_.get(), &GlobalStateWidget::stateChanged,
      this, &MainWindow::onGlobalStateChange);
  scene_->setToolWidget(annotation_widget_.get());
  QObject::connect(scene_.get(), &AnnotationScene::boxFinished,
      this, &MainWindow::addBoxAnnotation);
  QObject::connect(scene_.get(), &AnnotationScene::lineFinished,
      this, &MainWindow::addLineAnnotation);
  QObject::connect(scene_.get(), &AnnotationScene::dotFinished,
      this, &MainWindow::addDotAnnotation);
  QObject::connect(scene_.get(), &AnnotationScene::itemActivated,
    this, &MainWindow::setItemActive);
  QObject::connect(scene_.get(), &AnnotationScene::deleteAnn,
    this, &MainWindow::deleteCurrentAnn);
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
  QObject::connect(player, &Player::mediaLoadStart,
      this, &MainWindow::handlePlayerMediaLoadStart);
  QObject::connect(player, &Player::loadProgress,
      this, &MainWindow::handlePlayerLoadProgress);
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
  fs::path current_path(QDir::currentPath().toStdString());
  fs::path default_species = current_path / fs::path("default.species");
  if(fs::exists(default_species)) {
    species_controls_->loadSpeciesFile(
        QString(default_species.string().c_str()));
  }
  initGlobalStateAnnotations();
}

void MainWindow::resizeEvent(QResizeEvent *event) {
  view_->fitInView();
}

void MainWindow::on_play_clicked() {
  if(stopped_ == true) {
    emit requestPlay();
    ui_->play->setIcon(QIcon(":/icons/video_controls/pause.svg"));
    ui_->minusOneSecond->setEnabled(false);
    ui_->minusThreeSecond->setEnabled(false);
    ui_->plusOneFrame->setEnabled(false);
    ui_->minusOneFrame->setEnabled(false);
  }
  else {
    emit requestStop();
    ui_->play->setIcon(QIcon(":/icons/video_controls/play.svg"));
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
  frame -= static_cast<qint64>(std::round(native_rate_));
  emit requestSetFrame(frame);
}

void MainWindow::on_minusThreeSecond_clicked() {
  qint64 frame = last_position_;
  frame -= static_cast<qint64>(std::round(native_rate_ * 3.0));
  emit requestSetFrame(frame);
}

void MainWindow::on_plusOneFrame_clicked() {
  emit requestNextFrame();
}

void MainWindow::on_minusOneFrame_clicked() {
  emit requestPrevFrame();
}

void MainWindow::on_loadVideo_triggered() {
  QString file_str = QFileDialog::getOpenFileName(
      this,
      tr("Open Video"),
      QFileInfo(video_path_).dir().canonicalPath(),
      tr("Video Files (*.avi *.mpg *.mp4 *.mkv)"));
  QFileInfo file(file_str);
  if(file.exists() && file.isFile()) {
    ui_->currentSpeed->setText("Loading movie, please wait...");
    emit requestLoadVideo(file_str);
  }
}

void MainWindow::on_loadAnnotationFile_triggered() {
  QString file_str = QFileDialog::getOpenFileName(
      this,
      tr("Open Annotation File"),
      QFileInfo(video_path_).dir().canonicalPath(),
      tr("Annotation Files (*.json)"));
  QFileInfo file(file_str);
  if(file.exists() && file.isFile()) {
    annotation_->clear();
    initGlobalStateAnnotations();
    annotation_->read(file_str.toStdString());
    species_controls_->loadFromVector(annotation_->getAllSpecies());
    track_id_ = annotation_->earliestTrackID();
    if(track_id_ != 0) {
      updateSpeciesCounts();
      updateStats();
      drawAnnotations();
    }
  }
}

void MainWindow::on_saveAnnotationFile_triggered() {
  std::string filename = metadata_.file_name_;
  std::string reviewer = metadata_.reviewer_name_;
  if(filename.empty() == true && reviewer.empty() == true) {
    filename = video_path_.toStdString();
    fs::path replace(filename);
    replace.replace_extension(".json");
    filename = replace.string();
  }
  else if(filename.empty() == true && reviewer.empty() == false) {
    fs::path vid_path(video_path_.toStdString());
    fs::path out_path =
      vid_path.parent_path() /
      fs::path(vid_path.stem().string() +
      std::string("_") +
      reviewer +
      std::string(".json"));
    filename = out_path.string();
  }
  else {
    fs::path vid_path(video_path_.toStdString());
    fs::path out_path =
      vid_path.parent_path() /
      fs::path(filename +
      std::string("_") +
      reviewer +
      std::string(".json"));
    filename = out_path.string();
  }
  QString fname = QFileDialog::getSaveFileName(
      this,
      "Save annotation file",
      filename.c_str(),
      "*.json");
  if(fname.isNull() == false) {
    auto include_csv = QMessageBox::question(this, "CSV Output",
        "Include csv summary with output?",
        QMessageBox::Yes | QMessageBox::No);
    fs::path vid_path(fname.toStdString());
    annotation_->write(
        vid_path,
        std::to_string(metadata_.trip_id_),
        std::to_string(metadata_.tow_number_),
        metadata_.reviewer_name_,
        metadata_.tow_status_ ? "Open" : "Closed",
        native_rate_,
        include_csv == QMessageBox::Yes);
  }
}

void MainWindow::on_writeImage_triggered() {
  // filename needs to be procedurally generated.
  if (images_save_path_.isEmpty()) {
    images_save_path_ = QFileDialog::getExistingDirectory(
        this, tr("Choose save directory"));
  }
  QImage img(scene_->sceneRect().size().toSize(),
      QImage::Format_ARGB32_Premultiplied);
  QPainter p(&img);
  scene_->render(&p);
  p.end();
  img.save(
      images_save_path_ +
      QStringLiteral("/") +
      QStringLiteral("/Track_%1.png").arg(track_id_));
}

void MainWindow::on_writeImageSequence_triggered() {
  if (images_save_path_.isEmpty()) {
    images_save_path_ = QFileDialog::getExistingDirectory(
        this, tr("Choose save directory"));
  }
  write_image_enabled_ = true;
  setEnabled(false);
  qint64 max_frame = ui_->videoSlider->maximum();
  std::unique_ptr<QProgressDialog> prog(new QProgressDialog(
    "Writing images to disk...",
    "",
    0,
    max_frame,
    this,
    Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint));
  prog->setCancelButton(0);
  prog->setWindowTitle("Writing image sequence");
  prog->setMinimumDuration(10);
  for(qint64 frame = 0; frame <= max_frame; ++frame) {
    QCoreApplication::processEvents();
    prog->setValue(frame);
    emit requestSetFrame(frame);
  }
  setEnabled(true);
  write_image_enabled_ = false;
}

void MainWindow::on_colorizeByTrack_toggled(bool checked) {
  drawAnnotations();
}

void MainWindow::on_setMetadata_triggered() {
  MetadataDialog *dlg = new MetadataDialog(this);
  dlg->setMetadata(metadata_);
  if(dlg->exec()) {
    metadata_ = dlg->getMetadata();
  }
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
  auto trk = annotation_->findTrack(track_id_);
  if(trk != nullptr) {
    trk->species_ = text.toStdString();
    updateStats();
  }
}

void MainWindow::on_subTypeMenu_activated(const QString &text) {
  auto trk = annotation_->findTrack(track_id_);
  if(trk != nullptr) {
    trk->subspecies_ = text.toStdString();
  }
}

void MainWindow::on_countLabelMenu_activated(const QString &text) {
  auto trk = annotation_->findTrack(track_id_);
  if(trk != nullptr) {
    if(text.contains("ignore") == true) {
      trk->count_label_ = kIgnore;
    }
    else if(text.contains("entering") == true) {
      trk->count_label_ = kEntering;
    }
    else if(text.contains("exiting") == true) {
      trk->count_label_ = kExiting;
    }
  }
}

void MainWindow::on_prevTrack_clicked() {
  auto trk = annotation_->prevTrack(track_id_);
  if(trk != nullptr) {
    track_id_ = trk->id_;
    updateStats();
  }
}

void MainWindow::on_nextTrack_clicked() {
  auto trk = annotation_->nextTrack(track_id_);
  if(trk != nullptr) {
    track_id_ = trk->id_;
    updateStats();
  }
}

void MainWindow::on_removeTrack_clicked() {
  auto remove_id = track_id_;
  auto prev = annotation_->prevTrack(track_id_);
  auto next = annotation_->nextTrack(track_id_);
  if(prev != nullptr) {
    if(track_id_ == prev->id_) {
      annotation_->remove(remove_id);
      track_id_ = annotation_->earliestTrackID();
    }
    else {
      track_id_ = prev->id_;
    }
  }
  else if(next != nullptr) {
    if(track_id_ == next->id_) {
      annotation_->remove(remove_id);
      track_id_ = annotation_->earliestTrackID();
    }
    else {
      track_id_ = next->id_;
    }
  }
  else {
    track_id_ = 0;
  }
  annotation_->remove(remove_id);
  updateStats();
  updateSpeciesCounts();
  drawAnnotations();
}

void MainWindow::on_goToFrame_clicked() {
  qint64 frame = annotation_->trackFirstFrame(track_id_);
  emit requestSetFrame(frame);
}

void MainWindow::on_reassignTrack_clicked() {
  auto new_id = annotation_->nextId();
  ReassignDialog *dlg = new ReassignDialog(
      last_position_,
      annotation_->trackFirstFrame(track_id_),
      annotation_->trackLastFrame(track_id_),
      track_id_,
      new_id,
      this);
  if(dlg->exec()) {
    Reassignment reassign = dlg->getReassignment();
    auto from_trk = annotation_->findTrack(reassign.from_id_);
    auto from_det = annotation_->getDetectionAnnotationsById(
        reassign.from_id_);
    auto to_trk = annotation_->findTrack(reassign.to_id_);
    if(to_trk == nullptr) {
      // Detections assigned to new track
      auto new_trk = std::make_shared<TrackAnnotation>(
          reassign.to_id_,
          from_trk->species_,
          from_trk->subspecies_,
          from_trk->frame_added_,
          from_trk->count_label_);
      annotation_->insert(new_trk);
    }
    else {
      // Detections assigned to existing track, check for
      // overwrite of existing detections
      bool need_new = false;
      for(auto& det : from_det) {
        if(det->frame_ >= reassign.from_frame_ &&
            det->frame_ <= reassign.to_frame_) {
          auto exist = annotation_->findDetection(det->frame_, reassign.to_id_);
          if(exist != nullptr) {
            need_new = true;
            break;
          }
        }
      }
      if(need_new == true) {
        // Overwrite is necessary, make new track to store them
        auto new_trk = std::make_shared<TrackAnnotation>(
            new_id,
            from_trk->species_,
            from_trk->subspecies_,
            from_trk->frame_added_,
            from_trk->count_label_);
        annotation_->insert(new_trk);
      }
    }
    for(auto& det : from_det) {
      if(det->frame_ >= reassign.from_frame_ &&
          det->frame_ <= reassign.to_frame_) {
        auto exist = annotation_->findDetection(det->frame_, reassign.to_id_);
        if(exist != nullptr) {
          auto replace = std::make_shared<DetectionAnnotation>(
              exist->frame_,
              new_id,
              exist->area_,
              exist->type_,
              exist->species_,
              exist->prob_);
          annotation_->remove(exist->frame_, exist->id_);
          annotation_->insert(replace);
        }
        auto updated = std::make_shared<DetectionAnnotation>(
            det->frame_,
            reassign.to_id_,
            det->area_,
            det->type_,
            det->species_,
            det->prob_);
        annotation_->remove(det->frame_, det->id_);
        annotation_->insert(updated);
      }
    }
  }
  delete dlg;
  updateStats();
  updateSpeciesCounts();
  drawAnnotations();
}

void MainWindow::on_goToTrackVal_returnPressed() {
  auto trk = annotation_->findTrack(ui_->goToTrackVal->text().toInt());
  if(trk != nullptr) {
    track_id_ = ui_->goToTrackVal->text().toInt();
    updateStats();
  }
  else {
    handlePlayerError("Track with that ID does not exist!");
  }
}

void MainWindow::on_trackNumVal_activated(const QString &text) {
  track_id_ = text.toInt();
  auto trk = annotation_->findTrack(track_id_);
  if(trk != nullptr) {
    updateStats();
  }
}

void MainWindow::on_goToFrameVal_returnPressed() {
  auto frame = ui_->goToFrameVal->text().toInt();
  emit requestSetFrame(frame);
}

void MainWindow::on_addRegion_clicked() {
  if(annotation_->getTotal() < 1) {
    handlePlayerError("Please add a track before adding a region!");
  }
  else {
    view_->setFocus();
    scene_->setMode(kDraw);
  }
}

void MainWindow::on_removeRegion_clicked() {
  annotation_->remove(last_position_, track_id_);
  drawAnnotations();
}

void MainWindow::on_nextAndCopy_clicked() {
  auto det = annotation_->findDetection(last_position_, track_id_);
  if(det != nullptr) {
    annotation_->insert(std::make_shared<DetectionAnnotation>(
          last_position_ + 1,
          track_id_,
          det->area_,
          det->type_,
          det->species_,
          det->prob_));
    on_plusOneFrame_clicked();
  }
  else {
    QMessageBox msgBox;
    msgBox.setText("Could not find region to copy!");
    msgBox.exec();
  }
}

void MainWindow::on_viewId_changed() {
  drawAnnotations();
}

void MainWindow::on_viewSpecies_changed() {
  drawAnnotations();
}

void MainWindow::on_viewProbability_changed() {
  drawAnnotations();
}

void MainWindow::on_viewCount_changed() {
  drawAnnotations();
}

void MainWindow::onGlobalStateChange() {
  annotation_->insertGlobalStateAnnotation(
      last_position_,
      *current_global_state_);
}

void MainWindow::showFrame(QImage image, qint64 frame) {
  last_frame_ = image;
  auto pixmap = QPixmap::fromImage(image);
  pixmap_item_->setPixmap(pixmap);
  last_position_ = frame;
  ui_->currentTime->setText(frameToTime(frame));
  drawAnnotations();
  ui_->videoSlider->setValue(static_cast<int>(frame));
  if(zoom_reset_needed_ == true) {
    view_->fitInView();
    zoom_reset_needed_ = false;
  }
  scene_->setMode(kSelect);
  if(write_image_enabled_ == true) {
    QImage img(scene_->sceneRect().size().toSize(),
        QImage::Format_ARGB32_Premultiplied);
    QPainter p(&img);
    scene_->render(&p);
    p.end();
    img.save(
        images_save_path_ +
        QStringLiteral("/") +
        QStringLiteral("/frame_%1.png").arg(frame, 5, 10, QChar('0')));
  }
}

void MainWindow::addIndividual(std::string species, std::string subspecies) {
  track_id_ = annotation_->nextId();
  annotation_->insert(std::make_shared<TrackAnnotation>(
        track_id_, species, subspecies, last_position_, kIgnore));
  on_addRegion_clicked();
  updateSpeciesCounts();
  updateStats();
  drawAnnotations();
}

void MainWindow::colorChanged(QMap<QString, QColor> color_map) {
  color_map_ = color_map;
  drawAnnotations();
}

void MainWindow::handlePlayerDurationChanged(qint64 duration) {
  annotation_->setVideoLength(duration);
  ui_->videoSlider->setRange(0, duration);
  ui_->videoSlider->setSingleStep(1);
  ui_->videoSlider->setPageStep(duration / 20);
  ui_->totalTime->setText(frameToTime(duration));
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

void MainWindow::handlePlayerMediaLoadStart(int max_progress) {
  if(load_progress_ == nullptr) {
    load_progress_.reset(new QProgressDialog(
          "Extracting video timestamps...",
          "",
          0,
          max_progress,
          this,
          Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint));
    load_progress_->setCancelButton(0);
    load_progress_->setWindowTitle("Loading video");
    load_progress_->setMinimumDuration(10);
  }
}

void MainWindow::handlePlayerLoadProgress(int progress) {
  if(load_progress_ != nullptr) {
    load_progress_->setValue(progress);
  }
}

void MainWindow::handlePlayerMediaLoaded(
  QString video_path,
  qreal native_rate) {
  if(load_progress_ != nullptr) {
    load_progress_->setValue(load_progress_->maximum());
    load_progress_.reset(nullptr);
  }
  video_path_ = video_path;
  native_rate_ = native_rate;
  setEnabled(true);
  ui_->currentSpeed->setText("Current Speed: 100%");
  this->setWindowTitle(video_path_);
  annotation_->clear();
  initGlobalStateAnnotations();
  scene_->clear();
  QPixmap pixmap(width_, height_);
  pixmap_item_ = scene_->addPixmap(pixmap);
  scene_->setSceneRect(0, 0, width_, height_);
  view_->show();
  updateSpeciesCounts();
  updateStats();
  drawAnnotations();
  zoom_reset_needed_ = true;
  emit requestSetFrame(0);
}

void MainWindow::handlePlayerError(QString err) {
  QMessageBox msgBox;
  msgBox.setText(err);
  msgBox.exec();
}

void MainWindow::addBoxAnnotation(const QRectF &rect) {
  annotation_->insert(std::make_shared<DetectionAnnotation>(
    last_position_,
    track_id_,
    Rect(rect.x(), rect.y(), rect.width(), rect.height()),
    kBox,
    getSpecies(track_id_),
    1.0));
  drawAnnotations();
}

void MainWindow::addLineAnnotation(const QLineF &line) {
  annotation_->insert(std::make_shared<DetectionAnnotation>(
    last_position_,
    track_id_,
    Rect(line.x1(), line.y1(), line.x2(), line.y2()),
    kLine,
    getSpecies(track_id_),
    1.0));
  drawAnnotations();
}

void MainWindow::addDotAnnotation(const QPointF &dot) {
  annotation_->insert(std::make_shared<DetectionAnnotation>(
    last_position_,
    track_id_,
    Rect(dot.x(), dot.y(), 0, 0),
    kDot,
    getSpecies(track_id_),
    1.0));
  drawAnnotations();
}

QColor MainWindow::getColor(qint64 id) {
  if(ui_->colorizeByTrack->isChecked()) {
    return track_colors[id % track_colors.size()];
  } else {
    auto species = annotation_->findTrack(id)->getSpecies();
    QString name = species.c_str();
    return color_map_[name.toLower()];
  }
}

std::string MainWindow::getSpecies(qint64 id) {
  return annotation_->findTrack(id)->getSpecies();
}

void MainWindow::updateSpeciesCounts() {
  auto counts = annotation_->getCounts();
  for(const auto &species : species_controls_->getSpecies()) {
    auto it = counts.find(species.getName());
    if(it != counts.end()) {
      species_controls_->setCount(it->second, it->first);
    }
    else {
      species_controls_->setCount(0, species.getName());
    }
  }
}

void MainWindow::setItemActive(
  const QGraphicsItem &item) {
  for(auto ann : current_annotations_) {
    if(ann.second == &item) {
      track_id_ = ann.first;
      updateStats();
    }
  }
}

void MainWindow::deleteCurrentAnn() {
  on_removeRegion_clicked();
  auto current_track_anns = annotation_->getDetectionAnnotationsById(track_id_);
  if (current_track_anns.empty()) {
    on_removeTrack_clicked();
  }
}

void MainWindow::updateStats() {
  ui_->typeMenu->clear();
  ui_->subTypeMenu->clear();
  ui_->trackNumVal->clear();
  auto trk = annotation_->findTrack(track_id_);
  if(trk == nullptr) {
    ui_->trackNumVal->setCurrentText("-");
    ui_->totalTrackVal->setText("-");
    ui_->frameCountedVal->setText("-");
    return;
  }
  else {
    for (auto trkid : annotation_->getTrackIDs()) {
      ui_->trackNumVal->addItem(QString::number(trkid));
    }
    ui_->trackNumVal->setCurrentText(QString::number(track_id_));
    ui_->totalTrackVal->setText(QString::number(annotation_->getTotal()));
    ui_->frameCountedVal->setText(QString::number(trk->frame_added_));
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
    if(trk->count_label_ == kIgnore) {
      ui_->countLabelMenu->setCurrentText("ignore");
    }
    else if(trk->count_label_ == kEntering) {
      ui_->countLabelMenu->setCurrentText("entering");
    }
    else if(trk->count_label_ == kExiting) {
      ui_->countLabelMenu->setCurrentText("exiting");
    }
  }
}

void MainWindow::drawAnnotations() {
  for(auto ann : current_annotations_) {
    scene_->removeItem(ann.second);
  }
  current_annotations_.clear();
  for(auto ann : annotation_->getDetectionAnnotationsByFrame(last_position_)) {
    AnnotatedRegion<DetectionAnnotation> *box = nullptr;
    AnnotatedLine<DetectionAnnotation> *line = nullptr;
    AnnotatedDot<DetectionAnnotation> *dot = nullptr;
    QColor color = getColor(ann->id_);
    int64_t id = -1;
    if(ui_->viewId->isChecked()) {
      id = ann->id_;
    }
    QString species = "";
    if(ui_->viewSpecies->isChecked()) {
      species = ann->species_.c_str();
    }
    double prob = -1.0;
    if(ui_->viewProbability->isChecked()) {
      prob = ann->prob_;
    }
    switch(ann->type_) {
      case kBox:
        box = new AnnotatedRegion<DetectionAnnotation>(
            id,
            ann,
            pixmap_item_->pixmap().toImage().rect(),
            color,
            species,
            prob);
        if (box->isValid() == true) {
          scene_->addItem(box);
          current_annotations_.emplace_back(ann->id_, box);
        }
        break;
      case kLine:
        line = new AnnotatedLine<DetectionAnnotation>(
            ann->id_, ann, pixmap_item_->pixmap().toImage().rect(), color);
        if(line->isValid() == true) {
          scene_->addItem(line);
          current_annotations_.emplace_back(ann->id_, line);
        }
        break;
      case kDot:
        dot = new AnnotatedDot<DetectionAnnotation>(
            ann->id_, ann, pixmap_item_->pixmap().toImage().rect(), color);
        if(dot->isValid() == true) {
          scene_->addItem(dot);
          current_annotations_.emplace_back(ann->id_, dot);
        }
        break;
    }
  }
  *current_global_state_ = annotation_->getGlobalStateAnnotation(last_position_);
  global_state_widget_->setStates(current_global_state_);
  view_->setBoundingRect(scene_->sceneRect());
  if(count_text_ != nullptr) {
    scene_->removeItem(count_text_);
    count_text_ = nullptr;
  }
  if(ui_->viewCount->isChecked()) {
    auto counts = annotation_->getCounts(0,
      static_cast<uint64_t>(last_position_));
    if(counts.size() > 0) {
      QString count_str;
      for(const auto& cnt : counts) {
        QString species_str = QString(
          "%1: %2\n").arg(cnt.first.c_str()).arg(cnt.second);
        count_str.append(species_str);
      }
      QFont font;
      font.setPixelSize(100);
      font.setBold(true);
      count_text_ = scene_->addText(count_str, font);
      count_text_->setDefaultTextColor(QColor(255, 0, 0));
    }
  }
}

QString MainWindow::frameToTime(qint64 frame_number) {
  qint64 seconds = frame_number / native_rate_;
  qint64 mm = seconds / 60;
  qint64 ss = seconds % 60;
  return QString("%1:%2 | %3")
      .arg(mm, 2, 10, QChar('0'))
      .arg(ss, 2, 10, QChar('0'))
      .arg(QString::number(frame_number));
}

void MainWindow::initGlobalStateAnnotations() {
  fs::path current_path(QDir::currentPath().toStdString());
  fs::path default_global_state = current_path / fs::path("default.global");
  if(fs::exists(default_global_state)) {
    deserialize(*current_global_state_, default_global_state.string());
    annotation_->insertGlobalStateAnnotation(0, *current_global_state_);
  }
  global_state_widget_->setStates(current_global_state_);
}

void MainWindow::setEnabled(bool enable) {
  ui_->videoSlider->setEnabled(enable);
  ui_->play->setEnabled(enable);
  ui_->faster->setEnabled(enable);
  ui_->slower->setEnabled(enable);
  ui_->minusOneSecond->setEnabled(enable);
  ui_->minusThreeSecond->setEnabled(enable);
  ui_->minusOneFrame->setEnabled(enable);
  ui_->plusOneFrame->setEnabled(enable);
  ui_->loadVideo->setEnabled(enable);
  ui_->loadAnnotationFile->setEnabled(enable);
  ui_->saveAnnotationFile->setEnabled(enable);
  ui_->writeImage->setEnabled(enable);
  ui_->writeImageSequence->setEnabled(enable);
  ui_->setMetadata->setEnabled(enable);
  ui_->typeLabel->setEnabled(enable);
  ui_->typeMenu->setEnabled(enable);
  ui_->countLabelLabel->setEnabled(enable);
  ui_->subTypeLabel->setEnabled(enable);
  ui_->subTypeMenu->setEnabled(enable);
  ui_->countLabelMenu->setEnabled(enable);
  ui_->prevTrack->setEnabled(true);
  ui_->nextTrack->setEnabled(true);
  ui_->removeTrack->setEnabled(true);
  ui_->reassignTrack->setEnabled(true);
  ui_->goToFrame->setEnabled(enable);
  ui_->goToTrackVal->setEnabled(true);
  ui_->goToTrackVal->setEnabled(enable);
  ui_->goToFrameLabel->setEnabled(enable);
  ui_->goToFrameVal->setEnabled(enable);
  ui_->addRegion->setEnabled(enable);
  ui_->removeRegion->setEnabled(enable);
  ui_->nextAndCopy->setEnabled(enable);
}

#include "moc_mainwindow.cpp"

}} // namespace tator::video_annotator
