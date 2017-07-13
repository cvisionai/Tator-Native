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
  , codec_(nullptr)
  , codec_context_(nullptr)
  , format_context_(nullptr)
  , frame_(nullptr)
  , in_buf_(nullptr)
  , capture_(nullptr)
  , delay_(0.0)
  , frame_index_(0) 
  , mutex_()
  , condition_() {
  av_register_all();
  format_context_ = avformat_alloc_context();
}

Player::~Player() {
  QMutexLocker locker(&mutex_);
  if(codec_context_ != nullptr) {
    avcodec_close(codec_context_);
    codec_context_ = nullptr;
  }
  if(format_context_ != nullptr) {
    avformat_close_input(&format_context_);
    format_context_ = nullptr;
  }
  stopped_ = true;
  condition_.wakeOne();
}

void Player::loadVideo(QString filename) {
  video_path_ = filename;
  int status = avformat_open_input(
    &format_context_, 
    filename.toStdString().c_str(),
    nullptr,
    nullptr);
  if(status != 0) {
    std::string msg(
        std::string("Failed to load media at ") +
        filename.toStdString() +
        std::string("!"));
    emit error(QString(msg.c_str()));
    return;
  }
  status = avformat_find_stream_info(format_context_, nullptr);
  if(status < 0) {
    std::string msg(
        std::string("Couldn't find stream information for video ") +
        filename.toStdString() +
        std::string("!"));
    emit error(QString(msg.c_str()));
    return;
  }
  int video_stream_index = -1;
  status = av_find_best_stream(
      format_context_, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
  if(status < 0) {
    std::string msg(
        std::string("File does not contain a video stream ") +
        filename.toStdString() +
        std::string("!"));
    emit error(QString(msg.c_str()));
    return;
  }
  video_stream_index = status;
  AVStream *stream = format_context_->streams[video_stream_index];
  codec_ = avcodec_find_decoder(stream->codecpar->codec_id);
  if(codec_ == nullptr) {
    std::string msg(
        std::string("Unsupported codec in file ") +
        filename.toStdString() +
        std::string("!"));
    emit error(QString(msg.c_str()));
    return;
  }
  codec_context_ = avcodec_alloc_context3(codec_);
  if(codec_context_ == nullptr) {
    std::string msg(
        std::string("Failed to allocate codec context for file ") +
        filename.toStdString() +
        std::string("!"));
    emit error(QString(msg.c_str()));
    return;
  }
  status = avcodec_parameters_to_context(codec_context_, stream->codecpar);
  if(status < 0) {
    std::string msg(
        std::string("Failed to copy codec parameters to codec context for ") +
        filename.toStdString() +
        std::string("!"));
    emit error(QString(msg.c_str()));
    return;
  }
  status = avcodec_open2(codec_context_, codec_, nullptr);
  if(status < 0) {
    std::string msg(
        std::string("Could not open codec for file ") +
        filename.toStdString() +
        std::string("!"));
    emit error(QString(msg.c_str()));
    return;
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
    auto tmp_frame = getOneFrame();
    auto tmp_frame_index = frame_index_;
    emit processedImage(tmp_frame, tmp_frame_index);
    double usec = 1000.0 * t.restart();
    processWait(std::round(delay_ - usec));
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
  frame_index_ = capture_->get(CV_CAP_PROP_POS_FRAMES)-1;
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
  //setCurrentFrame(frame_index_ + 1);
  auto tmp_frame = getOneFrame();
  auto tmp_frame_index = frame_index_;
  emit processedImage(tmp_frame, tmp_frame_index);
}

void Player::prevFrame() {
  setCurrentFrame(frame_index_ - 1);
  auto tmp_frame = getOneFrame();
  auto tmp_frame_index = frame_index_;
  emit processedImage(tmp_frame, tmp_frame_index);
}

void Player::setCurrentFrame(qint64 frame_num) {
  QMutexLocker locker(&mutex_);
  capture_->set(CV_CAP_PROP_POS_MSEC, 
      1000.0 * static_cast<double>(frame_num) / frame_rate_);
  if (frame_num > 0) {
		frame_index_ = capture_->get(CV_CAP_PROP_POS_FRAMES)-1;
  }
	else {
		frame_index_ = 0;
  }
}

void Player::setFrame(qint64 frame) {
  setCurrentFrame(frame);
  auto tmp_frame = getOneFrame();
  auto tmp_frame_index = frame_index_;
  emit processedImage(tmp_frame, tmp_frame_index);
}

void Player::processWait(qint64 usec) {
  QTime die_time = QTime::currentTime().addMSecs(usec / 1000);
  QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
  while(QTime::currentTime() < die_time) {
    QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
  }
}

#include "../../include/fish_annotator/video_annotator/moc_player.cpp"

}} // namespace fish_annotator::gui

