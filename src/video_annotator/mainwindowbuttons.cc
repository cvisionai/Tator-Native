#include <QProgressDialog>
#include <QProgressBar>

#include "fish_detector/video_annotator/mainwindow.h"
#include "ui_mainwindow.h"

namespace fish_detector { namespace video_annotator {

namespace Ui {
  class MainWidget;
}

void MainWindow::on_addRound_clicked() {
  addFish((FishTypeEnum) ROUND);
}

void MainWindow::on_addFlat_clicked() {
  addFish((FishTypeEnum) FLAT);
}

void MainWindow::on_addSkate_clicked() {
  addFish((FishTypeEnum) SKATE);
}

void MainWindow::on_addOther_clicked() {
  addFish((FishTypeEnum) OTHER);
}

void MainWindow::on_loadAnnotate_clicked() {
  QString filename = QFileDialog::getOpenFileName(this,
    tr("Open Annotation File"), ".",
    tr("CSV Files (*.csv)"));
  std::string filenameBase = base_name(filename.toStdString());
  std::string filenameBaseNoExt = remove_extension(filenameBase);
  std::string filenameJSON = remove_extension(filename.toStdString()) + ".json";
  std::ifstream inputJSON(filenameJSON.c_str(), std::ios::in);

  QProgressBar * myBar = new QProgressBar(this);
  QProgressDialog * progress = gen_progress_dialog("Loading", myBar);

  if (!inputJSON.fail()) {
  progress->setValue(2);
  Document* newDoc = new Document;
  deserialize(*newDoc, inputJSON);
  document_.reset(newDoc);
  }
  progress->setValue(3);

  std::string filenameBaseNoReviewer = remove_reviewer(filenameBaseNoExt);
  QString qFilename = QString::fromStdString(filenameBaseNoReviewer);
  ui_->fileNameValue->setText(qFilename);
  std::ifstream inFile(filename.toLatin1().data());
  std::string line, tripID, reviewer, towType, fishNum, fishType, species;
  int frame, towNum, curID;
  double timeInVid;
  getline(inFile,line);
  line.clear();
  bool first = true;
  progress->setValue(6);
  while(getline(inFile,line)) {
    std::stringstream linestream(line);
    std::string tempToken;
    std::getline(linestream,tripID,',');
    std::getline(linestream,tempToken,',');
    std::string strtowNum = tempToken;
    std::stringstream tempConvert(tempToken);
    tempConvert >> towNum;
    tempToken.clear();
    tempConvert.str("");
    tempConvert.clear();
    std::getline(linestream,reviewer,',');
    std::getline(linestream,towType,',');
    std::getline(linestream,fishNum,',');
    std::getline(linestream,fishType,',');
    std::getline(linestream,species,',');
    std::getline(linestream,tempToken,',');
    if (first) {
      QString qreviewer = QString::fromStdString(reviewer);
      ui_->reviewerNameValue->setText(qreviewer);
      QString qtripID = QString::fromStdString(tripID);
      ui_->tripIDValue->setText(qtripID);
      QString qtowID = QString::fromStdString(strtowNum);
      ui_->towIDValue->setText(qtowID);
    }
    first = false;
    tempConvert << tempToken; tempConvert >> frame; tempToken.clear();
    tempConvert.str(""); tempConvert.clear();
    std::getline(linestream,tempToken,',');
    tempConvert << tempToken; tempConvert >> timeInVid; tempToken.clear();
    tempConvert.str(""); tempConvert.clear();
    FishTypeEnum fType = getFishType(fishType);
    curID = std::stoi(fishNum,nullptr,10);
    std::unique_ptr<Fish> tempFish(new Fish(fType,frame,curID));
    if (curID >= next_id_) next_id_ = curID + 1;
    tempFish->setFishSubType(getFishSpecies(fType,species));
    my_fish_list_.push_back(*tempFish);
    linestream.str(""); linestream.clear();
  }
  inFile.close();
  ui_->totalFishVal->setText(QString::number(my_fish_list_.size()));
  list_pos_ = my_fish_list_.begin();
  ui_->typeMenu->setCurrentIndex((int) list_pos_->getFishType());
  ui_->subTypeMenu->setCurrentIndex((int) list_pos_->getFishSubType());
  updateVecIndex();
  progress->setValue(10);
  progress->cancel();
  delete progress;
  delete myBar;
}

void MainWindow::on_saveAnnotate_clicked() {
  QString dir_name = QFileDialog::getExistingDirectory(this,tr("Choose save directory"));
  saveAnnotations(dir_name);
}

void MainWindow::saveAnnotations(const QString &dir_name) {
  QProgressBar * myBar = new QProgressBar(this);
  QProgressDialog * progress = gen_progress_dialog("Saving", myBar);
  std::string filename;
  std::string filenameJSON;
  filename = dir_name.toStdString() + "/" + filename + ui_->fileNameValue->text().toStdString() + "_" + ui_->reviewerNameValue->text().toStdString();
  filenameJSON = filename + ".json";
  filename = filename + ".csv";
  std::ofstream jsonFile (filenameJSON.c_str(), std::ofstream::out);
  progress->setValue(3);
  serialize(*document_, jsonFile);
  progress->setValue(5);
  std::ofstream outFile(filename);
  outFile << "Trip_ID" << "," << "Tow_Number" << "," << "Reviewer" << "," << "Tow_Type" << ",";
  outFile << "Fish_Number" << "," << "Fish_Type" << "," << "Species" << "," << "Frame" << "," << "Time_In_Video" << std::endl;
  std::string towStatus;
  if (ui_->towStatus->isChecked()) {
    towStatus = "Open";
  }
  else {
    towStatus = "Closed";
  }
  int fishCount = 1;
  progress->setValue(7);
  for(auto it = my_fish_list_.begin(); it != my_fish_list_.end(); ++it) {
    outFile << ui_->tripIDValue->text().toStdString() << "," << ui_->towIDValue->text().toStdString() << "," << ui_->reviewerNameValue->text().toStdString() << "," << towStatus << ",";
    outFile << it->getID() << "," << getFishTypeString(it->getFishType()) << ",";
    outFile << getFishSpeciesString(it->getFishType(),it->getFishSubType()) << ",";
    outFile << it->frameCounted << ",";
    outFile << (double) it->frameCounted / player_->getFrameRate() / 60.0 / 60.0 << std::endl;
    fishCount++;
  }
  outFile.close();
  progress->setValue(10);
  progress->cancel();
  delete progress;
  delete myBar;
}

void MainWindow::on_prevFish_clicked() {
  if (list_pos_!=my_fish_list_.begin()) {
    list_pos_ = list_pos_-1;
    ui_->typeMenu->setCurrentIndex((int) list_pos_->getFishType());
    ui_->subTypeMenu->setCurrentIndex((int) list_pos_->getFishSubType());
    updateVecIndex();
  }
}

void MainWindow::on_nextFish_clicked() {
    if (list_pos_!=my_fish_list_.end()-1) {
        list_pos_ = list_pos_+1;
        ui_->typeMenu->setCurrentIndex((int) list_pos_->getFishType());
        ui_->subTypeMenu->setCurrentIndex((int) list_pos_->getFishSubType());
        updateVecIndex();
    }
}

void MainWindow::on_removeFish_clicked() {
    /*Steps to removing fish:
     *
     * 1. Remove any regions from scene
     * 2. Remove all frame annotations with ID
     * 3. Remove annotation with ID
     * 4. Remove from fish list
     */
    if (my_fish_list_.begin() != my_fish_list_.end()) {
        auto id = list_pos_->getID();

        auto it = find_if(current_annotations_.begin(), current_annotations_.end(), \
                          [&id](AnnotatedRegion<AnnotationLocation>* obj) {return obj->getUID() == id;});
        if (it != current_annotations_.end()) {
            scene_->removeItem(*it);
            current_annotations_.erase(it);
        }
        document_->removeFrameAnnotation(id);
        document_->removeAnnotation(id);
        list_pos_ = my_fish_list_.erase(list_pos_);
        if (list_pos_ == my_fish_list_.end()) list_pos_ = my_fish_list_.end()-1;
        updateVecIndex();
        ui_->totalFishVal->setText(QString::number(my_fish_list_.size()));
        ui_->typeMenu->setCurrentIndex((int) list_pos_->getFishType());
        ui_->subTypeMenu->setCurrentIndex((int) list_pos_->getFishSubType());
    }
}

void MainWindow::on_writeImage_clicked() {
  // filename needs to be procedurally generated

  if (images_save_path_.isNull())
    images_save_path_ = QFileDialog::getExistingDirectory(this, tr("Choose save directory"));

  QImage img(scene_->sceneRect().size().toSize(), QImage::Format_ARGB32_Premultiplied);
  QPainter p(&img);
  scene_->render(&p);
  p.end();
  img.save(images_save_path_ + QStringLiteral("\\Fish_%1.png").arg(list_pos_->getID()));
}

QProgressDialog * MainWindow::gen_progress_dialog(QString dialog_text, QProgressBar * myBar) {
  myBar->setStyleSheet(progress_bar_stylesheet_);
  QProgressDialog * progress = new QProgressDialog(dialog_text, "Cancel", 0, 10, this, Qt::WindowTitleHint);
  progress->setBar(myBar);
  format_progress_dialog(*progress);

  return progress;
}

void MainWindow::format_progress_dialog(QProgressDialog &progress_dialog) {
  progress_dialog.setWindowModality(Qt::WindowModal);
  progress_dialog.setCancelButton(0);
  progress_dialog.setMinimumDuration(0);
  progress_dialog.show();
  progress_dialog.setValue(1);

}

}} // namespace fish_detector::video_annotator
