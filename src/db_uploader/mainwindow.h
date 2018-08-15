/// @file
/// @brief Defines main window class for database uploader.

#ifndef DB_UPLOADER_MAINWINDOW_H
#define DB_UPLOADER_MAINWINDOW_H

#include <memory>

#include <QMainWindow>
#include <QtSql>
#include <QSqlDatabase>

#include "species.h"
#include "global_state_annotation.h"
#include "ui_mainwindow.h"

namespace tator { namespace db_uploader {

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  /// Constructor.
  ///
  /// @param parent Parent widget.
  explicit MainWindow(QWidget *parent = 0);
private slots:
  /// Connects to output database.
  void on_connectOutputDb_clicked();

  /// Opens a browser window to specify annotated image directory.
  void on_browseImageDir_clicked();

  /// Cancels the upload.
  void on_cancel_clicked();

  /// Uploads the annotations.
  void on_upload_clicked();
private:
  /// Widget loaded from ui file.
  std::unique_ptr<Ui::MainWindow> ui_;

  /// Output database.
  std::unique_ptr<QSqlDatabase> output_db_;

  /// Species (must match database field name).
  SpeciesList default_species_;

  /// Global states (must match database field name).
  GlobalStateAnnotation default_states_;

  /// Fields from survey raw data table (must match database field names).
  std::vector<std::string> raw_fields_;
};

}} // namespace tator::db_uploader

#endif // DB_UPLOADER_MAINWINDOW_H

