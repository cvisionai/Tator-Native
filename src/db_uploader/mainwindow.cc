#include <algorithm>

#include <boost/filesystem.hpp>

#include <QMessageBox>
#include <QFileDialog>
#include <QSqlTableModel>
#include <QProgressDialog>

#include "image_annotation.h"
#include "database_info.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <fstream>
std::ofstream debug("DEBUG.txt");

namespace fish_annotator { namespace db_uploader {

namespace fs = boost::filesystem;

namespace { //anonymous

static const std::vector<std::string> kDirExtensions = {
  ".jpg", ".png", ".bmp", ".tif", ".jpeg",
  ".JPG", ".PNG", ".BMP", ".TIF", ".JPEG"};

} // anonymous namespace

MainWindow::MainWindow(QWidget *parent)
  : ui_(new Ui::MainWindow) 
  , output_db_(new QSqlDatabase()) {
  ui_->setupUi(this);
  setWindowTitle("Database Uploader");
#ifdef _WIN32
  setWindowIcon(QIcon(":/icons/cvision/cvision_no_text.ico"));
#endif
  fs::path current_path(QDir::currentPath().toStdString());
  fs::path default_output = current_path / fs::path("default.output_database");
  if(fs::exists(default_output)) {
    DatabaseInfo output;
    deserialize(output, default_output.string());
    ui_->outputServer->setText(output.getServer().c_str());
    ui_->outputDatabase->setText(output.getDatabase().c_str());
    ui_->outputUsername->setText(output.getUsername().c_str());
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
  //@TODO UNCOMMENT THESE FOR CUSTOMER
  //output_db_->setUserName(ui_->outputUsername->text());
  //output_db_->setPassword(ui_->outputPassword->text());
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
  if(output_db_->isOpen() == true) {
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

  // Verify that image directory exists
  QString image_dir = ui_->imageDirectory->text();
  if(fs::exists(image_dir.toStdString()) == false) {
    QMessageBox err;
    err.critical(0, "Error", "Image directory does not exist!");
    return;
  }
  std::vector<boost::filesystem::path> image_files;
  fs::recursive_directory_iterator dir_it(image_dir.toStdString());
  fs::recursive_directory_iterator dir_end;
  for(; dir_it != dir_end; ++dir_it) {
    fs::path ext(dir_it->path().extension());
    for(auto &ok_ext : kDirExtensions) {
      if(ext == ok_ext) {
        image_files.push_back(dir_it->path());
      }
    }
  }

  // Read in the annotations
  std::sort(image_files.begin(), image_files.end());
  image_annotator::ImageAnnotationList annotations;
  annotations.read(image_files);
  int num_img = static_cast<int>(image_files.size());

  // Make a progress dialog
  QProgressDialog progress(
      "Uploading annotations...",
      "Abort",
      0,
      num_img,
      this,
      Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
  progress.setCancelButton(0);
  progress.setWindowTitle("Uploading Annotations");
  progress.setMinimumDuration(10);

  // Enable identity insert for relevant tables
  output_db_->exec("SET IDENTITY_INSERT dbo.SURVEY_DATA ON");
  if(output_db_->lastError().isValid()) {
    QMessageBox err;
    err.critical(0, "Error", "Unable to enable IDENTITY_INSERT for SURVEY_DATA.");
    return;
  }
  // @TODO ENABLE THIS FOR CUSTOMER
  //output_db_->exec("SET IDENTITY_INSERT dbo.DOT_HISTORY ON");
  //if(output_db_->lastError().isValid()) {
  //  QMessageBox err;
  //  err.critical(0, "Error", "Unable to enable IDENTITY_INSERT for DOT_HISTORY.");
  //  return;
  //}
  
  // Set up table models
  QSqlTableModel survey_raw_data(this, *output_db_);
  QSqlTableModel survey_data(this, *output_db_);
  QSqlTableModel dot_history(this, *output_db_);
  survey_raw_data.setTable("dbo.SURVEY_RAW_DATA");
  survey_data.setTable("dbo.SURVEY_DATA");
  dot_history.setTable("dbo.DOT_HISTORY");
  survey_data.setEditStrategy(QSqlTableModel::OnManualSubmit);
  dot_history.setEditStrategy(QSqlTableModel::OnManualSubmit);
  survey_raw_data.select();
  survey_data.select();
  dot_history.select();

  // Iterate through images
  bool ok = true;
  for(int img_index = 0; img_index < num_img; ++img_index) {
    debug << "IMAGE FILE: " << image_files[img_index] << std::endl;

    // Parse metadata from directory structure
    std::vector<std::string> dir_parts;
    for(const auto &sub : image_files[img_index]) {
      dir_parts.push_back(sub.string());
    }
    auto dir_len = dir_parts.size();
    if(dir_len < 6) {
      QMessageBox err;
      err.critical(0, "Error", 
          std::string(
            std::string("Invalid directory structure for image ") +
            image_files[img_index].string() +
            std::string("!  Aborting...")).c_str());
      ok = false;
      break;
    }
    std::string filename = dir_parts[dir_len - 1];
    std::string image_state = dir_parts[dir_len - 2];
    if(image_state != "Original") {
      continue;
    }
    std::string camera_control_camera_name = dir_parts[dir_len - 3];
    std::string area_control_short_name = dir_parts[dir_len - 4];
    std::string area_control_long_name = dir_parts[dir_len - 5];
    std::string area_control_year = dir_parts[dir_len - 6];
    debug << "CAMERA NAME: " << camera_control_camera_name << std::endl;
    debug << "SHORT NAME: " << area_control_short_name << std::endl;
    debug << "LONG NAME: " << area_control_long_name << std::endl;
    debug << "YEAR: " << area_control_year << std::endl;

    // Parse metadata from filename
    std::vector<std::string> file_parts;
    std::string file_part;
    std::istringstream file_stream(filename.c_str());
    while(std::getline(file_stream, file_part, '_')) {
      file_parts.push_back(file_part);
    }
    auto file_len = file_parts.size();
    if(file_len < 5) {
      QMessageBox err;
      err.critical(0, "Error", 
          std::string(
            std::string("Invalid filename for image ") +
            image_files[img_index].string() +
            std::string("!  Aborting...")).c_str());
      ok = false;
      break;
    }
    std::string survey_raw_data_station = file_parts[0].substr(0, 3);
    std::string survey_raw_data_quadrat_str = file_parts[1];
    int survey_raw_data_quadrat = 0;
    if(1 != sscanf_s(
          survey_raw_data_quadrat_str.c_str(), 
          "%dQ", 
          &survey_raw_data_quadrat)) {
      QMessageBox err;
      err.critical(0, "Error", 
          std::string(
            std::string("Could not parse quadrat from filename for image ") +
            image_files[img_index].string() +
            std::string("!  Aborting...")).c_str());
      ok = false;
      break;
    }
    debug << "STATION: " << survey_raw_data_station << std::endl;
    debug << "QUADRAT: " << survey_raw_data_quadrat << std::endl;

    // Update progress bar
    progress.setValue(img_index);
    if(progress.wasCanceled()) {
      ok = false;
      break;
    }

    // Find entry in AREA_CONTROL corresponding to this image
    QSqlQuery area_control(*output_db_);
    area_control.prepare(
        QString("SELECT areaControlPK FROM AREA_CONTROL WHERE ") +
        QString("surveyYear = :area_control_year AND ") + 
        QString("areaShortName = :area_control_short_name"));
    area_control.bindValue(
        ":area_control_year", 
        area_control_year.c_str());
    area_control.bindValue(
        ":area_control_short_name", 
        area_control_short_name.c_str());
    ok = area_control.exec();
    debug << "AREA CONTROL QUERY: " << area_control.lastQuery().toStdString() << std::endl;
    debug << "AREA CONTROL ACTIVE: " << area_control.isActive() << std::endl;
    debug << "AREA CONTROL EXEC SUCCESS: " << ok << std::endl;
    if(ok == false || area_control.next() == false) {
      QMessageBox err;
      err.critical(0, "Error", std::string(
            std::string("Could not find a AREA_CONTROL entry") +
            std::string(" for surveyYear=") +
            area_control_year +
            std::string(" and areaShortName=") +
            area_control_short_name +
            std::string("!")).c_str());
      ok = false;
      break;
    }
    int area_control_pk = area_control.value(0).toInt();
    area_control.finish();

    // Find entry in CAMERA_CONTROL corresponding to this image.
    QSqlQuery camera_control(*output_db_);
    camera_control.prepare(
        QString("SELECT cameraControlPK FROM CAMERA_CONTROL WHERE ") +
        QString("cameraName = :camera_control_camera_name"));
    camera_control.bindValue(
        ":camera_control_camera_name",
        camera_control_camera_name.c_str());
    ok = camera_control.exec();
    if(ok == false || camera_control.next() == false) {
      QMessageBox err;
      err.critical(0, "Error", std::string(
            std::string("Could not find a CAMERA_CONTROL entry") +
            std::string(" for cameraName=") +
            camera_control_camera_name +
            std::string("!")).c_str());
      ok = false;
      break;
    }
    int camera_control_pk = camera_control.value(0).toInt();
    camera_control.finish();

    // Find entry in SURVEY_RAW_DATA corresponding to this image.
    QSqlQuery survey_raw_data(*output_db_);
    survey_raw_data.prepare(
        QString("SELECT * FROM SURVEY_RAW_DATA WHERE ") +
        QString("areaControlPK = :area_control_pk AND ") +
        QString("quadrat = :quadrat AND ") +
        QString("station = :station"));
    survey_raw_data.bindValue(
        ":area_control_pk",
        area_control_pk);
    survey_raw_data.bindValue(
        ":quadrat",
        survey_raw_data_quadrat);
    survey_raw_data.bindValue(
        ":station",
        survey_raw_data_station.c_str());
    ok = survey_raw_data.exec();
    if(ok == false || survey_raw_data.next() == false) {
      QMessageBox err;
      err.critical(0, "Error", std::string(
            std::string("Could not find a SURVEY_RAW_DATA entry") +
            std::string(" for areaControlPK=") +
            std::to_string(area_control_pk) +
            std::string(" and station=") +
            survey_raw_data_station +
            std::string(" and quadrat=") +
            std::to_string(survey_raw_data_quadrat) +
            std::string("!")).c_str());
      ok = false;
      break;
    }
    QSqlRecord survey_raw_data_record = survey_raw_data.record();
    survey_raw_data.finish();

    auto row_count = survey_data.rowCount();
    auto ann = annotations.getImageAnnotations(image_files[img_index]);
    int num_ann = static_cast<int>(ann.size());
    for(int ai = 0; ai < num_ann; ++ai) {
      if(survey_data.insertRows(row_count, 1) == false) {
        QMessageBox err;
        err.critical(0, "Error", "Unable to insert row into table.");
        ok = false;
        break;
      }
      // measurementPK
      survey_data.setData(survey_data.index(row_count, 0), 0);
      // measurementControlPK
      survey_data.setData(survey_data.index(row_count, 1), 0);
      // updatedPK
      survey_data.setData(survey_data.index(row_count, 2), 0);
      // dfo
      survey_data.setData(survey_data.index(row_count, 3), 0);
      // measurement
      double meas = 0.0;
      if(ann[ai]->type_ == kLine) {
        double xdiff = ann[ai]->area_.x - ann[ai]->area_.w;
        double ydiff = ann[ai]->area_.y - ann[ai]->area_.h;
        meas = std::sqrt(xdiff * xdiff + ydiff * ydiff);
      }
      survey_data.setData(survey_data.index(row_count, 4), meas);
      // areacontrolpk
      survey_data.setData(survey_data.index(row_count, 5), 0);
      // station
      survey_data.setData(survey_data.index(row_count, 6), "---");
      // quadrat
      survey_data.setData(survey_data.index(row_count, 7), 0);
      // cameracontrolpk
      survey_data.setData(survey_data.index(row_count, 8), 0);
      // area
      survey_data.setData(survey_data.index(row_count, 9), "");
      // year
      survey_data.setData(survey_data.index(row_count, 10), "2017");
    }
    if(ok == false) {
      break;
    }
  }
  if(ok == true) {
    ok = survey_data.submitAll();
  }
  if(ok == true) {
    survey_data.database().commit();
    progress.setValue(num_img);
    QMessageBox status;
    status.information(0, "Success", "Database upload succeeded!");
  }
  else {
    survey_data.database().rollback();
    progress.close();
    if(output_db_->lastError().isValid()) {
      QMessageBox err;
      err.critical(0, "Error", survey_data.lastError().text());
    }
  }
  output_db_->exec("SET IDENTITY_INSERT dbo.STAGING_MEASUREMENTS OFF");
  if(output_db_->lastError().isValid()) {
    QMessageBox err;
    err.warning(0, "Warning", "Unable to disable IDENTITY_INSERT.");
    return;
  }
}

#include "moc_mainwindow.cpp"

}} // namespace fish_annotator::db_uploader

