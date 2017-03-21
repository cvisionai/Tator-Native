#ifndef VIDEO_ANNOTATOR_PLAYER_H
#define VIDEO_ANNOTATOR_PLAYER_H

#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <memory>

#include <QMutex>
#include <QThread>
#include <QImage>
#include <QWaitCondition>
#include <QMutexLocker>
#include <QCoreApplication>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace fish_annotator { namespace video_annotator {

class Player : public QThread {	
    Q_OBJECT
public:
    /// @brief Constructor.
    ///
    /// @param parent Parent widget.
    Player(QObject *parent = 0);

    /// @brief Destructor.
    ~Player();

    /// @brief Loads the video.
    void loadVideo(const std::string &filename);

    /// @brief Gets loaded video path.
    ///
    /// @return Path to loaded video.
    const std::string &getVideoPath();

    /// @brief Gets loaded video frame rate.
    ///
    /// @return Video frame rate (fps).
    double getFrameRate();

    /// @brief Plays the video.
    void play();
    
    /// @brief Stops the video.
    void stop();

    /// @brief Gets the play/stop state of the video.
    ///
    /// @return True if stopped, false otherwise.
    bool isStopped() const;

    /// @brief Increases the speed of the video by a factor of two.
    void speedUp();

    /// @brief Decreases the speed of the video by a factor of two.
    void slowDown();

    /// @brief Get number of frames from loaded video.
    double getNumberOfFrames();

    /// @brief Get current playback rate.
    double getCurrentSpeed();

    /// @brief Emits next frame.
    void nextFrame();

    /// @brief Emits previous frame.
    void prevFrame();

    /// @brief Emits specified frame.
    ///
    /// @param frame Frame to emit.
    void setFrame(int64_t frame);

    /// @brief 
    int64_t getCurrentFrame();

    /// @brief Processes a single frame and emits it.
    void getOneFrame();

    /// @brief Writes the last image to file.
    void writeImage(const QString &filename);
protected:
    /// @brief Plays the video.
    void run();

    /// @brief Delays by specified number of microseconds.
    ///
    /// @param usec Number of microseconds to delay.
    void usleep(int usec);
signals:
    /// @brief Emitted when a frame is ready to display.
    //
    /// @param image Captured image.
    /// @param frame Frame number that corresponds to this image.
    void processedImage(std::shared_ptr<QImage> image, uint64_t frame);

    /// @brief Emitted when duration changes.
    void durationChanged(uint64_t);

    /// @brief Emitted when position changes.
    void positionChanged(uint64_t);

    /// @brief Emitted when playback rate changes.
    void playbackRateChanged(double);

    /// @brief Emitted when new media is loaded.
    void mediaLoaded();

    /// @brief Emitted on error.
    void error(const std::string &err);
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

    /// @brief Ben explain.
    QMutex mutex_;

    /// @brief Ben explain.
    QWaitCondition condition_;

    /// @brief Current playback rate.
    double current_speed_;

    /// @brief Video capture object.
    std::unique_ptr<cv::VideoCapture> capture_;

    /// @brief Delay between frames in microseconds.
    double delay_;

    /// @brief Current frame index.
    uint64_t frame_index_;

    /// @brief Sets the current frame.
    void setCurrentFrame(int64_t frame_num);
};

}} // namespace fish_annotator::video_annotator

#endif // VIDEO_ANNOTATOR_PLAYER_H
