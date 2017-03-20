#ifndef PLAYER_H
#define PLAYER_H

#include <cstdint>

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
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace fish_annotator { namespace video_annotator {

class Player : public QThread
{	
    Q_OBJECT
private:
    std::string video_path_;
    double frame_rate_;
    bool stop;
    QMutex mutex;
    QWaitCondition condition;
    cv::Mat frame;
    double currentSpeed;
    std::unique_ptr<cv::VideoCapture> capture;
    cv::Mat RGBframe;
    QImage img;
    int delay;
    std::int64_t frameIndex;
signals:
    /// @brief Emitted when a frame is ready to display.
    //
    /// @param image Captured image.
    /// @param frame Frame number that corresponds to this image.
    void processedImage(const QImage &image);//, uint64_t frame);

    /// @brief Emitted when duration changes.
    void durationChanged(uint64_t);

    /// @brief Emitted when position changes.
    void positionChanged(uint64_t);

    /// @brief Emitted when playback rate changes.
    void playbackRateChanged(double);

    /// @brief Emitted when new media is loaded.
    void mediaLoaded();

    /// @brief Emitted on error.
    void error(std::string err);
protected:
    void run();
    void msleep(int ms);
public:

    /// @brief Constructor.
    ///
    /// @param parent Parent widget.
    Player(QObject *parent = 0);

    /// @brief Destructor.
    ~Player();

    /// @brief Loads the video.
    bool loadVideo(const std::string &filename);

    /// @brief Gets loaded video path.
    ///
    /// @return Path to loaded video.
    const std::string &getVideoPath();

    /// @brief Gets loaded video frame rate.
    ///
    /// @return Video frame rate (fps).
    double getFrameRate();

    /// @brief Plays the video.
    void Play();
    
    /// @brief Stops the video.
    void Stop();

    /// @brief Gets the play/stop state of the video.
    ///
    /// @return True if stopped, false otherwise.
    bool isStopped() const;

    /// @brief Increases the speed of the video by a factor of two.
    void speedUp();

    /// @brief Decreases the speed of the video by a factor of two.
    void slowDown();

    /// @brief Increments the frame index.
    void incrementFrameIndex(){++frameIndex;}
    double getNumberOfFrames();
    double getCurrentSpeed();
    QImage nextFrame();
    QImage prevFrame();
    QImage setFrame(std::int64_t frame);
    std::int64_t getCurrentFrame();
    QImage getOneFrame();
    void write_image(QString filename);
private:
    void setCurrentFrame(std::int64_t frameNumber);
};

}} // namespace fish_annotator::video_annotator

#endif // PLAYER_H
