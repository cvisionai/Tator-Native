#include <boost/filesystem.hpp>

#include <QFileDialog>
#include <QMessageBox>

#include "fish_detector/common/species_dialog.h"
#include "fish_detector/common/species_widget.h"
#include "fish_detector/image_annotator/mainwindow.h"
#include "ui_mainwindow.h"

namespace fish_detector { namespace image_annotator {

namespace fs = boost::filesystem;

namespace { //anonymous

static const std::vector<std::string> kDirExtensions = {
  ".jpg", ".png", ".bmp", ".tif", ".jpeg",
  ".JPG", ".PNG", ".BMP", ".TIF", ".JPEG"};

} // anonymous namespace

MainWindow::MainWindow(QWidget *parent)
  : annotations_(nullptr)
  , scene_(new QGraphicsScene)
  , pixmap_(nullptr)
  , ui_(new Ui::MainWidget)
  , species_widgets_inserted_(0)
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
}

void MainWindow::on_addSpecies_clicked() {
  SpeciesDialog *dlg = new SpeciesDialog(this);
  if(dlg->exec()) {
    Species species = dlg->getSpecies();
    if(!species.getName().empty()) {
      SpeciesWidget *widget = new SpeciesWidget(species, this);
      ui_->speciesLayout->insertWidget(species_widgets_inserted_, widget);
      ++species_widgets_inserted_;
    }
  }
  delete dlg;
}

void MainWindow::on_loadSpecies_clicked() {
}

void MainWindow::on_saveSpecies_clicked() {
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
}

void MainWindow::on_imageSlider_valueChanged() {
  if(pixmap_ != nullptr) {
    scene_->removeItem(pixmap_.get());
  }
  QString filename(image_files_[ui_->imageSlider->value()].c_str());
  QImage current(filename);
  if(!current.isNull()) {
    pixmap_.reset(scene_->addPixmap(QPixmap::fromImage(current)));
    scene_->setSceneRect(current.rect());
    ui_->imageWindow->setScene(scene_.get());
    ui_->imageWindow->fitInView(scene_->sceneRect(), Qt::KeepAspectRatio);
    ui_->imageWindow->show();
    ui_->fileNameValue->setText(filename);
  }
  else {
    QMessageBox err;
    err.critical(0, "Error", std::string(
        std::string("Error loading image ")
      + filename.toStdString()
      + std::string(".")).c_str());
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
    on_imageSlider_valueChanged();
  }
  else {
    QMessageBox err;
    err.critical(0, "Error", "No images found in this directory.");
  }
}

#include "../../include/fish_detector/image_annotator/moc_mainwindow.cpp"

}} // namespace fish_detector::image_annotator
