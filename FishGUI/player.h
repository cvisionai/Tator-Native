#include <QMutex>
#include <QThread>
#include <QImage>
#include <QWaitCondition>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <string>

using namespace cv;
class Player : public QThread
{	Q_OBJECT
private:
 	bool stop;
 	QMutex mutex;
 	QWaitCondition condition;
 	Mat frame;
 	int frameRate;
 	VideoCapture capture;
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
};
