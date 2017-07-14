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
  , packet_()
  , stream_index_(-1)
  , frame_(av_frame_alloc())
  , buffer_(nullptr)
  , capture_(nullptr)
  , delay_(0.0)
  , frame_index_(0) 
  , mutex_()
  , condition_() {
  av_register_all();
  format_context_ = avformat_alloc_context();
  av_init_packet(&packet_);
  frame_ = av_frame_alloc();
  frame_rgb_ = av_frame_alloc();
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
  stream_index_ = -1;
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
  stream_index_ = status;
  AVStream *stream = format_context_->streams[stream_index_];
  codec_ = avcodec_find_decoder(stream->codecpar->codec_id);
  if(codec_ == nullptr) {
    std::string msg(
        std::string("Unsupported codec in file ") +
        filename.toStdString() +
        std::string("!"));
    emit error(QString(msg.c_str()));
    return;
  }
  if(codec_context_ != nullptr) {
    avcodec_free_context(&codec_context_);
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
  sws_context_ = sws_getContext(
    codec_context_->width, 
    codec_context_->height,
    codec_context_->pix_fmt,
    codec_context_->width, 
    codec_context_->height,
    AV_PIX_FMT_RGB24,
    SWS_BICUBIC,
    nullptr, nullptr, nullptr);
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
    processWait(std::round(delay_ - usec));
  }
}

void Player::stop() {
  stopped_ = true;
  emit stateChanged(stopped_);
}

QImage Player::getOneFrame() {
  QMutexLocker locker(&mutex_);
  while(true) {
    int status = av_read_frame(format_context_, &packet_);
    if(status < 0) {
      break;
    }
    if(packet_.stream_index == stream_index_) {
      status = avcodec_send_packet(codec_context_, &packet_);
      if(status < 0) {
        emit error("Error while sending packet to the decoder!");
        return image_;
      }
      while(status >= 0) {
        status = avcodec_receive_frame(codec_context_, frame_);
        if(status == AVERROR(EAGAIN) || status == AVERROR_EOF) {
          stopped_ = true;
          break;
        }
        else if(status < 0) {
          emit error("Error while receiving a frame from the decoder!");
          return image_;
        }
        if(status >= 0) {
          frame_->pts = av_frame_get_best_effort_timestamp(frame_);
          int num_bytes = avpicture_get_size(AV_PIX_FMT_RGB24, 
              codec_context_->width, codec_context_->height);
          buffer_ = (uint8_t*)malloc(num_bytes);
          avpicture_fill((AVPicture*)frame_rgb_, buffer_, AV_PIX_FMT_RGB24,
              codec_context_->width, codec_context_->height);
          uint8_t *src = (uint8_t*)(frame_rgb_->data[0]);
          image_ = QImage(
              codec_context_->width,
              codec_context_->height,
              QImage::Format_RGB32);
          for(int y = 0; y < codec_context_->height; ++y) {
            QRgb *scan_line = (QRgb*)image_.scanLine(y);
            for(int x = 0; x < codec_context_->width; ++x) {
              scan_line[x] = qRgb(src[3*x], src[3*x+1], src[3*x+2]);
            }
            src += frame_rgb_->linesize[0];
          }
        }
      }
    }
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

