#include <mainwindow.h>

MainWindow::MainWindow(QWidget *parent)
	:QWidget(parent),
	ui(new Ui::MainWidget)
	{
		myPlayer = new Player();
		QObject::connect(myPlayer, 									SIGNAL(processedImage(QImage)),
			this, SLOT(updatePlayerUI(QImage)));
		ui->setupUi(this);
		ui->Play->setEnabled(false);
		ui->videoSlider->setEnabled(false);
	}

void MainWindow::updatePlayerUI(QImage img)
{
	if (!img.isNull())
	{
		imgPointer->setPixmap(QPixmap::fromImage(img));
		scene->setSceneRect(img.rect());
		ui->videoWindow->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
		ui->videoSlider->setValue(myPlayer->getCurrentFrame());
		ui->currentTime->setText(getFormattedTime((int)myPlayer->
			getCurrentFrame() / (int)myPlayer->getFrameRate()));
	}
}

MainWindow::~MainWindow()
{
	delete myPlayer;
	delete ui;
	//delete imgPointer;
	//delete scene;
	myFishList.clear();
	delete tempFish;
}

void MainWindow::on_LoadVideo_clicked()
{
	QString filename = QFileDialog::getOpenFileName(this,
		tr("Open Video"), ".",
		tr("Video Files (*.avi *.mpg *.mp4)"));

	QFileInfo name = filename;
	if (!filename.isEmpty())
	{
		if (!myPlayer->loadVideo(filename.toAscii().data()))
		{
			QMessageBox msgBox;
			msgBox.setText("The selected video could not be opened!");
			msgBox.exec();
		}
		else
		{
			this->setWindowTitle(name.fileName());
			ui->Play->setEnabled(true);
			ui->videoSlider->setEnabled(true);
			ui->videoSlider->setMaximum(myPlayer->getNumberOfFrames());
			ui->totalTime->setText(getFormattedTime((int)myPlayer-> 
				getNumberOfFrames() / (int)myPlayer->getFrameRate()));
			QImage firstImage = myPlayer->getOneFrame();
			scene = new QGraphicsScene(this);
			imgPointer = scene->addPixmap(QPixmap::fromImage(firstImage));
			scene->setSceneRect(firstImage.rect());
			ui->videoWindow->setScene(scene);
			ui->videoWindow->fitInView(scene->sceneRect());
			ui->videoWindow->show();
		}
	}

}

void MainWindow::on_Play_clicked()
{
	if (myPlayer->isStopped())
	{
		myPlayer->Play();
		ui->Play->setText(tr("Stop"));
	}
	else
	{
		myPlayer->Stop();
		ui->Play->setText(tr("Play"));
	}
}

QString MainWindow::getFormattedTime(int timeInSeconds)
{
	int seconds = (int) (timeInSeconds) % 60;
	int minutes = (int)((timeInSeconds / 60) % 60);
	int hours = (int)((timeInSeconds / (60 * 60)) % 24);

	QTime t(hours, minutes, seconds);
	if (hours == 0)
		return t.toString("mm:ss");
	else
		return t.toString("h:mm:ss");
}

void MainWindow::on_videoSlider_sliderPressed()
{
	myPlayer->Stop();
}

void MainWindow::on_videoSlider_sliderReleased()
{
	myPlayer->Play();
}

void MainWindow::on_videoSlider_sliderMoved(int position)
{
	myPlayer->setCurrentFrame(position);
	ui->currentTime->setText(getFormattedTime(position / (int)myPlayer->getFrameRate()));
}

void MainWindow::on_SpeedUp_clicked()
{
	myPlayer->speedUp();
}

void MainWindow::on_SlowDown_clicked()
{
	myPlayer->slowDown();
}

void MainWindow::on_minusOneFrame_clicked()
{
	if (!myPlayer->isStopped())
	{
		myPlayer->Stop();
		ui->Play->setText(tr("Play"));
	}
	myPlayer->minusOneFrame();
	QImage thisFrame = myPlayer->getOneFrame();
	if (!thisFrame.isNull())
	{
        	imgPointer->setPixmap(QPixmap::fromImage(thisFrame));
		scene->setSceneRect(thisFrame.rect());
        	ui->videoWindow->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
		ui->videoSlider->setValue(myPlayer->getCurrentFrame());
		ui->currentTime->setText(getFormattedTime((int)myPlayer->
			getCurrentFrame() / (int)myPlayer->getFrameRate()));
	}
}

void MainWindow::on_plusOneFrame_clicked()
{
	if (!myPlayer->isStopped())
	{
		myPlayer->Stop();
		ui->Play->setText(tr("Play"));
	}
	QImage thisFrame = myPlayer->getOneFrame();
	if (!thisFrame.isNull())
	{
		imgPointer->setPixmap(QPixmap::fromImage(thisFrame));
		scene->setSceneRect(thisFrame.rect());
		ui->videoWindow->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
		ui->videoSlider->setValue(myPlayer->getCurrentFrame());
		ui->currentTime->setText(getFormattedTime((int)myPlayer->
			getCurrentFrame() / (int)myPlayer->getFrameRate()));
	}

}

void MainWindow::on_addFlounder_clicked()
{
	FishTypeEnum fType = (FishTypeEnum) 0;
	tempFish = new Fish(fType,1);
	myFishList.push_back(*tempFish);
//	for (int i=0; i < (int) myFishList.size();i++)
//	{
//	  fType = myFishList[i].getFishType();
//	  std::cout << fType << endl;
//	}
}

void MainWindow::on_addCod_clicked()
{
	return;
}

void MainWindow::on_addHaddock_clicked()
{
	return;
}

void MainWindow::on_addSkate_clicked()
{
	return;
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow* w = new MainWindow();
	w->setAttribute(Qt::WA_DeleteOnClose, true);

	w->show();

	return a.exec();
}
