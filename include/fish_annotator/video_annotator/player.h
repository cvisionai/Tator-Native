#ifndef VIDEO_ANNOTATOR_PLAYER_H
#define VIDEO_ANNOTATOR_PLAYER_H

#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <memory>

#include <QImage>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/frame.h>
}

namespace fish_annotator { namespace video_annotator {

class Player : public QObject {	
    Q_OBJECT
public:
    /// @brief Constructor.
    explicit Player();

    /// @brief Destructor.
    ~Player();

    /// @brief Processes a single frame.
    inline QImage getOneFrame();
public slots:
    /// @brief Plays the video.
    void play();
    
    /// @brief Stops the video.
    void stop();

    /// @brief Loads the video.
    ///
    /// @param filename Path to video.
    void loadVideo(QString filename);

    /// @brief Increases the speed of the video by a factor of two.
    void speedUp();

    /// @brief Decreases the speed of the video by a factor of two.
    void slowDown();

    /// @brief Sets position to specified frame.
    ///
    /// @param frame Frame to seek to.
    void setFrame(qint64 frame);

    /// @brief Sets position to next frame.
    void nextFrame();

    /// @brief Sets position to previous frame.
    void prevFrame();
signals:
    /// @brief Emitted when a frame is ready to display.
    //
    /// @param image Captured image.
    /// @param frame Frame number that corresponds to this image.
    void processedImage(QImage image, qint64 frame);

    /// @brief Emitted when duration changes.
    ///
    /// @param New video duration.
    void durationChanged(qint64 duration);

    /// @brief Emitted when playback rate changes.
    ///
    /// @param rate Playback rate (fps).
    void playbackRateChanged(double rate);

    /// @brief Emitted when video resolution changes.
    ///
    /// @param width Video width.
    /// @param height Video height.
    void resolutionChanged(qint64 width, qint64 height);

    /// @brief Emitted when play/pause state changed.
    ///
    /// @param stopped True if stopped, false otherwise.
    void stateChanged(bool stopped);

    /// @brief Emitted when new media is loaded.
    ///
    /// @param video_path Path to loaded video file.
    /// @param native_rate Native playback rate of the video.
    void mediaLoaded(QString video_path, qreal native_rate);

    /// @brief Emitted on error.
    ///
    /// @param err Error message.
    void error(QString err);
private:
    /// @brief Path to loaded video.
    QString video_path_;

    /// @brief Native frame rate of loaded video.
    double frame_rate_;

    /// @brief True if player is stopped, false otherwise.
    bool stopped_;

    /// @brief Stores most recent frame.
    cv::Mat frame_mat_;

    /// @brief Stores most recent RGB frame.
    cv::Mat rgb_frame_mat_;

    /// @brief Stores most recent image.
    QImage image_;

    /// @brief Current playback rate.
    double current_speed_;

    /// @brief Codec.
    AVCodec *codec_;

    /// @brief Codec context.
    AVCodecContext *codec_context_;

    /// @brief Format context.
    AVFormatContext *format_context_;

    /// @brief File pointer.
    FILE *file_;

    /// @brief Most recent frame.
    AVFrame *frame_;

    /// @brief Input buffer.
    uint8_t *in_buf_;

    /// @brief Video capture object.
    std::unique_ptr<cv::VideoCapture> capture_;

    /// @brief Delay between frames in microseconds.
    double delay_;

    /// @brief Current frame index.
    qint64 frame_index_;

    /// @brief Mutex for deletion.
    QMutex mutex_;

    /// @brief Wait condition for deletion.
    QWaitCondition condition_;

    /// @brief Sets the current frame.
    void setCurrentFrame(qint64 frame_num);

    /// @brief Waits for specified time while allowing events to process.
    void processWait(qint64 usec);
};

}} // namespace fish_annotator::video_annotator

#endif // VIDEO_ANNOTATOR_PLAYER_H
