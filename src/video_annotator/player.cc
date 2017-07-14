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
  , image_()
  , current_speed_(0.0)
  , codec_context_(nullptr)
  , format_context_(avformat_alloc_context())
  , packet_()
  , stream_index_(-1)
  , frame_(av_frame_alloc())
  , frame_rgb_(av_frame_alloc())
  , sws_context_(nullptr)
  , delay_(0.0)
  , frame_index_(0) 
  , seek_map_()
  , mutex_()
  , condition_() {
  av_register_all();
  av_init_packet(&packet_);
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
  if(frame_rgb_ != nullptr) {
    if(frame_rgb_->data[0] != nullptr) {
      av_freep(&frame_rgb_->data[0]);
    }
    av_frame_free(&frame_rgb_);
    frame_rgb_ = nullptr;
  }
  if(frame_ != nullptr) {
    av_frame_free(&frame_);
    frame_ = nullptr;
  }
  stopped_ = true;
  condition_.wakeOne();
}

void Player::loadVideo(QString filename) {
  video_path_ = filename;
  if(format_context_ != nullptr) {
    avformat_close_input(&format_context_);
    format_context_ = nullptr;
  }
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
  AVCodec *codec = avcodec_find_decoder(stream->codecpar->codec_id);
  if(codec == nullptr) {
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
  codec_context_ = avcodec_alloc_context3(codec);
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
  status = avcodec_open2(codec_context_, codec, nullptr);
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
  seek_map_.clear();
  qint64 frame_index = 0;
  while(true) {
    status = av_read_frame(format_context_, &packet_);
    if(status < 0) {
      break;
    }
    if(packet_.stream_index == stream_index_) {
      seek_map_.left.insert({frame_index, packet_.dts});
      ++frame_index;
    }
  }
  av_seek_frame(
      format_context_, 
      stream_index_, 
      seek_map_.right.begin()->first,
      AVSEEK_FLAG_BACKWARD);
  frame_rate_ = 
    static_cast<double>(stream->avg_frame_rate.num) / 
    static_cast<double>(stream->avg_frame_rate.den);
  current_speed_ = frame_rate_;
  image_ = QImage(
      codec_context_->width,
      codec_context_->height,
      QImage::Format_RGB32);
  if(frame_rgb_ != nullptr) {
    if(frame_rgb_->data[0] != nullptr) {
      av_freep(&frame_rgb_->data[0]);
    }
  }
  av_image_alloc(
      frame_rgb_->data,
      frame_rgb_->linesize,
      codec_context_->width,
      codec_context_->height,
      AV_PIX_FMT_RGB24,
      1);
  emit mediaLoaded(filename, frame_rate_);
  emit playbackRateChanged(current_speed_);
  emit durationChanged(seek_map_.size());
  emit resolutionChanged(codec_context_->width, codec_context_->height);
}

void Player::play() {
  if(stopped_ == true) {
    stopped_ = false;
  }
  emit stateChanged(stopped_);
  while(stopped_ == false) {
    QTime t;
    t.start();
    getOneFrame();
    emit processedImage(image_, frame_index_);
    double usec = 1000.0 * t.restart();
    processWait(std::round(delay_ - usec));
  }
}

void Player::stop() {
  stopped_ = true;
  emit stateChanged(stopped_);
}

void Player::getOneFrame() {
  QMutexLocker locker(&mutex_);
  while(true) {
    int status = av_read_frame(format_context_, &packet_);
    if(status < 0) {
      break;
    }
    if(packet_.stream_index == stream_index_) {
      auto it = seek_map_.right.find(packet_.dts);
      if(it == seek_map_.right.end()) {
        emit error("Could not find decompression timestamp!");
        return;
      }
      else {
        frame_index_ = it->second;
      }
      status = avcodec_send_packet(codec_context_, &packet_);
      if(status < 0) {
        emit error("Error while sending packet to the decoder!");
        return;
      }
      while(status >= 0) {
        status = avcodec_receive_frame(codec_context_, frame_);
        if(status == AVERROR_EOF) {
          stopped_ = true;
          break;
        }
        else if(status == AVERROR(EAGAIN)) {
          break;
        }
        else if(status < 0) {
          emit error("Error while receiving a frame from the decoder!");
          return;
        }
        else {
          sws_scale(
              sws_context_, 
              frame_->data, 
              frame_->linesize, 
              0, 
              codec_context_->height, 
              frame_rgb_->data, 
              frame_rgb_->linesize);
          uint8_t *src = (uint8_t*)(frame_rgb_->data[0]);
          for(int y = 0; y < codec_context_->height; ++y) {
            QRgb *scan_line = (QRgb*)image_.scanLine(y);
            for(int x = 0; x < codec_context_->width; ++x) {
              scan_line[x] = qRgb(src[3*x], src[3*x+1], src[3*x+2]);
            }
            src += frame_rgb_->linesize[0];
          }
          return;
        }
      }
    }
  }
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
  getOneFrame();
  emit processedImage(image_, frame_index_);
}

void Player::prevFrame() {
  setCurrentFrame(frame_index_ - 1);
  emit processedImage(image_, frame_index_);
}

void Player::setCurrentFrame(qint64 frame_num) {
  qint64 bounded = frame_num < 0 ? 0 : frame_num;
  const qint64 max_frame = seek_map_.left.rbegin()->first;
  bounded = bounded > max_frame ? max_frame : bounded;
  qint64 seek_to = bounded - 3;
  seek_to = seek_to < 0 ? 0 : seek_to;
  auto it = seek_map_.left.find(seek_to);
  if(it != seek_map_.left.end()) {
    int status = av_seek_frame(
        format_context_, 
        stream_index_, 
        it->second, 
        AVSEEK_FLAG_BACKWARD);
    if(status < 0) {
      emit error("Error seeking to frame!");
      return;
    }
    avcodec_flush_buffers(codec_context_);
    while(true) {
      getOneFrame();
      if(frame_index_ >= bounded) {
        break;
      }
    }
  }
}

void Player::setFrame(qint64 frame) {
  setCurrentFrame(frame);
  emit processedImage(image_, frame_index_);
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

