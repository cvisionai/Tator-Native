#include <mainwindow.h>

MainWindow::MainWindow(QWidget *parent)
	:QWidget(parent),
	ui(new Ui::MainWidget)
	{
		myPlayer = new Player();
		QObject::connect(myPlayer, 									SIGNAL(processedImage(QImage)),
			this, SLOT(updatePlayerUI(QImage)));
		ui->setupUi(this);
	}

void MainWindow::updatePlayerUI(QImage img)
{
	if (!img.isNull())
	{
		imgPointer->setPixmap(QPixmap::fromImage(img));
		scene->setSceneRect(img.rect());
		ui->videoWindow->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
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
			QImage firstImage = myPlayer->getFirstFrame();
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

void MainWindow::on_SpeedUp_clicked()
{
	myPlayer->speedUp();
}

void MainWindow::on_SlowDown_clicked()
{
	myPlayer->slowDown();
}

void MainWindow::on_addFlounder_clicked()
{
	FishTypeEnum fType = (FishTypeEnum) 0;
	tempFish = new Fish(fType,1);
	myFishList.push_back(*tempFish);
	for (int i=0; i < (int) myFishList.size();i++)
	{
	  fType = myFishList[i].getFishType();
	  std::cout << fType << endl;
	}
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
