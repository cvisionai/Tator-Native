#include <algorithm>

#include <boost/filesystem.hpp>

#include <QMessageBox>
#include <QFileDialog>

#include "fish_annotator/db_uploader/database_info.h"
#include "fish_annotator/db_uploader/mainwindow.h"
#include "ui_mainwindow.h"

namespace fish_annotator { namespace db_uploader {

namespace fs = boost::filesystem;

namespace { //anonymous

static const std::vector<std::string> kDirExtensions = {
  ".jpg", ".png", ".bmp", ".tif", ".jpeg",
  ".JPG", ".PNG", ".BMP", ".TIF", ".JPEG"};

} // anonymous namespace

MainWindow::MainWindow(QWidget *parent)
  : ui_(new Ui::MainWindow) 
  , input_db_(new QSqlDatabase())
  , output_db_(new QSqlDatabase()) {
  ui_->setupUi(this);
  setWindowTitle("Database Uploader");
#ifdef _WIN32
  setWindowIcon(QIcon(":/icons/cvision/cvision_no_text.ico"));
#endif
  fs::path current_path(QDir::currentPath().toStdString());
  fs::path default_input = current_path / fs::path("default.input_database");
  fs::path default_output = current_path / fs::path("default.output_database");
  if(fs::exists(default_input)) {
    DatabaseInfo input;
    deserialize(input, default_input.string());
    ui_->inputServer->setText(input.getServer().c_str());
    ui_->inputDatabase->setText(input.getDatabase().c_str());
    ui_->inputUsername->setText(input.getUsername().c_str());
  }
  if(fs::exists(default_output)) {
    DatabaseInfo output;
    deserialize(output, default_output.string());
    ui_->outputServer->setText(output.getServer().c_str());
    ui_->outputDatabase->setText(output.getDatabase().c_str());
    ui_->outputUsername->setText(output.getUsername().c_str());
  }
}

void MainWindow::on_connectInputDb_clicked() {
  ui_->inputDbStatus->setText("Attempting to connect...");
  ui_->inputDbStatus->repaint();
  *input_db_ = QSqlDatabase::addDatabase("QODBC3", "input");
  if(input_db_->isDriverAvailable("QODBC") == false) {
    QMessageBox err;
    err.critical(0, "Error", "ODBC driver is not available!");
  }
  input_db_->setDatabaseName(
      "DRIVER={SQL Server};SERVER={" + ui_->inputServer->text() + 
      "};DATABASE=" + ui_->inputDatabase->text() + 
      ";Trusted_Connection=no;user_id=" + ui_->inputUsername->text() + 
      ";password=" + ui_->inputPassword->text() + ";WSID=.");
  if(input_db_->isValid() == false) {
    ui_->inputDbStatus->setText("Not connected");
    QMessageBox err;
    err.critical(0, "Error", "Not a valid database!");
  }
  if(input_db_->open() == false) {
    ui_->inputDbStatus->setText("Not connected");
    QMessageBox err;
    err.critical(0, "Error", input_db_->lastError().text());
  }
  else {
    ui_->inputDbStatus->setText("Connected");
  }
  if(output_db_->isOpen() == true && input_db_->isOpen() == true) {
    ui_->upload->setEnabled(true);
  }
}

void MainWindow::on_connectOutputDb_clicked() {
  ui_->outputDbStatus->setText("Attempting to connect...");
  ui_->outputDbStatus->repaint();
  *output_db_ = QSqlDatabase::addDatabase("QODBC3", "output");
  if(output_db_->isDriverAvailable("QODBC") == false) {
    QMessageBox err;
    err.critical(0, "Error", "ODBC driver is not available!");
  }
  output_db_->setDatabaseName(
      "DRIVER={SQL Server};SERVER={" + ui_->outputServer->text() + 
      "};DATABASE=" + ui_->outputDatabase->text() + 
      ";Trusted_Connection=no;user_id=" + ui_->outputUsername->text() + 
      ";password=" + ui_->outputPassword->text() + ";WSID=.");
  if(output_db_->isValid() == false) {
    ui_->outputDbStatus->setText("Not connected");
    QMessageBox err;
    err.critical(0, "Error", "Not a valid database!");
  }
  if(output_db_->open() == false) {
    ui_->outputDbStatus->setText("Not connected");
    QMessageBox err;
    err.critical(0, "Error", output_db_->lastError().text());
  }
  else {
    ui_->outputDbStatus->setText("Connected");
  }
  if(output_db_->isOpen() == true && input_db_->isOpen() == true) {
    ui_->upload->setEnabled(true);
  }
}

void MainWindow::on_browseImageDir_clicked() {
  ui_->imageDirectory->setText(QFileDialog::getExistingDirectory(
    this, "Select annotated image directory"));
}

void MainWindow::on_cancel_clicked() {
  QApplication::quit();
}

void MainWindow::on_upload_clicked() {
  QString image_dir = ui_->imageDirectory->text();
  std::vector<boost::filesystem::path> image_files;
  fs::directory_iterator dir_it(image_dir.toStdString());
  fs::directory_iterator dir_end;
  for(; dir_it != dir_end; ++dir_it) {
    fs::path ext(dir_it->path().extension());
    for(auto &ok_ext : kDirExtensions) {
      if(ext == ok_ext) {
        image_files.push_back(dir_it->path());
      }
    }
  }
  std::sort(image_files.begin(), image_files.end());
}

#include "../../include/fish_annotator/db_uploader/moc_mainwindow.cpp"

}} // namespace fish_annotator::db_uploader

