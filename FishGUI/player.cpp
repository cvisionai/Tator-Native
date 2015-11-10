#include <player.h>

Player::Player(QObject *parent)
	: QThread(parent)
	{
		stop = true;
	}

bool Player::loadVideo(std::string filename) {
    capture.reset(new cv::VideoCapture(filename));
	if (capture->isOpened())
	{
		frameRate = (int) capture->get(CV_CAP_PROP_FPS);
		delay = (1000/frameRate);
		return true;
	}
	else
		return false;
}

void Player::Play()
{
	if (!isRunning()) 
	{
		if (isStopped())
		{
			stop = false;
		}
		start(LowPriority);
	}
}

void Player::run()
{
	while(!stop) {
		if (!capture->read(frame))
		{
			stop = true;
		}
		if (frame.channels() == 3)
		{
			cv::cvtColor(frame, RGBframe, CV_BGR2RGB);
			img = QImage((const unsigned char*)(RGBframe.data),
				RGBframe.cols, RGBframe.rows, QImage::Format_RGB888);
		}
		else
		{
			img = QImage((const unsigned char*)(frame.data),
				frame.cols, frame.rows, QImage::Format_Indexed8);
		}
		emit processedImage(img);
		this->msleep(delay);
	}
}

QImage Player::getOneFrame()
{
	capture->read(frame);
	if (frame.channels() == 3)
	{
		cv::cvtColor(frame, RGBframe, CV_BGR2RGB);
		img = QImage((const unsigned char*)(RGBframe.data),
			RGBframe.cols, RGBframe.rows, QImage::Format_RGB888);
	}
	else
	{
		img = QImage((const unsigned char*)(frame.data),
			frame.cols, frame.rows, QImage::Format_Indexed8);
	}

	return img;
}

Player::~Player()
{
    if (1)
    {
        QMutexLocker locker(&mutex);
        stop = true;
        //capture->release();
        condition.wakeOne();
        wait();
    }
}
void Player::Stop()
{
	stop = true;
}

void Player::msleep(int ms)
{
    std::this_thread::sleep_for(std::chrono::microseconds(ms*1000));
}

bool Player::isStopped() const{
	return this->stop;
}

void Player::speedUp()
{
	delay = delay/2;
	if (delay<1)
	  delay = 1;
}

void Player::slowDown()
{
	delay = delay*2;
}

void Player::minusOneFrame()
{
	double currentFrame = getCurrentFrame();
	currentFrame = currentFrame - (double)2;
	setCurrentFrame((int)currentFrame);
}

double Player::getCurrentFrame()
{
	return capture->get(CV_CAP_PROP_POS_FRAMES);
}

double Player::getNumberOfFrames()
{
	return capture->get(CV_CAP_PROP_FRAME_COUNT);
}

double Player::getFrameRate()
{
	return frameRate;
}

void Player::setCurrentFrame(int frameNumber)
{
	capture->set(CV_CAP_PROP_POS_FRAMES, frameNumber);
}
