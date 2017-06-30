#include "fish_annotator/db_uploader/mainwindow.h"

#include <QMessageBox>

#include "ui_mainwindow.h"

namespace fish_annotator { namespace db_uploader {

MainWindow::MainWindow(QWidget *parent)
  : ui_(new Ui::MainWindow) 
  , input_db_()
  , output_db_() {
  ui_->setupUi(this);
#ifdef _WIN32
  setWindowIcon(QIcon(":/icons/cvision/cvision_no_text.ico"));
#endif
}

void MainWindow::on_connectInputDb_clicked() {
  input_db_ = QSqlDatabase::addDatabase("QODBC3");
  if(input_db_.isDriverAvailable("QODBC") == false) {
    QMessageBox err;
    err.critical(0, "Error", "ODBC driver is not available!");
  }
  input_db_.setDatabaseName("DRIVER={SQL Server};SERVER={JON-PC2};DATABASE=scallop;Trusted_Connection=no;user_id=Jonathan Takahashi;password=;WSID=.");
  if(input_db_.isValid() == false) {
    QMessageBox err;
    err.critical(0, "Error", "Not a valid database!");
  }
  if(input_db_.open() == false) {
    QMessageBox err;
    err.critical(0, "Error", input_db_.lastError().text());
  }
  else {
    QMessageBox success;
    success.information(0, "SUCCESS", "SUCCESS");
  }
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

