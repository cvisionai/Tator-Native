/// @file
/// @brief Defines main window class for database uploader.

#ifndef DB_UPLOADER_MAINWINDOW_H
#define DB_UPLOADER_MAINWINDOW_H

#include <memory>

#include <QMainWindow>
#include <QtSql>
#include <QSqlDatabase>

#include "ui_mainwindow.h"

namespace fish_annotator { namespace db_uploader {

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  /// Constructor.
  ///
  /// @param parent Parent widget.
  explicit MainWindow(QWidget *parent = 0);
private slots:
  /// Connects to input database.
  void on_connectInputDb_clicked();

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

  /// Input database.
  std::unique_ptr<QSqlDatabase> input_db_;

  /// Output database.
  std::unique_ptr<QSqlDatabase> output_db_;
};

}} // namespace fish_annotator::db_uploader

#endif // DB_UPLOADER_MAINWINDOW_H

