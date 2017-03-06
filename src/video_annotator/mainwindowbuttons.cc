#include <QProgressDialog>
#include <QProgressBar>

#include "fish_annotator/video_annotator/mainwindow.h"
#include "ui_mainwindow.h"

namespace fish_annotator { namespace video_annotator {

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

  QProgressDialog * progress = genProgressDialog("Loading");

  if (!inputJSON.fail()) {
  progress->setValue(20);
  Document* newDoc = new Document;
  deserialize(*newDoc, inputJSON);
  document_.reset(newDoc);
  }
  progress->setValue(30);

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
  progress->setValue(60);
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
  progress->setValue(100);
  progress->cancel();
  delete progress;
}

void MainWindow::on_saveAnnotate_clicked() {
  QString dir_name = QFileDialog::getExistingDirectory(this,tr("Choose save directory"));
  if(!dir_name.isEmpty() && !dir_name.isNull()) {
    saveAnnotations(dir_name);
  }
}

void MainWindow::saveAnnotations(const QString &dir_name) {
  QProgressDialog * progress = genProgressDialog("Saving");
  std::string basename = ui_->fileNameValue->text().toStdString()
    + "_" + ui_->reviewerNameValue->text().toStdString();
  std::string path_csv = QDir(dir_name).filePath(QString::fromUtf8(
    std::string(basename + ".csv").c_str())).toStdString();
  std::string path_json = QDir(dir_name).filePath(QString::fromUtf8(
    std::string(basename + ".json").c_str())).toStdString();
  std::ofstream json_file(path_json.c_str(), std::ofstream::out);
  if(json_file.fail()) {
    QMessageBox err;
    err.critical( 0, "Error", std::string(
        std::string( "Failed to open file " )
      + path_json
      + std::string( "!" )
      ).c_str() );
  }
  else {
    progress->setValue(30);
    serialize(*document_, json_file);
    progress->setValue(50);
  }
  std::ofstream csv_file(path_csv);
  if(csv_file.fail()) {
    QMessageBox err;
    err.critical( 0, "Error", std::string(
        std::string( "Failed to open file " )
      + path_csv
      + std::string( "!" )
      ).c_str() );
  }
  else {
    csv_file << "Trip_ID" << "," << "Tow_Number" << "," << "Reviewer"
      << "," << "Tow_Type" << "," << "Fish_Number" << "," << "Fish_Type"
      << "," << "Species" << "," << "Frame" << "," << "Time_In_Video"
      << std::endl;
    std::string towStatus;
    if (ui_->towStatus->isChecked()) {
      towStatus = "Open";
    }
    else {
      towStatus = "Closed";
    }
    int fishCount = 1;
    progress->setValue(70);
    for(auto it = my_fish_list_.begin(); it != my_fish_list_.end(); ++it) {
      csv_file << ui_->tripIDValue->text().toStdString() << ","
        << ui_->towIDValue->text().toStdString() << ","
        << ui_->reviewerNameValue->text().toStdString() << "," << towStatus
        << "," << it->getID() << "," << getFishTypeString(it->getFishType())
        << "," << getFishSpeciesString(it->getFishType(),it->getFishSubType())
        << "," << it->frameCounted << ","
        << (double) it->frameCounted / player_->getFrameRate() / 60.0 / 60.0
        << std::endl;
      fishCount++;
    }
    csv_file.close();
  }
  progress->setValue(100);
  progress->cancel();
  delete progress;
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

void MainWindow::writeFrameWithAnnotations(QString filename) {
	QPixmap tempPixmap = display_image_->pixmap();
	QImage tempImage = tempPixmap.toImage();
	auto frame = uint64_t(player_->getCurrentFrame());
	tempImage.save(filename + "_" + std::to_string(frame).c_str() + ".png");
	//player_->write_image(filename + ".png");
	std::ofstream annotation_file(filename.toStdString() + "_" + std::to_string(frame).c_str() + ".txt");
//	std::ofstream label_file(filename.toStdString() + "_" + std::to_string(frame).c_str() + ".bboxes.labels.tsv");
	// write the annotations out now.
	std::string tmp_fish_type;
	for (auto ann : current_annotations_) {
		QRectF tmpRect = ann->getAnnBox();
		auto tmp_fish = my_fish_list_.begin()+ann->getUID()-1;
		switch (tmp_fish->getFishType()) {
			case ROUND:
				tmp_fish_type.assign("Car");
				break;
			case FLAT:
				tmp_fish_type.assign("Van");
				break;
			case SKATE:
				tmp_fish_type.assign("Truck");
				break;
			case OTHER:
				tmp_fish_type.assign("Misc");
				break;
			case DNC:
				tmp_fish_type.assign("DontCare");
				break;
			default:
				tmp_fish_type.assign("DontCare");
				break;
		}

		annotation_file << tmp_fish_type << " " << "0 0 0 " << (int)tmpRect.x() << " " << (int)tmpRect.y() << " " << (int)(tmpRect.x() + tmpRect.width()) << " " << (int)(tmpRect.y() + tmpRect.height()) << " 0 0 0 0 0 0 0" << std::endl;
		//annotation_file << (int)tmpRect.x() << "\t" << (int)tmpRect.y() << "\t" << (int)(tmpRect.x() + tmpRect.width()) << "\t" << (int)(tmpRect.y() + tmpRect.height()) << std::endl;
		//label_file << "flat" << std::endl;
	}
	annotation_file.close();
}

void MainWindow::on_writeImage_clicked() {
  // filename needs to be procedurally generated. 

  if (images_save_path_.isEmpty())
    images_save_path_ = QFileDialog::getExistingDirectory(this, tr("Choose save directory"));

  writeFrameWithAnnotations(images_save_path_ + QStringLiteral("/") + QStringLiteral("/%1").arg(list_pos_->getID()));
/*
  QImage img(scene_->sceneRect().size().toSize(), QImage::Format_ARGB32_Premultiplied);
  QPainter p(&img);
  scene_->render(&p);
  p.end();
  img.save(images_save_path_ + QStringLiteral("/") + QStringLiteral("/Fish_%1.png").arg(list_pos_->getID()));
*/
}

QProgressDialog * MainWindow::genProgressDialog(QString dialog_text) {
  QProgressBar * myBar = new QProgressBar(this);
  myBar->setStyleSheet(progress_bar_stylesheet_);
  QProgressDialog * progress = new QProgressDialog(
      dialog_text, "Cancel", 0, 100, this, Qt::WindowTitleHint);
  // progress owns myBar and will delete it.
  progress->setBar(myBar);
  progress->setWindowModality(Qt::WindowModal);
  progress->setCancelButton(0);
  progress->setMinimumDuration(0);
  progress->show();
  progress->setValue(1);
  return progress;
}

}} // namespace fish_annotator::video_annotator
