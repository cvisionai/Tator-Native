#include <algorithm>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include <QMessageBox>
#include <QFileDialog>
#include <QSqlTableModel>
#include <QProgressDialog>

#include "image_annotation.h"
#include "database_info.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

namespace fish_annotator { namespace db_uploader {

namespace fs = boost::filesystem;

namespace { //anonymous

static const std::vector<std::string> kDirExtensions = {
  ".jpg", ".png", ".bmp", ".tif", ".jpeg",
  ".JPG", ".PNG", ".BMP", ".TIF", ".JPEG"};

const char *getCount(
    const std::map<std::string, uint64_t> &count_map,
    const std::string &species) {
  std::string species_lower = species;
  boost::algorithm::to_lower(species_lower);
  if(count_map.find(species_lower) == count_map.end()) {
    return "0";
  }
  else {
    const uint64_t count = count_map.at(species_lower);
    return std::to_string(count).c_str();
  }
}

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

  // Grab the image names recursively
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
  progress.setValue(0);
  progress.setMinimumDuration(10);

  // Set up table models
  QSqlTableModel survey_data(this, *output_db_);
  QSqlTableModel dot_history(this, *output_db_);
  survey_data.setTable("dbo.SURVEY_DATA");
  dot_history.setTable("dbo.DOT_HISTORY");
  survey_data.setEditStrategy(QSqlTableModel::OnManualSubmit);
  dot_history.setEditStrategy(QSqlTableModel::OnManualSubmit);
  survey_data.select();
  dot_history.select();

  // Find the max id in SURVEY_DATA
  bool ok = true;
  QSqlQuery survey_data_max_id(*output_db_);
  survey_data_max_id.prepare(
      QString("SELECT MAX(updatedPK) from SURVEY_DATA"));
  ok = survey_data_max_id.exec();
  if(ok == false || survey_data_max_id.next() == false) {
    QMessageBox err;
    err.critical(0, "Error", 
        "Could not find the max updatedPK in SURVEY_DATA!");
    return;
  }
  int survey_data_updated_pk = survey_data_max_id.value(0).toInt();
  survey_data_max_id.finish();

  // Find the max id in DOT_HISTORY
  QSqlQuery dot_history_max_id(*output_db_);
  dot_history_max_id.prepare(
      QString("SELECT MAX(dotHistoryPK) from DOT_HISTORY"));
  ok = dot_history_max_id.exec();
  if(ok == false || dot_history_max_id.next() == false) {
    QMessageBox err;
    err.critical(0, "Error",
        "Could not find the max dotHistoryPK in DOT_HISTORY!");
    return;
  }
  int dot_history_dot_history_pk = dot_history_max_id.value(0).toInt();
  dot_history_max_id.finish();

