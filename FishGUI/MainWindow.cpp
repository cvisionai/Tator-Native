#include <MainWindow.h>

MainWindow::MainWindow(QWidget *parent)
	:QWidget(parent),
	ui(new Ui::MainWidget)
	{
		myPlayer = new Player();
		QObject::connect(myPlayer, SIGNAL(processedImage(QImage)),
			this, SLOT(updatePlayerUI(QImage)));
		ui->setupUi(this);
	}

void MainWindow::updatePlayerUI(QImage img)
{
	if (!img.isNull())
	{
		ui->VideoWindow->setAlignment(Qt::AlignCenter);
		ui->VideoWindow->setPixmap(QPixmap::fromImage(img).scaled(ui->VideoWindow->size(),
			Qt::KeepAspectRatio, Qt::FastTransformation));
	}
}

MainWindow::~MainWindow()
{
	delete myPlayer;
	delete ui;
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

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow* w = new MainWindow();
	w->setAttribute(Qt::WA_DeleteOnClose, true);

	w->show();

	return a.exec();
}
