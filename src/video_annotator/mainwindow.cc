#include "fish_annotator/video_annotator/mainwindow.h"
#include "ui_mainwindow.h"

namespace fish_annotator { namespace video_annotator {

MainWindow::MainWindow(QWidget *parent)
  : QWidget(parent)
  , document_(new Document)
  , ui_(new Ui::MainWidget)
  , navigator_widget_(new NavigatorWidget(this))
  , player_(new Player)
  , my_fish_list_()
  , list_pos_(my_fish_list_.end())
  , scene_(new Scene(this))
  , f_index_(0)
  , next_id_(1)
  , display_image_(nullptr)
  , current_annotations_()
  , images_save_path_("")
  , progress_bar_stylesheet_("QProgressBar {"
	  "border: 2px solid grey; border-radius: 5px; text-align: center; }"
	  "QProgressBar::chunk{ background-color: #05B8CC; width: 20px;}")
{
  QObject::connect(player_.get(), SIGNAL(processedImage(QImage)),
           this, SLOT(updatePlayerUI(QImage)));
  QObject::connect(scene_.get(), SIGNAL(lineFinished(QLineF)),
	  this, SLOT(addRegionSlot(QLineF)));
  ui_->setupUi(this);
  ui_->navigatorLayout->addWidget(navigator_widget_.get());
#ifdef _WIN32
  setWindowIcon(QIcon(":/icons/FishAnnotator.ico"));
#endif
  disableControls();
  updateTypeMenu();
  QObject::connect(ui_->typeMenu, SIGNAL(currentIndexChanged(int)),
           this, SLOT(updateSubTypeMenu(int)));
  ui_->typeMenu->setCurrentIndex(3);
  QObject::connect(ui_->goToFishVal,SIGNAL(returnPressed()),
           this, SLOT(goToFish()));
  setStyleSheet("QPushButton { background-color: rgb(230, 230, 230);"
	  "border-style: outset; border-radius: 5px; border-width: 2px; border-color: grey; padding: 6px;}"
	  "QPushButton:pressed{background-color: rgb(190, 190, 190); border-style: outset; border-radius: 5px;"
	  "border-width: 2px; border-color: grey; padding: 6px;}");

  auto next_button = navigator_widget_->findChild<QPushButton *>("next_button");
  connect(next_button, SIGNAL(clicked()), this, SLOT(on_plusOneFrame_clicked()));

  auto prev_button = navigator_widget_->findChild<QPushButton *>("prev_button");
  connect(prev_button, SIGNAL(clicked()), this, SLOT(on_minusOneFrame_clicked()));

  auto add_region_button = navigator_widget_->findChild<QPushButton *>("add_region_button");
  connect(add_region_button, SIGNAL(clicked()), this, SLOT(on_addRegion_clicked()));

  auto remove_region_button = navigator_widget_->findChild<QPushButton *>("remove_region_button");
  connect(remove_region_button, SIGNAL(clicked()), this, SLOT(on_removeRegion_clicked()));

  auto next_and_copy_button = navigator_widget_->findChild<QPushButton *>("next_with_copy_button");
  connect(next_and_copy_button, SIGNAL(clicked()), this, SLOT(on_nextAndCopy_clicked()));
}

void MainWindow::nextFrame() {
  updateImage(player_->nextFrame()); // get next frame
  processAnnotations(player_->getCurrentFrame());
}

void MainWindow::prevFrame() {
  updateImage(player_->prevFrame()); // get next frame
  processAnnotations(player_->getCurrentFrame());
}

void MainWindow::processAnnotations(uint64_t frame) {
  // remove old annotations
  for (auto ann : current_annotations_) {
    scene_->removeItem(ann);
  }
  current_annotations_.clear();
  // add new annotations
  for (auto ann : document_->getAnnotations(frame)) {
	auto region = new LineAnnotation<AnnotationLocation>(ann.first, ann.second);
    scene_->addItem(region);
    current_annotations_.push_back(region);
  }
}

void MainWindow::updatePlayerUI(QImage img) {
  if (!img.isNull())
  {
    display_image_->setPixmap(QPixmap::fromImage(img));
    scene_->setSceneRect(img.rect());
    ui_->videoWindow->fitInView(scene_->sceneRect(),Qt::KeepAspectRatio);
    ui_->videoSlider->setValue(player_->getCurrentFrame());
    ui_->currentTime->setText(getFormattedTime((int)player_->
    getCurrentFrame() / (int)player_->getFrameRate()));
    processAnnotations(player_->getCurrentFrame());
  }
}

void MainWindow::goToFish() {
  int fNumber = ui_->goToFishVal->text().toInt();
  if ((fNumber > 0) && (fNumber < int(my_fish_list_.size())))
  {
    list_pos_ = my_fish_list_.begin()+fNumber-1;
    ui_->typeMenu->setCurrentIndex((int) list_pos_->getFishType());
    ui_->subTypeMenu->setCurrentIndex((int) list_pos_->getFishSubType());
    updateVecIndex();
  }
  else
  {
    list_pos_ = my_fish_list_.end()-1;
    ui_->typeMenu->setCurrentIndex((int) list_pos_->getFishType());
    ui_->subTypeMenu->setCurrentIndex((int) list_pos_->getFishSubType());
    updateVecIndex();
  }
}

QString MainWindow::getFormattedTime(int timeInSeconds) {
  int seconds = (int) (timeInSeconds) % 60;
  int minutes = (int)((timeInSeconds / 60) % 60);
  int hours = (int)((timeInSeconds / (60 * 60)) % 24);
  QTime t(hours, minutes, seconds);
  if (hours == 0)
  return t.toString("mm:ss");
  else
  return t.toString("h:mm:ss");
}

void MainWindow::on_LoadVideo_clicked() {
  QString filename = QFileDialog::getOpenFileName(this,
  tr("Open Video"), ".",
  tr("Video Files (*.avi *.mpg *.mp4 *.mkv)"));
  QFileInfo name = filename;
  if (!filename.isEmpty()) {
    if (!player_->loadVideo(filename.toLatin1().data())) {
      QMessageBox msgBox;
      msgBox.setText("The selected video could not be opened!");
      msgBox.exec();
    }
    else {
      onLoadVideoSuccess(name);
    }
  }
}

void MainWindow::onLoadVideoSuccess(const QFileInfo &name) {
  enableControls();
  this->setWindowTitle(name.fileName());
  ui_->Play->setEnabled(true);
  ui_->videoSlider->setEnabled(true);
  ui_->videoSlider->setMaximum(player_->getNumberOfFrames());
  ui_->totalTime->setText(getFormattedTime((int)player_->
  getNumberOfFrames() / (int)player_->getFrameRate()));
  QImage firstImage = player_->getOneFrame();
  //scene_.reset(new Scene(this));
  display_image_ = scene_->addPixmap(QPixmap::fromImage(firstImage));
  scene_->setSceneRect(firstImage.rect());
  ui_->videoWindow->setScene(scene_.get());
  ui_->videoWindow->fitInView(scene_->sceneRect(),Qt::KeepAspectRatio);
  ui_->videoWindow->show();
  ui_->currentSpeed->setText("Current Speed: 100%");
  ui_->Play->setFocus();
  // TODO: should first try to load this if the data
  // file exists
  document_.reset(new Document());
}

void MainWindow::on_Play_clicked() {
  if (player_->isStopped())
  {
    player_->Play();
    ui_->Play->setText(tr("Stop"));
  }
  else
  {
    player_->Stop();
    ui_->Play->setText(tr("Play"));
  }
}

void MainWindow::on_videoSlider_sliderPressed() {
  player_->Stop();
}

void MainWindow::on_videoSlider_sliderReleased() {
  player_->Play();
  ui_->Play->setText(tr("Stop"));
}

void MainWindow::on_videoSlider_sliderMoved(int position) {
  player_->setFrame(position);
  ui_->currentTime->setText(getFormattedTime((int)(position / player_->getFrameRate())));
}

void MainWindow::on_SpeedUp_clicked() {
  player_->speedUp();
  QString tempSpeed;
  tempSpeed.setNum((int)(player_->getCurrentSpeed()));
  ui_->currentSpeed->setText("Current Speed: " + tempSpeed + "%");
}

void MainWindow::on_SlowDown_clicked() {
  player_->slowDown();
  QString tempSpeed;
  tempSpeed.setNum((int)(player_->getCurrentSpeed()));
  ui_->currentSpeed->setText("Current Speed: " + tempSpeed + "%");
}

void MainWindow::on_minusOneSecond_clicked()
{
	skipVideo(-1);
}

void MainWindow::on_minusThreeSecond_clicked() {
	skipVideo(-3);
}

void MainWindow::skipVideo(int seconds_to_skip) {
	if (!(player_ == nullptr))
	{
		if (!player_->isStopped())
		{
			player_->Stop();
			ui_->Play->setText(tr("Play"));
		}

		bool valid_rewind = false;
		if (seconds_to_skip > 0) {
			valid_rewind = (player_->getCurrentFrame()
				+ seconds_to_skip * player_->getFrameRate()) < 
				player_->getNumberOfFrames();
		}
		else {
			valid_rewind = player_->getCurrentFrame() >
				std::abs(seconds_to_skip) * player_->getFrameRate();
		}
		QImage image = player_->setFrame(player_->getCurrentFrame()
			+ valid_rewind * seconds_to_skip * player_->getFrameRate());

		if (!image.isNull())
		{
			display_image_->setPixmap(QPixmap::fromImage(image));
			scene_->setSceneRect(image.rect());
			ui_->videoWindow->fitInView(scene_->sceneRect(), Qt::KeepAspectRatio);
			ui_->videoSlider->setValue(player_->getCurrentFrame());
			ui_->currentTime->setText(getFormattedTime((int)player_->
				getCurrentFrame() / (int)player_->getFrameRate()));
		}
		processAnnotations(player_->getCurrentFrame());
	}
}

void MainWindow::updateImage(const QImage &image)
{
  if (!player_->isStopped())
  {
  player_->Stop();
  ui_->Play->setText(tr("Play"));
  }

  display_image_->setPixmap(QPixmap::fromImage(image));
  scene_->setSceneRect(image.rect());
  ui_->videoWindow->fitInView(scene_->sceneRect(), Qt::KeepAspectRatio);
  ui_->videoSlider->setValue(player_->getCurrentFrame());
  ui_->currentTime->setText(getFormattedTime((int)player_->
    getCurrentFrame() / (int)player_->getFrameRate()));
}

void MainWindow::on_minusOneFrame_clicked()
{
  if (!player_->isStopped())
  {
    player_->Stop();
    ui_->Play->setText(tr("Play"));
  }
  prevFrame();
}

void MainWindow::on_plusOneFrame_clicked()
{
  if (!player_->isStopped())
  {
    player_->Stop();
    ui_->Play->setText(tr("Play"));
  }
  nextFrame();
}

void MainWindow::on_goToFrame_clicked()
{
  if (!(player_==nullptr))
  {
    if (!player_->isStopped())
    {
      player_->Stop();
      ui_->Play->setText(tr("Play"));
    }
    QImage image = player_->setFrame(list_pos_->getFrameCounted());

    if (!image.isNull())
    {
      display_image_->setPixmap(QPixmap::fromImage(image));
      scene_->setSceneRect(image.rect());
      ui_->videoWindow->fitInView(scene_->sceneRect(),Qt::KeepAspectRatio);
      ui_->videoSlider->setValue(player_->getCurrentFrame());
      ui_->currentTime->setText(getFormattedTime((int)player_->
        getCurrentFrame() / (int)player_->getFrameRate()));
    }
    processAnnotations(player_->getCurrentFrame());
  }
}

void MainWindow::on_typeMenu_currentIndexChanged(int tIdx)
{
  if (!my_fish_list_.empty())
    list_pos_->setFishType((FishTypeEnum) tIdx);
}

void MainWindow::on_subTypeMenu_currentIndexChanged(int sIdx)
{
  if (!my_fish_list_.empty())
    list_pos_->setFishSubType(sIdx);
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
    ui_->addFlat->animateClick();
    ui_->Play->setFocus();
    break;
  case 1:
    ui_->addRound->animateClick();
    ui_->Play->setFocus();
    break;
  case 2:
    ui_->addSkate->animateClick();
    ui_->Play->setFocus();
    break;
  case 3:
    ui_->addOther->animateClick();
    ui_->Play->setFocus();
    break;
  case 4:
    navigator_widget_->findChild<QPushButton *>("next_with_copy_button")->animateClick();
    ui_->Play->setFocus();
    break;
  case 5:
    navigator_widget_->findChild<QPushButton *>("prev_button")->animateClick();
    ui_->Play->setFocus();
    break;
  case 6:
    navigator_widget_->findChild<QPushButton *>("next_button")->animateClick();
    ui_->Play->setFocus();
    break;
  case 7:
    navigator_widget_->findChild<QPushButton *>("add_region_button")->animateClick();
    ui_->Play->setFocus();
    break;
  case 8:
    navigator_widget_->findChild<QPushButton *>("remove_region_button")->animateClick();
    ui_->Play->setFocus();
    break;
  case 9:
    ui_->Play->animateClick();
    break;
  case 10:
    QWidget::keyPressEvent(e);
    break;
  }
}

void MainWindow::on_addRegion_clicked() {
	scene_->setMode(Scene::Mode::DrawLine);
}

void MainWindow::addRegionSlot(QLineF line_to_add) {
	addRegion(line_to_add);
}

bool MainWindow::addRegion(QLineF line_to_add) {
  if(list_pos_ != my_fish_list_.end()) {
    Rect area((uint64_t) line_to_add.x1(), (uint64_t) line_to_add.y1(),
      (uint64_t)line_to_add.x2(), (uint64_t)line_to_add.y2());
    auto fishID = uint64_t(list_pos_->getID());
    auto frame = uint64_t(player_->getCurrentFrame());
    //First check to see if there's an annotation for this ID already.
    if (document_->keyExists(fishID))
    { 
      removeRegion(fishID, frame);
    }
    else {
      document_->addAnnotation(fishID);
    }
    auto loc = document_->addAnnotationLocation(fishID, frame, area);
	  auto annotation_area = new LineAnnotation<AnnotationLocation>(fishID, loc);
	  current_annotations_.push_back(annotation_area);
	  annotation_area->setPen(QPen(Qt::black, 7, Qt::SolidLine));
	  scene_->addItem(annotation_area);
	  return true;
  }
  return false;
}

void MainWindow::on_removeRegion_clicked()
{
  if(list_pos_ != my_fish_list_.end()) {
    auto fishID = uint64_t(list_pos_->getID());
    auto frame = uint64_t(player_->getCurrentFrame());
    removeRegion(fishID, frame);
  }
}

void MainWindow::removeRegion(uint64_t id, uint64_t frame) {
  //First check to see if there's an annotation for this ID already.
  if (document_->keyExists(id))
  {
    //check to see if there's an annotation location for this frame already
    auto currentAnn = document_->getAnnotation(id);
    if (currentAnn->frameHasAnn(frame)) {
      currentAnn->removeFrameAnn(frame);
      document_->removeFrameAnnotation(id, frame);
      auto it = find_if(current_annotations_.begin(), current_annotations_.end(), \
                [&id](LineAnnotation<AnnotationLocation>* obj) {return obj->getUID() == id;});
      if (it != current_annotations_.end()) {
        scene_->removeItem(*it);
        current_annotations_.erase(it);
      }
    }
  }
  //This function will do nothing if there's no annotation to erase
}

void MainWindow::on_nextAndCopy_clicked()
{
  // remove old annotations
  for (auto ann : current_annotations_) {
    scene_->removeItem(ann);
  }
  current_annotations_.clear();
  auto prevFrame = player_->getCurrentFrame();
  updateImage(player_->nextFrame());
  for (auto ann: document_->getAnnotations()) {
    auto id = ann.first;
    if(list_pos_ != my_fish_list_.end()) {
      if(list_pos_->getID() == id) {
        removeRegion(id, prevFrame+1);
        break;
      }
    }
  }
  // copy annotation
  for (auto ann : document_->getAnnotations(prevFrame)) {
    auto id = ann.first;
    if(list_pos_ != my_fish_list_.end()) {
      if(list_pos_->getID() == id) {
        auto frame = ann.second->frame+1;
        auto area = ann.second->area_;
        auto loc = document_->addAnnotationLocation(ann.first,frame, area);
        break;
      }
    }
  }
  processAnnotations(player_->getCurrentFrame());
}

void MainWindow::addFish(FishTypeEnum fType)
{
  player_->Stop();
  ui_->Play->setText(tr("Play"));
  double currentFrame = player_->getCurrentFrame();
  int id;
  if (my_fish_list_.size() == 0) {
    id = 1;
    next_id_ = 2;
  }
  else {
    id = next_id_;
    next_id_++;

  }
  std::unique_ptr<Fish> tempFish(new Fish(fType,currentFrame,id));
  tempFish->setFishSubType(0);

  if (my_fish_list_.size() == 0) {
    my_fish_list_.push_back(*tempFish);
    list_pos_ = my_fish_list_.end()-1;
  }
  else {
    std::vector<Fish>::iterator tempLoc = my_fish_list_.begin();
    int tempIndex = 0;
    while(tempLoc->getFrameCounted() < currentFrame) {
      tempLoc++;
      if(tempLoc==my_fish_list_.end()) break;
      tempIndex++;
    }
    if (tempLoc == my_fish_list_.end()) {
      my_fish_list_.push_back(*tempFish);
      list_pos_ = my_fish_list_.end()-1;
    }
    else {
      if (tempLoc == my_fish_list_.begin()) {
        my_fish_list_.insert(tempLoc, *tempFish);
        list_pos_ = my_fish_list_.begin();
      }
      else {
        my_fish_list_.insert(my_fish_list_.begin()+tempIndex, *tempFish);
        list_pos_ = my_fish_list_.begin()+tempIndex;
      }
    }
  }
  ui_->fishNumVal->setText(QString::number(id));
  ui_->frameCountedVal->setText(QString::number(currentFrame));
  ui_->totalFishVal->setText(QString::number(my_fish_list_.size()));
  ui_->typeMenu->setCurrentIndex((int) fType);
  ui_->subTypeMenu->setCurrentIndex((int) 0);
}

void MainWindow::updateVecIndex()
{
  if(list_pos_ != my_fish_list_.end()) {
    ui_->fishNumVal->setText(QString::number(list_pos_->getID()));
    ui_->frameCountedVal->setText(QString::number(list_pos_->getFrameCounted()));
  }
}

void MainWindow::disableControls()
{
  ui_->Play->setEnabled(false);
  ui_->videoSlider->setEnabled(false);
  ui_->SlowDown->setEnabled(false);
  ui_->SpeedUp->setEnabled(false);
  ui_->minusOneFrame->setEnabled(false);
  ui_->plusOneFrame->setEnabled(false);
  ui_->removeFish->setEnabled(false);
  ui_->goToFrame->setEnabled(false);
  ui_->addRound->setEnabled(false);
  ui_->addFlat->setEnabled(false);
  ui_->addSkate->setEnabled(false);
  ui_->addOther->setEnabled(false);
  ui_->saveAnnotate->setEnabled(false);
  ui_->removeFish->setEnabled(false);
  ui_->goToFrame->setEnabled(false);
  ui_->prevFish->setEnabled(false);
  ui_->nextFish->setEnabled(false);
  ui_->loadAnnotate->setEnabled(false);
  ui_->minusOneSecond->setEnabled(false);
  ui_->minusThreeSecond->setEnabled(false);
  ui_->writeImage->setEnabled(false);
  navigator_widget_->findChild<QPushButton *>("next_button")->setEnabled(false);
  navigator_widget_->findChild<QPushButton *>("prev_button")->setEnabled(false);
  navigator_widget_->findChild<QPushButton *>("add_region_button")->setEnabled(false);
  navigator_widget_->findChild<QPushButton *>("remove_region_button")->setEnabled(false);
  navigator_widget_->findChild<QPushButton *>("next_with_copy_button")->setEnabled(false);
}

void MainWindow::enableControls()
{
  ui_->Play->setEnabled(true);
  ui_->videoSlider->setEnabled(true);
  ui_->SlowDown->setEnabled(true);
  ui_->SpeedUp->setEnabled(true);
  ui_->minusOneFrame->setEnabled(true);
  ui_->plusOneFrame->setEnabled(true);
  ui_->removeFish->setEnabled(true);
  ui_->goToFrame->setEnabled(true);
  ui_->addRound->setEnabled(true);
  ui_->addFlat->setEnabled(true);
  ui_->addSkate->setEnabled(true);
  ui_->addOther->setEnabled(true);
  ui_->saveAnnotate->setEnabled(true);
  ui_->removeFish->setEnabled(true);
  ui_->goToFrame->setEnabled(true);
  ui_->prevFish->setEnabled(true);
  ui_->nextFish->setEnabled(true);
  ui_->loadAnnotate->setEnabled(true);
  ui_->minusOneSecond->setEnabled(true);
  ui_->minusThreeSecond->setEnabled(true);
  ui_->writeImage->setEnabled(true);
  navigator_widget_->findChild<QPushButton *>("next_button")->setEnabled(true);
  navigator_widget_->findChild<QPushButton *>("prev_button")->setEnabled(true);
  navigator_widget_->findChild<QPushButton *>("add_region_button")->setEnabled(true);
  navigator_widget_->findChild<QPushButton *>("remove_region_button")->setEnabled(true);
  navigator_widget_->findChild<QPushButton *>("next_with_copy_button")->setEnabled(true);
}

#include "../../include/fish_annotator/video_annotator/moc_mainwindow.cpp"

}} // namespace fish_annotator::video_annotator