  // Iterate through images
  for(int img_index = 0; img_index < num_img; ++img_index) {

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

    // Get annotations for this image
    auto ann = annotations.getImageAnnotations(image_files[img_index]);
    int num_ann = static_cast<int>(ann.size());

    // Get global state annotations for this image
    auto global_state = annotations.getGlobalStateAnnotation(
        image_files[img_index].filename().string());

    // Get counts for each species in this image
    auto species_counts = annotations.getCounts(
        image_files[img_index].filename().string());

    // Enable identity insert for survey data
    output_db_->exec("SET IDENTITY_INSERT dbo.SURVEY_DATA ON");
    if(output_db_->lastError().isValid()) {
      QMessageBox err;
      err.critical(0, "Error", 
          "Unable to enable IDENTITY_INSERT for SURVEY_DATA.");
      ok = false;
      break;
    }
    
    // Create a new row in SURVEY_DATA
    auto row_count = survey_data.rowCount();
    if(survey_data.insertRows(row_count, 1) == false) {
      QMessageBox err;
      err.critical(0, "Error", "Unable to insert row into SURVEY_DATA.");
      ok = false;
      break;
    }

    // Update the record values
    survey_data_updated_pk++;
    QSqlRecord survey_data_record = survey_data.record(row_count);
    survey_data.setData(
        survey_data.index(
          row_count, 
          survey_data.fieldIndex("updatedPK")), 
        survey_data_updated_pk);
    survey_data.setData(
        survey_data.index(
          row_count, 
          survey_data.fieldIndex("surveyRawDataPK")), 
        survey_raw_data_record.value("surveyRawDataPK"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("cameraControlPK")), 
        camera_control_pk);
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("imageExists")), 
        1);
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("isImageOfInterest")),
        0);
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("sand")),
        global_state->states_["sand"]);
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("sandRipple")),
        global_state->states_["sandRipple"]);
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("shellDebris")),
        global_state->states_["shellDebris"]);
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("silt")),
        global_state->states_["silt"]);
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("gravel")),
        global_state->states_["gravel"]);
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("cobble")),
        global_state->states_["cobble"]);
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("rock")),
        global_state->states_["rock"]);
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("wasVisible")),
        global_state->states_["wasVisible"]);
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("scallops")),
        getCount(species_counts, "scallops"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("clappers")),
        getCount(species_counts, "clappers"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("seed")),
        global_state->states_["seed"]);
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("seaStars")),
        getCount(species_counts, "seaStars"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("crabs")),
        getCount(species_counts, "crabs"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("hermitCrabs")),
        getCount(species_counts, "hermitCrabs"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("echinodermOther")),
        getCount(species_counts, "echinodermOther"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("lobster")),
        getCount(species_counts, "lobster"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("sandDollars")),
        getCount(species_counts, "sandDollars"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("ad")),
        global_state->states_["ad"]);
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("anemone")),
        getCount(species_counts, "anemone"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("bHydra")),
        getCount(species_counts, "bHydra"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("brittleStar")),
        getCount(species_counts, "brittleStar"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("buccinum")),
        getCount(species_counts, "buccinum"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("clams")),
        getCount(species_counts, "clams"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("coral")),
        getCount(species_counts, "coral"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("ctenophores")),
        getCount(species_counts, "ctenophores"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("detritus")),
        global_state->states_["detritus"]);
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("euphasids")),
        getCount(species_counts, "euphasids"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("filo")),
        global_state->states_["filo"]);
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("holes")),
        global_state->states_["holes"]);
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("jellyFish")),
        getCount(species_counts, "jellyFish"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("moonsnail")),
        getCount(species_counts, "moonsnail"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("moonsnailEggCase")),
        getCount(species_counts, "moonsnailEggCase"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("mouse")),
        getCount(species_counts, "mouse"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("mussels")),
        getCount(species_counts, "mussels"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("otherCrustaceans")),
        getCount(species_counts, "otherCrustaceans"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("otherMolluscs")),
        getCount(species_counts, "otherMolluscs"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("seaweed")),
        global_state->states_["seaweed"]);
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("skateEggCase")),
        getCount(species_counts, "skateEggCase"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("sponges")),
        getCount(species_counts, "sponges"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("squid")),
        getCount(species_counts, "squid"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("urchin")),
        getCount(species_counts, "urchin"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("tunicate")),
        getCount(species_counts, "tunicate"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("cod")),
        getCount(species_counts, "cod"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("dogfish")),
        getCount(species_counts, "dogfish"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("eel")),
        getCount(species_counts, "eel"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("oceanPout")),
        getCount(species_counts, "oceanPout"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("flounder")),
        getCount(species_counts, "flounder"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("haddock")),
        getCount(species_counts, "haddock"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("hagfish")),
        getCount(species_counts, "hagfish"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("hake")),
        getCount(species_counts, "hake"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("herring")),
        getCount(species_counts, "herring"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("mackerel")),
        getCount(species_counts, "mackerel"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("monkFish")),
        getCount(species_counts, "monkFish"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("otherFish")),
        getCount(species_counts, "otherFish"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("sandlance")),
        getCount(species_counts, "sandlance"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("sculpin")),
        getCount(species_counts, "sculpin"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("seaRaven")),
        getCount(species_counts, "seaRaven"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("seaRobin")),
        getCount(species_counts, "seaRobin"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("skate")),
        getCount(species_counts, "skate"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("silverHake")),
        getCount(species_counts, "silverHake"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("unidentifiedFish")),
        getCount(species_counts, "unidentifiedFish"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("comments")),
        survey_raw_data_record.value("comments"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("scallopsAtEdge")),
        getCount(species_counts, "scallopsAtEdge"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("imageHasBeenChecked")),
        global_state->states_["imageHasBeenChecked"]);
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("imageHasBeenMeasured")),
        global_state->states_["imageHasBeenMeasured"]);
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("rowIsLocked")),
        global_state->states_["rowIsLocked"]);
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("Icelandic")),
        getCount(species_counts, "Icelandic"));
    survey_data.setData(
        survey_data.index(
          row_count,
          survey_data.fieldIndex("SeaCuke")),
        getCount(species_counts, "SeaCuke"));

    // Disable identity insert for survey data
    output_db_->exec("SET IDENTITY_INSERT dbo.SURVEY_DATA OFF");
    if(output_db_->lastError().isValid()) {
      QMessageBox err;
      err.critical(0, "Error", 
          "Unable to disable IDENTITY_INSERT for SURVEY_DATA.");
      ok = false;
      break;
    }

    for(auto ann_index = 0; ann_index < num_ann; ++ann_index) {

      // Only create a row for dot annotations
      if(ann[ann_index]->type_ != kDot) {
        continue;
      }

      // Enable identity insert for dot history
      output_db_->exec("SET IDENTITY_INSERT dbo.DOT_HISTORY ON");
      if(output_db_->lastError().isValid()) {
        QMessageBox err;
        err.critical(0, "Error", 
            "Unable to enable IDENTITY_INSERT for DOT_HISTORY.");
        ok = false;
        break;
      }

      // Create a new row in DOT_HISTORY
      auto dot_row_count = dot_history.rowCount();
      if(dot_history.insertRows(dot_row_count, 1) == false) {
        QMessageBox err;
        err.critical(0, "Error", "Unable to insert row into DOT_HISTORY.");
        ok = false;
        break;
      }

      // Update the record values
      dot_history_dot_history_pk++;
      dot_history.setData(
          dot_history.index(
            dot_row_count,
            dot_history.fieldIndex("dotHistoryPK")),
          dot_history_dot_history_pk);
      dot_history.setData(
          dot_history.index(
            dot_row_count,
            dot_history.fieldIndex("updatedPK")),
          survey_data_updated_pk);
      dot_history.setData(
          dot_history.index(
            dot_row_count,
            dot_history.fieldIndex("topLeftX")),
          ann[ann_index]->area_.x);
      dot_history.setData(
          dot_history.index(
            dot_row_count,
            dot_history.fieldIndex("topLeftY")),
          ann[ann_index]->area_.y);
      dot_history.setData(
          dot_history.index(
            dot_row_count,
            dot_history.fieldIndex("width")),
          ann[ann_index]->area_.w);
      dot_history.setData(
          dot_history.index(
            dot_row_count,
            dot_history.fieldIndex("height")),
          ann[ann_index]->area_.h);
      dot_history.setData(
          dot_history.index(
            dot_row_count,
            dot_history.fieldIndex("color")),
          ann[ann_index]->species_.c_str());

      // Disable identity insert for dot history
      output_db_->exec("SET IDENTITY_INSERT dbo.DOT_HISTORY OFF");
      if(output_db_->lastError().isValid()) {
        QMessageBox err;
        err.critical(0, "Error", 
            "Unable to disable IDENTITY_INSERT for DOT_HISTORY.");
        ok = false;
        break;
      }
    }
    if(ok == false) {
      break;
    }
  }
  if(ok == true) {
    // Enable identity insert for survey data
    output_db_->exec("SET IDENTITY_INSERT dbo.SURVEY_DATA ON");
    if(output_db_->lastError().isValid()) {
      QMessageBox err;
      err.critical(0, "Error", 
          "Unable to enable IDENTITY_INSERT for SURVEY_DATA.");
      ok = false;
    }
    ok = survey_data.submitAll();

    // Disable identity insert for survey data
    output_db_->exec("SET IDENTITY_INSERT dbo.SURVEY_DATA OFF");
    if(output_db_->lastError().isValid()) {
      QMessageBox err;
      err.critical(0, "Error", 
          "Unable to disable IDENTITY_INSERT for SURVEY_DATA.");
      ok = false;
    }

    // Enable identity insert for dot history
    output_db_->exec("SET IDENTITY_INSERT dbo.DOT_HISTORY ON");
    if(output_db_->lastError().isValid()) {
      QMessageBox err;
      err.critical(0, "Error", 
          "Unable to enable IDENTITY_INSERT for DOT_HISTORY.");
      ok = false;
    }

    ok = ok && dot_history.submitAll();

    // Disable identity insert for dot history
    output_db_->exec("SET IDENTITY_INSERT dbo.DOT_HISTORY OFF");
    if(output_db_->lastError().isValid()) {
      QMessageBox err;
      err.critical(0, "Error", 
          "Unable to disable IDENTITY_INSERT for DOT_HISTORY.");
      ok = false;
    }
  }
  if(ok == true) {
    output_db_->commit();
    progress.setValue(num_img);
    QMessageBox status;
    status.information(0, "Success", "Database upload succeeded!");
  }
  else {
    if(survey_data.lastError().isValid()) {
      QMessageBox err;
      err.critical(0, "Error", survey_data.lastError().text());
    }
    if(dot_history.lastError().isValid()) {
      QMessageBox err;
      err.critical(0, "Error", dot_history.lastError().text());
    }
    output_db_->rollback();
    progress.close();
  }
}

#include "moc_mainwindow.cpp"

}} // namespace fish_annotator::db_uploader

