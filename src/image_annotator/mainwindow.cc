#include "fish_detector/image_annotator/mainwindow.h"
#include "ui_mainwindow.h"

namespace fish_detector { namespace image_annotator {

MainWindow::MainWindow(QWidget *parent)
  : annotations_(new ImageAnnotationList)
  , ui_(new Ui::MainWidget) {
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
