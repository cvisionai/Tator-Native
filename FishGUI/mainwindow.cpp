#include <mainwindow.h>

MainWindow::MainWindow(QWidget *parent)
	:QWidget(parent),
	ui(new Ui::MainWidget)
	{
        m_fIndex = 0;
		myPlayer = new Player();
		fList = new fishSerialize::FishList();
        myFishList.clear();
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
			this, SLOT(updateSubTypeMenu(int)));
		ui->typeMenu->setCurrentIndex(3);
        QObject::connect(ui->goToFishVal,SIGNAL(returnPressed()),
                         this, SLOT(goToFish()));
        QObject::connect(ui->towStatus, SIGNAL(toggled(bool)),
                         this, SLOT(setTowType(bool)));
        setTowType(true);
	}

MainWindow::~MainWindow()
{
	delete myPlayer;
	delete ui;
    //myFishList.clear();
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
    ui->subTypeMenu->blockSignals(true);
	ui->subTypeMenu->clear();
	if (typeIndex == 0) {
		sTypeList.append("Round");
		sTypeList.append("Haddock");
		sTypeList.append("Cod");
		sTypeList.append("Whiting");
		sTypeList.append("Red Hake");
		sTypeList.append("Pollock");
		sTypeList.append("Herring");
		sTypeList.append("Unknown");
	}
	else if (typeIndex == 1) {
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
	else if (typeIndex == 2) {
		sTypeList.append("Skate");
		sTypeList.append("Barndoor");
		sTypeList.append("Unknown");
	}
	else if (typeIndex == 3) {
		sTypeList.append("Other");
		sTypeList.append("Dogfish");
		sTypeList.append("Monkfish");
		sTypeList.append("Lobster");
		sTypeList.append("Scallop");
	}
	ui->subTypeMenu->addItems(sTypeList);
    ui->subTypeMenu->setCurrentIndex(0);
    ui->subTypeMenu->blockSignals(false);
}

void MainWindow::goToFish()
{
    int fNumber = ui->goToFishVal->text().toInt();
    if (fNumber > 0)
    {
        listPos = myFishList.begin()+fNumber-1;
        ui->typeMenu->setCurrentIndex((int) listPos->getFishType());
        ui->subTypeMenu->setCurrentIndex((int) listPos->getFishSubType());
        updateVecIndex();
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
            ui->videoWindow->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
			ui->videoWindow->show();
		}
	}

}

void MainWindow::on_loadAnnotate_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Open Annotation File"), ".",
        tr("Annotation Files (*.dat,*.csv)"));
    QFileInfo name = filename;
    if (!filename.isEmpty())
    {
        //Load the protobuf file here
    }
    return;
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
    ui->Play->setText(tr("Stop"));
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
}

void MainWindow::on_addFlat_clicked()
{
	addFish((FishTypeEnum) FLAT);
}

void MainWindow::on_addSkate_clicked()
{
	addFish((FishTypeEnum) SKATE);
}

void MainWindow::on_addOther_clicked()
{
	addFish((FishTypeEnum) OTHER);
}

void MainWindow::on_prevFish_clicked()
{
    if (listPos!=myFishList.begin())
    {
        listPos = listPos-1;
        ui->typeMenu->setCurrentIndex((int) listPos->getFishType());
        ui->subTypeMenu->setCurrentIndex((int) listPos->getFishSubType());
        updateVecIndex();
    }
}

void MainWindow::on_nextFish_clicked()
{
    if (listPos!=myFishList.end()-1)
    {
        listPos = listPos+1;
        ui->typeMenu->setCurrentIndex((int) listPos->getFishType());
        ui->subTypeMenu->setCurrentIndex((int) listPos->getFishSubType());
        updateVecIndex();
    }
}

void MainWindow::on_goToFrame_clicked()
{
    if (!myPlayer->isStopped())
    {
        myPlayer->Stop();
        ui->Play->setText(tr("Play"));
    }
    myPlayer->setCurrentFrame(listPos->getFrameCounted()-1);
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

void MainWindow::on_updateFishFrame_clicked()
{
    int currentFrame = (int) myPlayer->getCurrentFrame();
    listPos->frameCounted = currentFrame;
    updateVecIndex();
}

void MainWindow::on_typeMenu_currentIndexChanged(int tIdx)
{
    if (!myFishList.empty())
        listPos->setFishType((FishTypeEnum) tIdx);
}

void MainWindow::on_subTypeMenu_currentIndexChanged(int sIdx)
{
    if (!myFishList.empty())
        listPos->setFishSubType(sIdx);
}

void MainWindow::addFish(FishTypeEnum fType)
{

	myPlayer->Stop();
	ui->Play->setText(tr("Play"));
	double currentFrame = myPlayer->getCurrentFrame();
    Fish* tempFish = new Fish(fType,currentFrame);
    tempFish->setFishSubType(0);
	myFishList.push_back(*tempFish);
    //std::sort(myFishList.begin(), myFishList.end(), [](Fish a, Fish b){ return a.getFrameCounted() < b.getFrameCounted(); });
    std::sort(myFishList.begin(),myFishList.end());
   /*
              [ ](Fish a, Fish b) {
        return b.frameCounted < a.frameCounted;
    });
              */
    listPos = myFishList.end()-1;
    updateVecIndex();
    ui->totalFishVal->setText(QString::number(myFishList.size()));
	ui->typeMenu->setCurrentIndex((int) fType);
    //addFishSerialize(fList->add_fish(),fType,(int)currentFrame);
}

void MainWindow::addFishSerialize(fishSerialize::FishEntry* newFish, FishTypeEnum newFType, int frame)
{
    int newFTypeSerial = (int) newFType;
    newFish->set_ftype((fishSerialize::FishEntry::fTypeEnum) newFTypeSerial);
	newFish->set_fframe(frame);
}

void MainWindow::setTowType(bool towOpenStatus)
{
	//towOpenStatus = true corresponds to tow open
	fList->set_towopen(towOpenStatus);
}

void MainWindow::updateVecIndex()
{
    ui->fishNumVal->setText(QString::number(listPos - myFishList.begin()+1));
    ui->frameCountedVal->setText(QString::number(listPos->getFrameCounted()));
}

void MainWindow::convertFishToSerialize()
{
    fishSerialize::FishEntry* newFish;
    for(vector<Fish>::iterator it = myFishList.begin(); it != myFishList.end(); ++it)
    {
        newFish = fList->add_fish();
        newFish->set_ftype((fishSerialize::FishEntry_fTypeEnum)it->getFishType());
        newFish->set_fspecies(it->getFishSubType());
        newFish->set_fframe(it->frameCounted);
    }
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow* w = new MainWindow();
	w->setAttribute(Qt::WA_DeleteOnClose, true);

	w->show();

	return a.exec();
}
