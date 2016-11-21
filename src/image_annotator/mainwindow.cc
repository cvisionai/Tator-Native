#include "fish_detector/image_annotator/mainwindow.h"
#include "ui_mainwindow.h"

namespace fish_detector { namespace image_annotator {

MainWindow::MainWindow(QWidget *parent)
  : annotations_(new ImageAnnotationList)
  , ui_(new Ui::MainWidget) {
  ui_->setupUi(this);
  setStyleSheet("QPushButton { background-color: rgb(230, 230, 230);"
	  "border-style: outset; border-radius: 5px; border-width: 2px; "
    "border-color: grey; padding: 6px;}"
	  "QPushButton:pressed{background-color: rgb(190, 190, 190); "
    "border-style: outset; border-radius: 5px;"
	  "border-width: 2px; border-color: grey; padding: 6px;}");
}

void MainWindow::on_addSpecies_clicked() {
}

void MainWindow::on_loadSpecies_clicked() {
}

void MainWindow::on_saveSpecies_clicked() {
}

void MainWindow::on_next_clicked() {
}

void MainWindow::on_prev_clicked() {
}

void MainWindow::on_loadImageDir_clicked() {
}

void MainWindow::on_saveAnnotations_clicked() {
}

void MainWindow::on_imageSlider_sliderMoved() {
}

#include "../../include/fish_detector/image_annotator/moc_mainwindow.cpp"

}} // namespace fish_detector::image_annotator
