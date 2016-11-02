#include "fish_detector/gui/mainwindow.h"
#include "ui_mainwindow.h"


namespace fish_detector { namespace gui {
/// @brief Mainwindow constructor.
///
/// @param parent The parent widget for mainwindow.
/// @return no return.
MainWindow::MainWindow(QWidget *parent)
  :QWidget(parent)
{
  ui.reset(new Ui::MainWidget);
  fIndex = 0;
  nextID = 1;
  player.reset(new Player());
  myFishList.clear();
  QObject::connect(player.get(), SIGNAL(processedImage(QImage)),
           this, SLOT(updatePlayerUI(QImage)));
  ui->setupUi(this);
  disableControls();
  updateTypeMenu();
  QObject::connect(ui->typeMenu, SIGNAL(currentIndexChanged(int)),
           this, SLOT(updateSubTypeMenu(int)));
  ui->typeMenu->setCurrentIndex(3);
  QObject::connect(ui->goToFishVal,SIGNAL(returnPressed()),
           this, SLOT(goToFish()));

  auto next_button = ui->navigator->findChild<QPushButton *>("next_button");
  connect(next_button, SIGNAL(clicked()), this, SLOT(on_plusOneFrame_clicked()));

  auto prev_button = ui->navigator->findChild<QPushButton *>("prev_button");
  connect(prev_button, SIGNAL(clicked()), this, SLOT(on_minusOneFrame_clicked()));

  auto add_region_button = ui->navigator->findChild<QPushButton *>("add_region_button");
  connect(add_region_button, SIGNAL(clicked()), this, SLOT(on_addRegion_clicked()));

  auto remove_region_button = ui->navigator->findChild<QPushButton *>("remove_region_button");
  connect(remove_region_button, SIGNAL(clicked()), this, SLOT(on_removeRegion_clicked()));

  auto next_and_copy_button = ui->navigator->findChild<QPushButton *>("next_with_copy_button");
  connect(next_and_copy_button, SIGNAL(clicked()), this, SLOT(on_nextAndCopy_clicked()));
}

/// @brief Retrieves next frame from video player and adds annotations to frame.
///
/// @return no return.
void MainWindow::nextFrame()
{
  updateImage(player->nextFrame()); // get next frame
  processAnnotations(player->getCurrentFrame());
}

/// @brief Retrieves previous frame from video player and adds annotations to frame.
///
/// @return no return.
void MainWindow::prevFrame()
{
  updateImage(player->prevFrame()); // get next frame
  processAnnotations(player->getCurrentFrame());
}

/// @brief Retrieves annotations associated with frame.
///
/// @param frame The frame for which you want to retrieve annotations.
/// @return no return.

void MainWindow::processAnnotations(uint64_t frame) {
  // remove old annotations
  for (auto ann : currentAnnotations) {
    scene->removeItem(ann);
  }
  currentAnnotations.clear();
  // add new annotations
  for (auto ann : document->getAnnotations(frame)) {
    auto rect = QRectF(ann.second->area.x, ann.second->area.y, ann.second->area.w, ann.second->area.h);
    auto region = new AnnotatedRegion(ann.first, ann.second, rect);
    scene->addItem(region);
    currentAnnotations.push_back(region);
  }
}

/// @brief Updates the GUI window with image img.
///
/// @param img The image to display in the GUI window
/// @return no return.
void MainWindow::updatePlayerUI(QImage img)
{
  if (!img.isNull())
  {
    displayImage->setPixmap(QPixmap::fromImage(img));
    scene->setSceneRect(img.rect());
    ui->videoWindow->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
    ui->videoSlider->setValue(player->getCurrentFrame());
    ui->currentTime->setText(getFormattedTime((int)player->
    getCurrentFrame() / (int)player->getFrameRate()));
    processAnnotations(player->getCurrentFrame());
  }
}

/// @brief Retrieves information for fish number currently in goToFishVal.
///
/// @return no return.
void MainWindow::goToFish()
{
  int fNumber = ui->goToFishVal->text().toInt();
  if ((fNumber > 0) && (fNumber < int(myFishList.size())))
  {
    listPos = myFishList.begin()+fNumber-1;
    ui->typeMenu->setCurrentIndex((int) listPos->getFishType());
    ui->subTypeMenu->setCurrentIndex((int) listPos->getFishSubType());
    updateVecIndex();
  }
  else
  {
    listPos = myFishList.end()-1;
    ui->typeMenu->setCurrentIndex((int) listPos->getFishType());
    ui->subTypeMenu->setCurrentIndex((int) listPos->getFishSubType());
    updateVecIndex();
  }
}

/// @brief Converts time in seconds into hours, minutes, seconds.
///
/// @param timeInSeconds The time from the start of the video in seconds.
/// @return no return.
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

/// @brief Loads a video file user chooses with popup dialog.
///
/// @return no return.
void MainWindow::on_LoadVideo_clicked()
{
  QString filename = QFileDialog::getOpenFileName(this,
  tr("Open Video"), ".",
  tr("Video Files (*.avi *.mpg *.mp4 *.mkv)"));
  QFileInfo name = filename;
  if (!filename.isEmpty())
  {
    if (!player->loadVideo(filename.toLatin1().data()))
    {
      QMessageBox msgBox;
      msgBox.setText("The selected video could not be opened!");
      msgBox.exec();
    }
    else
    {
      enableControls();
      this->setWindowTitle(name.fileName());
      ui->Play->setEnabled(true);
      ui->videoSlider->setEnabled(true);
      ui->videoSlider->setMaximum(player->getNumberOfFrames());
      ui->totalTime->setText(getFormattedTime((int)player->
      getNumberOfFrames() / (int)player->getFrameRate()));
      QImage firstImage = player->getOneFrame();
      //player->incrementFrameIndex();
      scene.reset(new QGraphicsScene(this));
      displayImage = scene->addPixmap(QPixmap::fromImage(firstImage));
      scene->setSceneRect(firstImage.rect());
      ui->videoWindow->setScene(scene.get());
      ui->videoWindow->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
      ui->videoWindow->show();
      ui->currentSpeed->setText("Current Speed: 100%");
      ui->Play->setFocus();
      // TODO: should first try to load this if the data
      // file exists
      document.reset(new Document());
    }
  }
}

/// @brief Starts video player.
///
/// @return no return.
void MainWindow::on_Play_clicked()
{
  if (player->isStopped())
  {
    player->Play();
    ui->Play->setText(tr("Stop"));
  }
  else
  {
    player->Stop();
    ui->Play->setText(tr("Play"));
  }
}

/// @brief Stops the vido player for seeking within video using slider.
///
/// @return no return.
void MainWindow::on_videoSlider_sliderPressed()
{
  player->Stop();
}

/// @brief Starts the video player after seeking within video using slider.
///
/// @return no return.
void MainWindow::on_videoSlider_sliderReleased()
{
  player->Play();
  ui->Play->setText(tr("Stop"));
}

/// @brief Sets the frame that the video player starts playing at within the video after seeking using the slider.
///
/// @param position The position of the video slider.
/// @return no return.
void MainWindow::on_videoSlider_sliderMoved(int position)
{
  player->setFrame(position);
  ui->currentTime->setText(getFormattedTime((int)(position / player->getFrameRate())));
}

/// @brief Speeds up the playback rate of the video player.
///
/// @return no return.
void MainWindow::on_SpeedUp_clicked()
{
  player->speedUp();
  QString tempSpeed;
  tempSpeed.setNum((int)(player->getCurrentSpeed()));
  ui->currentSpeed->setText("Current Speed: " + tempSpeed + "%");
}

/// @brief Slows down the playback rate of the video player.
///
/// @return no return.
void MainWindow::on_SlowDown_clicked()
{
  player->slowDown();
  QString tempSpeed;
  tempSpeed.setNum((int)(player->getCurrentSpeed()));
  ui->currentSpeed->setText("Current Speed: " + tempSpeed + "%");
}

/// @brief Rewinds the video by 1 second.
///
/// @return no return.
void MainWindow::on_minusOneSecond_clicked()
{
	rewind_video(1);
}

/// @brief Rewinds the video by 3 seconds.
///
/// @return no return.
void MainWindow::on_minusThreeSecond_clicked() {
	rewind_video(3);
}

/// @brief Rewinds the video by seconds_to_rewind seconds.
///
/// @param seconds_to_rewind The number of seconds in the video to rewind.
/// @return no return.
void MainWindow::rewind_video(int seconds_to_rewind) {
	if (!(player == NULL))
	{
		if (!player->isStopped())
		{
			player->Stop();
			ui->Play->setText(tr("Play"));
		}

		bool valid_rewind = player->getCurrentFrame() > seconds_to_rewind * player->getFrameRate();

		QImage image = player->setFrame(player->getCurrentFrame() - valid_rewind * seconds_to_rewind * player->getFrameRate());

		if (!image.isNull())
		{
			displayImage->setPixmap(QPixmap::fromImage(image));
			scene->setSceneRect(image.rect());
			ui->videoWindow->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
			ui->videoSlider->setValue(player->getCurrentFrame());
			ui->currentTime->setText(getFormattedTime((int)player->
				getCurrentFrame() / (int)player->getFrameRate()));
		}
		processAnnotations(player->getCurrentFrame());
		player->Play();
		ui->Play->setText(tr("Stop"));
	}
}

void MainWindow::updateImage(const QImage &image)
{
  if (!player->isStopped())
  {
  player->Stop();
  ui->Play->setText(tr("Play"));
  }

  displayImage->setPixmap(QPixmap::fromImage(image));
  scene->setSceneRect(image.rect());
  ui->videoWindow->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
  ui->videoSlider->setValue(player->getCurrentFrame());
  ui->currentTime->setText(getFormattedTime((int)player->
    getCurrentFrame() / (int)player->getFrameRate()));
}

void MainWindow::on_minusOneFrame_clicked()
{
  prevFrame();
}

void MainWindow::on_plusOneFrame_clicked()
{
  nextFrame();
}

void MainWindow::on_goToFrame_clicked()
{
  if (!(player==NULL))
  {
    if (!player->isStopped())
    {
      player->Stop();
      ui->Play->setText(tr("Play"));
    }
    QImage image = player->setFrame(listPos->getFrameCounted());

    if (!image.isNull())
    {
      displayImage->setPixmap(QPixmap::fromImage(image));
      scene->setSceneRect(image.rect());
      ui->videoWindow->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
      ui->videoSlider->setValue(player->getCurrentFrame());
      ui->currentTime->setText(getFormattedTime((int)player->
        getCurrentFrame() / (int)player->getFrameRate()));
    }
    processAnnotations(player->getCurrentFrame());
  }
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

void MainWindow::keyPressEvent(QKeyEvent* e)
{
  QString keypress = e->text();
  int keycode;
  if (keypress == "f")
    keycode = 0;
  else if (keypress == "r")
    keycode = 1;
  else if (keypress == "s")
    keycode = 2;
  else if (keypress == "t")
    keycode = 3;
  else if (keypress == "n")
    keycode = 4;
  else if (keypress == "b")
    keycode = 5;
  else if (keypress == "m")
    keycode = 6;
  else if (keypress == "a")
    keycode = 7;
  else if (keypress == "q")
    keycode = 8;
  else if (keypress == "p")
    keycode = 9;
  else
    keycode = 10;

  switch (keycode)
  {
  case 0:
    ui->addFlat->animateClick();
    ui->Play->setFocus();
    break;
  case 1:
    ui->addRound->animateClick();
    ui->Play->setFocus();
    break;
  case 2:
    ui->addSkate->animateClick();
    ui->Play->setFocus();
    break;
  case 3:
    ui->addOther->animateClick();
    ui->Play->setFocus();
    break;
  case 4:
    ui->navigator->findChild<QPushButton *>("next_with_copy_button")->animateClick();
    ui->Play->setFocus();
    break;
  case 5:
    ui->navigator->findChild<QPushButton *>("prev_button")->animateClick();
    ui->Play->setFocus();
    break;
  case 6:
    ui->navigator->findChild<QPushButton *>("next_button")->animateClick();
    ui->Play->setFocus();
    break;
  case 7:
    ui->navigator->findChild<QPushButton *>("add_region_button")->animateClick();
    ui->Play->setFocus();
    break;
  case 8:
    ui->navigator->findChild<QPushButton *>("remove_region_button")->animateClick();
    ui->Play->setFocus();
    break;
  case 9:
    ui->Play->animateClick();
    break;
  case 10:
    QWidget::keyPressEvent(e);
    break;
  }
}

void MainWindow::on_addRegion_clicked()
{
  Rect area(0, 0, 100, 100);
  auto fishID = uint64_t(listPos->getID());
  auto frame = uint64_t(player->getCurrentFrame());
  //First check to see if there's an annotation for this ID already.
  if (document->keyExists(fishID))
  {
    removeRegion(fishID, frame);
  }
  else {
    document->addAnnotation(fishID);
  }
  auto loc = document->addAnnotationLocation(fishID, frame, area);
  auto annotationArea = new AnnotatedRegion(fishID,loc, QRect(0, 0, 100, 100));
  currentAnnotations.push_back(annotationArea);
  scene->addItem(annotationArea);
}

void MainWindow::on_removeRegion_clicked()
{
  auto fishID = uint64_t(listPos->getID());
  auto frame = uint64_t(player->getCurrentFrame());
  removeRegion(fishID, frame);
}

void MainWindow::removeRegion(std::uint64_t id, std::uint64_t frame) {
  //First check to see if there's an annotation for this ID already.
  if (document->keyExists(id))
  {
    //check to see if there's an annotation location for this frame already
    auto currentAnn = document->getAnnotation(id);
    if (currentAnn->frameHasAnn(frame)) {
      currentAnn->removeFrameAnn(frame);
      document->removeFrameAnnotation(id, frame);
      auto it = find_if(currentAnnotations.begin(), currentAnnotations.end(), \
                [&id](AnnotatedRegion* obj) {return obj->getUID() == id;});
      if (it != currentAnnotations.end()) {
        scene->removeItem(*it);
        currentAnnotations.erase(it);
      }
    }
  }
  //This function will do nothing if there's no annotation to erase
}

void MainWindow::on_nextAndCopy_clicked()
{
  // remove old annotations
  for (auto ann : currentAnnotations) {
    scene->removeItem(ann);
  }
  currentAnnotations.clear();
  auto prevFrame = player->getCurrentFrame();
  updateImage(player->nextFrame());
  for (auto ann: document->getAnnotations()) {
    auto id = ann.first;
    if(listPos->getID() == id) {
      removeRegion(id, prevFrame+1);
      break;
    }
  }
  // copy annotation
  for (auto ann : document->getAnnotations(prevFrame)) {
    auto id = ann.first;
    if(listPos->getID() == id) {
      auto frame = ann.second->frame+1;
      auto area = ann.second->area;
      auto loc = document->addAnnotationLocation(ann.first,frame, area);
      break;
    }
  }
  processAnnotations(player->getCurrentFrame());
}
/* Saving to turn into next and copy all
void MainWindow::on_nextAndCopy_clicked()
{
  // remove old annotations
  for (auto ann : currentAnnotations) {
    scene->removeItem(ann);
  }
  currentAnnotations.clear();
  auto prevFrame = player->getCurrentFrame();
  updateImage(player->nextFrame());
  //Currently delete existing annotations and overwrites. Want to change
  //to don't overwrite existing annotations.  Probably going to have to
  //do an exhaustive search of existing annotations for each potential
  //copied annotation.  Research code templates to shortcut this.  Make
  //sure to understand data types involved.

  //check this frame for annotations. We're going to delete anything
  //that currently exists.
  for (auto ann: document->getAnnotations()) {
    auto id = ann.first;
    removeRegion(id, prevFrame+1);
  }
  // copy annotations
  for (auto ann : document->getAnnotations(prevFrame)) {
    auto frame = ann.second->frame+1;
    auto area = ann.second->area;
    auto loc = document->addAnnotationLocation(ann.first,frame, area);
    auto rect = QRectF(area.x,area.y,area.w,area.h);
    auto region = new AnnotatedRegion(ann.first, loc, rect);
    scene->addItem(region);
    currentAnnotations.push_back(region);
  }
}
*/
void MainWindow::addFish(FishTypeEnum fType)
{
  player->Stop();
  ui->Play->setText(tr("Play"));
  double currentFrame = player->getCurrentFrame();
  int id;
  if (myFishList.size() == 0) {
    id = 1;
    nextID = 2;
  }
  else {
    id = nextID;
    nextID++;

  }
  std::unique_ptr<Fish> tempFish(new Fish(fType,currentFrame,id));
  tempFish->setFishSubType(0);

  if (myFishList.size() == 0) {
    myFishList.push_back(*tempFish);
    listPos = myFishList.end()-1;
  }
  else {
    std::vector<Fish>::iterator tempLoc = myFishList.begin();
    int tempIndex = 0;
    while(tempLoc->getFrameCounted() < currentFrame) {
      tempLoc++;
      if(tempLoc==myFishList.end()) break;
      tempIndex++;
    }
    if (tempLoc == myFishList.end()) {
      myFishList.push_back(*tempFish);
      listPos = myFishList.end()-1;
    }
    else {
      if (tempLoc == myFishList.begin()) {
        myFishList.insert(tempLoc, *tempFish);
        listPos = myFishList.begin();
      }
      else {
        myFishList.insert(myFishList.begin()+tempIndex, *tempFish);
        listPos = myFishList.begin()+tempIndex;
      }
    }
  }
  ui->fishNumVal->setText(QString::number(id));
  ui->frameCountedVal->setText(QString::number(currentFrame));
  ui->totalFishVal->setText(QString::number(myFishList.size()));
  ui->typeMenu->setCurrentIndex((int) fType);
  ui->subTypeMenu->setCurrentIndex((int) 0);
}

void MainWindow::updateVecIndex()
{
  ui->fishNumVal->setText(QString::number(listPos->getID()));
  ui->frameCountedVal->setText(QString::number(listPos->getFrameCounted()));
}

void MainWindow::disableControls()
{
  ui->Play->setEnabled(false);
  ui->videoSlider->setEnabled(false);
  ui->SlowDown->setEnabled(false);
  ui->SpeedUp->setEnabled(false);
  ui->minusOneFrame->setEnabled(false);
  ui->plusOneFrame->setEnabled(false);
  ui->removeFish->setEnabled(false);
  ui->goToFrame->setEnabled(false);
  ui->addRound->setEnabled(false);
  ui->addFlat->setEnabled(false);
  ui->addSkate->setEnabled(false);
  ui->addOther->setEnabled(false);
  ui->saveAnnotate->setEnabled(false);
  ui->removeFish->setEnabled(false);
  ui->goToFrame->setEnabled(false);
  ui->prevFish->setEnabled(false);
  ui->nextFish->setEnabled(false);
  ui->loadAnnotate->setEnabled(false);
  ui->minusOneSecond->setEnabled(false);
  ui->minusThreeSecond->setEnabled(false);
  ui->writeImage->setEnabled(false);
  ui->navigator->findChild<QPushButton *>("next_button")->setEnabled(false);
  ui->navigator->findChild<QPushButton *>("prev_button")->setEnabled(false);
  ui->navigator->findChild<QPushButton *>("add_region_button")->setEnabled(false);
  ui->navigator->findChild<QPushButton *>("remove_region_button")->setEnabled(false);
  ui->navigator->findChild<QPushButton *>("next_with_copy_button")->setEnabled(false);
}

void MainWindow::enableControls()
{
  ui->Play->setEnabled(true);
  ui->videoSlider->setEnabled(true);
  ui->SlowDown->setEnabled(true);
  ui->SpeedUp->setEnabled(true);
  ui->minusOneFrame->setEnabled(true);
  ui->plusOneFrame->setEnabled(true);
  ui->removeFish->setEnabled(true);
  ui->goToFrame->setEnabled(true);
  ui->addRound->setEnabled(true);
  ui->addFlat->setEnabled(true);
  ui->addSkate->setEnabled(true);
  ui->addOther->setEnabled(true);
  ui->saveAnnotate->setEnabled(true);
  ui->removeFish->setEnabled(true);
  ui->goToFrame->setEnabled(true);
  ui->prevFish->setEnabled(true);
  ui->nextFish->setEnabled(true);
  ui->loadAnnotate->setEnabled(true);
  ui->minusOneSecond->setEnabled(true);
  ui->minusThreeSecond->setEnabled(true);
  ui->writeImage->setEnabled(true);
  ui->navigator->findChild<QPushButton *>("next_button")->setEnabled(true);
  ui->navigator->findChild<QPushButton *>("prev_button")->setEnabled(true);
  ui->navigator->findChild<QPushButton *>("add_region_button")->setEnabled(true);
  ui->navigator->findChild<QPushButton *>("remove_region_button")->setEnabled(true);
  ui->navigator->findChild<QPushButton *>("next_with_copy_button")->setEnabled(true);
}

#include "../../include/fish_detector/gui/moc_mainwindow.cpp"

}} // namespace fish_detector::gui
