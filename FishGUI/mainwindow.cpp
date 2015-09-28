#include <mainwindow.h>

MainWindow::MainWindow(QWidget *parent)
	:QWidget(parent),
	ui(new Ui::MainWidget)
	{
		myPlayer = new Player();
		fList = new fishSerialize::FishList();
		QObject::connect(myPlayer, SIGNAL(processedImage(QImage)),
			this, SLOT(updatePlayerUI(QImage)));
		ui->setupUi(this);
		ui->Play->setEnabled(false);
		ui->videoSlider->setEnabled(false);
		QStringList typeList;
		typeList.append("Round");
		typeList.append("Flat");
		typeList.append("Skate");
		typeList.append("Other");
		ui->typeMenu->addItems(typeList);
		QObject::connect(ui->typeMenu, SIGNAL(currentIndexChanged(int)),
			ui->subTypeMenu, SLOT(setCurrentIndex(int));
		ui->typeMenu->setCurrentIndex(3);
	}

MainWindow::~MainWindow()
{
	delete myPlayer;
	delete ui;
	//myFishList.clear();
	delete tempFish;
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

void MainWindow::updateSubTypeMenu(int typeIndex)
{
	QStringList sTypeList;
	ui->subTypeMenu->clear();
	if typeIndex == 0 {
		sTypeList.append("Round");
		sTypeList.append("Haddock");
		sTypeList.append("Cod");
		sTypeList.append("Whiting");
		sTypeList.append("Red Hake");
		sTypeList.append("Pollock");
		sTypeList.append("Herring");
		sTypeList.append("Unknown");
	}
	else if typeIndex == 1 {
		sTypeList.append("Flat");
		sTypeList.append("Yellowtail");
		sTypeList.append("Windowpane");
		sTypeList.append("Summer");
		sTypeList.append("Winter");
		sTypeList.append("Fourspot");
		sTypeList.append("Grey Sole");
		sTypeList.append("Halibut");
		sTypeList.append("Unknown");
	}
	else if typeIndex == 2 {
		sTypeList.append("Skate");
		sTypeList.append("Barndoor");
		sTypeList.append("Unknown");
	}
	else if typeIndex == 3 {
		sTypeList.append("Other");
		sTypeList.append("Dogfish");
		sTypelist.append("Monkfish");
		sTypelist.append("Lobster");
		sTypelist.append("Scallop");
	}

	ui->subTypeMenu->setCurrentIndex(0);
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

void MainWindow::on_addRound_clicked()
{
	addFish((FishTypeEnum) ROUND);
	/*
	myPlayer->Stop();
	double currentFrame = myPlayer->getCurrentFrame();
	FishTypeEnum fType = (FishTypeEnum) 0;
	tempFish = new Fish(fType,currentFrame); //VALUE HERE IS FRAME, REMEMBER
	myFishList.push_back(*tempFish);
	ui->typeMenu->setCurrentIndex(0);
	//updateSubTypeMenu(0);
	*/
}

void MainWindow::on_addFlat_clicked()
{
	addFish((FishTypeEnum) FLAT);
	/*
	myPlayer->Stop();
	double currentFrame = myPlayer->getCurrentFrame();
	FishTypeEnum fType = (FishTypeEnum) 1;
	tempFish = new Fish(fType,currentFrame);
	myFishList.push_back(*tempFish);

	ui->typeMenu->setCurrentIndex(1);
	//updateSubTypeMenu(1);
	*/
}

void MainWindow::on_addSkate_clicked()
{
	addFish((FishTypeEnum) SKATE);
	/*
	myPlayer->Stop();
	double currentFrame = myPlayer->getCurrentFrame();
	FishTypeEnum fType = (FishTypeEnum) 2;
	tempFish = new Fish(fType,currentFrame);
	myFishList.push_back(*tempFish);

	ui->typeMenu->setCurrentIndex(2);
	//updateSubTypeMenu(2);
	*/
}

void MainWindow::on_addOther_clicked()
{
	addFish((FishTypeEnum) OTHER);
	/*
	myPlayer->Stop();
	double currentFrame = myPlayer->getCurrentFrame();
	FishTypeEnum fType = (FishTypeEnum) 3;
	tempFish = new Fish(fType,currentFrame);
	myFishList.push_back(*tempFish);

	ui->typeMenu->setCurrentIndex(3);
	//updateSubTypeMenu(3);
	*/
}

void addFish(FishTypeEnum fType)
{
	myPlayer->Stop();
	double currentFrame = myPlayer->getCurrentFrame();
	tempFish = new Fish(fType,currentFrame);
	myFishList.push_back(*tempFish);

	ui->typeMenu->setCurrentIndex((int) fType);
	//updateSubTypeMenu(3);
}

void addFishSerialize(fishSerialize::Fish* newFish, fishSerialize::fTypeEnum newFType, int frame)
{
	newFish->set_fType(newFType);
	newFish->set_fFrame(frame);
}

void setTowType(bool towOpenStatus)
{
	//towOpenStatus = true corresponds to tow open
	fList->set_towOpen(towOpenStatus);
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow* w = new MainWindow();
	w->setAttribute(Qt::WA_DeleteOnClose, true);

	w->show();

	return a.exec();
}
