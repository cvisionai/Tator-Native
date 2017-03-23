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

namespace fish_annotator { namespace video_annotator {

class Player : public QThread {	
    Q_OBJECT
public:
    /// @brief Constructor.
    explicit Player();

    /// @brief Destructor.
    ~Player();
protected:
    /// @brief Plays the video.
    void run() override final;
public slots:
    /// @brief Plays the video.
    void play();
    
    /// @brief Stops the video.
    void stop();

    /// @brief Loads the video.
    ///
    /// @param filename Path to video.
    void loadVideo(std::string filename);

    /// @brief Increases the speed of the video by a factor of two.
    void speedUp();

    /// @brief Decreases the speed of the video by a factor of two.
    void slowDown();

    /// @brief Sets position to specified frame.
    ///
    /// @param frame Frame to emit.
    void setFrame(uint64_t frame);

    /// @brief Sets position to next frame.
    void nextFrame();

    /// @brief Sets position to previous frame.
    void prevFrame();
signals:
    /// @brief Emitted when a frame is ready to display.
    //
    /// @param image Captured image.
    /// @param frame Frame number that corresponds to this image.
    void processedImageFromThread(QImage image, uint64_t frame);

    /// @brief Emitted when a frame is ready to display.
    //
    /// @param image Captured image.
    /// @param frame Frame number that corresponds to this image.
    void processedImage(QImage image, uint64_t frame);

    /// @brief Emitted when duration changes.
    ///
    /// @param New video duration.
    void durationChanged(uint64_t duration);

    /// @brief Emitted when position changes.
    ///
    /// @param position New video position.
    void positionChanged(uint64_t position);

    /// @brief Emitted when playback rate changes.
    ///
    /// @param rate Playback rate (fps).
    void playbackRateChanged(double rate);

    /// @brief Emitted when play/pause state changed.
    ///
    /// @param stopped True if stopped, false otherwise.
    void stateChanged(bool stopped);

    /// @brief Emitted when new media is loaded.
    ///
    /// @param video_path Path to loaded video file.
    void mediaLoaded(std::string video_path);

    /// @brief Emitted on error.
    ///
    /// @param err Error message.
    void error(std::string err);
private:
    /// @brief Path to loaded video.
    std::string video_path_;

    /// @brief Native frame rate of loaded video.
    double frame_rate_;

    /// @brief True if player is stopped, false otherwise.
    bool stopped_;

    /// @brief Stores most recent frame.
    cv::Mat frame_mat_;

    /// @brief Stores most recent RGB frame.
    cv::Mat rgb_frame_mat_;

    /// @brief Current playback rate.
    double current_speed_;

    /// @brief Video capture object.
    std::unique_ptr<cv::VideoCapture> capture_;

    /// @brief Delay between frames in microseconds.
    double delay_;

    /// @brief Current frame index.
    uint64_t frame_index_;

    /// @brief Mutex for deletion.
    QMutex mutex_;

    /// @brief Wait condition for deletion.
    QWaitCondition condition_;

    /// @brief Sets the current frame.
    void setCurrentFrame(uint64_t frame_num);

    /// @brief Processes a single frame and emits it.
    inline QImage getOneFrame();

    /// @brief Waits for specified time while allowing events to process.
    void processWait(uint64_t usec);
};

}} // namespace fish_annotator::video_annotator

#endif // VIDEO_ANNOTATOR_PLAYER_H
