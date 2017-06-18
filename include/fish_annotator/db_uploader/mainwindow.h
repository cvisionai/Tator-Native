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
  /// @brief Constructor.
  ///
  /// @param parent Parent widget.
  explicit MainWindow(QWidget *parent = 0);
private slots:
  /// @brief Connects to input database.
  void on_connectInputDb_clicked();

  /// @brief Connects to output database.
  void on_connectOutputDb_clicked();

  /// @brief Opens a browser window to specify annotated image directory.
  void on_browseImageDir_clicked();

  /// @brief Cancels the upload.
  void on_cancel_clicked();

  /// @brief Uploads the annotations.
  void on_upload_clicked();
private:
  /// @brief Widget loaded from ui file.
  std::unique_ptr<Ui::MainWindow> ui_;

  /// @brief Input database.
  QSqlDatabase input_db_;

  /// @brief Output database.
  QSqlDatabase output_db_;
};

}} // namespace fish_annotator::db_uploader

#endif // DB_UPLOADER_MAINWINDOW_H

