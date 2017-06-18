#include "fish_annotator/db_uploader/mainwindow.h"

#include "ui_mainwindow.h"

namespace fish_annotator { namespace db_uploader {

MainWindow::MainWindow(QWidget *parent)
  : ui_(new Ui::MainWindow) {
  ui_->setupUi(this);
#ifdef _WIN32
  setWindowIcon(QIcon(":/icons/cvision/cvision_no_text.ico"));
#endif
}

void MainWindow::on_connectInputDb_clicked() {
}

void MainWindow::on_connectOutputDb_clicked() {
}

void MainWindow::on_browseImageDir_clicked() {
}

void MainWindow::on_cancel_clicked() {
}

void MainWindow::on_upload_clicked() {
}

#include "../../include/fish_annotator/db_uploader/moc_mainwindow.cpp"

}} // namespace fish_annotator::db_uploader

