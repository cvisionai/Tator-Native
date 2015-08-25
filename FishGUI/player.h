#include ui_mainwindow.h
#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui/hpp>

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
  signals:
  	void processedImage(const Qimage &image);
  protected:
  	void run();
  	void msleep(int ms);
  public:
  	Player(QObject *parent = 0);
  	~Player();
  	bool loadVideo(string filename);
  	void Play();
  	void Stop();
  	bool isStopped() const;
};
