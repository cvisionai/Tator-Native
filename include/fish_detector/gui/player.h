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

namespace fish_detector { namespace gui {

using namespace cv;
class Player : public QThread
{	
  Q_OBJECT
private:
    bool stop;
    QMutex mutex;
    QWaitCondition condition;
    Mat frame;
    double frameRate;
    double currentSpeed;
    std::unique_ptr<VideoCapture> capture;
    Mat RGBframe;
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
    bool loadVideo(std::string filename);
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

}} // namespace fish_detector::gui

#endif // PLAYER_H
