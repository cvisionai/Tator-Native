#include "fish_annotator/video_annotator/player.h"

#include <QTime>
#include <QCoreApplication>
#include <QEventLoop>
#include <QMutexLocker>

namespace fish_annotator { namespace video_annotator {

Player::Player()
  : QObject()
  , video_path_()
  , frame_rate_(0.0)
  , stopped_(true)
  , frame_mat_()
  , rgb_frame_mat_()
  , image_()
  , current_speed_(0.0)
  , capture_(nullptr)
  , delay_(0.0)
  , frame_index_(0) 
  , mutex_()
  , condition_() {
}

Player::~Player() {
  QMutexLocker locker(&mutex_);
  stopped_ = true;
  condition_.wakeOne();
}

void Player::loadVideo(QString filename) {
  video_path_ = filename;
  capture_.reset(new cv::VideoCapture(filename.toStdString()));
  if (capture_->isOpened()) {
    frame_rate_ = capture_->get(CV_CAP_PROP_FPS);
    current_speed_ = frame_rate_;
    delay_ = (1000000.0 / frame_rate_);
    emit mediaLoaded(filename, frame_rate_);
    emit playbackRateChanged(current_speed_);
    emit durationChanged(capture_->get(CV_CAP_PROP_FRAME_COUNT));
    emit resolutionChanged(
        capture_->get(static_cast<qint64>(CV_CAP_PROP_FRAME_WIDTH)),
        capture_->get(static_cast<qint64>(CV_CAP_PROP_FRAME_HEIGHT)));
  }
  else {
    std::string msg(
        std::string("Failed to load media at ") + 
        filename.toStdString() +
        std::string("!"));
    emit error(QString(msg.c_str()));
  }
}

void Player::play() {
  if(stopped_ == true) {
    stopped_ = false;
  }
  emit stateChanged(stopped_);
  while(stopped_ == false) {
    QTime t;
    t.start();
    emit processedImage(getOneFrame(), frame_index_);
    double usec = 1000.0 * t.restart();
    if (usec < delay_) {
      processWait(std::round(delay_ - usec));
    }
  }
}

void Player::stop() {
  stopped_ = true;
  emit stateChanged(stopped_);
}

QImage Player::getOneFrame() {
  QMutexLocker locker(&mutex_);
  if (capture_->read(frame_mat_) == false) {
    stopped_ = true;
  }
  frame_index_ = capture_->get(CV_CAP_PROP_POS_FRAMES);
  if (frame_mat_.channels() == 3) {
    cv::cvtColor(frame_mat_, rgb_frame_mat_, CV_BGR2RGB);
    image_ = QImage(
      (const unsigned char*)(rgb_frame_mat_.data),
      frame_mat_.cols,
      frame_mat_.rows,
      QImage::Format_RGB888);
  }
  else {
    image_ = QImage(
      (const unsigned char*)(frame_mat_.data),
      frame_mat_.cols,
      frame_mat_.rows,
      QImage::Format_Indexed8);
  }
  return image_;
}

void Player::speedUp() {
  delay_ /= 2.0;
  if(delay_ < 1.0) delay_ = 1.0;
  current_speed_ *= 2.0;
  emit playbackRateChanged(current_speed_);
}

void Player::slowDown() {
  delay_ *= 2.0;
  current_speed_ /= 2.0;
  emit playbackRateChanged(current_speed_);
}

void Player::nextFrame() {
  setCurrentFrame(frame_index_ + 1);
  emit processedImage(getOneFrame(), frame_index_);
}

void Player::prevFrame() {
  setCurrentFrame(frame_index_ - 1);
  emit processedImage(getOneFrame(), frame_index_);
}

void Player::setCurrentFrame(qint64 frame_num) {
  QMutexLocker locker(&mutex_);
  capture_->set(CV_CAP_PROP_POS_MSEC, 
      1000.0 * static_cast<double>(frame_num - 1) / frame_rate_);
  if (frame_num > 0) {
		frame_index_ = capture_->get(CV_CAP_PROP_POS_FRAMES);
  }
	else {
		frame_index_ = 0;
  }
}

void Player::setFrame(qint64 frame) {
  setCurrentFrame(frame);
  emit processedImage(getOneFrame(), frame_index_);
}

void Player::processWait(qint64 usec) {
  QTime die_time = QTime::currentTime().addMSecs(usec / 1000);
  while(QTime::currentTime() < die_time) {
    QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
  }
}

#include "../../include/fish_annotator/video_annotator/moc_player.cpp"

}} // namespace fish_annotator::gui

