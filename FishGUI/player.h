#ifndef PLAYER_H
#define PLAYER_H
#include <QMutex>
#include <QThread>
#include <QImage>
#include <QWaitCondition>
#include <QMutexLocker>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <memory>

using namespace cv;
class Player : public QThread
{	Q_OBJECT
private:
 	bool stop;
 	QMutex mutex;
 	QWaitCondition condition;
 	Mat frame;
 	int frameRate;
    std::unique_ptr<VideoCapture> capture;
 	Mat RGBframe;
 	QImage img;
    int delay;
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
  void minusOneFrame();
  QImage getOneFrame();
  void setCurrentFrame(int frameNumber);
  double getFrameRate();
  double getCurrentFrame();
  double getNumberOfFrames();
};
#endif
