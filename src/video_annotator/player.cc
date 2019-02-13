
#include <QTime>
#include <QCoreApplication>
#include <QEventLoop>
#include <QMutexLocker>

#include "player.h"

namespace tator { namespace video_annotator {

namespace {
  static const int kMaxBufferSize = 50;
  static const int kTrimBound = kMaxBufferSize / 2;
}

Player::Player()
  : QObject()
  , video_path_()
  , frame_rate_(0.0)
  , stopped_(true)
  , image_()
  , current_speed_(0.0)
  , codec_context_(nullptr)
  , format_context_(nullptr)
  , packet_()
  , stream_index_(-1)
  , frame_(nullptr)
  , frame_rgb_(nullptr)
  , sws_context_(nullptr)
  , delay_(0.0)
  , dec_frame_(0)
  , req_frame_(0) 
  , seek_map_()
  , frame_buffer_()
  , frame_mutex_()
  , buffering_(false)
  , condition_() {
  av_register_all();
  av_init_packet(&packet_);
}

Player::~Player() {
  reinit();
  condition_.wakeOne();
}

void Player::loadVideo(QString filename) {
  reinit();
  video_path_ = filename;
  frame_buffer_.clear();
  format_context_ = avformat_alloc_context();
  frame_ = av_frame_alloc();
  frame_rgb_ = av_frame_alloc();
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
  frame_rate_ = 
    static_cast<double>(stream->avg_frame_rate.num) / 
    static_cast<double>(stream->avg_frame_rate.den);
  uint64_t num_frames_est = format_context_->duration;
  num_frames_est /= AV_TIME_BASE;
  num_frames_est *= frame_rate_;
  emit mediaLoadStart(num_frames_est / 1000);
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
  avcodec_flush_buffers(codec_context_);
  while(true) {
    av_packet_unref(&packet_);
    status = av_read_frame(format_context_, &packet_);
    if(status < 0) {
      break;
    }
    if(packet_.stream_index == stream_index_) {
      if(frame_index % 1000 == 0) {
        emit loadProgress(frame_index / 1000);
      }
      int got_picture = 1;
      if(frame_index < 10) {
        status = avcodec_decode_video2(
          codec_context_, 
          frame_,
          &got_picture,
          &packet_);
      }
      if(got_picture) {
        seek_map_.left.insert({frame_index, packet_.dts});
        ++frame_index;
      }
    }
  }
  av_seek_frame(
      format_context_, 
      stream_index_, 
      seek_map_.right.begin()->first,
      AVSEEK_FLAG_BACKWARD);
  current_speed_ = frame_rate_;
  delay_ = 1000000.0 / frame_rate_;
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

  emit resolutionChanged(codec_context_->width, codec_context_->height);
  emit mediaLoaded(filename, frame_rate_);
  emit playbackRateChanged(current_speed_);
  emit durationChanged(seek_map_.size());
  emit fileLoadDone();
}

void Player::play() {
  if(stopped_ == true) {
    stopped_ = false;
  }
  emit stateChanged(stopped_);
  while(stopped_ == false) {
    QTime t;
    t.start();
    setCurrentFrame(req_frame_ + 1);
    emit processedImage(image_, req_frame_);
    double usec = 1000.0 * t.restart();
    processWait(std::round(delay_ - usec));
  }
}

void Player::stop() {
  stopped_ = true;
  emit stateChanged(stopped_);
}

void Player::getOneFrame() {
  QMutexLocker locker(&frame_mutex_);
  bool valid = false;
  int count_errs = 0;
  const int max_errs = 1 << 9;
  int got_picture;
  while(!valid) {
    av_packet_unref(&packet_);
    int status = av_read_frame(format_context_, &packet_);
    if(status == AVERROR(EAGAIN)) {
      continue;
    }
    if(status < 0) {
      stop();
      break;
    }
    if(packet_.stream_index != stream_index_) {
      av_packet_unref(&packet_);
    }
    else {
      auto it = seek_map_.right.find(packet_.dts);
      if(!(it == seek_map_.right.end())) {
        dec_frame_ = it->second;
      }
      status = avcodec_decode_video2(
        codec_context_, 
        frame_,
        &got_picture,
        &packet_);
      if(got_picture) {
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
        frame_buffer_.insert({dec_frame_, image_});
        if(frame_buffer_.size() > kMaxBufferSize) {
          for(auto buf_it = frame_buffer_.cbegin(); 
              buf_it != frame_buffer_.cend();) {
            if(qAbs(buf_it->first - req_frame_) > kTrimBound) {
              buf_it = frame_buffer_.erase(buf_it);
            }
            else {
              ++buf_it;
            }
          }
        }
        valid = true;
      }
      else {
        count_errs++;
        if(count_errs > max_errs) {
          break;
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
  setCurrentFrame(req_frame_ + 1);
  emit processedImage(image_, req_frame_);
}

void Player::prevFrame() {
  setCurrentFrame(req_frame_ - 1);
  emit processedImage(image_, req_frame_);
  if(buffering_ == false) {
    buffering_ = true;
    auto buf_it = frame_buffer_.begin();
    if(req_frame_ - buf_it->first < kTrimBound / 2) {
      buffer(req_frame_, 1000);
    }
    buffering_ = false;
  }
}

void Player::setCurrentFrame(qint64 frame_num) {
  qint64 bounded = frame_num < 0 ? 0 : frame_num;
  const qint64 max_frame = seek_map_.left.rbegin()->first;
  bounded = bounded > max_frame ? max_frame : bounded;
  req_frame_ = bounded;
  if(frame_num - dec_frame_ == 1) {
    getOneFrame();
  }
  else {
    buffering_ = true;
    auto buf_it = frame_buffer_.find(frame_num);
    if(buf_it != frame_buffer_.end()) {
      image_ = buf_it->second;
    }
    else {
      buffer(bounded, 0);
    }
    buffering_ = false;
  }
}

void Player::buffer(qint64 frame_num, qint64 wait) {
  qint64 seek_to = frame_num - kTrimBound;
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
      if(dec_frame_ >= frame_num) {
        break;
      }
      if(wait > 0) {
        processWait(wait);
      }
    }
  }
}

void Player::setFrame(qint64 frame) {
  setCurrentFrame(frame);
  emit processedImage(image_, req_frame_);
}

void Player::processWait(qint64 usec) {
  QTime die_time = QTime::currentTime().addMSecs(usec / 1000);
  QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
  while(QTime::currentTime() < die_time) {
    QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
  }
}

void Player::reinit() {
  QMutexLocker locker(&frame_mutex_);
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
}

#include "moc_player.cpp"

}} // namespace tator::gui

