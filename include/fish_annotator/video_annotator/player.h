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
    bool stop;
    QMutex mutex;
    QWaitCondition condition;
    cv::Mat frame;
    double frameRate;
    double currentSpeed;
    std::unique_ptr<cv::VideoCapture> capture;
    cv::Mat RGBframe;
    QImage img;
    int delay;
    std::int64_t frameIndex;
signals:
    void processedImage(const QImage &image);
protected:
    void run();
    void msleep(int ms);
public:
    Player(QObject *parent = 0);
    ~Player();
    bool loadVideo(const std::string &filename);
    void Play();
    void Stop();
    bool isStopped() const;
    void speedUp();
    void slowDown();
    void incrementFrameIndex(){++frameIndex;}
    double getFrameRate();
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
