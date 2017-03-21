#include "fish_annotator/video_annotator/player.h"

#include <QTime>
#include <QCoreApplication>
#include <QEventLoop>

#include <fstream>
std::ofstream fout("BLAHPLAYER.txt");

namespace fish_annotator { namespace video_annotator {

Player::Player(QObject *parent)
  : video_path_()
  , frame_rate_(0.0)
  , stopped_(true)
  , frame_mat_()
  , rgb_frame_mat_()
  , current_speed_(0.0)
  , capture_(nullptr)
  , delay_(0.0)
  , frame_index_(0) {
}

void Player::loadVideo(const std::string &filename) {
  video_path_ = filename;
  capture_.reset(new cv::VideoCapture(filename));
  if (capture_->isOpened()) {
    frame_rate_ = capture_->get(CV_CAP_PROP_FPS);
    current_speed_ = frame_rate_;
    delay_ = (1000000.0 / frame_rate_);
    fout << "DELAY IS " << delay_ << std::endl;
    emit mediaLoaded();
  }
  else {
    std::string msg(
        std::string("Failed to load media at ") + 
        filename +
        std::string("!"));
    emit error(msg);
  }
}

const std::string &Player::getVideoPath() {
  return video_path_;
}

double Player::getFrameRate() {
  return frame_rate_;
}

void Player::play() {
  if(stopped_ == true) {
    stopped_ = false;
    fout << "RUNNING FOR REAL" << std::endl;
    runForReal();
  }
}

void Player::runForReal() {
  while(!stopped_) {
    auto time = QTime::currentTime();
    getOneFrame();
    double usec = 1000.0 * (QTime::currentTime().msec() - time.msec());
    usleep(std::round(delay_ - usec));
  }
}

void Player::getOneFrame() {
  if (!capture_->read(frame_mat_)) {
    stopped_ = true;
  }
  frame_index_ = capture_->get(CV_CAP_PROP_POS_FRAMES);
  fout << "GETTING FRAME AT " << frame_index_ << std::endl;
  if (frame_mat_.channels() == 3) {
    cv::cvtColor(frame_mat_, rgb_frame_mat_, CV_BGR2RGB);
    emit processedImage(
        std::make_shared<QImage>(
          (const unsigned char*)(rgb_frame_mat_.data),
          frame_mat_.cols,
          frame_mat_.rows,
          QImage::Format_RGB888),
        frame_index_);
  }
  else {
    emit processedImage(
        std::make_shared<QImage>(
          (const unsigned char*)(frame_mat_.data),
          frame_mat_.cols,
          frame_mat_.rows,
          QImage::Format_Indexed8),
        frame_index_);
  }
}

Player::~Player() {
  stopped_ = true;
}

void Player::stop() {
  stopped_ = true;
}

void Player::usleep(int usec) {
  QTime die_time = QTime::currentTime().addMSecs(usec / 1000);
  while(QTime::currentTime() < die_time) {
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
  }
}

bool Player::isStopped() const{
  return stopped_;
}

void Player::speedUp() {
  delay_ /= 2.0;
  if(delay_ < 1.0) delay_ = 1.0;
  current_speed_ *= 2.0;
}

void Player::slowDown() {
  delay_ *= 2.0;
  current_speed_ /= 2.0;
}

void Player::nextFrame() {
  setCurrentFrame(++frame_index_);
  getOneFrame();
}

void Player::prevFrame() {
  setCurrentFrame(--frame_index_);
  getOneFrame();
}

int64_t Player::getCurrentFrame() {
  return frame_index_;
}

double Player::getNumberOfFrames() {
  return capture_->get(CV_CAP_PROP_FRAME_COUNT);
}

void Player::setCurrentFrame(int64_t frame_num) {
  capture_->set(CV_CAP_PROP_POS_MSEC, 
      1000.0 * static_cast<double>(frame_num) / frame_rate_);
  if (frame_num > 0) {
		frame_index_ = capture_->get(CV_CAP_PROP_POS_FRAMES);
  }
	else {
		frame_index_ = 0;
  }
}

void Player::setFrame(std::int64_t frame) {
  setCurrentFrame(frame);
  return getOneFrame();
}

double Player::getCurrentSpeed() {
  return current_speed_;
}

void Player::writeImage(const QString &filename) {
	cv::Mat mat = frame_mat_;
	cv::imwrite(filename.toStdString(), mat);
}

#include "../../include/fish_annotator/video_annotator/moc_player.cpp"

}} // namespace fish_annotator::gui

