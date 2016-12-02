#include <boost/filesystem.hpp>

#include <QFileDialog>
#include <QMessageBox>

#include "fish_annotator/common/species_dialog.h"
#include "fish_annotator/image_annotator/image_annotation.h"
#include "fish_annotator/image_annotator/mainwindow.h"
#include "ui_mainwindow.h"

namespace fish_annotator { namespace image_annotator {

namespace fs = boost::filesystem;

namespace { //anonymous

static const std::vector<std::string> kDirExtensions = {
  ".jpg", ".png", ".bmp", ".tif", ".jpeg",
  ".JPG", ".PNG", ".BMP", ".TIF", ".JPEG"};

} // anonymous namespace

MainWindow::MainWindow(QWidget *parent)
  : annotations_(new ImageAnnotationList)
  , scene_(new QGraphicsScene)
  , ui_(new Ui::MainWidget)
  , species_controls_(new SpeciesControls(this))
  , image_files_() {
  ui_->setupUi(this);
  setStyleSheet("QPushButton { background-color: rgb(230, 230, 230);"
	  "border-style: outset; border-radius: 5px; border-width: 2px; "
    "border-color: grey; padding: 6px;}"
	  "QPushButton:pressed{background-color: rgb(190, 190, 190); "
    "border-style: outset; border-radius: 5px;"
	  "border-width: 2px; border-color: grey; padding: 6px;}");
  ui_->next->setEnabled(false);
  ui_->prev->setEnabled(false);
  ui_->saveAnnotations->setEnabled(false);
  ui_->imageSlider->setEnabled(false);
  ui_->sideBarLayout->addWidget(species_controls_.get());
  QObject::connect(species_controls_.get(), 
      SIGNAL(individualAdded(std::string, std::string)), 
      this, SLOT(addIndividual(std::string, std::string)));
}

void MainWindow::on_next_clicked() {
  int next_val = ui_->imageSlider->value() + 1;
  if(next_val <= ui_->imageSlider->maximum()) {
    ui_->imageSlider->setValue(next_val);
  }
}

void MainWindow::on_prev_clicked() {
  int prev_val = ui_->imageSlider->value() - 1;
  if(prev_val >= ui_->imageSlider->minimum()) {
    ui_->imageSlider->setValue(prev_val);
  }
}

void MainWindow::on_loadImageDir_clicked() {
  QString image_dir = QFileDialog::getExistingDirectory(this, 
    "Select an image directory.");
  if(!image_dir.isEmpty()) {
    onLoadDirectorySuccess(image_dir);
  }
}

void MainWindow::on_saveAnnotations_clicked() {
  if(image_files_.size() > 0) {
    annotations_->write(image_files_);
  }
}

void MainWindow::on_imageSlider_valueChanged() {
  scene_->clear();
  ui_->idSelection->clear();
  ui_->speciesValue->setText("");
  ui_->subspeciesValue->setText("");
  QString filename(image_files_[ui_->imageSlider->value()].c_str());
  QImage current(filename);
  if(!current.isNull()) {
    scene_->addPixmap(QPixmap::fromImage(current));
    scene_->setSceneRect(current.rect());
    ui_->imageWindow->setScene(scene_.get());
    ui_->imageWindow->fitInView(scene_->sceneRect(), Qt::KeepAspectRatio);
    ui_->imageWindow->show();
    ui_->fileNameValue->setText(filename);
    auto annotations = 
      annotations_->getImageAnnotations(filename.toStdString());
    for(auto annotation : annotations) {
      if(ui_->showAnnotations->isChecked()) {
          auto region = new AnnotatedRegion<ImageAnnotation>(
                annotation->id_, annotation, current.rect());
          scene_->addItem(region);
      }
      ui_->idSelection->addItem(QString::number(annotation->id_));
    }
    species_controls_->resetCounts();
    auto counts = annotations_->getCounts(filename.toStdString());
    for(auto it = counts.begin(); it != counts.end(); it++) {
      species_controls_->setCount(it->second, it->first);
    }
  }
  else {
    QMessageBox err;
    err.critical(0, "Error", std::string(
        std::string("Error loading image ")
      + filename.toStdString()
      + std::string(".")).c_str());
  }
}

void MainWindow::on_showAnnotations_stateChanged() {
  on_imageSlider_valueChanged();
}

void MainWindow::on_idSelection_currentIndexChanged(const QString &id) {
  if(image_files_.size() > 0 && ui_->imageSlider->isEnabled()) {
    std::string current_image = image_files_[ui_->imageSlider->value()];
    auto annotations = 
      annotations_->getImageAnnotations(current_image);
    for(auto annotation : annotations) {
      if(annotation->id_ == id.toInt()) {
        ui_->speciesValue->setText(annotation->species_.c_str());
        ui_->subspeciesValue->setText(annotation->subspecies_.c_str());
      }
    }
  }
}

void MainWindow::on_removeAnnotation_clicked() {
  if(image_files_.size() > 0 && ui_->imageSlider->isEnabled()) {
    std::string current_image = image_files_[ui_->imageSlider->value()];
    int id = ui_->idSelection->currentText().toInt();
    annotations_->remove(current_image, id);
    on_imageSlider_valueChanged();
  }
}

void MainWindow::addIndividual(std::string species, std::string subspecies) {
  if(image_files_.size() > 0 && ui_->imageSlider->isEnabled()) {
    std::string current_image = image_files_[ui_->imageSlider->value()];
    uint64_t id = annotations_->nextId(current_image);
    auto annotation = std::make_shared<ImageAnnotation>(
      current_image, species, subspecies, id, Rect(0, 0, 0, 0));
    annotations_->insert(annotation);
    on_imageSlider_valueChanged();
  }
}

void MainWindow::onLoadDirectorySuccess(const QString &image_dir) {
  image_files_.clear();
  fs::directory_iterator dir_it(image_dir.toStdString());
  fs::directory_iterator dir_end;
  for(; dir_it != dir_end; ++dir_it) {
    fs::path ext(dir_it->path().extension());
    for(auto &ok_ext : kDirExtensions) {
      if(ext == ok_ext) {
        image_files_.push_back(dir_it->path().generic_string());
      }
    }
  }
  if(image_files_.size() > 0) {
    ui_->next->setEnabled(true);
    ui_->prev->setEnabled(true);
    ui_->saveAnnotations->setEnabled(true);
    ui_->imageSlider->setEnabled(true);
    ui_->imageSlider->setMinimum(0);
    ui_->imageSlider->setMaximum(static_cast<int>(image_files_.size() - 1));
    ui_->imageSlider->setSingleStep(1);
    ui_->imageSlider->setValue(0);
    annotations_->read(image_files_);
    species_controls_->loadFromVector(annotations_->getAllSpecies());
    on_imageSlider_valueChanged();
  }
  else {
    QMessageBox err;
    err.critical(0, "Error", "No images found in this directory.");
  }
}

#include "../../include/fish_annotator/image_annotator/moc_mainwindow.cpp"

}} // namespace fish_annotator::image_annotator
