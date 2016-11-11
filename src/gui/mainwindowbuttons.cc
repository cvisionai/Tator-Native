#include <QProgressDialog>

#include "fish_detector/gui/mainwindow.h"
#include "ui_mainwindow.h"

namespace fish_detector { namespace gui {

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

	QProgressDialog progress("Loading","Cancel",0,10,this, Qt::WindowTitleHint);
	progress.setWindowModality(Qt::WindowModal);
	progress.setCancelButton(0);
	progress.setMinimumDuration(0);
	progress.show();
	progress.setValue(1);

    if (!inputJSON.fail()) {
		progress.setValue(2);
        Document* newDoc = new Document(deserialize<Document>(inputJSON));
        document.reset(newDoc);
    }
	progress.setValue(3);

    std::string filenameBaseNoReviewer = remove_reviewer(filenameBaseNoExt);
    QString qFilename = QString::fromStdString(filenameBaseNoReviewer);
    ui->fileNameValue->setText(qFilename);
    std::ifstream inFile(filename.toLatin1().data());
    std::string line, tripID, reviewer, towType, fishNum, fishType, species;
    int frame, towNum, curID;
    double timeInVid;
    getline(inFile,line);
    line.clear();
    bool first = true;
	progress.setValue(6);
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
            ui->reviewerNameValue->setText(qreviewer);
            QString qtripID = QString::fromStdString(tripID);
            ui->tripIDValue->setText(qtripID);
            QString qtowID = QString::fromStdString(strtowNum);
            ui->towIDValue->setText(qtowID);
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
        if (curID >= nextID) nextID = curID + 1;
        tempFish->setFishSubType(getFishSpecies(fType,species));
        myFishList.push_back(*tempFish);
        linestream.str(""); linestream.clear();
    }
    inFile.close();
    ui->totalFishVal->setText(QString::number(myFishList.size()));
    listPos = myFishList.begin();
    ui->typeMenu->setCurrentIndex((int) listPos->getFishType());
    ui->subTypeMenu->setCurrentIndex((int) listPos->getFishSubType());
    updateVecIndex();
	progress.setValue(10);
	progress.cancel();
}

void MainWindow::on_saveAnnotate_clicked() {

    QString dirName = QFileDialog::getExistingDirectory(this,tr("Choose save directory"));

	QProgressDialog progress("Saving", "Cancel", 0, 10, this, Qt::WindowTitleHint);
	progress.setWindowModality(Qt::WindowModal);
	progress.setCancelButton(0);
	progress.setMinimumDuration(0);
	progress.show();
	progress.setValue(1);

    std::string filename;
    std::string filenameJSON;
    filename = dirName.toStdString() + "/" + filename + ui->fileNameValue->text().toStdString() + "_" + ui->reviewerNameValue->text().toStdString();
    filenameJSON = filename + ".json";
    filename = filename + ".csv";
    std::ofstream jsonFile (filenameJSON.c_str(), std::ofstream::out);
	progress.setValue(3);
	serialize(*document, jsonFile);
	progress.setValue(5);
    std::ofstream outFile(filename);
    outFile << "Trip_ID" << "," << "Tow_Number" << "," << "Reviewer" << "," << "Tow_Type" << ",";
    outFile << "Fish_Number" << "," << "Fish_Type" << "," << "Species" << "," << "Frame" << "," << "Time_In_Video" << std::endl;
    std::string towStatus;
    if (ui->towStatus->isChecked()) {
        towStatus = "Open";
    }
    else {
        towStatus = "Closed";
    }
    int fishCount = 1;
	progress.setValue(7);
    for(auto it = myFishList.begin(); it != myFishList.end(); ++it) {
        outFile << ui->tripIDValue->text().toStdString() << "," << ui->towIDValue->text().toStdString() << "," << ui->reviewerNameValue->text().toStdString() << "," << towStatus << ",";
        outFile << it->getID() << "," << getFishTypeString(it->getFishType()) << ",";
        outFile << getFishSpeciesString(it->getFishType(),it->getFishSubType()) << ",";
        outFile << it->frameCounted << ",";
        outFile << (double) it->frameCounted / player->getFrameRate() / 60.0 / 60.0 << std::endl;
        fishCount++;
    }
    outFile.close();
	progress.setValue(10);
	progress.cancel();
}

void MainWindow::on_prevFish_clicked() {
    if (listPos!=myFishList.begin()) {
        listPos = listPos-1;
        ui->typeMenu->setCurrentIndex((int) listPos->getFishType());
        ui->subTypeMenu->setCurrentIndex((int) listPos->getFishSubType());
        updateVecIndex();
    }
}

void MainWindow::on_nextFish_clicked() {
    if (listPos!=myFishList.end()-1) {
        listPos = listPos+1;
        ui->typeMenu->setCurrentIndex((int) listPos->getFishType());
        ui->subTypeMenu->setCurrentIndex((int) listPos->getFishSubType());
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
    if (myFishList.begin() != myFishList.end()) {
        auto id = listPos->getID();

        auto it = find_if(currentAnnotations.begin(), currentAnnotations.end(), \
                          [&id](AnnotatedRegion* obj) {return obj->getUID() == id;});
        if (it != currentAnnotations.end()) {
            scene->removeItem(*it);
            currentAnnotations.erase(it);
        }
        document->removeFrameAnnotation(id);
        document->removeAnnotation(id);
        listPos = myFishList.erase(listPos);
        if (listPos == myFishList.end()) listPos = myFishList.end()-1;
        updateVecIndex();
        ui->totalFishVal->setText(QString::number(myFishList.size()));
        ui->typeMenu->setCurrentIndex((int) listPos->getFishType());
        ui->subTypeMenu->setCurrentIndex((int) listPos->getFishSubType());
    }
}

void MainWindow::on_writeImage_clicked() {
	// filename needs to be procedurally generated

	if (images_save_path_.isNull())
		images_save_path_ = QFileDialog::getExistingDirectory(this, tr("Choose save directory"));
	//QString filename = "D:\\Projects\\FishDetector\\testimage.jpg";
	player->write_image(images_save_path_ + QStringLiteral("\\Fish_%1.jpg").arg(listPos->getID()));

	/*
	Now to write out each annotation that exists in the frame. Use currentannotations

	for (auto ann : currentannotations) {

	// Retrieve bounding box info for each annotation, and write it to file. Question about writing
	// sequentially or generating a string and writing all at once.

	}
	*/
}

}} // namespace fish_detector::gui
