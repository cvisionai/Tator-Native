#include "fish_annotator/db_uploader/mainwindow.h"

#include <QMessageBox>

#include "ui_mainwindow.h"

namespace fish_annotator { namespace db_uploader {

MainWindow::MainWindow(QWidget *parent)
  : ui_(new Ui::MainWindow) 
  , input_db_()
  , output_db_() {
  ui_->setupUi(this);
  setWindowTitle("Database Uploader");
#ifdef _WIN32
  setWindowIcon(QIcon(":/icons/cvision/cvision_no_text.ico"));
#endif
}

void MainWindow::on_connectInputDb_clicked() {
  ui_->inputDbStatus->setText("Attempting to connect...");
  ui_->inputDbStatus->repaint();
  input_db_ = QSqlDatabase::addDatabase("QODBC3");
  if(input_db_.isDriverAvailable("QODBC") == false) {
    QMessageBox err;
    err.critical(0, "Error", "ODBC driver is not available!");
  }
  input_db_.setDatabaseName(
      "DRIVER={SQL Server};SERVER={" + ui_->inputServer->text() + 
      "};DATABASE=" + ui_->inputDatabase->text() + 
      ";Trusted_Connection=no;user_id=" + ui_->inputUsername->text() + 
      ";password=" + ui_->inputPassword->text() + ";WSID=.");
  if(input_db_.isValid() == false) {
    ui_->inputDbStatus->setText("Not connected");
    QMessageBox err;
    err.critical(0, "Error", "Not a valid database!");
  }
  if(input_db_.open() == false) {
    ui_->inputDbStatus->setText("Not connected");
    QMessageBox err;
    err.critical(0, "Error", input_db_.lastError().text());
  }
  else {
    ui_->inputDbStatus->setText("Connected");
  }
}

void MainWindow::on_connectOutputDb_clicked() {
  ui_->outputDbStatus->setText("Attempting to connect...");
  ui_->outputDbStatus->repaint();
  output_db_ = QSqlDatabase::addDatabase("QODBC3");
  if(output_db_.isDriverAvailable("QODBC") == false) {
    QMessageBox err;
    err.critical(0, "Error", "ODBC driver is not available!");
  }
  output_db_.setDatabaseName(
      "DRIVER={SQL Server};SERVER={" + ui_->outputServer->text() + 
      "};DATABASE=" + ui_->outputDatabase->text() + 
      ";Trusted_Connection=no;user_id=" + ui_->outputUsername->text() + 
      ";password=" + ui_->outputPassword->text() + ";WSID=.");
  if(output_db_.isValid() == false) {
    ui_->outputDbStatus->setText("Not connected");
    QMessageBox err;
    err.critical(0, "Error", "Not a valid database!");
  }
  if(output_db_.open() == false) {
    ui_->outputDbStatus->setText("Not connected");
    QMessageBox err;
    err.critical(0, "Error", output_db_.lastError().text());
  }
  else {
    ui_->outputDbStatus->setText("Connected");
  }
}

void MainWindow::on_browseImageDir_clicked() {
}

void MainWindow::on_cancel_clicked() {
  QApplication::quit();
}

void MainWindow::on_upload_clicked() {
}

#include "../../include/fish_annotator/db_uploader/moc_mainwindow.cpp"

}} // namespace fish_annotator::db_uploader

